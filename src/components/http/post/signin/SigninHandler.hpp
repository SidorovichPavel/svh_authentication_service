#pragma once

#include <string_view>

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svh::auth::components::http::post::signin {

class SigninHandler final
    : public userver::server::handlers::HttpHandlerJsonBase {
 public:
  constexpr static std::string_view kName = "signin";

  SigninHandler(const userver::components::ComponentConfig& config,
               const userver::components::ComponentContext& context);

  userver::formats::json::Value HandleRequestJsonThrow(
      const userver::server::http::HttpRequest& request,
      const userver::formats::json::Value& request_json,
      userver::server::request::RequestContext& context) const override;

 private:
  constexpr static std::string_view db_name = "svh_authentication_service_db";
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace svh::auth::components::http::post::login