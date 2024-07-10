# static_antics

Fun with "P2741: user-generated static_assert messages"

Installation:

- Requires either clang >= 19 or GCC >= 14
- Download dependencies using [external/download.sh](external/download.sh).

## Why is P2741 special

P2741 adds a fantastic new feature to C++. Instead of being limited to `static_assert`s of the form

```c++
static_assert(false, "This has to be an _unevaluated_ literal");
```

we can now get away with silliness such as

```c++
static_assert(false, fmt::format("This can be {}", "whatever I want!"));
```

This opens the door to using `static_assert` messages as procedural output, which in turn means we can implement programs that take user input, do some computation, and then print out output to stderr from static_assert formatted output.

### [Wordle](antics/static_wordle.cpp)

Usage:
```
clang static_wordle.cpp -std=c++2c -Wno-everything -fconstexpr-steps=10000000 -DGUESS="CRANE HOUSE ETC"
```

## #embed

With the advent of C23's `#embed` feature, we can now take user input at compile-time not just from preprocessor defines, but also from file input! That opens the door to a bunch more programs we can implement:

### [cat](antics/static_cat.cpp)

Usage:
```
clang static_cat.cpp -std=c++2c -Wno-everything -fconstexpr-steps=10000000 -DCAT_FILE="path/to/file"
```

### [grep](antics/static_grep.cpp)

Usage:
```
clang static_grep.cpp -std=c++2c -Wno-everything -fconstexpr-steps=10000000 -DGREP_FILE="path/to/file" -DGREP_PATTERN="regex.*to.*match"
```

### [llama](antics/static_llama.cpp)

Usage:
```
clang static_llama.cpp -std=c++2c -Wno-everything -fconstexpr-steps=10000000 -DPROMPT="Once upon a time, there was a C compiler named clang"
```

Yes, we can actually perform LLM inference inside of a single `static_assert(false)` now.

## Pretty colors

Out of curiosity, I tried playing with some [ANSI escape codes](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797) in the static_assert output. Unfortunately, I was thwarted by clang, who smartly recognized that it would be too fun if I could use those as well.

But GCC on the other hand...

TODO: Insert image

So... we can now do a few more completely wack things. If you compile [Wordle](antics/static_wordle.cpp) using GCC14, you'll get pixel-perfect output to the official game:

TODO: Insert image

If you compile [cat](antics/static_cat.cpp) using GCC14 on an image, you'll get the image printed out using Kitty image protocol:

TODO: Insert image

If you compile [grep](antics/static_grep.cpp) using GCC14, you'll get colored text for matches:

TODO: Insert image
