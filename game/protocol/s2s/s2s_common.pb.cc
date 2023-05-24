// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: s2s_common.proto

#include "s2s_common.pb.h"

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

namespace S2S {
PROTOBUF_CONSTEXPR S2SHeartBeat::S2SHeartBeat(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.app_id_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.state_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct S2SHeartBeatDefaultTypeInternal {
  PROTOBUF_CONSTEXPR S2SHeartBeatDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~S2SHeartBeatDefaultTypeInternal() {}
  union {
    S2SHeartBeat _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 S2SHeartBeatDefaultTypeInternal _S2SHeartBeat_default_instance_;
PROTOBUF_CONSTEXPR ServerInfo::ServerInfo(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.app_id_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.internal_ip_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.internal_port_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct ServerInfoDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ServerInfoDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ServerInfoDefaultTypeInternal() {}
  union {
    ServerInfo _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ServerInfoDefaultTypeInternal _ServerInfo_default_instance_;
PROTOBUF_CONSTEXPR S2SServerRegisterReq::S2SServerRegisterReq(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.server_info_)*/nullptr
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct S2SServerRegisterReqDefaultTypeInternal {
  PROTOBUF_CONSTEXPR S2SServerRegisterReqDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~S2SServerRegisterReqDefaultTypeInternal() {}
  union {
    S2SServerRegisterReq _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 S2SServerRegisterReqDefaultTypeInternal _S2SServerRegisterReq_default_instance_;
PROTOBUF_CONSTEXPR S2SServerRegisterRes::S2SServerRegisterRes(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.success_)*/false
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct S2SServerRegisterResDefaultTypeInternal {
  PROTOBUF_CONSTEXPR S2SServerRegisterResDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~S2SServerRegisterResDefaultTypeInternal() {}
  union {
    S2SServerRegisterRes _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 S2SServerRegisterResDefaultTypeInternal _S2SServerRegisterRes_default_instance_;
}  // namespace S2S
static ::_pb::Metadata file_level_metadata_s2s_5fcommon_2eproto[4];
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_s2s_5fcommon_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_s2s_5fcommon_2eproto = nullptr;

const uint32_t TableStruct_s2s_5fcommon_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::S2S::S2SHeartBeat, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::S2S::S2SHeartBeat, _impl_.app_id_),
  PROTOBUF_FIELD_OFFSET(::S2S::S2SHeartBeat, _impl_.state_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::S2S::ServerInfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::S2S::ServerInfo, _impl_.app_id_),
  PROTOBUF_FIELD_OFFSET(::S2S::ServerInfo, _impl_.internal_ip_),
  PROTOBUF_FIELD_OFFSET(::S2S::ServerInfo, _impl_.internal_port_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::S2S::S2SServerRegisterReq, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::S2S::S2SServerRegisterReq, _impl_.server_info_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::S2S::S2SServerRegisterRes, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::S2S::S2SServerRegisterRes, _impl_.success_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::S2S::S2SHeartBeat)},
  { 8, -1, -1, sizeof(::S2S::ServerInfo)},
  { 17, -1, -1, sizeof(::S2S::S2SServerRegisterReq)},
  { 24, -1, -1, sizeof(::S2S::S2SServerRegisterRes)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::S2S::_S2SHeartBeat_default_instance_._instance,
  &::S2S::_ServerInfo_default_instance_._instance,
  &::S2S::_S2SServerRegisterReq_default_instance_._instance,
  &::S2S::_S2SServerRegisterRes_default_instance_._instance,
};

const char descriptor_table_protodef_s2s_5fcommon_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\020s2s_common.proto\022\003S2S\"-\n\014S2SHeartBeat\022"
  "\016\n\006app_id\030\001 \001(\t\022\r\n\005state\030\002 \001(\005\"H\n\nServer"
  "Info\022\016\n\006app_id\030\001 \001(\t\022\023\n\013internal_ip\030\002 \001("
  "\t\022\025\n\rinternal_port\030\003 \001(\005\"<\n\024S2SServerReg"
  "isterReq\022$\n\013server_info\030\001 \001(\0132\017.S2S.Serv"
  "erInfo\"\'\n\024S2SServerRegisterRes\022\017\n\007succes"
  "s\030\001 \001(\010*\365\002\n\nS2S_MSG_ID\022\017\n\013MSG_ID_NONE\020\000\022"
  "\024\n\020MSG_ID_HEARTBEAT\020e\022\035\n\031MSG_ID_SERVER_R"
  "EGSTER_REQ\020f\022\035\n\031MSG_ID_SERVER_REGSTER_RE"
  "S\020g\022\030\n\024MSG_ID_DB_INSERT_REQ\020\001\022\030\n\024MSG_ID_"
  "DB_INSERT_RES\020\002\022\030\n\024MSG_ID_DB_REMOVE_REQ\020"
  "\003\022\030\n\024MSG_ID_DB_REMOVE_RES\020\004\022\026\n\022MSG_ID_DB"
  "_SAVE_REQ\020\005\022\026\n\022MSG_ID_DB_SAVE_RES\020\006\022\026\n\022M"
  "SG_ID_DB_FIND_REQ\020\007\022\026\n\022MSG_ID_DB_FIND_RE"
  "S\020\010\022\034\n\030MSG_ID_DB_BATCH_FIND_REQ\020\t\022\034\n\030MSG"
  "_ID_DB_BATCH_FIND_RES\020\nb\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_s2s_5fcommon_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_s2s_5fcommon_2eproto = {
    false, false, 631, descriptor_table_protodef_s2s_5fcommon_2eproto,
    "s2s_common.proto",
    &descriptor_table_s2s_5fcommon_2eproto_once, nullptr, 0, 4,
    schemas, file_default_instances, TableStruct_s2s_5fcommon_2eproto::offsets,
    file_level_metadata_s2s_5fcommon_2eproto, file_level_enum_descriptors_s2s_5fcommon_2eproto,
    file_level_service_descriptors_s2s_5fcommon_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_s2s_5fcommon_2eproto_getter() {
  return &descriptor_table_s2s_5fcommon_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_s2s_5fcommon_2eproto(&descriptor_table_s2s_5fcommon_2eproto);
namespace S2S {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* S2S_MSG_ID_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_s2s_5fcommon_2eproto);
  return file_level_enum_descriptors_s2s_5fcommon_2eproto[0];
}
bool S2S_MSG_ID_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 101:
    case 102:
    case 103:
      return true;
    default:
      return false;
  }
}


// ===================================================================

class S2SHeartBeat::_Internal {
 public:
};

S2SHeartBeat::S2SHeartBeat(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:S2S.S2SHeartBeat)
}
S2SHeartBeat::S2SHeartBeat(const S2SHeartBeat& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  S2SHeartBeat* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.app_id_){}
    , decltype(_impl_.state_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.app_id_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.app_id_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_app_id().empty()) {
    _this->_impl_.app_id_.Set(from._internal_app_id(), 
      _this->GetArenaForAllocation());
  }
  _this->_impl_.state_ = from._impl_.state_;
  // @@protoc_insertion_point(copy_constructor:S2S.S2SHeartBeat)
}

inline void S2SHeartBeat::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.app_id_){}
    , decltype(_impl_.state_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.app_id_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.app_id_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

S2SHeartBeat::~S2SHeartBeat() {
  // @@protoc_insertion_point(destructor:S2S.S2SHeartBeat)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void S2SHeartBeat::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.app_id_.Destroy();
}

void S2SHeartBeat::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void S2SHeartBeat::Clear() {
// @@protoc_insertion_point(message_clear_start:S2S.S2SHeartBeat)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.app_id_.ClearToEmpty();
  _impl_.state_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* S2SHeartBeat::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string app_id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_app_id();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "S2S.S2SHeartBeat.app_id"));
        } else
          goto handle_unusual;
        continue;
      // int32 state = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
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

uint8_t* S2SHeartBeat::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:S2S.S2SHeartBeat)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string app_id = 1;
  if (!this->_internal_app_id().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_app_id().data(), static_cast<int>(this->_internal_app_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "S2S.S2SHeartBeat.app_id");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_app_id(), target);
  }

  // int32 state = 2;
  if (this->_internal_state() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(2, this->_internal_state(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:S2S.S2SHeartBeat)
  return target;
}

size_t S2SHeartBeat::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:S2S.S2SHeartBeat)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string app_id = 1;
  if (!this->_internal_app_id().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_app_id());
  }

  // int32 state = 2;
  if (this->_internal_state() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_state());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData S2SHeartBeat::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    S2SHeartBeat::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*S2SHeartBeat::GetClassData() const { return &_class_data_; }


