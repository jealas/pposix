#ifndef PPOSIX_FILE_OPEN_MODE_HPP
#define PPOSIX_FILE_OPEN_MODE_HPP

#include <fcntl.h>


namespace pposix::file {

    enum class mode : unsigned {
        read = O_RDONLY,
        write = O_WRONLY,
        read_write = O_RDWR
    };

}

#endif //PPOSIX_FILE_OPEN_MODE_HPP
