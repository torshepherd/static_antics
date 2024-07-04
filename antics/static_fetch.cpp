#include "../external/ctre.hpp"

#include <string_view>

// TODO: Needs embedding procfiles to actually give interesting info

// /etc/hostname:
//
// DESKTOP-0M124BO

// /etc/lsb-release:
//
// DISTRIB_ID=Ubuntu
// DISTRIB_RELEASE=22.04
// DISTRIB_CODENAME=jammy
// DISTRIB_DESCRIPTION="Ubuntu 22.04.3 LTS"

static constexpr const char raw_contents[] = {
#embed "/sys/fs/9p/caches" limit(20)
};
static constexpr std::string_view contents{raw_contents,
                                           raw_contents + sizeof(raw_contents)};

static_assert(false, contents);