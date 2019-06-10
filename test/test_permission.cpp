#include <catch2/catch.hpp>

#include "pposix/capi/file/permission.hpp"

using permission = pposix::capi::file::permission;

SCENARIO("Permission literals can be used in place of permission flags", "[pposix][permission]") {
  using namespace pposix::capi::file::literals::permission_literals;

  WHEN("using the 'rwx' user permission literal") {
    THEN("all permission bits are set for user and only user") {
      REQUIRE("rwx"_user == permission::user_all);

      REQUIRE("rwx"_user != permission::group_all);
      REQUIRE("rwx"_user != permission::other_all);
    }
  }

  WHEN("using the 'rwx' group permission literal") {
    THEN("all permission bits are set for group and only group") {
      REQUIRE("rwx"_group == permission::group_all);

      REQUIRE("rwx"_group != permission::user_all);
      REQUIRE("rwx"_group != permission::other_all);
    }
  }

  WHEN("using the 'rwx' other permission literal") {
    THEN("all permission bits are set for other and only other") {
      REQUIRE("rwx"_other == permission::other_all);

      REQUIRE("rwx"_other != permission::user_all);
      REQUIRE("rwx"_other != permission::group_all);
    }
  }

  WHEN("using the 'r' user permission literal") {
    THEN("the permission bit is set for user and only user") {
      REQUIRE("r"_user == permission::user_read);

      REQUIRE("r"_user != permission::group_read);
      REQUIRE("r"_user != permission::other_read);
    }
  }

  WHEN("using the 'r' group permission literal") {
    THEN("the permission bit is set for group and only group") {
      REQUIRE("r"_group == permission::group_read);

      REQUIRE("r"_group != permission::user_read);
      REQUIRE("r"_group != permission::other_read);
    }
  }

  WHEN("using the 'r' other permission literal") {
    THEN("the permission bit is set for other and only other") {
      REQUIRE("r"_other == permission::other_read);

      REQUIRE("r"_other != permission::user_read);
      REQUIRE("r"_other != permission::group_read);
    }
  }

  WHEN("using the 'w' user permission literal") {
    THEN("the permission bit is set for user and only user") {
      REQUIRE("w"_user == permission::user_write);

      REQUIRE("w"_user != permission::group_write);
      REQUIRE("w"_user != permission::other_write);
    }
  }

  WHEN("using the 'w' group permission literal") {
    THEN("the permission bit is set for group and only group") {
      REQUIRE("w"_group == permission::group_write);

      REQUIRE("w"_group != permission::user_write);
      REQUIRE("w"_group != permission::other_write);
    }
  }

  WHEN("using the 'w' other permission literal") {
    THEN("the permission bit is set for other and only other") {
      REQUIRE("w"_other == permission::other_write);

      REQUIRE("w"_other != permission::user_write);
      REQUIRE("w"_other != permission::group_write);
    }
  }

  WHEN("using the 'x' user permission literal") {
    THEN("the permission bit is set for user and only user") {
      REQUIRE("x"_user == permission::user_execute);

      REQUIRE("x"_user != permission::group_execute);
      REQUIRE("x"_user != permission::other_execute);
    }
  }

  WHEN("using the 'x' group permission literal") {
    THEN("the permission bit is set for group and only group") {
      REQUIRE("x"_group == permission::group_execute);

      REQUIRE("x"_group != permission::user_execute);
      REQUIRE("x"_group != permission::other_execute);
    }
  }

  WHEN("using the 'x' other permission literal") {
    THEN("the permission bit is set for other and only other") {
      REQUIRE("x"_other == permission::other_execute);

      REQUIRE("x"_other != permission::user_execute);
      REQUIRE("x"_other != permission::group_execute);
    }
  }

  WHEN("using an empty '' permission literal") {
    THEN("none of the permission bits are set") {
      REQUIRE(""_user == permission::none);
      REQUIRE(""_group == permission::none);
      REQUIRE(""_other == permission::none);
    }
  }

  WHEN("using invalid (not 'r', 'w' or 'x') permission literal characters") {
    THEN("none of the permission bits are set") {
      REQUIRE("RWX"_user == permission::none);
      REQUIRE("RWX"_group == permission::none);
      REQUIRE("RWX"_other == permission::none);

      REQUIRE(" "_user == permission::none);
      REQUIRE(" "_group == permission::none);
      REQUIRE(" "_other == permission::none);
    }
  }
}

SCENARIO("Permission literals set unique permission bits") {
  using namespace pposix::capi::file::literals::permission_literals;

  REQUIRE((("r"_user != "w"_user) && ("w"_user != "x"_user) && ("x"_user != "r"_user)));
  REQUIRE((("r"_group != "w"_group) && ("w"_group != "x"_group) && ("x"_group != "r"_group)));
  REQUIRE((("r"_other != "w"_other) && ("w"_other != "x"_other) && ("x"_other != "r"_other)));

  REQUIRE((("r"_user != "r"_group) && ("r"_group != "r"_other) && ("r"_other != "r"_user)));
  REQUIRE((("w"_user != "w"_group) && ("w"_group != "w"_other) && ("w"_other != "w"_user)));
  REQUIRE((("x"_user != "x"_group) && ("x"_group != "x"_other) && ("x"_other != "x"_user)));

  REQUIRE((("rwx"_user != "rwx"_group) && ("rwx"_group != "rwx"_other) &&
           ("rwx"_other != "rwx"_user)));
}

SCENARIO("Permission literals can be or'ed together", "[pposix][permission]") {
  using namespace pposix::capi::file::literals::permission_literals;

  REQUIRE(("r"_user | "w"_user | "x"_user) == permission::user_all);
  REQUIRE(("r"_group | "w"_group | "x"_group) == permission::group_all);
  REQUIRE(("r"_other | "w"_other | "x"_other) == permission::other_all);

  REQUIRE(("r"_user | "w"_user | "x"_user) ==
          (permission::user_read | permission::user_write | permission::user_execute));

  REQUIRE(("r"_group | "w"_group | "x"_group) ==
          (permission::group_read | permission::group_write | permission::group_execute));

  REQUIRE(("r"_other | "w"_other | "x"_other) ==
          (permission::other_read | permission::other_write | permission::other_execute));

  REQUIRE((""_user | ""_group | ""_other) == permission::none);
}
