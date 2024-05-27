#include "SigninHandler.hpp"
#include "fwd.hpp"

#include <signin/io.hpp>
#include <signin/signin_credentials.hpp>

#include <userver/components/component.hpp>
#include <userver/formats/json.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svh::auth::components::http::post::signin {

SigninHandler::SigninHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : userver::server::handlers::HttpHandlerJsonBase(config, context),
      pg_cluster_(context.FindComponent<userver::components::Postgres>(db_name)
                      .GetCluster()) {}

userver::formats::json::Value SigninHandler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext& context) const {
  auto creditions = request_json.As<model::signin::signin_credentials>();
  userver::formats::json::ValueBuilder json_builder;
  return userver::formats::json::MakeObject();
}

void Append(userver::components::ComponentList& component_list) {
  component_list.Append<SigninHandler>();
}

}  // namespace svh::auth::components::http::post::signin