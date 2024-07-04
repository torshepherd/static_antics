#pragma once

#include "../external/fmt-10.2.1/include/fmt/compile.h"
#include "../include/support.h"

#define DEL "\x7f"
#define BS "\x08"
#define ESC "\x1b"

#define ESC_MOVE_TO_START ESC "[0G"
#define ESC_CLEAR_LINE ESC "[2K"

#define ESC_SET_FG_DEFAULT ESC "[0;39m"
#define ESC_SET_FG_GREEN ESC "[0;32m"

#define ESC_SET_INVISIBLE ESC "[8m"

#define BOX_ARC_DOWN_RIGHT "╭"
#define BOX_ARC_DOWN_LEFT "╮"
#define BOX_ARC_UP_LEFT "╯"
#define BOX_ARC_UP_RIGHT "╰"
#define BOX_HORIZONTAL "─"