#ifndef PPOSIX_FILE_FD_HPP
#define PPOSIX_FILE_FD_HPP

#include "pposix/capi/rawfd.hpp"

namespace pposix::file {

struct filefd : capi::rawfd {};

}  // namespace pposix::file

#endif  // PPOSIX_FILE_FD_HPP
