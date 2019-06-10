#include <catch2/catch.hpp>

#include "pposix/capi/file/permission.hpp"

using perm = pposix::capi::file::permission;

SCENARIO("Permission flags can be or'ed together", "[pposix][capi][permission]") {
  WHEN("or'ing all user permission bits") {
    const auto all_user_permissions = perm::user_read | perm::user_write | perm::user_execute;

    THEN("the resulting value is equal to the 'all' user permission flag") {
      REQUIRE(perm::user_all == all_user_permissions);
    }
  }

  WHEN("or'ing all group permission bits") {
    const auto all_group_permissions = perm::group_read | perm::group_write | perm::group_execute;

    THEN("the resulting value is equal to the 'all' group permission flag") {
      REQUIRE(perm::group_all == all_group_permissions);
    }
  }

  WHEN("or'ing all other permission bits") {
    const auto all_other_permissions = perm::other_read | perm::other_write | perm::other_execute;

    THEN("the resulting value is equal to the 'all' other permission flag") {
      REQUIRE(perm::other_all == all_other_permissions);
    }
  }
}

SCENARIO("Permission flags represent distinct permission bits") {
  REQUIRE(((perm::user_read != perm::group_read) && (perm::group_read != perm::other_read)));
  REQUIRE(((perm::user_write != perm::group_write) && (perm::group_write != perm::other_write)));
  REQUIRE(((perm::user_execute != perm::group_execute) &&
           (perm::group_execute != perm::other_execute)));

  REQUIRE(((perm::user_read != perm::user_write) && (perm::user_write != perm::user_execute)));
  REQUIRE(((perm::group_read != perm::group_write) && (perm::group_write != perm::group_execute)));
  REQUIRE(((perm::other_read != perm::other_write) && (perm::other_write != perm::other_execute)));

  REQUIRE(((perm::user_all != perm::group_all) && (perm::group_all != perm::other_all)));
}