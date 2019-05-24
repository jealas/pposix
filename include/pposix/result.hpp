#ifndef PPOSIX_RESULT_HPP
#define PPOSIX_RESULT_HPP

#include <system_error>
#include <variant>


namespace pposix {

    template<class T>
    class result {

        struct error_visitor {
            std::error_code operator()(const std::error_code &ec) const noexcept {
                return ec;
            }

            std::error_code operator()(const T &) const noexcept {
                return {};
            }
        };

    public:
        /*implicit*/ result(const std::error_code ec) : result_{ec} {}

        constexpr /*implicit*/ result(const T &value) : result_{value} {}

        constexpr /*implicit*/ result(T &&value) : result_{std::move(value)} {}

        std::error_code error() const noexcept {
            return std::visit(error_visitor{}, *this);
        }

        T *value() noexcept {
            return std::get_if<T>(&result_);
        }

        T const *value() const noexcept {
            return std::get_if<T>(&result_);
        }

    private:
        std::variant<T, std::error_code> result_;
    };

}

#endif //PPOSIX_RESULT_HPP
