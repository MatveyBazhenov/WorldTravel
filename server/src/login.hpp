#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace service_template {

enum class Loginstatus { kSuccess, kInvalid, kExists };
std::string LoginResponse(std::string_view login, Loginstatus type);

void AppendLogin(userver::components::ComponentList& component_list);

}  // namespace service_template
