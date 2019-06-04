#include <catch2/catch.hpp>

#include <functional>
#include <memory>

#include <pposix/file/fd.hpp>

struct nop_close {
  std::error_code operator()(pposix::rawfd) const noexcept { return {}; }
};

template <class Close>
void require_is_empty(const pposix::unique_fd<Close> &fd) {
  REQUIRE(fd.empty());
  REQUIRE(not fd);
  REQUIRE(fd.raw() == pposix::rawfd::null);
}

template <class Close>
void require_is_not_empty(const pposix::unique_fd<Close> &fd) {
  REQUIRE(not fd.empty());
  REQUIRE(fd);
  REQUIRE(fd.raw() != pposix::rawfd::null);
}

SCENARIO("File descriptors can be constructed", "[pposix][unique_fd]") {
  GIVEN("a default constructed file descriptor") {
    pposix::unique_fd<nop_close> fd;

    THEN("the file descriptor should be empty") { require_is_empty(fd); }
  }

  GIVEN("a file descriptor constructed with a nullfd") {
    pposix::unique_fd<nop_close> fd{pposix::nullfd};

    THEN("then file descriptor should be empty") { require_is_empty(fd); }
  }

  GIVEN("a file descriptor constructed with rawfd null") {
    pposix::unique_fd<nop_close> fd{pposix::rawfd::null};

    THEN("the file descriptor should be empty") { require_is_empty(fd); }
  }
}

SCENARIO("File descriptors can be released", "[pposix][unique_fd]") {
  GIVEN("an empty file descriptor") {
    pposix::unique_fd<nop_close> fd;

    REQUIRE(fd.empty());
    REQUIRE(not fd);

    WHEN("released") {
      const auto released_rawfd = fd.release();

      THEN("the file descriptor remains empty") { require_is_empty(fd); }

      AND_THEN("the released raw file descriptor is null") {
        REQUIRE(released_rawfd == pposix::rawfd::null);
      }
    }
  }

  GIVEN("a non-empty file descriptor") {
    constexpr pposix::rawfd VALID_FD{1};

    pposix::unique_fd<nop_close> fd{VALID_FD};

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

  std::error_code operator()(pposix::rawfd rawfd) {
    ++(*close_count_);
    return {};
  }

  unsigned close_count() const noexcept { return *close_count_; }

 private:
  std::shared_ptr<unsigned> close_count_{};
};

SCENARIO("File descriptors can be closed", "[pposix][unique_fd]") {
  GIVEN("a null file descriptor") {
    auto fd = std::make_unique<pposix::unique_fd<close_counter_policy>>(pposix::rawfd::null);

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
    auto fd = std::make_unique<pposix::unique_fd<close_counter_policy>>(pposix::rawfd{1});

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
