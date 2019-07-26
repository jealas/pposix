#include <catch2/catch.hpp>

#include <functional>
#include <memory>

#include "pposix/unique_fd.hpp"
#include "pposix/fd.hpp"

struct nop_close {
  std::error_code operator()(pposix::raw_fd) const noexcept { return {}; }
};

template <class Close>
void require_is_empty(const pposix::unique_fd<pposix::raw_fd, Close> &fd) {
  REQUIRE(fd.empty());
  REQUIRE(not fd);
  REQUIRE(fd.raw() == pposix::nullfd);
}

template <class Close>
void require_is_not_empty(const pposix::unique_fd<pposix::raw_fd, Close> &fd) {
  REQUIRE(not fd.empty());
  REQUIRE(fd);
  REQUIRE(fd.raw() != pposix::nullfd);
}

SCENARIO("File descriptors can be constructed", "[pposix][unique_fd]") {
  GIVEN("a default constructed file descriptor") {
    pposix::unique_fd<pposix::raw_fd, nop_close> fd;

    THEN("the file descriptor should be empty") { require_is_empty(fd); }
  }

  GIVEN("a file descriptor constructed with a nullfd") {
    pposix::unique_fd<pposix::raw_fd, nop_close> fd{pposix::nullfd};

    THEN("then file descriptor should be empty") { require_is_empty(fd); }
  }

  GIVEN("a file descriptor constructed with rawfd null") {
    pposix::unique_fd<pposix::raw_fd, nop_close> fd{pposix::nullfd};

    THEN("the file descriptor should be empty") { require_is_empty(fd); }
  }
}

SCENARIO("File descriptors can be released", "[pposix][unique_fd]") {
  GIVEN("an empty file descriptor") {
    pposix::unique_fd<pposix::raw_fd, nop_close> fd;

    REQUIRE(fd.empty());
    REQUIRE(not fd);

    WHEN("released") {
      const auto released_rawfd = fd.release();

      THEN("the file descriptor remains empty") { require_is_empty(fd); }

      AND_THEN("the released raw file descriptor is null") {
        REQUIRE(released_rawfd == pposix::nullfd);
      }
    }
  }

  GIVEN("a non-empty file descriptor") {
    constexpr pposix::raw_fd VALID_FD{1};

    pposix::unique_fd<pposix::raw_fd, nop_close> fd{VALID_FD};

    require_is_not_empty(fd);

    WHEN("released") {
      const auto released_rawfd = fd.release();

      THEN("the file descriptor is emptied") { require_is_empty(fd); }

      AND_THEN(
          "the released raw file descriptor is the same as what was used to construct the file "
          "descriptor") {
        REQUIRE(released_rawfd == VALID_FD);
      }
    }
  }
}

class close_counter_policy {
 public:
  close_counter_policy() : close_count_{std::make_shared<unsigned>(0u)} {}

  std::error_code operator()(pposix::raw_fd rawfd) {
    ++(*close_count_);
    return {};
  }

  unsigned close_count() const noexcept { return *close_count_; }

 private:
  std::shared_ptr<unsigned> close_count_{};
};

SCENARIO("File descriptors can be closed", "[pposix][unique_fd]") {
  GIVEN("a null file descriptor") {
    auto fd = std::make_unique<pposix::unique_fd<pposix::raw_fd, close_counter_policy>>(
        pposix::nullfd);

    require_is_empty(*fd);

    auto close_policy = fd->get_close_policy();

    WHEN("destroyed") {
      REQUIRE(close_policy.close_count() == 0u);
      fd.reset();

      THEN("the file descriptor doesn't invoke the close policy") {
        REQUIRE(close_policy.close_count() == 0u);
      }
    }

    WHEN("manually closed") {
      REQUIRE(close_policy.close_count() == 0u);

      const auto close_error = fd->close();
      REQUIRE(not close_error);

      THEN("the close call does not invoke the close policy") {
        REQUIRE(close_policy.close_count() == 0u);
      }

      AND_WHEN("destroyed") {
        fd.reset();

        THEN("the destructor does not invoke the close policy again") {
          REQUIRE(close_policy.close_count() == 0u);
        }
      }
    }
  }

  GIVEN("a non-null file descriptor") {
    auto fd = std::make_unique<pposix::unique_fd<pposix::raw_fd, close_counter_policy>>(
        pposix::raw_fd{1});

    require_is_not_empty(*fd);

    auto close_policy = fd->get_close_policy();

    WHEN("destroyed") {
      REQUIRE(close_policy.close_count() == 0u);

      fd.reset();

      THEN("the file descriptor invokes the close policy") {
        REQUIRE(close_policy.close_count() == 1u);
      }
    }

    WHEN("manually closed") {
      REQUIRE(close_policy.close_count() == 0u);

      const auto close_error = fd->close();
      REQUIRE(not close_error);

      THEN("the file descriptor invokes the close policy") {
        REQUIRE(close_policy.close_count() == 1u);
      }

      AND_WHEN("destroyed") {
        REQUIRE(close_policy.close_count() == 1u);

        fd.reset();

        THEN("the destructor does not invoke the close policy again") {
          REQUIRE(close_policy.close_count() == 1u);
        }
      }
    }
  }
}

SCENARIO("File descriptors can be moved", "[pposix][unique_fd]") {
  GIVEN("a null file descriptor") {
    pposix::unique_fd<pposix::raw_fd, nop_close> null_fd{pposix::nullfd};

    REQUIRE(null_fd.empty());

    WHEN("moved from") {
      pposix::unique_fd<pposix::raw_fd, nop_close> new_fd{std::move(null_fd)};

      THEN("both file descriptors are empty") {
        REQUIRE(null_fd.empty());  // NOLINT use after move
        REQUIRE(new_fd.empty());
      }
    }
  }

  GIVEN("a non-null file descriptor") {
    constexpr pposix::raw_fd VALID_FD{1u};

    pposix::unique_fd<pposix::raw_fd, nop_close> old_fd{VALID_FD};

    REQUIRE(not old_fd.empty());

    WHEN("moved from") {
      pposix::unique_fd<pposix::raw_fd, nop_close> new_fd{std::move(old_fd)};

      THEN("the old file descriptor is emptied") {
        REQUIRE(old_fd.empty());  // NOLINT use after move
        REQUIRE(old_fd.raw() != VALID_FD);
        REQUIRE(old_fd.raw() == pposix::nullfd);

        AND_THEN("the new file descriptor contains the old file descriptor") {
          REQUIRE(not new_fd.empty());
          REQUIRE(new_fd.raw() == VALID_FD);
        }
      }
    }
  }
}
