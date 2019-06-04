#ifndef PPOSIX_RESOURCE_HPP
#define PPOSIX_RESOURCE_HPP

#include <memory>

#include <dirent.h>


namespace pposix {

    template <class T, class Deleter>
    class resource {
    public:
        constexpr resource() = default;
        constexpr resource(T *resource)

    private:
        std::unique_ptr<T> resource_;
    };

}


#endif //PPOSIX_RESOURCE_HPP
