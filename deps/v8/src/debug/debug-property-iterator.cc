// Copyright 2018 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/debug/debug-property-iterator.h"

#include "src/api/api-inl.h"
#include "src/base/flags.h"
#include "src/objects/js-array-buffer-inl.h"
#include "src/objects/keys.h"
#include "src/objects/property-descriptor.h"
#include "src/objects/property-details.h"

namespace v8 {
namespace internal {

std::unique_ptr<DebugPropertyIterator> DebugPropertyIterator::Create(
    Isolate* isolate, Handle<JSReceiver> receiver, bool skip_indices) {
  // Can't use std::make_unique as Ctor is private.
  auto iterator = std::unique_ptr<DebugPropertyIterator>(
      new DebugPropertyIterator(isolate, receiver, skip_indices));

  if (receiver->IsJSProxy()) {
    iterator->AdvanceToPrototype();
  }

  if (!iterator->FillKeysForCurrentPrototypeAndStage()) return nullptr;
  if (iterator->should_move_to_next_stage() && !iterator->AdvanceInternal()) {
    return nullptr;
  }

  return iterator;
}

DebugPropertyIterator::DebugPropertyIterator(Isolate* isolate,
                                             Handle<JSReceiver> receiver,
                                             bool skip_indices)
    : isolate_(isolate),
      prototype_iterator_(isolate, receiver, kStartAtReceiver,
                          PrototypeIterator::END_AT_NULL),
      skip_indices_(skip_indices),
      current_key_index_(0),
      current_keys_(isolate_->factory()->empty_fixed_array()),
      current_keys_length_(0) {}

bool DebugPropertyIterator::Done() const { return is_done_; }

void DebugPropertyIterator::AdvanceToPrototype() {
  stage_ = kExoticIndices;
  is_own_ = false;
  if (!prototype_iterator_.HasAccess()) is_done_ = true;
  prototype_iterator_.AdvanceIgnoringProxies();
  if (prototype_iterator_.IsAtEnd()) is_done_ = true;
}

bool DebugPropertyIterator::AdvanceInternal() {
  ++current_key_index_;
  calculated_native_accessor_flags_ = false;
  while (should_move_to_next_stage()) {
    switch (stage_) {
      case kExoticIndices:
        stage_ = kEnumerableStrings;
        break;
      case kEnumerableStrings:
        stage_ = kAllProperties;
        break;
      case kAllProperties:
        AdvanceToPrototype();
        break;
    }
    if (!FillKeysForCurrentPrototypeAndStage()) return false;
  }
  return true;
}

bool DebugPropertyIterator::is_native_accessor() {
  CalculateNativeAccessorFlags();
  return native_accessor_flags_;
}

bool DebugPropertyIterator::has_native_getter() {
  CalculateNativeAccessorFlags();
  return native_accessor_flags_ &
         static_cast<int>(debug::NativeAccessorType::HasGetter);
}

bool DebugPropertyIterator::has_native_setter() {
  CalculateNativeAccessorFlags();
  return native_accessor_flags_ &
         static_cast<int>(debug::NativeAccessorType::HasSetter);
}

Handle<Name> DebugPropertyIterator::raw_name() const {
  DCHECK(!Done());
  if (stage_ == kExoticIndices) {
    return isolate_->factory()->SizeToString(current_key_index_);
  } else {
    return Handle<Name>::cast(FixedArray::get(
        *current_keys_, static_cast<int>(current_key_index_), isolate_));
  }
}

v8::Local<v8::Name> DebugPropertyIterator::name() const {
  return Utils::ToLocal(raw_name());
}

v8::Maybe<v8::PropertyAttribute> DebugPropertyIterator::attributes() {
  Handle<JSReceiver> receiver =
      PrototypeIterator::GetCurrent<JSReceiver>(prototype_iterator_);
  auto result = JSReceiver::GetPropertyAttributes(receiver, raw_name());
  if (result.IsNothing()) return Nothing<v8::PropertyAttribute>();
  DCHECK(result.FromJust() != ABSENT);
  return Just(static_cast<v8::PropertyAttribute>(result.FromJust()));
}

v8::Maybe<v8::debug::PropertyDescriptor> DebugPropertyIterator::descriptor() {
  Handle<JSReceiver> receiver =
      PrototypeIterator::GetCurrent<JSReceiver>(prototype_iterator_);

  PropertyDescriptor descriptor;
  Maybe<bool> did_get_descriptor = JSReceiver::GetOwnPropertyDescriptor(
      isolate_, receiver, raw_name(), &descriptor);
  if (did_get_descriptor.IsNothing()) {
    return Nothing<v8::debug::PropertyDescriptor>();
  }
  DCHECK(did_get_descriptor.FromJust());
  return Just(v8::debug::PropertyDescriptor{
      descriptor.enumerable(), descriptor.has_enumerable(),
      descriptor.configurable(), descriptor.has_configurable(),
      descriptor.writable(), descriptor.has_writable(),
      descriptor.has_value() ? Utils::ToLocal(descriptor.value())
                             : v8::Local<v8::Value>(),
      descriptor.has_get() ? Utils::ToLocal(descriptor.get())
                           : v8::Local<v8::Value>(),
      descriptor.has_set() ? Utils::ToLocal(descriptor.set())
                           : v8::Local<v8::Value>(),
  });
}

bool DebugPropertyIterator::is_own() { return is_own_; }

bool DebugPropertyIterator::is_array_index() {
  if (stage_ == kExoticIndices) return true;
  PropertyKey key(isolate_, raw_name());
  return key.is_element();
}

bool DebugPropertyIterator::FillKeysForCurrentPrototypeAndStage() {
  current_key_index_ = 0;
  current_keys_ = isolate_->factory()->empty_fixed_array();
  current_keys_length_ = 0;
  if (is_done_) return true;
  Handle<JSReceiver> receiver =
      PrototypeIterator::GetCurrent<JSReceiver>(prototype_iterator_);
  if (stage_ == kExoticIndices) {
    if (skip_indices_ || !receiver->IsJSTypedArray()) return true;
    Handle<JSTypedArray> typed_array = Handle<JSTypedArray>::cast(receiver);
    current_keys_length_ =
        typed_array->WasDetached() ? 0 : typed_array->length();
    return true;
  }
  PropertyFilter filter =
      stage_ == kEnumerableStrings ? ENUMERABLE_STRINGS : ALL_PROPERTIES;
  if (KeyAccumulator::GetKeys(receiver, KeyCollectionMode::kOwnOnly, filter,
                              GetKeysConversion::kConvertToString, false,
                              skip_indices_ || receiver->IsJSTypedArray())
          .ToHandle(&current_keys_)) {
    current_keys_length_ = current_keys_->length();
    return true;
  }
  return false;
}

bool DebugPropertyIterator::should_move_to_next_stage() const {
  return !is_done_ && current_key_index_ >= current_keys_length_;
}

namespace {
base::Flags<debug::NativeAccessorType, int> GetNativeAccessorDescriptorInternal(
    Handle<JSReceiver> object, Handle<Name> name) {
  Isolate* isolate = object->GetIsolate();
  PropertyKey key(isolate, name);
  if (key.is_element()) return debug::NativeAccessorType::None;
  LookupIterator it(isolate, object, key, LookupIterator::OWN);
  if (!it.IsFound()) return debug::NativeAccessorType::None;
  if (it.state() != LookupIterator::ACCESSOR) {
    return debug::NativeAccessorType::None;
  }
  Handle<Object> structure = it.GetAccessors();
  if (!structure->IsAccessorInfo()) return debug::NativeAccessorType::None;
  base::Flags<debug::NativeAccessorType, int> result;
#define IS_BUILTIN_ACCESSOR(_, name, ...)                   \
  if (*structure == *isolate->factory()->name##_accessor()) \
    return debug::NativeAccessorType::None;
  ACCESSOR_INFO_LIST_GENERATOR(IS_BUILTIN_ACCESSOR, /* not used */)
#undef IS_BUILTIN_ACCESSOR
  Handle<AccessorInfo> accessor_info = Handle<AccessorInfo>::cast(structure);
  if (accessor_info->getter() != Object()) {
    result |= debug::NativeAccessorType::HasGetter;
  }
  if (accessor_info->setter() != Object()) {
    result |= debug::NativeAccessorType::HasSetter;
  }
  return result;
}
}  // anonymous namespace

void DebugPropertyIterator::CalculateNativeAccessorFlags() {
  if (calculated_native_accessor_flags_) return;
  if (stage_ == kExoticIndices) {
    native_accessor_flags_ = 0;
  } else {
    Handle<JSReceiver> receiver =
        PrototypeIterator::GetCurrent<JSReceiver>(prototype_iterator_);
    native_accessor_flags_ =
        GetNativeAccessorDescriptorInternal(receiver, raw_name());
  }
  calculated_native_accessor_flags_ = true;
}
}  // namespace internal
}  // namespace v8
