#pragma once
// IWYU pragma: always_keep

#include "../external/fmt-master/include/fmt/compile.h"
#include "../include/support.h"

#define DEL "\x7f"
#define BS "\x08"
#define ESC "\x1b"

#define ESC_MOVE_TO_START ESC "[0G"
#define ESC_CLEAR_LINE ESC "[2K"

#define ESC_SET_FG_DEFAULT ESC "[0;39m"
#define ESC_SET_FG_GREEN ESC "[0;32m"

#define ESC_SET_BG_DEFAULT ESC "[0;49m"
#define ESC_SET_BG_GREEN ESC "[0;42m"
#define ESC_SET_BG_YELLOW ESC "[0;43m"

#define ESC_SET_BG_WORDLE_GREEN ESC "[48;2;108;169;101m"
#define ESC_SET_BG_WORDLE_YELLOW ESC "[48;2;200;182;83m"
#define ESC_SET_BG_WORDLE_GRAY ESC "[48;2;120;124;127m"

#define ESC_SET_INVISIBLE ESC "[8m"

#define BOX_ARC_DOWN_RIGHT "╭"
#define BOX_ARC_DOWN_LEFT "╮"
#define BOX_ARC_UP_LEFT "╯"
#define BOX_ARC_UP_RIGHT "╰"
#define BOX_HORIZONTAL "─"