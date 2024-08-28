/// The following code should NOT be copied or used in other codebases. Not
/// because it's secret, but because it's highly *terrible*

#include "../../external/fmt-master/include/fmt/compile.h"
#include "../../include/escape.h"
#include "../../include/support.h"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>

#if __has_embed("/proc/self/environ")
static constexpr const char raw_contents[] = {
#  embed "/proc/self/environ"
};
static constexpr std::string_view contents{std::begin(raw_contents),
                                           std::end(raw_contents)};
#endif

constexpr auto done = false;
static_assert(
    // clang-format off: align left for nice formatting!
done
    // clang-format on
    ,

    [] -> std::string {
      char formatted_buffer[10000000]{};
      char buffer[10000000]{};

      auto *running = formatted_buffer;

      for (auto line : contents | std::views::split('\0')) {
        auto line_view = std::string_view{line.begin(), line.end()};
        running = fmt::format_to(running, FMT_COMPILE("{}\n"), line_view);
      }

      fmt::format_to(buffer,
                     FMT_COMPILE(
#if SUPPORTS_ESCAPES
                         ESC_MOVE_TO_START ESC_CLEAR_LINE ESC_SET_FG_DEFAULT
#else
                         "\n\n"
#endif
                         "{}"
#if SUPPORTS_ESCAPES
                         ESC_SET_INVISIBLE
#endif
                         ),
                     formatted_buffer);

      return buffer;
    }());