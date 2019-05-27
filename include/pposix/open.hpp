#ifndef PPOSIX_OPEN_HPP
#define PPOSIX_OPEN_HPP

#include <filesystem>

#include <fcntl.h>

#include "fd.hpp"
#include "result.hpp"
#include "errno_code.hpp"
#include "rawfd.hpp"
#include "nullfd.hpp"


namespace pposix {

    result<fd<>> open(const std::filesystem::path &path) {
        const auto * native_path = path.native().c_str();
        const rawfd new_fd{::open(native_path)};

        if (new_fd == nullfd) {
            return errno_code();
        }
        else {
            return fd{new_fd};
        }
    }

}

#endif //PPOSIX_OPEN_HPP
