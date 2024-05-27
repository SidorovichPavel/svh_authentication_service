#pragma once

#include "signin_credentials.hpp"

#include <userver/formats/parse/to.hpp>
#include <userver/storages/postgres/io/type_mapping.hpp>
#include <userver/storages/postgres/io/user_types.hpp>

namespace svh::auth::model::signin {

template <typename T>
signin_credentials Parse(const T& data,
                         userver::formats::parse::To<signin_credentials>) {
  return signin_credentials{data["age"].template As<std::int16_t>(),
                            data["nickname"].template As<std::string>(),
                            data["password_hash"].template As<std::string>(),
                            data["salt"].template As<std::string>()};
}

}  // namespace svh::auth::model::signin

template <>
struct userver::storages::postgres::io::CppToUserPg<
    svh::auth::model::signin::signin_credentials> {
  static constexpr DBTypeName postgres_name =
      "svh_authentication_schema.signin_credentials_type";
};