#include <catch2/catch.hpp>

#include "pposix/file/permission.hpp"

using permission = pposix::file::permission;

SCENARIO("Permission literals return the same result as or-ing the permission flags manually",
         "[pposix][permission]") {
  REQUIRE(permission::user_all ==
          (permission::user_read | permission::user_write | permission::user_execute));

  REQUIRE(permission::group_all ==
          (permission::group_read | permission::group_write | permission::group_execute));

  REQUIRE(permission::other_all ==
          (permission::other_read | permission::other_write | permission::other_execute));

  using namespace pposix::file::literals::permission_literals;

  REQUIRE("rwx"_user == permission::user_all);
  REQUIRE("rwx"_group == permission::group_all);
  REQUIRE("rwx"_other == permission::other_all);

  REQUIRE("r"_user == permission::user_read);
  REQUIRE("r"_group == permission::group_read);
  REQUIRE("r"_other == permission::other_read);

  REQUIRE("w"_user == permission::user_write);
  REQUIRE("w"_group == permission::group_write);
  REQUIRE("w"_other == permission::other_write);

  REQUIRE("x"_user == permission::user_execute);
  REQUIRE("x"_group == permission::group_execute);
  REQUIRE("x"_other == permission::other_execute);

  REQUIRE(("r"_user | "w"_user | "x"_user) == permission::user_all);
  REQUIRE(("r"_group | "w"_group | "x"_group) == permission::group_all);
  REQUIRE(("r"_other | "w"_other | "x"_other) == permission::other_all);

  REQUIRE(("r"_user | "w"_user | "x"_user) ==
          (permission::user_read | permission::user_write | permission::user_execute));

  REQUIRE(("r"_group | "w"_group | "x"_group) ==
          (permission::group_read | permission::group_write | permission::group_execute));

  REQUIRE(("r"_other | "w"_other | "x"_other) ==
          (permission::other_read | permission::other_write | permission::other_execute));

  REQUIRE(""_user == permission::none);
  REQUIRE(""_group == permission::none);
  REQUIRE(""_other == permission::none);

  REQUIRE((("rwx"_user != "rwx"_group) && ("rwx"_group != "rwx"_other) &&
          ("rwx"_other != "rwx"_user)));
}
