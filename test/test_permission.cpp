#include <catch2/catch.hpp>

#include "pposix/file.hpp"

using namespace pposix;
using namespace pposix::permission_literals;

SCENARIO("Permission literals can be used in place of permission flags", "[pposix][permission]") {
  WHEN("using the 'rwx' user permission literal") {
    THEN("all permission bits are set for user and only user") {
      REQUIRE("rwx"_user == file_permission::user_all);

      REQUIRE("rwx"_user != file_permission::group_all);
      REQUIRE("rwx"_user != file_permission::other_all);
    }
  }

  WHEN("using the 'rwx' group permission literal") {
    THEN("all permission bits are set for group and only group") {
      REQUIRE("rwx"_group == file_permission::group_all);

      REQUIRE("rwx"_group != file_permission::user_all);
      REQUIRE("rwx"_group != file_permission::other_all);
    }
  }

  WHEN("using the 'rwx' other permission literal") {
    THEN("all permission bits are set for other and only other") {
      REQUIRE("rwx"_other == file_permission::other_all);

      REQUIRE("rwx"_other != file_permission::user_all);
      REQUIRE("rwx"_other != file_permission::group_all);
    }
  }

  WHEN("using the 'r' user permission literal") {
    THEN("the permission bit is set for user and only user") {
      REQUIRE("r"_user == file_permission::user_read);

      REQUIRE("r"_user != file_permission::group_read);
      REQUIRE("r"_user != file_permission::other_read);
    }
  }

  WHEN("using the 'r' group permission literal") {
    THEN("the permission bit is set for group and only group") {
      REQUIRE("r"_group == file_permission::group_read);

      REQUIRE("r"_group != file_permission::user_read);
      REQUIRE("r"_group != file_permission::other_read);
    }
  }

  WHEN("using the 'r' other permission literal") {
    THEN("the permission bit is set for other and only other") {
      REQUIRE("r"_other == file_permission::other_read);

      REQUIRE("r"_other != file_permission::user_read);
      REQUIRE("r"_other != file_permission::group_read);
    }
  }

  WHEN("using the 'w' user permission literal") {
    THEN("the permission bit is set for user and only user") {
      REQUIRE("w"_user == file_permission::user_write);

      REQUIRE("w"_user != file_permission::group_write);
      REQUIRE("w"_user != file_permission::other_write);
    }
  }

  WHEN("using the 'w' group permission literal") {
    THEN("the permission bit is set for group and only group") {
      REQUIRE("w"_group == file_permission::group_write);

      REQUIRE("w"_group != file_permission::user_write);
      REQUIRE("w"_group != file_permission::other_write);
    }
  }

  WHEN("using the 'w' other permission literal") {
    THEN("the permission bit is set for other and only other") {
      REQUIRE("w"_other == file_permission::other_write);

      REQUIRE("w"_other != file_permission::user_write);
      REQUIRE("w"_other != file_permission::group_write);
    }
  }

  WHEN("using the 'x' user permission literal") {
    THEN("the permission bit is set for user and only user") {
      REQUIRE("x"_user == file_permission::user_execute);

      REQUIRE("x"_user != file_permission::group_execute);
      REQUIRE("x"_user != file_permission::other_execute);
    }
  }

  WHEN("using the 'x' group permission literal") {
    THEN("the permission bit is set for group and only group") {
      REQUIRE("x"_group == file_permission::group_execute);

      REQUIRE("x"_group != file_permission::user_execute);
      REQUIRE("x"_group != file_permission::other_execute);
    }
  }

  WHEN("using the 'x' other permission literal") {
    THEN("the permission bit is set for other and only other") {
      REQUIRE("x"_other == file_permission::other_execute);

      REQUIRE("x"_other != file_permission::user_execute);
      REQUIRE("x"_other != file_permission::group_execute);
    }
  }

  WHEN("using an empty '' permission literal") {
    THEN("none of the permission bits are set") {
      REQUIRE(""_user == file_permission::none);
      REQUIRE(""_group == file_permission::none);
      REQUIRE(""_other == file_permission::none);
    }
  }

  WHEN("using invalid (not 'r', 'w' or 'x') permission literal characters") {
    THEN("none of the permission bits are set") {
      REQUIRE("RWX"_user == file_permission::none);
      REQUIRE("RWX"_group == file_permission::none);
      REQUIRE("RWX"_other == file_permission::none);

      REQUIRE(" "_user == file_permission::none);
      REQUIRE(" "_group == file_permission::none);
      REQUIRE(" "_other == file_permission::none);
    }
  }
}

SCENARIO("Permission literals set unique permission bits") {
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
  REQUIRE(("r"_user | "w"_user | "x"_user) == file_permission::user_all);
  REQUIRE(("r"_group | "w"_group | "x"_group) == file_permission::group_all);
  REQUIRE(("r"_other | "w"_other | "x"_other) == file_permission::other_all);

  REQUIRE(
      ("r"_user | "w"_user | "x"_user) ==
      (file_permission::user_read | file_permission::user_write | file_permission::user_execute));

  REQUIRE(("r"_group | "w"_group | "x"_group) ==
          (file_permission::group_read | file_permission::group_write |
           file_permission::group_execute));

  REQUIRE(("r"_other | "w"_other | "x"_other) ==
          (file_permission::other_read | file_permission::other_write |
           file_permission::other_execute));

  REQUIRE((""_user | ""_group | ""_other) == file_permission::none);
}

SCENARIO("Permission flags can be or'ed together", "[pposix][permission]") {
  WHEN("or'ing all user permission bits") {
    const auto all_user_permissions =
        file_permission::user_read | file_permission::user_write | file_permission::user_execute;

    THEN("the resulting value is equal to the 'all' user permission flag") {
      REQUIRE(file_permission::user_all == all_user_permissions);
    }
  }

  WHEN("or'ing all group permission bits") {
    const auto all_group_permissions = file_permission::group_read | file_permission::group_write |
                                       file_permission::group_execute;

    THEN("the resulting value is equal to the 'all' group permission flag") {
      REQUIRE(file_permission::group_all == all_group_permissions);
    }
  }

  WHEN("or'ing all other permission bits") {
    const auto all_other_permissions = file_permission::other_read | file_permission::other_write |
                                       file_permission::other_execute;

    THEN("the resulting value is equal to the 'all' other permission flag") {
      REQUIRE(file_permission::other_all == all_other_permissions);
    }
  }
}

SCENARIO("Permission flags represent distinct permission bits") {
  REQUIRE(((file_permission::user_read != file_permission::group_read) &&
           (file_permission::group_read != file_permission::other_read)));
  REQUIRE(((file_permission::user_write != file_permission::group_write) &&
           (file_permission::group_write != file_permission::other_write)));
  REQUIRE(((file_permission::user_execute != file_permission::group_execute) &&
           (file_permission::group_execute != file_permission::other_execute)));

  REQUIRE(((file_permission::user_read != file_permission::user_write) &&
           (file_permission::user_write != file_permission::user_execute)));
  REQUIRE(((file_permission::group_read != file_permission::group_write) &&
           (file_permission::group_write != file_permission::group_execute)));
  REQUIRE(((file_permission::other_read != file_permission::other_write) &&
           (file_permission::other_write != file_permission::other_execute)));

  REQUIRE(((file_permission::user_all != file_permission::group_all) &&
           (file_permission::group_all != file_permission::other_all)));
}
