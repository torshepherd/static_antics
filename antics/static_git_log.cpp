/// The following code should NOT be copied or used in other codebases. Not
/// because it's secret, but because it's highly *terrible*

#include "../include/escape.h" // IWYU pragma: keep
#include "../include/support.h"
#include "../include/utils.hpp"

#include "../external/fmt-master/include/fmt/compile.h"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>

#if !__has_embed(".git/logs/refs/heads/main")
static_assert(false, "Fatal: not a git repository!");
static constexpr const char raw_contents[] = "";
#else
static constexpr const char raw_contents[] = {
#  embed ".git/logs/refs/heads/main"
};
#endif

static constexpr std::string_view contents{std::begin(raw_contents),
                                           std::end(raw_contents)};

// static constexpr

constexpr auto done = false;
static_assert(
    // clang-format off: align left for nice formatting!
done
    // clang-format on
    ,
    [] -> std::string {
      char formatted_buffer[contents.size() * 2]{};
      char buffer[contents.size() * 2]{};

      auto *running = formatted_buffer;
      for (auto [line_number, line] :
           contents | std::views::split('\n') | std::views::enumerate) {
        auto line_view = std::string_view{line.begin(), line.end()};
        running = fmt::format_to(running, FMT_COMPILE("{0:>{1}} │ {2}\n"),
                                 line_number + 1, max_digits + 2, line_view);
      }
      running = fmt::format_to(running, FMT_COMPILE("╭{0:─^{1}}┴{0:─^{2}}╮\n"),
                               "", max_digits + 2,
                               __builtin_strlen(filename) +
                                   numPlaces(contents.size()) - max_digits + 3);
      running = fmt::format_to(running, FMT_COMPILE("│ {} ({}B) │\n"), filename,
                               contents.size());
      running = fmt::format_to(
          running, FMT_COMPILE("╰{0:─^{1}}┬{0:─^{2}}╯"), "", 5,
          __builtin_strlen(filename) + numPlaces(contents.size()));
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
