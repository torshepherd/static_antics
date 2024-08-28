// TODO: When compiling with gcc, output with colors, or better yet with kitty images

#include "../include/escape.h" // IWYU pragma: keep
#include "../include/support.h"
#include "../include/utils.hpp"

#include "../external/fmt-master/include/fmt/compile.h"
#include "../external/wordle_words.h"

#include <cctype>
#include <cstddef>
#include <ranges>
#include <string>

#ifndef GUESS
#  error Usage: CC static_wordle.cpp -std=c++2c -Wno-everything -fconstexpr-steps=10000000 -DGUESS="CRANE HOUSE etc"
#else

static constexpr auto actual_word = wordle_words[42];
constexpr auto get_bgcolor(char letter, size_t pos) {
  if (actual_word[pos] == letter) {
    return ESC_SET_BG_WORDLE_GREEN;
  }
  for (char c : actual_word)
    if (actual_word[pos] == letter) {
      return ESC_SET_BG_WORDLE_YELLOW;
    }
}

static constexpr auto guesses = dequote(STRINGIZE_VALUE_OF(GUESS));

constexpr auto done = false;
static_assert(
    // clang-format off: align left for nice formatting!
done
    // clang-format on
    ,
    [] -> std::string {
      char formatted_buffer[200]{};
      char buffer[200]{};

      auto *running = formatted_buffer;
      for (auto chunk : guesses | std::views::filter([](const char c) {
                          return constexpr_isalpha(c);
                        }) | std::views::transform([](const char c) -> char {
                          return constexpr_toupper(c);
                        }) | std::views::chunk(5) |
                            std::views::take(6)) {
        char guess[5]{};
        size_t i = 0UL;
        for (auto c : chunk) {
          guess[i++] = c;
        }
        if (i == 5)
          // Grade the guess
          running = fmt::format_to(running,
                                   FMT_COMPILE("{1}{2}{0}"
                                               "{3}{4}{0}"
                                               "{5}{6}{0}"
                                               "{7}{8}{0}"
                                               "{9}{10}{0}"
                                               "\n"),
                                   ESC_SET_BG_DEFAULT,
                                   //
                                   get_bgcolor(guess[0], 0), guess[0],
                                   //
                                   get_bgcolor(guess[1], 1), guess[1],
                                   //
                                   get_bgcolor(guess[2], 2), guess[2],
                                   //
                                   get_bgcolor(guess[3], 3), guess[3],
                                   //
                                   get_bgcolor(guess[4], 4), guess[4]);
        else
          running =
              fmt::format_to(running, FMT_COMPILE("{}{}{}{}{}"),
                             //
                             guess[0], guess[1], guess[2], guess[3], guess[4]);
      }

      fmt::format_to(buffer,
                     FMT_COMPILE(
#  if SUPPORTS_ESCAPES
                         ESC_MOVE_TO_START ESC_CLEAR_LINE ESC_SET_FG_DEFAULT
#  else
                         "\n\n"
#  endif
                         "{}"
#  if SUPPORTS_ESCAPES
                         ESC_SET_INVISIBLE
#  endif
                         ),
                     formatted_buffer);

      return buffer;
    }());
#endif