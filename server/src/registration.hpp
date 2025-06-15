#pragma once

#include <string>
#include <string_view>
#include <userver/components/component_list.hpp>

namespace my_service {

enum class RegistrationStatus { kSuccess, kConflict, kInvalid };
std::string RegistrationResponse(std::string_view username,
                                 RegistrationStatus status);

void AppendRegistration(userver::components::ComponentList& component_list);

}  // namespace my_service
