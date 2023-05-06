// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: c2s_common.proto

#include "c2s_common.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace C2SMsg {
PROTOBUF_CONSTEXPR C2SHeartBeat::C2SHeartBeat(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.state_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct C2SHeartBeatDefaultTypeInternal {
  PROTOBUF_CONSTEXPR C2SHeartBeatDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~C2SHeartBeatDefaultTypeInternal() {}
  union {
    C2SHeartBeat _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 C2SHeartBeatDefaultTypeInternal _C2SHeartBeat_default_instance_;
PROTOBUF_CONSTEXPR C2SLoginReq::C2SLoginReq(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.token_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct C2SLoginReqDefaultTypeInternal {
  PROTOBUF_CONSTEXPR C2SLoginReqDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~C2SLoginReqDefaultTypeInternal() {}
  union {
    C2SLoginReq _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 C2SLoginReqDefaultTypeInternal _C2SLoginReq_default_instance_;
PROTOBUF_CONSTEXPR C2SLoginRes::C2SLoginRes(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.error_code_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct C2SLoginResDefaultTypeInternal {
  PROTOBUF_CONSTEXPR C2SLoginResDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~C2SLoginResDefaultTypeInternal() {}
  union {
    C2SLoginRes _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 C2SLoginResDefaultTypeInternal _C2SLoginRes_default_instance_;
}  // namespace C2SMsg
static ::_pb::Metadata file_level_metadata_c2s_5fcommon_2eproto[3];
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_c2s_5fcommon_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_c2s_5fcommon_2eproto = nullptr;

const uint32_t TableStruct_c2s_5fcommon_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::C2SMsg::C2SHeartBeat, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::C2SMsg::C2SHeartBeat, _impl_.state_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::C2SMsg::C2SLoginReq, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::C2SMsg::C2SLoginReq, _impl_.token_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::C2SMsg::C2SLoginRes, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::C2SMsg::C2SLoginRes, _impl_.error_code_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::C2SMsg::C2SHeartBeat)},
  { 7, -1, -1, sizeof(::C2SMsg::C2SLoginReq)},
  { 14, -1, -1, sizeof(::C2SMsg::C2SLoginRes)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::C2SMsg::_C2SHeartBeat_default_instance_._instance,
  &::C2SMsg::_C2SLoginReq_default_instance_._instance,
  &::C2SMsg::_C2SLoginRes_default_instance_._instance,
};

const char descriptor_table_protodef_c2s_5fcommon_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\020c2s_common.proto\022\006C2SMsg\"\035\n\014C2SHeartBe"
  "at\022\r\n\005state\030\001 \001(\005\"\034\n\013C2SLoginReq\022\r\n\005toke"
  "n\030\001 \001(\t\"!\n\013C2SLoginRes\022\022\n\nerror_code\030\001 \001"
  "(\005*b\n\nC2S_MSG_ID\022\017\n\013C2S_ID_NONE\020\000\022\025\n\020C2S"
  "_ID_HEARTBEAT\020\221N\022\025\n\020C2S_ID_LOGIN_REQ\020\222N\022"
  "\025\n\020C2S_ID_LOGIN_RES\020\223Nb\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_c2s_5fcommon_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_c2s_5fcommon_2eproto = {
    false, false, 230, descriptor_table_protodef_c2s_5fcommon_2eproto,
    "c2s_common.proto",
    &descriptor_table_c2s_5fcommon_2eproto_once, nullptr, 0, 3,
    schemas, file_default_instances, TableStruct_c2s_5fcommon_2eproto::offsets,
    file_level_metadata_c2s_5fcommon_2eproto, file_level_enum_descriptors_c2s_5fcommon_2eproto,
    file_level_service_descriptors_c2s_5fcommon_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_c2s_5fcommon_2eproto_getter() {
  return &descriptor_table_c2s_5fcommon_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_c2s_5fcommon_2eproto(&descriptor_table_c2s_5fcommon_2eproto);
namespace C2SMsg {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* C2S_MSG_ID_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_c2s_5fcommon_2eproto);
  return file_level_enum_descriptors_c2s_5fcommon_2eproto[0];
}
bool C2S_MSG_ID_IsValid(int value) {
  switch (value) {
    case 0:
    case 10001:
    case 10002:
    case 10003:
      return true;
    default:
      return false;
  }
}


// ===================================================================

class C2SHeartBeat::_Internal {
 public:
};

C2SHeartBeat::C2SHeartBeat(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:C2SMsg.C2SHeartBeat)
}
C2SHeartBeat::C2SHeartBeat(const C2SHeartBeat& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  C2SHeartBeat* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.state_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.state_ = from._impl_.state_;
  // @@protoc_insertion_point(copy_constructor:C2SMsg.C2SHeartBeat)
}

inline void C2SHeartBeat::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.state_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

C2SHeartBeat::~C2SHeartBeat() {
  // @@protoc_insertion_point(destructor:C2SMsg.C2SHeartBeat)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void C2SHeartBeat::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void C2SHeartBeat::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void C2SHeartBeat::Clear() {
// @@protoc_insertion_point(message_clear_start:C2SMsg.C2SHeartBeat)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.state_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* C2SHeartBeat::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 state = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.state_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* C2SHeartBeat::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:C2SMsg.C2SHeartBeat)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 state = 1;
  if (this->_internal_state() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_state(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:C2SMsg.C2SHeartBeat)
  return target;
}

size_t C2SHeartBeat::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:C2SMsg.C2SHeartBeat)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 state = 1;
  if (this->_internal_state() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_state());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData C2SHeartBeat::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    C2SHeartBeat::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*C2SHeartBeat::GetClassData() const { return &_class_data_; }


void C2SHeartBeat::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<C2SHeartBeat*>(&to_msg);
  auto& from = static_cast<const C2SHeartBeat&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:C2SMsg.C2SHeartBeat)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_state() != 0) {
    _this->_internal_set_state(from._internal_state());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void C2SHeartBeat::CopyFrom(const C2SHeartBeat& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:C2SMsg.C2SHeartBeat)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool C2SHeartBeat::IsInitialized() const {
  return true;
}

void C2SHeartBeat::InternalSwap(C2SHeartBeat* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.state_, other->_impl_.state_);
}

::PROTOBUF_NAMESPACE_ID::Metadata C2SHeartBeat::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_c2s_5fcommon_2eproto_getter, &descriptor_table_c2s_5fcommon_2eproto_once,
      file_level_metadata_c2s_5fcommon_2eproto[0]);
}

// ===================================================================

class C2SLoginReq::_Internal {
 public:
};

C2SLoginReq::C2SLoginReq(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:C2SMsg.C2SLoginReq)
}
C2SLoginReq::C2SLoginReq(const C2SLoginReq& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  C2SLoginReq* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.token_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.token_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.token_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_token().empty()) {
    _this->_impl_.token_.Set(from._internal_token(), 
      _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:C2SMsg.C2SLoginReq)
}

