#pragma once

// Escape sequences within static_assert errors
#ifdef __clang__
#  define SUPPORTS_ESCAPES 0
#elif defined(__GNUG__)
#  define SUPPORTS_ESCAPES 1
#endif
// TODO: MSVC?

#if __has_builtin(__builtin_embed)
#  define SUPPORTS_EMBED 1
#else
#  define SUPPORTS_EMBED 0
#endif
// TODO: get a better check for pp_embed

// #if !SUPPORTS_EMBED
// #warning Compiler does not support #embed.
// #endif

// #if !SUPPORTS_ESCAPES
// #warning Compiler does not support escape sequences.
// #endif