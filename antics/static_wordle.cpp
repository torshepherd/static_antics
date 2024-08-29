#include "../external/fmt-master/include/fmt/compile.h"
#include "../include/escape.h"

#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>

constexpr char WORDLE_GREEN[] = "\x1b[48;2;83;141;78m";
constexpr char WORDLE_YELLOW[] = "\x1b[48;2;181;159;59m";
constexpr char WORDLE_GRAY[] = "\x1b[48;2;58;58;60m";
constexpr char WORDLE_TEXT[] = "\x1b[38;2;248;248;248m";
constexpr char ACTUAL_WORD[] = "ROBOT";

constexpr std::string_view all_guesses(GUESS);

constexpr bool contains(std::string_view letters, char one_letter) {
  for (auto l : letters) {
    if (one_letter == l) return true;
  }
  return false;
}

static_assert(false, []() {
  char buffer[4096]{};

  auto *running = buffer;
  running = fmt::format_to(running, FMT_COMPILE("\n\n  {}"), WORDLE_TEXT);

  size_t col = 0;
  size_t row = 0;

  for (auto guessed_letter : all_guesses) {
    if (guessed_letter >= 'a' && guessed_letter <= 'z')
      guessed_letter -= 32; // Capitalize

    if (guessed_letter < 'A' || guessed_letter > 'Z')
      continue; // Skip non-alpha

    auto *color = WORDLE_GRAY;
    if (ACTUAL_WORD[col] == guessed_letter) {
      color = WORDLE_GREEN;
    } else if (contains(ACTUAL_WORD, guessed_letter)) {
      color = WORDLE_YELLOW;
    }

    running = fmt::format_to(running, FMT_COMPILE("{} {} \x1b[49m"), color,
                             guessed_letter);

    col++;
    if (col == 5) {
      col = 0;
      row++;
      running = fmt::format_to(running, FMT_COMPILE("\n  "));
      if (row == 6) {
        break;
      }
    }
  }

  // Print empty guesses
  for (; row < 6; ++row) {
  }

  return std::string(buffer);
}());