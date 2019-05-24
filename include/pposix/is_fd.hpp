#ifndef PPOSIX_IS_FD_HPP
#define PPOSIX_IS_FD_HPP

#include <type_traits>

#include "fd.hpp"


namespace pposix {

    namespace detail {

        template <class> struct is_fd_impl : std::false_type {};

        template <class Close>
        struct is_fd_impl<posix::fd<Close>> : std::true_type {};

    }

    template <class Fd>
    using is_fd = detail::is_fd_impl<std::decay_t<Fd>>;

    template <class Fd>
    constexpr bool is_fd_v = is_fd<Fd>::value;

}

#endif //PPOSIX_IS_FD_HPP