inline void C2SLoginReq::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.token_){}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.token_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.token_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

C2SLoginReq::~C2SLoginReq() {
  // @@protoc_insertion_point(destructor:C2SMsg.C2SLoginReq)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void C2SLoginReq::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.token_.Destroy();
}

void C2SLoginReq::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void C2SLoginReq::Clear() {
// @@protoc_insertion_point(message_clear_start:C2SMsg.C2SLoginReq)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.token_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* C2SLoginReq::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string token = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_token();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "C2SMsg.C2SLoginReq.token"));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* C2SLoginReq::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:C2SMsg.C2SLoginReq)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string token = 1;
  if (!this->_internal_token().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_token().data(), static_cast<int>(this->_internal_token().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "C2SMsg.C2SLoginReq.token");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_token(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:C2SMsg.C2SLoginReq)
  return target;
}

size_t C2SLoginReq::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:C2SMsg.C2SLoginReq)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string token = 1;
  if (!this->_internal_token().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_token());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData C2SLoginReq::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    C2SLoginReq::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*C2SLoginReq::GetClassData() const { return &_class_data_; }


void C2SLoginReq::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<C2SLoginReq*>(&to_msg);
  auto& from = static_cast<const C2SLoginReq&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:C2SMsg.C2SLoginReq)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_token().empty()) {
    _this->_internal_set_token(from._internal_token());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void C2SLoginReq::CopyFrom(const C2SLoginReq& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:C2SMsg.C2SLoginReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool C2SLoginReq::IsInitialized() const {
  return true;
}

void C2SLoginReq::InternalSwap(C2SLoginReq* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.token_, lhs_arena,
      &other->_impl_.token_, rhs_arena
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata C2SLoginReq::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_c2s_5fcommon_2eproto_getter, &descriptor_table_c2s_5fcommon_2eproto_once,
      file_level_metadata_c2s_5fcommon_2eproto[1]);
}

// ===================================================================

class C2SLoginRes::_Internal {
 public:
};

C2SLoginRes::C2SLoginRes(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:C2SMsg.C2SLoginRes)
}
C2SLoginRes::C2SLoginRes(const C2SLoginRes& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  C2SLoginRes* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.error_code_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.error_code_ = from._impl_.error_code_;
  // @@protoc_insertion_point(copy_constructor:C2SMsg.C2SLoginRes)
}

