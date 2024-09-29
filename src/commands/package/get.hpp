#pragma once

#include "commands/package/package.hpp"

namespace commands {
    namespace package {
        utils::fn_attempt get(const str& git_repo);
    }
}