void S2SHeartBeat::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<S2SHeartBeat*>(&to_msg);
  auto& from = static_cast<const S2SHeartBeat&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:S2S.S2SHeartBeat)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_app_id().empty()) {
    _this->_internal_set_app_id(from._internal_app_id());
  }
  if (from._internal_state() != 0) {
    _this->_internal_set_state(from._internal_state());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void S2SHeartBeat::CopyFrom(const S2SHeartBeat& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:S2S.S2SHeartBeat)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool S2SHeartBeat::IsInitialized() const {
  return true;
}

void S2SHeartBeat::InternalSwap(S2SHeartBeat* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.app_id_, lhs_arena,
      &other->_impl_.app_id_, rhs_arena
  );
  swap(_impl_.state_, other->_impl_.state_);
}

::PROTOBUF_NAMESPACE_ID::Metadata S2SHeartBeat::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_s2s_5fcommon_2eproto_getter, &descriptor_table_s2s_5fcommon_2eproto_once,
      file_level_metadata_s2s_5fcommon_2eproto[0]);
}

// ===================================================================

class ServerInfo::_Internal {
 public:
};

ServerInfo::ServerInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:S2S.ServerInfo)
}
ServerInfo::ServerInfo(const ServerInfo& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  ServerInfo* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.app_id_){}
    , decltype(_impl_.internal_ip_){}
    , decltype(_impl_.internal_port_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.app_id_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.app_id_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_app_id().empty()) {
    _this->_impl_.app_id_.Set(from._internal_app_id(), 
      _this->GetArenaForAllocation());
  }
  _impl_.internal_ip_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.internal_ip_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_internal_ip().empty()) {
    _this->_impl_.internal_ip_.Set(from._internal_internal_ip(), 
      _this->GetArenaForAllocation());
  }
  _this->_impl_.internal_port_ = from._impl_.internal_port_;
  // @@protoc_insertion_point(copy_constructor:S2S.ServerInfo)
}

