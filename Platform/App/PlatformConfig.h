#pragma once

#include <cstdint>

/* Widescreen support - 16:9 aspect ratio */
/* Original 4:3: 640x480 | Widescreen 16:9: 864x480 (+224 pixels horizontally) */
constexpr int GAME_FRAMEBUFFER_BASE_WIDTH = 640;
constexpr int GAME_FRAMEBUFFER_BASE_HEIGHT = 480;
constexpr int GAME_FRAMEBUFFER_WIDESCREEN_PADDING = 112;
constexpr int GAME_FRAMEBUFFER_WIDTH = GAME_FRAMEBUFFER_BASE_WIDTH + (2 * GAME_FRAMEBUFFER_WIDESCREEN_PADDING);
constexpr int GAME_FRAMEBUFFER_HEIGHT = GAME_FRAMEBUFFER_BASE_HEIGHT;

constexpr uint64_t GAME_FRAME_DURATION_NS_50 = 1000000000ull / 50ull;
constexpr uint64_t GAME_FRAME_DURATION_NS_60 = 1000000000ull / 60ull;
