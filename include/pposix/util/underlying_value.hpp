#ifndef PPOSIX_UTIL_UNDERLYING_VALUE_HPP
#define PPOSIX_UTIL_UNDERLYING_VALUE_HPP

#include <type_traits>


namespace pposix::util {

    template<class Enum>
    auto underlying_value(const Enum e) {
        using enum_t = std::decay_t<Enum>;

        static_assert(std::is_enum_v<enum_t>);

        return static_cast<std::underlying_type_t<enum_t>>(e);
    }

}

#endif //PPOSIX_UTIL_UNDERLYING_VALUE_HPP
