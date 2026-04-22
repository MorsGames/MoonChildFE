#pragma once

#include <cstdint>

constexpr int GAME_FRAMEBUFFER_WIDTH = 640;
constexpr int GAME_FRAMEBUFFER_HEIGHT = 480;
constexpr uint64_t GAME_FRAME_DURATION_NS_50 = 1000000000ull / 50ull;
constexpr uint64_t GAME_FRAME_DURATION_NS_60 = 1000000000ull / 60ull;