inline void ServerInfo::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.app_id_){}
    , decltype(_impl_.internal_ip_){}
    , decltype(_impl_.internal_port_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.app_id_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.app_id_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.internal_ip_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.internal_ip_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

ServerInfo::~ServerInfo() {
  // @@protoc_insertion_point(destructor:S2S.ServerInfo)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void ServerInfo::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.app_id_.Destroy();
  _impl_.internal_ip_.Destroy();
}

void ServerInfo::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void ServerInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:S2S.ServerInfo)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.app_id_.ClearToEmpty();
  _impl_.internal_ip_.ClearToEmpty();
  _impl_.internal_port_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ServerInfo::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string app_id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_app_id();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "S2S.ServerInfo.app_id"));
        } else
          goto handle_unusual;
        continue;
      // string internal_ip = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_internal_ip();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "S2S.ServerInfo.internal_ip"));
        } else
          goto handle_unusual;
        continue;
      // int32 internal_port = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          _impl_.internal_port_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
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

uint8_t* ServerInfo::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:S2S.ServerInfo)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string app_id = 1;
  if (!this->_internal_app_id().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_app_id().data(), static_cast<int>(this->_internal_app_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "S2S.ServerInfo.app_id");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_app_id(), target);
  }

  // string internal_ip = 2;
  if (!this->_internal_internal_ip().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_internal_ip().data(), static_cast<int>(this->_internal_internal_ip().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "S2S.ServerInfo.internal_ip");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_internal_ip(), target);
  }

  // int32 internal_port = 3;
  if (this->_internal_internal_port() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(3, this->_internal_internal_port(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:S2S.ServerInfo)
  return target;
}

size_t ServerInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:S2S.ServerInfo)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string app_id = 1;
  if (!this->_internal_app_id().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_app_id());
  }

  // string internal_ip = 2;
  if (!this->_internal_internal_ip().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_internal_ip());
  }

  // int32 internal_port = 3;
  if (this->_internal_internal_port() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_internal_port());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData ServerInfo::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    ServerInfo::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*ServerInfo::GetClassData() const { return &_class_data_; }


