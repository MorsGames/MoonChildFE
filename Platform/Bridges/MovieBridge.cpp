#include "MovieBridge.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "frm_int.hpp"
#include "IAudio.h"
#include "MediaCatalog.h"
#include "Resources.h"
#include "movie.hpp"

#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"

static plm_t* PlmInstance = nullptr;
static IAudio* MovieAudioBackend = nullptr;

static uint8_t* HostFramebuffer = nullptr;
static int HostFramebufferWidth = 0;
static int HostFramebufferHeight = 0;

static std::vector<uint8_t> DecodedFrameRGBA;
static bool MoviePlaybackEnded = true;

static const char* ResolveMoviePath(const char* mpgFileName)
{
    static char path[2048];
    char          rel[512];
    std::snprintf(rel, sizeof(rel), "movies/%s", mpgFileName);
    char* resolved = FullPath(rel);
    std::snprintf(path, sizeof(path), "%s", resolved);
    return path;
}

static void ClearFramebufferBlack()
{
    if (HostFramebuffer && HostFramebufferWidth > 0 && HostFramebufferHeight > 0)
    {
        std::memset(HostFramebuffer, 0,
            static_cast<size_t>(HostFramebufferWidth) * HostFramebufferHeight * 4);
    }
}

static void OnVideo(plm_t* /*plm*/, plm_frame_t* frame, void* /*user*/)
{
    if (!HostFramebuffer || HostFramebufferWidth <= 0 || HostFramebufferHeight <= 0)
        return;

    const int sw = static_cast<int>(frame->width);
    const int sh = static_cast<int>(frame->height);
    if (sw <= 0 || sh <= 0)
        return;

    DecodedFrameRGBA.resize(static_cast<size_t>(sw) * sh * 4);
    plm_frame_to_rgba(frame, DecodedFrameRGBA.data(), sw * 4);

    int dw;
    int dh;
    if (static_cast<int64_t>(sw) * HostFramebufferHeight
        >= static_cast<int64_t>(sh) * HostFramebufferWidth)
    {
        dw = HostFramebufferWidth;
        dh = static_cast<int>((static_cast<int64_t>(sh) * HostFramebufferWidth) / sw);
        if (dh < 1) dh = 1;
    }
    else
    {
        dh = HostFramebufferHeight;
        dw = static_cast<int>((static_cast<int64_t>(sw) * HostFramebufferHeight) / sh);
        if (dw < 1) dw = 1;
    }
    const int dx = (HostFramebufferWidth - dw) / 2;
    const int dy = (HostFramebufferHeight - dh) / 2;

    ClearFramebufferBlack();

    for (int y = 0; y < dh; y++)
    {
        const int sy = (y * sh) / dh;
        const uint8_t* srcRow = &DecodedFrameRGBA[static_cast<size_t>(sy) * sw * 4];
        uint8_t* dstRow =
            HostFramebuffer + (static_cast<size_t>(dy + y) * HostFramebufferWidth + dx) * 4;
        for (int x = 0; x < dw; x++)
        {
            const int sx = (x * sw) / dw;
            const uint8_t* s = srcRow + sx * 4;
            uint8_t* d = dstRow + x * 4;
            d[0] = s[0];
            d[1] = s[1];
            d[2] = s[2];
            d[3] = 0xFF;
        }
    }
}

static void OnAudio(plm_t* /*plm*/, plm_samples_t* samples, void* /*user*/)
{
    MovieAudioBackend->SubmitMovieAudio(samples->interleaved, samples->count * 2);
}

static void TeardownStream()
{
    MovieAudioBackend->CloseMovieStream();
    if (PlmInstance)
    {
        plm_destroy(PlmInstance);
        PlmInstance = nullptr;
    }
    MoviePlaybackEnded = true;
}

namespace MovieBridge
{
    void Attach(IAudio* audio, uint8_t* framebuffer, int width, int height)
    {
        MovieAudioBackend = audio;
        HostFramebuffer = framebuffer;
        HostFramebufferWidth = width;
        HostFramebufferHeight = height;
    }

    void Detach()
    {
        TeardownStream();
        MovieAudioBackend = nullptr;
        HostFramebuffer = nullptr;
        HostFramebufferWidth = 0;
        HostFramebufferHeight = 0;
    }

