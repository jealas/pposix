#pragma once

#include <string>
#include <system_error>

namespace pposix {

class errno_error : public std::system_error {
 public:
  explicit errno_error(std::string what) : errno_error(errno_code(), std::move(what)) {}

  errno_error(std::error_code ec, std::string what)
      : std::system_error{ec, what.c_str()}, what_{std::move(what)} {}

 private:
  std::string what_;
};

}  // namespace pposix