void ServerInfo::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<ServerInfo*>(&to_msg);
  auto& from = static_cast<const ServerInfo&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:S2S.ServerInfo)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_app_id().empty()) {
    _this->_internal_set_app_id(from._internal_app_id());
  }
  if (!from._internal_internal_ip().empty()) {
    _this->_internal_set_internal_ip(from._internal_internal_ip());
  }
  if (from._internal_internal_port() != 0) {
    _this->_internal_set_internal_port(from._internal_internal_port());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void ServerInfo::CopyFrom(const ServerInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:S2S.ServerInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ServerInfo::IsInitialized() const {
  return true;
}

void ServerInfo::InternalSwap(ServerInfo* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.app_id_, lhs_arena,
      &other->_impl_.app_id_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.internal_ip_, lhs_arena,
      &other->_impl_.internal_ip_, rhs_arena
  );
  swap(_impl_.internal_port_, other->_impl_.internal_port_);
}

::PROTOBUF_NAMESPACE_ID::Metadata ServerInfo::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_s2s_5fcommon_2eproto_getter, &descriptor_table_s2s_5fcommon_2eproto_once,
      file_level_metadata_s2s_5fcommon_2eproto[1]);
}

// ===================================================================

class S2SServerRegisterReq::_Internal {
 public:
  static const ::S2S::ServerInfo& server_info(const S2SServerRegisterReq* msg);
};

const ::S2S::ServerInfo&
S2SServerRegisterReq::_Internal::server_info(const S2SServerRegisterReq* msg) {
  return *msg->_impl_.server_info_;
}
S2SServerRegisterReq::S2SServerRegisterReq(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:S2S.S2SServerRegisterReq)
}
S2SServerRegisterReq::S2SServerRegisterReq(const S2SServerRegisterReq& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  S2SServerRegisterReq* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.server_info_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_server_info()) {
    _this->_impl_.server_info_ = new ::S2S::ServerInfo(*from._impl_.server_info_);
  }
  // @@protoc_insertion_point(copy_constructor:S2S.S2SServerRegisterReq)
}

inline void S2SServerRegisterReq::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.server_info_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

S2SServerRegisterReq::~S2SServerRegisterReq() {
  // @@protoc_insertion_point(destructor:S2S.S2SServerRegisterReq)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void S2SServerRegisterReq::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete _impl_.server_info_;
}

void S2SServerRegisterReq::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void S2SServerRegisterReq::Clear() {
// @@protoc_insertion_point(message_clear_start:S2S.S2SServerRegisterReq)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaForAllocation() == nullptr && _impl_.server_info_ != nullptr) {
    delete _impl_.server_info_;
  }
  _impl_.server_info_ = nullptr;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* S2SServerRegisterReq::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .S2S.ServerInfo server_info = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_server_info(), ptr);
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

uint8_t* S2SServerRegisterReq::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:S2S.S2SServerRegisterReq)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .S2S.ServerInfo server_info = 1;
  if (this->_internal_has_server_info()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, _Internal::server_info(this),
        _Internal::server_info(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:S2S.S2SServerRegisterReq)
  return target;
}

size_t S2SServerRegisterReq::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:S2S.S2SServerRegisterReq)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .S2S.ServerInfo server_info = 1;
  if (this->_internal_has_server_info()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.server_info_);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData S2SServerRegisterReq::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    S2SServerRegisterReq::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*S2SServerRegisterReq::GetClassData() const { return &_class_data_; }


