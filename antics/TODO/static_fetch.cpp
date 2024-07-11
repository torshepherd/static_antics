#include "../../external/fmt-master/include/fmt/compile.h"

#include <cstddef>
#include <string>
#include <string_view>

// TODO: Needs embedding procfiles to actually give interesting info like memory, and it needs gcc to give colors support

// /etc/hostname:
//
// DESKTOP-0M124BO

#if __has_embed("/etc/hostname")
static constexpr const char raw_hostname[] = {
#  embed "/etc/hostname" suffix(, )
    0};
static constexpr std::string_view hostname{raw_hostname};
#else
static constexpr std::string_view hostname = "(unknown hostname)";
#endif

// /etc/lsb-release:
//
// DISTRIB_ID=Ubuntu
// DISTRIB_RELEASE=22.04
// DISTRIB_CODENAME=jammy
// DISTRIB_DESCRIPTION="Ubuntu 22.04.3 LTS"

#if __has_embed("/etc/lsb-release")
static constexpr const char raw_lsb_release[] = {
#  embed "/etc/lsb-release" suffix(, )
    0};
static constexpr std::string_view lsb_release{raw_lsb_release};
static constexpr std::string_view last_line_of_lsb_release =
    lsb_release.substr(lsb_release.find("DISTRIB_DESCRIPTION"));
static constexpr std::string_view distrib_description =
    last_line_of_lsb_release.substr(
        last_line_of_lsb_release.find('\"') + 1,
        last_line_of_lsb_release.find_last_of('\"') -
            last_line_of_lsb_release.find('\"') - 1);
#else
static constexpr std::string_view distrib_description =
    "(unknown distribution)";
#endif

static constexpr std::string_view os_class =
#ifdef _WIN64
    "Windows 64-bit"
#elif defined(_WIN32)
    "Windows 32-bit"
#elif defined(__APPLE__)
    "Apple"
#elif defined(__MACH__)
    "Apple"
#elif defined(__ANDROID__)
    "Android"
#elif defined(__linux__)
    "Linux"
#elif defined(__sun)
    "Solaris"
#elif defined(__hpux)
    "HP UX"
#elif defined(__DragonFly__)
    "DragonFly BSD"
#elif defined(__FreeBSD__)
    "FreeBSD"
#elif defined(__NetBSD__)
    "NetBSD"
#elif defined(__OpenBSD__)
    "OpenBSD"
#elif defined(BSD)
    "BSD"
#elif defined(_POSIX_VERSION)
    "POSIX-based"
#elif defined(__unix__)
    "Unix-based OS"
#endif
    ;

// /etc/subuid:
//
// tor:XXX:XXX

#if __has_embed("/etc/subuid")
static constexpr const char raw_subuid[] = {
#  embed "/etc/subuid" suffix(, )
    0};
static constexpr std::string_view subuid{raw_subuid};
static constexpr std::string_view username = subuid.substr(0, subuid.find(':'));
#else
static constexpr std::string_view username = "(unknown user)";
#endif

// /etc/passwd
//
// tor:x:1000:1000:,,,:/home/tor:/usr/bin/zsh
// USERNAME:PASS:UID:GID:UID INFO:HOME:SHELL

#if __has_embed("/etc/passwd")
static constexpr const char raw_passwd[] = {
#  embed "/etc/passwd" suffix(, )
    0};
static constexpr std::string_view passwd{raw_passwd};
static constexpr auto passwd_offset_for_subuid = passwd.find(username);
static constexpr std::string_view passwd_entry_for_subuid =
    passwd_offset_for_subuid == std::string_view::npos
        ? ""
        : passwd.substr(passwd_offset_for_subuid,
                        passwd.find('\n', passwd_offset_for_subuid) -
                            passwd_offset_for_subuid);
static constexpr std::string_view subuid_shell =
    passwd_entry_for_subuid == ""
        ? "(unknown shell)"
        : passwd_entry_for_subuid.substr(
              passwd_entry_for_subuid.find_last_of(':') + 1);
#else
static constexpr std::string_view subuid_shell = "(unknown shell)";
#endif

constexpr bool done = false;
static_assert(
    // clang-format off
done
    // clang-format on
    ,
    [] -> std::string {
      char buffer[4096]{};
      auto running = buffer;
      running = fmt::format_to(running, FMT_COMPILE("\n\n{}@{}\n"), username,
                               hostname);
      const ptrdiff_t first_line_length = running - buffer;
      running = fmt::format_to(running, FMT_COMPILE("{0:-^{1}}\n"), "",
                               first_line_length - 3 /* The newlines */);
      running = fmt::format_to(running, FMT_COMPILE("OS: {} [{}]\n"),
                               distrib_description, os_class);
      running =
          fmt::format_to(running, FMT_COMPILE("Shell: {}\n\n"), subuid_shell);
      return buffer;
    }());