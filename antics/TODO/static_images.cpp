
#include "../../external/fmt-master/include/fmt/compile.h"

#include "../../include/escape.h" // IWYU pragma: keep
#include "../../include/support.h"
#include "../../include/utils.hpp"

#include <cstring>
#include <span>
#include <string>
#include <string_view>

#if !SUPPORTS_ESCAPES
// #  error Compiler doesn't support escapes :(
// #else
static constexpr std::byte rick_png_data[]{
#  include "../../external/rick-png.h"
};

static constexpr std::span<std::byte> rick{&rick_png_data[0],
                                           sizeof(rick_png_data)};

static constexpr std::string converted = [] -> std::string {
  char buffer[rick.size()];

  __builtin_memcpy(&buffer[0], &rick_png_data[0], sizeof(rick_png_data));

  return buffer;
}();

constexpr auto done = false;
static_assert(
    // clang-format off: align left for nice formatting!
done
    // clang-format on
    ,
    [] -> std::string {
      char buffer[9999999]{};

      fmt::format_to(
          buffer,
          FMT_COMPILE(ESC_MOVE_TO_START ESC_CLEAR_LINE ESC_SET_FG_DEFAULT ESC
                      "_Gf=100;{}" ESC "\\" ESC_SET_INVISIBLE),
          converted);

      return buffer;
    }());

#endif