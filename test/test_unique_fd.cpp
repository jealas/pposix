#include "pposix_test.hpp"

#include <functional>
#include <memory>

#include "pposix/fd.hpp"
#include "pposix/unique_fd.hpp"

struct nop_close {
  std::error_code operator()(const pposix::raw_fd &) const noexcept { return {}; }
};

template <class Close>
void require_is_empty(const pposix::unique_fd<pposix::raw_fd, Close> &fd) {
  require(fd.empty());
  require(not fd);
  require(fd.raw() == pposix::nullfd);
}

template <class Close>
void require_is_not_empty(const pposix::unique_fd<pposix::raw_fd, Close> &fd) {
  require(not fd.empty());
  require(fd);
  require(fd.raw() != pposix::nullfd);
}

scenario("File descriptors can be constructed") {
  given("a default constructed file descriptor") {
    pposix::unique_fd<pposix::raw_fd, nop_close> fd;

    then("the file descriptor should be empty") { require_is_empty(fd); }
  }

  given("a file descriptor constructed with a nullfd") {
    pposix::unique_fd<pposix::raw_fd, nop_close> fd{pposix::nullfd};

    then("then file descriptor should be empty") { require_is_empty(fd); }
  }

  given("a file descriptor constructed with rawfd null") {
    pposix::unique_fd<pposix::raw_fd, nop_close> fd{pposix::nullfd};

    then("the file descriptor should be empty") { require_is_empty(fd); }
  }
}

scenario("File descriptors can be released") {
  given("an empty file descriptor") {
    pposix::unique_fd<pposix::raw_fd, nop_close> fd;

    require(fd.empty());
    require(not fd);

    when("released") {
      const auto released_rawfd = fd.release();

      then("the file descriptor remains empty") { require_is_empty(fd); }

      and_then("the released raw file descriptor is null") {
        require(released_rawfd == pposix::nullfd);
      }
    }
  }

  given("a non-empty file descriptor") {
    pposix::unique_fd<pposix::raw_fd, nop_close> fd{pposix::raw_fd{1}};

    require_is_not_empty(fd);

    when("released") {
      const auto released_rawfd = fd.release();

      then("the file descriptor is emptied") { require_is_empty(fd); }

      and_then(
          "the released raw file descriptor is the same as what was used to construct the file "
          "descriptor") {
        require(released_rawfd == pposix::raw_fd{1});
      }
    }
  }
}

class close_counter_policy {
 public:
  close_counter_policy() : close_count_{std::make_shared<unsigned>(0u)} {}

  std::error_code operator()(const pposix::raw_fd &rawfd) {
    ++(*close_count_);
    return {};
  }

  unsigned close_count() const noexcept { return *close_count_; }

 private:
  std::shared_ptr<unsigned> close_count_{};
};

scenario("File descriptors can be closed") {
  given("a null file descriptor") {
    auto fd =
        std::make_unique<pposix::unique_fd<pposix::raw_fd, close_counter_policy>>(pposix::nullfd);

    require_is_empty(*fd);

    auto close_policy = fd->get_close_policy();

    when("destroyed") {
      require(close_policy.close_count() == 0u);
      fd.reset();

      then("the file descriptor doesn't invoke the close policy") {
        require(close_policy.close_count() == 0u);
      }
    }

    when("manually closed") {
      require(close_policy.close_count() == 0u);

      const auto close_error = fd->close();
      require(not close_error);

      then("the close call does not invoke the close policy") {
        require(close_policy.close_count() == 0u);
      }

      and_when("destroyed") {
        fd.reset();

        then("the destructor does not invoke the close policy again") {
          require(close_policy.close_count() == 0u);
        }
      }
    }
  }

  given("a non-null file descriptor") {
    auto fd = std::make_unique<pposix::unique_fd<pposix::raw_fd, close_counter_policy>>(
        pposix::raw_fd{1});

    require_is_not_empty(*fd);

    auto close_policy = fd->get_close_policy();

    when("destroyed") {
      require(close_policy.close_count() == 0u);

      fd.reset();

      then("the file descriptor invokes the close policy") {
        require(close_policy.close_count() == 1u);
      }
    }

    when("manually closed") {
      require(close_policy.close_count() == 0u);

      const auto close_error = fd->close();
      require(not close_error);

      then("the file descriptor invokes the close policy") {
        require(close_policy.close_count() == 1u);
      }

      and_when("destroyed") {
        require(close_policy.close_count() == 1u);

        fd.reset();

        then("the destructor does not invoke the close policy again") {
          require(close_policy.close_count() == 1u);
        }
      }
    }
  }
}

scenario("File descriptors can be moved") {
  given("a null file descriptor") {
    pposix::unique_fd<pposix::raw_fd, nop_close> null_fd{pposix::nullfd};

    require(null_fd.empty());

    when("moved from") {
      pposix::unique_fd<pposix::raw_fd, nop_close> new_fd{std::move(null_fd)};

      then("both file descriptors are empty") {
        require(null_fd.empty());  // NOLINT use after move
        require(new_fd.empty());
      }
    }
  }

  given("a non-null file descriptor") {
    pposix::unique_fd<pposix::raw_fd, nop_close> old_fd{pposix::raw_fd{1}};

    require(not old_fd.empty());

    when("moved from") {
      pposix::unique_fd<pposix::raw_fd, nop_close> new_fd{std::move(old_fd)};

      then("the old file descriptor is emptied") {
        require(old_fd.empty());  // NOLINT use after move
        require(old_fd.raw() != pposix::raw_fd{1});
        require(old_fd.raw() == pposix::nullfd);

        and_then("the new file descriptor contains the old file descriptor") {
          require(not new_fd.empty());
          require(new_fd.raw() == pposix::raw_fd{1});
        }
      }
    }
  }
}