inline void C2SLoginRes::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.error_code_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

C2SLoginRes::~C2SLoginRes() {
  // @@protoc_insertion_point(destructor:C2SMsg.C2SLoginRes)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void C2SLoginRes::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void C2SLoginRes::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void C2SLoginRes::Clear() {
// @@protoc_insertion_point(message_clear_start:C2SMsg.C2SLoginRes)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.error_code_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* C2SLoginRes::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 error_code = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.error_code_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* C2SLoginRes::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:C2SMsg.C2SLoginRes)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 error_code = 1;
  if (this->_internal_error_code() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_error_code(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:C2SMsg.C2SLoginRes)
  return target;
}

size_t C2SLoginRes::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:C2SMsg.C2SLoginRes)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 error_code = 1;
  if (this->_internal_error_code() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_error_code());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData C2SLoginRes::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    C2SLoginRes::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*C2SLoginRes::GetClassData() const { return &_class_data_; }


void C2SLoginRes::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<C2SLoginRes*>(&to_msg);
  auto& from = static_cast<const C2SLoginRes&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:C2SMsg.C2SLoginRes)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_error_code() != 0) {
    _this->_internal_set_error_code(from._internal_error_code());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void C2SLoginRes::CopyFrom(const C2SLoginRes& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:C2SMsg.C2SLoginRes)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool C2SLoginRes::IsInitialized() const {
  return true;
}

void C2SLoginRes::InternalSwap(C2SLoginRes* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.error_code_, other->_impl_.error_code_);
}

::PROTOBUF_NAMESPACE_ID::Metadata C2SLoginRes::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_c2s_5fcommon_2eproto_getter, &descriptor_table_c2s_5fcommon_2eproto_once,
      file_level_metadata_c2s_5fcommon_2eproto[2]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace C2SMsg
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::C2SMsg::C2SHeartBeat*
Arena::CreateMaybeMessage< ::C2SMsg::C2SHeartBeat >(Arena* arena) {
  return Arena::CreateMessageInternal< ::C2SMsg::C2SHeartBeat >(arena);
}
template<> PROTOBUF_NOINLINE ::C2SMsg::C2SLoginReq*
Arena::CreateMaybeMessage< ::C2SMsg::C2SLoginReq >(Arena* arena) {
  return Arena::CreateMessageInternal< ::C2SMsg::C2SLoginReq >(arena);
}
template<> PROTOBUF_NOINLINE ::C2SMsg::C2SLoginRes*
Arena::CreateMaybeMessage< ::C2SMsg::C2SLoginRes >(Arena* arena) {
  return Arena::CreateMessageInternal< ::C2SMsg::C2SLoginRes >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>