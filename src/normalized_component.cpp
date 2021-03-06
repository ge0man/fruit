/*
 * Copyright 2014 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define IN_FRUIT_CPP_FILE

#include <fruit/impl/storage/normalized_component.h>
#include <fruit/impl/storage/component_storage.h>

#include <fruit/impl/data_structures/semistatic_map.templates.h>
#include <fruit/impl/data_structures/semistatic_graph.templates.h>

using std::cout;
using std::endl;

using namespace fruit;
using namespace fruit::impl;

namespace fruit {
namespace impl {

NormalizedComponentStorage::NormalizedComponentStorage(ComponentStorage&& component, const std::vector<TypeId>& exposed_types)
  : bindingCompressionInfoMap(
      std::unique_ptr<InjectorStorage::BindingCompressionInfoMap>(
          new InjectorStorage::BindingCompressionInfoMap(
              createHashMap<TypeId, InjectorStorage::BindingCompressionInfo>(
                TypeId{nullptr}, getInvalidTypeId())))) {
  std::vector<std::pair<TypeId, BindingData>> bindings_vector(component.bindings.begin(), component.bindings.end());
  InjectorStorage::normalizeBindings(bindings_vector,
                                     fixed_size_allocator_data,
                                     std::vector<CompressedBinding>(component.compressed_bindings.begin(), component.compressed_bindings.end()),
                                     std::vector<std::pair<TypeId, MultibindingData>>(component.multibindings.begin(), component.multibindings.end()),
                                     exposed_types,
                                     *bindingCompressionInfoMap);
  
  bindings = SemistaticGraph<TypeId, NormalizedBindingData>(InjectorStorage::BindingDataNodeIter{bindings_vector.begin()},
                                                            InjectorStorage::BindingDataNodeIter{bindings_vector.end()},
                                                            TypeId{nullptr},
                                                            getInvalidTypeId());
  
  InjectorStorage::addMultibindings(multibindings, fixed_size_allocator_data, std::vector<std::pair<TypeId, MultibindingData>>(component.multibindings.begin(), component.multibindings.end()));
}

NormalizedComponentStorage::~NormalizedComponentStorage() {
}

} // namespace impl
} // namespace fruit
