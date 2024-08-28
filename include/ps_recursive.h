#include "utils.hpp"

#include <boost/preprocessor/arithmetic/add.hpp>

#ifndef PS_INCLUDE_LIMIT
#  define PS_INCLUDE_LIMIT 32768
#endif

#if __INCLUDE_LEVEL__ <= PS_INCLUDE_LIMIT / 256
// clang-format off
#  if __has_embed(STRINGIZE_VALUE_OF(/proc/__INCLUDE_LEVEL__/stat))
STRINGIZE_VALUE_OF(__INCLUDE_LEVEL__),
#  endif
// clang-format on
#  include "ps_recursive.h"
#endif