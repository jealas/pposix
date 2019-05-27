#ifndef PPOSIX_FILE_OPEN_FLAG_HPP
#define PPOSIX_FILE_OPEN_FLAG_HPP

#include <fcntl.h>


namespace pposix::file {

    enum class flags : unsigned {
        append = O_APPEND,
        create = O_CREAT,
        dsync = O_DSYNC,
        excl = O_EXCL,
        noctty = O_NOCTTY,
        nonblock = O_NONBLOCK
        rsync = O_RSYNC,
        sync = O_SYNC,
        truncate = O_TRUNC,
    };

}

#endif //PPOSIX_FILE_OPEN_FLAG_HPP
