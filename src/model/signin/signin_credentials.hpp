#pragma once

#include <cstdint>
#include <string>

#include <boost/uuid/uuid.hpp>

namespace svh::auth::model::signin {

struct signin_credentials {
  std::int16_t age;
  std::string nickname;
  std::string password_hash;
  std::string salt;
};

}  // namespace svh::auth::model::signin