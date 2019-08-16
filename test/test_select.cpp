#include "pposix_test.hpp"

#include <initializer_list>

#include "pposix/fd.hpp"
#include "pposix/select.hpp"

template <class Function>
void for_each_select_fd(Function function) {
  for (int i = 0; i < pposix::fd_set::max_fd(); ++i) {
    function(pposix::raw_fd{i});
  }
}

test_case("File descriptor set") {
  given("a default constructed file descriptor") {
    pposix::fd_set fds{};

    then("no file descriptors are set") {
      for_each_select_fd([&](pposix::raw_fd fd) { require(not fds.is_set(fd)); });
    }
  }

  given("a completely full file descriptor set") {
    pposix::fd_set fds{};

    for_each_select_fd([&](pposix::raw_fd fd) { fds.set(fd); });

    then("all file descriptors are set") {
      for_each_select_fd([&](pposix::raw_fd fd) { require(fds.is_set(fd)); });
    }

    when("calling clear") {
      fds.clear();

      then("all file descriptors are unset") {
        for_each_select_fd([&](pposix::raw_fd fd) { require(not fds.is_set(fd)); });
      }
    }

    when("unsetting one file descriptor") {
      int fd;
      std::initializer_list<int> all_fds{0, 1, 2, 3, pposix::fd_set::max_fd()};

      generate(fd, all_fds);

      const pposix::raw_fd unset_fd{fd};

      fds.unset(unset_fd);

      then("the unset file descriptor is not set") { require(not fds.is_set(unset_fd)); }
      then("all the other file descriptors are set") {
        for_each_select_fd([&](pposix::raw_fd fd) {
          if (fd == unset_fd) {
            return;
          } else {
            require(fds.is_set(fd));
          }
        });
      }
    }
  }
}