void S2SServerRegisterReq::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<S2SServerRegisterReq*>(&to_msg);
  auto& from = static_cast<const S2SServerRegisterReq&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:S2S.S2SServerRegisterReq)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_server_info()) {
    _this->_internal_mutable_server_info()->::S2S::ServerInfo::MergeFrom(
        from._internal_server_info());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void S2SServerRegisterReq::CopyFrom(const S2SServerRegisterReq& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:S2S.S2SServerRegisterReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool S2SServerRegisterReq::IsInitialized() const {
  return true;
}

void S2SServerRegisterReq::InternalSwap(S2SServerRegisterReq* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.server_info_, other->_impl_.server_info_);
}

::PROTOBUF_NAMESPACE_ID::Metadata S2SServerRegisterReq::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_s2s_5fcommon_2eproto_getter, &descriptor_table_s2s_5fcommon_2eproto_once,
      file_level_metadata_s2s_5fcommon_2eproto[2]);
}

// ===================================================================

class S2SServerRegisterRes::_Internal {
 public:
};

S2SServerRegisterRes::S2SServerRegisterRes(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:S2S.S2SServerRegisterRes)
}
S2SServerRegisterRes::S2SServerRegisterRes(const S2SServerRegisterRes& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  S2SServerRegisterRes* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.success_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.success_ = from._impl_.success_;
  // @@protoc_insertion_point(copy_constructor:S2S.S2SServerRegisterRes)
}

inline void S2SServerRegisterRes::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.success_){false}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

S2SServerRegisterRes::~S2SServerRegisterRes() {
  // @@protoc_insertion_point(destructor:S2S.S2SServerRegisterRes)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void S2SServerRegisterRes::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void S2SServerRegisterRes::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void S2SServerRegisterRes::Clear() {
// @@protoc_insertion_point(message_clear_start:S2S.S2SServerRegisterRes)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.success_ = false;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* S2SServerRegisterRes::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // bool success = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.success_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
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

uint8_t* S2SServerRegisterRes::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:S2S.S2SServerRegisterRes)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // bool success = 1;
  if (this->_internal_success() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteBoolToArray(1, this->_internal_success(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:S2S.S2SServerRegisterRes)
  return target;
}

size_t S2SServerRegisterRes::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:S2S.S2SServerRegisterRes)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bool success = 1;
  if (this->_internal_success() != 0) {
    total_size += 1 + 1;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData S2SServerRegisterRes::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    S2SServerRegisterRes::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*S2SServerRegisterRes::GetClassData() const { return &_class_data_; }


void S2SServerRegisterRes::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<S2SServerRegisterRes*>(&to_msg);
  auto& from = static_cast<const S2SServerRegisterRes&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:S2S.S2SServerRegisterRes)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_success() != 0) {
    _this->_internal_set_success(from._internal_success());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void S2SServerRegisterRes::CopyFrom(const S2SServerRegisterRes& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:S2S.S2SServerRegisterRes)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool S2SServerRegisterRes::IsInitialized() const {
  return true;
}

void S2SServerRegisterRes::InternalSwap(S2SServerRegisterRes* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.success_, other->_impl_.success_);
}

::PROTOBUF_NAMESPACE_ID::Metadata S2SServerRegisterRes::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_s2s_5fcommon_2eproto_getter, &descriptor_table_s2s_5fcommon_2eproto_once,
      file_level_metadata_s2s_5fcommon_2eproto[3]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace S2S
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::S2S::S2SHeartBeat*
Arena::CreateMaybeMessage< ::S2S::S2SHeartBeat >(Arena* arena) {
  return Arena::CreateMessageInternal< ::S2S::S2SHeartBeat >(arena);
}
template<> PROTOBUF_NOINLINE ::S2S::ServerInfo*
Arena::CreateMaybeMessage< ::S2S::ServerInfo >(Arena* arena) {
  return Arena::CreateMessageInternal< ::S2S::ServerInfo >(arena);
}
template<> PROTOBUF_NOINLINE ::S2S::S2SServerRegisterReq*
Arena::CreateMaybeMessage< ::S2S::S2SServerRegisterReq >(Arena* arena) {
  return Arena::CreateMessageInternal< ::S2S::S2SServerRegisterReq >(arena);
}
template<> PROTOBUF_NOINLINE ::S2S::S2SServerRegisterRes*
Arena::CreateMaybeMessage< ::S2S::S2SServerRegisterRes >(Arena* arena) {
  return Arena::CreateMessageInternal< ::S2S::S2SServerRegisterRes >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
