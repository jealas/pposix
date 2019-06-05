#ifndef PPOSIX_FILE_OPEN_HPP
#define PPOSIX_FILE_OPEN_HPP

#include <filesystem>

#include <fcntl.h>

#include "pposix/result.hpp"
#include "pposix/errno_code.hpp"
#include "pposix/file/fd.hpp"
#include "pposix/file/rawfd.hpp"
#include "pposix/file/nullfd.hpp"
#include "pposix/file/mode.hpp"
#include "pposix/util/underlying_value.hpp"


namespace pposix::file {

    result<fd<>> open(const std::filesystem::path &path, const mode m, const flags f, const permission p) {
        char const * const native_path = path.native().c_str();
        const rawfd new_fd{::open(native_path, util::underlying_value(m) | util::underlying_value(f), util::underlying_value(p)};

        if (new_fd == nullfd) {
            return errno_code();
        }
        else {
            return fd{new_fd};
        }
    }

}

#endif //PPOSIX_FILE_OPEN_HPP
