#pragma once

#include "signup_credentials.hpp"

#include <userver/formats/parse/to.hpp>
#include <userver/storages/postgres/io/user_types.hpp>

namespace svh::auth::model::signup {

template <typename T>
signup_credentials Parse(const T& data,
                         userver::formats::parse::To<signup_credentials>) {
  return signup_credentials{data["first_name"].template As<std::string>(),
                            data["last_name"].template As<std::string>(),
                            data["age"].template As<std::int16_t>(),
                            data["nickname"].template As<std::string>(),
                            data["password_hash"].template As<std::string>()};
}

}  // namespace svh::auth::model::signup

template <>
struct userver::storages::postgres::io::CppToUserPg<
    svh::auth::model::signup::signup_credentials> {
  static constexpr DBTypeName postgres_name =
      "svh_authentication_schema.signup_credentials_type";
};