/// The following code should NOT be copied or used in other codebases. Not
/// because it's secret, but because it's highly *terrible*

#include "../include/escape.h"
#include "../include/support.h"
#include "../include/utils.hpp"

#include "../external/ctre.hpp"
#include "../external/fmt-10.2.1/include/fmt/compile.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>

#ifndef GREP_FILE
#  error Usage: clang static_grep.cpp -std=c++2c -Wno-everything -fconstexpr-steps=10000000 -DGREP_FILE="path/to/file" -DGREP_PATTERN="^my.*pattern$" -DGREP_CONTEXT=2
#elif !defined(GREP_PATTERN)
#  error Usage: clang static_grep.cpp -std=c++2c -Wno-everything -fconstexpr-steps=10000000 -DGREP_FILE="path/to/file" -DGREP_PATTERN="^my.*pattern$" -DGREP_CONTEXT=2
#elif !defined(GREP_CONTEXT)
#  error Usage: clang static_grep.cpp -std=c++2c -Wno-everything -fconstexpr-steps=10000000 -DGREP_FILE="path/to/file" -DGREP_PATTERN="^my.*pattern$" -DGREP_CONTEXT=2
#else
#  define QUOTED_PATH STRINGIZE_VALUE_OF(GREP_FILE)

#  if __has_embed(QUOTED_PATH)
#    define FOUND_FILE
static constexpr auto filename = QUOTED_PATH;
static constexpr const char raw_contents[] = {
#    embed QUOTED_PATH
};
#  elif __has_embed(GREP_FILE)
#    define FOUND_FILE
static constexpr auto filename = GREP_FILE;
static constexpr const char raw_contents[] = {
#    embed GREP_FILE
};
#  endif

#  ifdef FOUND_FILE
static constexpr std::string_view contents{std::begin(raw_contents),
                                           std::end(raw_contents)};
static constexpr size_t number_of_lines =
    std::ranges::count(contents, '\n') + 1;
static constexpr size_t max_digits = numPlaces(number_of_lines);

static constexpr auto pattern = [] {
  // If the pattern itself contained a string already, we quote and unquote it once. Just trust
  if constexpr (STRINGIZE_VALUE_OF(GREP_PATTERN)[0] == '\"') {
    static constexpr char raw_pattern[] = STRINGIZE_VALUE_OF(GREP_PATTERN);
    static constexpr std::string_view pattern_view{std::begin(raw_pattern),
                                                   std::end(raw_pattern)};
    static constexpr auto dequoted = dequote(pattern_view);
    return ctll::fixed_string<dequoted.size()>{ctll::construct_from_pointer,
                                               dequoted.data()};
  } else
    return ctll::fixed_string{STRINGIZE_VALUE_OF(GREP_PATTERN)};
}();

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
      bool first_time = true;
      for (auto [line_number, line] :
           contents | std::views::split('\n') | std::views::enumerate) {
        // Get the middle line
        auto line_view = std::string_view{line.begin(), line.end()};
        if (auto matches = ctre::search_all<pattern>(line_view);
            !std::ranges::empty(matches)) {
          if (!first_time) {
            running = fmt::format_to(running, FMT_COMPILE("{0:─^{1}}┼───\n"),
                                     "", max_digits + 3);
          } else {
            first_time = false;
          }
          running = fmt::format_to(running, FMT_COMPILE("{0:>{1}} │ {2}\n"),
                                   line_number + 1, max_digits + 2, line_view);

          // Printing out the matches line. First, the stuff to the left and the bar
          running = fmt::format_to(running, FMT_COMPILE("{0:>{1}} │"), "",
                                   max_digits + 2);
          // Next, add a ▔▔▔▔ for each match
          ptrdiff_t previous_end = 0;
          for (const auto match : matches) {
            const auto space_before =
                (match.begin() - line_view.begin() + 1) - previous_end;
            running =
                fmt::format_to(running, FMT_COMPILE("{0:>{1}}{0:▔^{2}}"), "",
                               // Space before the underline
                               space_before,
                               // Length of the underline
                               match.size());
            previous_end = static_cast<ptrdiff_t>(match.size()) + space_before -
                           previous_end;
          }
          running = fmt::format_to(running, FMT_COMPILE("\n"));
        }
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
#    if SUPPORTS_ESCAPES
                         ESC_MOVE_TO_START ESC_CLEAR_LINE ESC_SET_FG_DEFAULT
#    else
                         "\n\n"
#    endif
                         "{}"
#    if SUPPORTS_ESCAPES
                         ESC_SET_INVISIBLE
#    endif
                         ),
                     formatted_buffer);
      return buffer;
    }());

#  else
#    error File not found!
#  endif
#endif
