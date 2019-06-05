#ifndef PPOSIX_FILE_FD_HPP
#define PPOSIX_FILE_FD_HPP

#include "pposix/unique_fd.hpp"


namespace pposix::file {

    struct fd : pposix::unique_fd<fd> {};

}

#endif //PPOSIX_FILE_FD_HPP