    void Tick(double deltaSeconds)
    {
        if (!PlmInstance || MoviePlaybackEnded)
            return;

        plm_decode(PlmInstance, deltaSeconds);

        if (plm_has_ended(PlmInstance))
        {
            MoviePlaybackEnded = true;
        }
    }
}

Cmovie::Cmovie(Caudio* audio)
{
    (void)audio;
    this->videoFilename = nullptr;
    this->videoReady = true;
}

Cmovie::Cmovie(void)
{
    this->videoFilename = nullptr;
    this->videoReady = true;
}

Cmovie::~Cmovie(void)
{
    TeardownStream();
}

Smack* Cmovie::open(char* filename)
{
    TeardownStream();

    const char* mpgFileName = GetMovieFile(filename);

    if (!mpgFileName)
    {
        std::fprintf(stderr, "Unknown movie! %s\n", filename);
        return nullptr;
    }

    const char* path = ResolveMoviePath(mpgFileName);
    PlmInstance = plm_create_with_filename(path);
    if (!PlmInstance)
    {
        std::fprintf(stderr, "Failed to open movie! %s\n", path);
        this->videoFilename = nullptr;
        this->videoReady = true;
        return nullptr;
    }

    if (!plm_probe(PlmInstance, 5000 * 1024))
    {
        std::fprintf(stderr, "No MPEG streams found! %s\n", path);
        plm_destroy(PlmInstance);
        PlmInstance = nullptr;
        this->videoFilename = nullptr;
        this->videoReady = true;
        return nullptr;
    }

    plm_set_video_decode_callback(PlmInstance, OnVideo, nullptr);
    plm_set_audio_decode_callback(PlmInstance, OnAudio, nullptr);
    plm_set_loop(PlmInstance, FALSE);
    plm_set_audio_enabled(PlmInstance, TRUE);
    plm_set_audio_stream(PlmInstance, 0);

    if (plm_get_num_audio_streams(PlmInstance) > 0)
    {
        const int sampleRate = plm_get_samplerate(PlmInstance);

        if (MovieAudioBackend->OpenMovieStream(sampleRate, 2))
        {
            plm_set_audio_lead_time(PlmInstance, 4096.0 / sampleRate);
        }
        else
        {
            std::fprintf(stderr, "Failed to open audio stream! %s\n", path);
            plm_set_audio_enabled(PlmInstance, FALSE);
        }
    }

    MoviePlaybackEnded = false;
    ClearFramebufferBlack();

    this->videoFilename = const_cast<char*>(mpgFileName);
    this->videoReady = false;

    return reinterpret_cast<Smack*>(1);
}

void Cmovie::close(Smack* /*smk*/)
{
    TeardownStream();
    this->videoFilename = nullptr;
    this->videoReady = true;
    ClearFramebufferBlack();
}

void Cmovie::playtovideo(Smack* /*smk*/, Cvideo* /*video*/, Cblitbuf* /*hulpbuf*/, UINT16 /*zoomfactor*/)
{
}

UINT16 Cmovie::stillplaying(void)
{
    if (!PlmInstance)
        return 0;
    return MoviePlaybackEnded ? 0 : 1;
}

void Cmovie::movieplay(void)
{
}

void Cmovie::returnpal(BYTE* destpal)
{
    (void)destpal;
}

void Cmovie::dump(FILE* fd)
{
    (void)fd;
}

void Cmovie::ClearBack(u32 flipafter)
{
    (void)flipafter;
}

void Cmovie::dopal(void)
{
}

void Cmovie::blitrect(u32 x, u32 y, u32 w, u32 h)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
}

void Cmovie::DoPaint()
{
}

void Cmovie::mergeinterrect(whRECT* r1, whRECT* r2, whRECT* m, whRECT* i)
{
    (void)r1;
    (void)r2;
    (void)m;
    (void)i;
}

void Cmovie::mergerects(whRECT* r1, u32 r1num, whRECT* r2, u32 r2num, whRECT** o, u32* onum)
{
    (void)r1;
    (void)r1num;
    (void)r2;
    (void)r2num;
    (void)o;
    (void)onum;
}

void Cmovie::DoAdvance()
{
}

void Cmovie::InitPal(void)
{
}
