#pragma once

#include <string>
#include <string_view>
#include <userver/components/component_list.hpp>

namespace service_template {

enum class RegistrationStatus { kSuccess, kConflict, kInvalid };
std::string RegistrationResponse(std::string_view username,
                                 RegistrationStatus status);

void AppendRegistration(userver::components::ComponentList& component_list);

}  // namespace service_template
