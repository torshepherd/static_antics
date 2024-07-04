#pragma once

#include <concepts>
#include <cstddef>
#include <limits>
#include <string_view>

#define STRINGIZE(...) #__VA_ARGS__
#define STRINGIZE_VALUE_OF(...) STRINGIZE(__VA_ARGS__)

constexpr std::string_view dequote(const std::string_view original) {
  if (original.size() < 2) {
    return original;
  }
  if (original.front() != '\"' || original.back() != '\"') {
    // If either front of back isn't a quote, return original
    return original;
  }
  return original.substr(1, original.size() - 2);
}

static_assert(dequote("") == "");
static_assert(dequote("\"") == "\"");
static_assert(dequote("\"Hello") == "\"Hello");
static_assert(dequote("\"Hello\"") == "Hello");

template <std::integral T>
  requires(!std::same_as<bool, T>)
constexpr size_t numPlaces(T n) {
  if (n < 0)
    n = (n == std::numeric_limits<T>::min()) ? std::numeric_limits<T>::max()
                                             : -n;
  if (n < 10)
    return 1;
  if (n < 100)
    return 2;
  if (n < 1000)
    return 3;
  if (n < 10000)
    return 4;
  if (n < 100000)
    return 5;
  if (n < 1000000)
    return 6;
  if (n < 10000000)
    return 7;
  if (n < 100000000)
    return 8;
  if (n < 1000000000)
    return 9;
  /*      2147483647 is 2^31-1 - add more ifs as needed
     and adjust this final return as well. */
  return 10;
}