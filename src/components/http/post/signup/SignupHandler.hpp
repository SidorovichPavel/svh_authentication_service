#pragma once

#include <string_view>

#include <signup/signup_credentials.hpp>

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svh::auth::components::http::post::signup {

class SignupHandler final
    : public userver::server::handlers::HttpHandlerJsonBase {
 public:
  constexpr static std::string_view kName = "signup";

  SignupHandler(const userver::components::ComponentConfig& config,
                const userver::components::ComponentContext& context);

  userver::formats::json::Value HandleRequestJsonThrow(
      const userver::server::http::HttpRequest& request,
      const userver::formats::json::Value& request_json,
      userver::server::request::RequestContext& context) const override;

 private:
  std::optional<boost::uuids::uuid> TryInsertUser(
      const model::signup::signup_credentials& creds) const;

  std::pair<std::string, std::string> SaltPassword(
      std::string_view nickname, std::string_view password) const;
      
  std::string MakeToken(const boost::uuids::uuid& uuid,
                        const model::signup::signup_credentials&) const;

  constexpr static std::string_view db_name = "svh_authentication_service_db";
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace svh::auth::components::http::post::signup