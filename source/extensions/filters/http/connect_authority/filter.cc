// Copyright Istio Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "source/extensions/filters/http/connect_authority/filter.h"

#include "envoy/registry/registry.h"
#include "envoy/server/factory_context.h"
#include "source/common/http/utility.h"
#include "source/extensions/filters/listener/set_internal_dst_address/filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace ConnectAuthority {

Http::FilterHeadersStatus Filter::decodeHeaders(Http::RequestHeaderMap& headers, bool) {
  const FilterConfig* per_route_settings =
      Http::Utility::resolveMostSpecificPerFilterConfig<FilterConfig>(decoder_callbacks_);
  if (per_route_settings && per_route_settings->enabled()) {
    decoder_callbacks_->streamInfo().filterState()->setData(
        Istio::SetInternalDstAddress::FilterStateKey,
        std::make_shared<Istio::SetInternalDstAddress::Authority>(headers.getHostValue(),
                                                                  per_route_settings->port()),
        StreamInfo::FilterState::StateType::Mutable, StreamInfo::FilterState::LifeSpan::FilterChain,
        StreamInfo::FilterState::StreamSharing::SharedWithUpstreamConnection);
  }
  return Http::FilterHeadersStatus::Continue;
}

REGISTER_FACTORY(FilterConfigFactory, Server::Configuration::NamedHttpFilterConfigFactory);

} // namespace ConnectAuthority
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
