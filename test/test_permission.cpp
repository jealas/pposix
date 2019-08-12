#include "pposix_test.hpp"

#include "pposix/file.hpp"

using namespace pposix;
using namespace pposix::permission_literals;

scenario("Permission literals can be used in place of permission flags") {
  when("using the 'rwx' user permission literal") {
    then("all permission bits are set for user and only user") {
      require("rwx"_user == file_permission::user_all);

      require("rwx"_user != file_permission::group_all);
      require("rwx"_user != file_permission::other_all);
    }
  }

  when("using the 'rwx' group permission literal") {
    then("all permission bits are set for group and only group") {
      require("rwx"_group == file_permission::group_all);

      require("rwx"_group != file_permission::user_all);
      require("rwx"_group != file_permission::other_all);
    }
  }

  when("using the 'rwx' other permission literal") {
    then("all permission bits are set for other and only other") {
      require("rwx"_other == file_permission::other_all);

      require("rwx"_other != file_permission::user_all);
      require("rwx"_other != file_permission::group_all);
    }
  }

  when("using the 'r' user permission literal") {
    then("the permission bit is set for user and only user") {
      require("r"_user == file_permission::user_read);

      require("r"_user != file_permission::group_read);
      require("r"_user != file_permission::other_read);
    }
  }

  when("using the 'r' group permission literal") {
    then("the permission bit is set for group and only group") {
      require("r"_group == file_permission::group_read);

      require("r"_group != file_permission::user_read);
      require("r"_group != file_permission::other_read);
    }
  }

  when("using the 'r' other permission literal") {
    then("the permission bit is set for other and only other") {
      require("r"_other == file_permission::other_read);

      require("r"_other != file_permission::user_read);
      require("r"_other != file_permission::group_read);
    }
  }

  when("using the 'w' user permission literal") {
    then("the permission bit is set for user and only user") {
      require("w"_user == file_permission::user_write);

      require("w"_user != file_permission::group_write);
      require("w"_user != file_permission::other_write);
    }
  }

  when("using the 'w' group permission literal") {
    then("the permission bit is set for group and only group") {
      require("w"_group == file_permission::group_write);

      require("w"_group != file_permission::user_write);
      require("w"_group != file_permission::other_write);
    }
  }

  when("using the 'w' other permission literal") {
    then("the permission bit is set for other and only other") {
      require("w"_other == file_permission::other_write);

      require("w"_other != file_permission::user_write);
      require("w"_other != file_permission::group_write);
    }
  }

  when("using the 'x' user permission literal") {
    then("the permission bit is set for user and only user") {
      require("x"_user == file_permission::user_execute);

      require("x"_user != file_permission::group_execute);
      require("x"_user != file_permission::other_execute);
    }
  }

  when("using the 'x' group permission literal") {
    then("the permission bit is set for group and only group") {
      require("x"_group == file_permission::group_execute);

      require("x"_group != file_permission::user_execute);
      require("x"_group != file_permission::other_execute);
    }
  }

  when("using the 'x' other permission literal") {
    then("the permission bit is set for other and only other") {
      require("x"_other == file_permission::other_execute);

      require("x"_other != file_permission::user_execute);
      require("x"_other != file_permission::group_execute);
    }
  }

  when("using an empty '' permission literal") {
    then("none of the permission bits are set") {
      require(""_user == file_permission::none);
      require(""_group == file_permission::none);
      require(""_other == file_permission::none);
    }
  }

  when("using invalid (not 'r', 'w' or 'x') permission literal characters") {
    then("none of the permission bits are set") {
      require("RWX"_user == file_permission::none);
      require("RWX"_group == file_permission::none);
      require("RWX"_other == file_permission::none);

      require(" "_user == file_permission::none);
      require(" "_group == file_permission::none);
      require(" "_other == file_permission::none);
    }
  }
}

scenario("Permission literals set unique permission bits") {
  require((("r"_user != "w"_user) && ("w"_user != "x"_user) && ("x"_user != "r"_user)));
  require((("r"_group != "w"_group) && ("w"_group != "x"_group) && ("x"_group != "r"_group)));
  require((("r"_other != "w"_other) && ("w"_other != "x"_other) && ("x"_other != "r"_other)));

  require((("r"_user != "r"_group) && ("r"_group != "r"_other) && ("r"_other != "r"_user)));
  require((("w"_user != "w"_group) && ("w"_group != "w"_other) && ("w"_other != "w"_user)));
  require((("x"_user != "x"_group) && ("x"_group != "x"_other) && ("x"_other != "x"_user)));

  require((("rwx"_user != "rwx"_group) && ("rwx"_group != "rwx"_other) &&
         ("rwx"_other != "rwx"_user)));
}

scenario("Permission literals can be or'ed together") {
  require(("r"_user | "w"_user | "x"_user) == file_permission::user_all);
  require(("r"_group | "w"_group | "x"_group) == file_permission::group_all);
  require(("r"_other | "w"_other | "x"_other) == file_permission::other_all);

  require(
      ("r"_user | "w"_user | "x"_user) ==
      (file_permission::user_read | file_permission::user_write | file_permission::user_execute));

  require(("r"_group | "w"_group | "x"_group) ==
        (file_permission::group_read | file_permission::group_write |
         file_permission::group_execute));

  require(("r"_other | "w"_other | "x"_other) ==
        (file_permission::other_read | file_permission::other_write |
         file_permission::other_execute));

  require((""_user | ""_group | ""_other) == file_permission::none);
}

scenario("Permission flags can be or'ed together") {
  when("or'ing all user permission bits") {
    const auto all_user_permissions =
        file_permission::user_read | file_permission::user_write | file_permission::user_execute;

    then("the resulting value is equal to the 'all' user permission flag") {
      require(file_permission::user_all == all_user_permissions);
    }
  }

  when("or'ing all group permission bits") {
    const auto all_group_permissions = file_permission::group_read | file_permission::group_write |
                                       file_permission::group_execute;

    then("the resulting value is equal to the 'all' group permission flag") {
      require(file_permission::group_all == all_group_permissions);
    }
  }

  when("or'ing all other permission bits") {
    const auto all_other_permissions = file_permission::other_read | file_permission::other_write |
                                       file_permission::other_execute;

    then("the resulting value is equal to the 'all' other permission flag") {
      require(file_permission::other_all == all_other_permissions);
    }
  }
}

scenario("Permission flags represent distinct permission bits") {
  require(((file_permission::user_read != file_permission::group_read) &&
         (file_permission::group_read != file_permission::other_read)));
  require(((file_permission::user_write != file_permission::group_write) &&
         (file_permission::group_write != file_permission::other_write)));
  require(((file_permission::user_execute != file_permission::group_execute) &&
         (file_permission::group_execute != file_permission::other_execute)));

  require(((file_permission::user_read != file_permission::user_write) &&
         (file_permission::user_write != file_permission::user_execute)));
  require(((file_permission::group_read != file_permission::group_write) &&
         (file_permission::group_write != file_permission::group_execute)));
  require(((file_permission::other_read != file_permission::other_write) &&
         (file_permission::other_write != file_permission::other_execute)));

  require(((file_permission::user_all != file_permission::group_all) &&
         (file_permission::group_all != file_permission::other_all)));
}
