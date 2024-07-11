#pragma once

#include <cctype>
#include <concepts>
#include <cstddef>
#include <limits>
#include <string_view>

#define STRINGIZE(...) #__VA_ARGS__
#define STRINGIZE_VALUE_OF(...) STRINGIZE(__VA_ARGS__)

#define INCREMENT(...) __VA_ARGS__ + 1
#define INCREMENT_VALUE_OF(...) INCREMENT(__VA_ARGS__)

constexpr std::string_view dequote(const std::string_view original) {
  if (original.size() < 3) {
    return original;
  }
  if (original.front() != '\"' || original.back() != '\"') {
    // If either front of back isn't a quote, return original
    return original;
  }
  return dequote(original.substr(1, original.size() - 2));
}

static_assert(dequote("") == "");
static_assert(dequote(R"(")") == R"(")");
static_assert(dequote(R"("")") == R"("")");
static_assert(dequote(R"("a")") == "a");
static_assert(dequote(R"("Hello)") == R"("Hello)");
static_assert(dequote(R"("Hello")") == "Hello");
static_assert(dequote(R"(""Hello"")") == "Hello");

constexpr bool constexpr_isalpha(char c) {
  char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";
  char *letter = alphabet;

  while (*letter != '\0' && *letter != c)
    ++letter;

  if (*letter) return 1;

  return 0;
}

constexpr char constexpr_toupper(char ch) {
  switch (ch) {
  case 'a': return 'A';
  case 'b': return 'B';
  case 'c': return 'C';
  case 'd': return 'D';
  case 'e': return 'E';
  case 'f': return 'F';
  case 'g': return 'G';
  case 'h': return 'H';
  case 'i': return 'I';
  case 'j': return 'J';
  case 'k': return 'K';
  case 'l': return 'L';
  case 'm': return 'M';
  case 'n': return 'N';
  case 'o': return 'O';
  case 'p': return 'P';
  case 'q': return 'Q';
  case 'r': return 'R';
  case 's': return 'S';
  case 't': return 'T';
  case 'u': return 'U';
  case 'v': return 'V';
  case 'w': return 'W';
  case 'x': return 'X';
  case 'y': return 'Y';
  case 'z': return 'Z';
  default: return ch;
  }
}

template <std::integral T>
  requires(!std::same_as<bool, T>)
constexpr size_t numPlaces(T n) {
  if (n < 0)
    n = (n == std::numeric_limits<T>::min()) ? std::numeric_limits<T>::max()
                                             : -n;
  if (n < 10) return 1;
  if (n < 100) return 2;
  if (n < 1000) return 3;
  if (n < 10000) return 4;
  if (n < 100000) return 5;
  if (n < 1000000) return 6;
  if (n < 10000000) return 7;
  if (n < 100000000) return 8;
  if (n < 1000000000) return 9;
  /*      2147483647 is 2^31-1 - add more ifs as needed
     and adjust this final return as well. */
  return 10;
}