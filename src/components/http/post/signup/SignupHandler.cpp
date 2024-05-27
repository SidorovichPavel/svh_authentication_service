#include "SignupHandler.hpp"
#include "fwd.hpp"

#include <signup/io.hpp>

#include <jwt-cpp/jwt.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <userver/components/component.hpp>
#include <userver/crypto/base64.hpp>
#include <userver/formats/json.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/io/composite_types.hpp>
#include <userver/storages/postgres/postgres.hpp>
#include <userver/storages/query.hpp>

namespace svh::auth::components::http::post::signup {

namespace {
const userver::storages::Query kUserInsertQuery(
    R"(INSERT INTO svh_authentication_schema.users (
    first_name,
    last_name,
    age,
    nickname,
    password_hash,
    salt)
    VALUES (
    $1.first_name,
    $1.last_name,
    $1.age,
    $1.nickname,
    $2,
    $3
) RETURNING id)",
    userver::storages::Query::Name("kUserInsert"),
    userver::storages::Query::LogMode::kFull);
}

SignupHandler::SignupHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : userver::server::handlers::HttpHandlerJsonBase(config, context),
      pg_cluster_(context.FindComponent<userver::components::Postgres>(db_name)
                      .GetCluster()) {}

userver::formats::json::Value SignupHandler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    [[maybe_unused]] userver::server::request::RequestContext& context) const {
  auto creditions = request_json.As<model::signup::signup_credentials>();

  std::optional<boost::uuids::uuid> uuid;

  try {
    uuid = TryInsertUser(creditions);
  } catch (const std::exception& e) {
    LOG_DEBUG() << e.what();
  }

  userver::formats::json::ValueBuilder json_biulder;
  if (uuid) {
    json_biulder["token"] = MakeToken(*uuid, creditions);
    return json_biulder.ExtractValue();
  } else {
    request.GetHttpResponse().SetStatus(
        userver::server::http::HttpStatus::kConflict);
    return json_biulder.ExtractValue();
  }
}

std::optional<boost::uuids::uuid> SignupHandler::TryInsertUser(
    const model::signup::signup_credentials& ucreds) const {
  constexpr size_t salt_size = 10u;
  auto salt = userver::crypto::base64::Base64Encode(
                  ucreds.nickname, userver::crypto::base64::Pad::kWith)
                  .substr(0, salt_size);

  auto salt_pass_hash = ucreds.password_hash + salt;
  auto double_hashed_pass = userver::crypto::base64::Base64Encode(
      salt_pass_hash, userver::crypto::base64::Pad::kWithout);

  auto pg_result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, kUserInsertQuery,
      ucreds, double_hashed_pass, salt);
  return pg_result.AsOptionalSingleRow<boost::uuids::uuid>();
}

std::string SignupHandler::MakeToken(
    const boost::uuids::uuid& uuid,
    const model::signup::signup_credentials& uc) const {
  auto secret = "json+web+" + uc.first_name;
  std::shuffle(secret.begin(), secret.end(), std::random_device{});
  auto token =
      jwt::create()
          .set_type("JWS")
          .set_issuer("svh_auth0")
          .set_payload_claim("id", jwt::claim(boost::uuids::to_string(uuid)))
          .set_payload_claim("nickname", jwt::claim(uc.nickname))
          .set_payload_claim("age", jwt::claim(std::to_string(uc.age)))
          .sign(jwt::algorithm::hs256{secret});
  return std::string(token.c_str());
}

void Append(userver::components::ComponentList& component_list) {
  component_list.Append<SignupHandler>();
}

}  // namespace svh::auth::components::http::post::signup