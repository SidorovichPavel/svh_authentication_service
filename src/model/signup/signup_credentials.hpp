#pragma once

#include <cstdint>
#include <string>

#include <boost/uuid/uuid.hpp>

namespace svh::auth::model::signup {

struct signup_credentials {
  std::string first_name;
  std::string last_name;
  std::int16_t age;
  std::string nickname;
  std::string password_hash;
};

}  // namespace svh::auth::model::signin