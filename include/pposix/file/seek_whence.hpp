#ifndef PPOSIX_FILE_SEEK_WHENCE_HPP
#define PPOSIX_FILE_SEEK_WHENCE_HPP

#include <unistd.h>


namespace pposix::file {

    enum class seek_whence {
        set = SEEK_SET,
        current = SEEK_CUR,
        end = SEEK_END
    };

}

#endif //PPOSIX_FILE_SEEK_WHENCE_HPP
