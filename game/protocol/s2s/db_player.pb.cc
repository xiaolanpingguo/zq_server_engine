// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: db_player.proto

#include "db_player.pb.h"

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
PROTOBUF_CONSTEXPR DBPlayerData::DBPlayerData(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.player_base_info_)*/nullptr
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct DBPlayerDataDefaultTypeInternal {
  PROTOBUF_CONSTEXPR DBPlayerDataDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~DBPlayerDataDefaultTypeInternal() {}
  union {
    DBPlayerData _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 DBPlayerDataDefaultTypeInternal _DBPlayerData_default_instance_;
PROTOBUF_CONSTEXPR DBPlayerBaseInfo::DBPlayerBaseInfo(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.nickname_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.ip_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.uid_)*/int64_t{0}
  , /*decltype(_impl_.openid_)*/int64_t{0}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct DBPlayerBaseInfoDefaultTypeInternal {
  PROTOBUF_CONSTEXPR DBPlayerBaseInfoDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~DBPlayerBaseInfoDefaultTypeInternal() {}
  union {
    DBPlayerBaseInfo _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 DBPlayerBaseInfoDefaultTypeInternal _DBPlayerBaseInfo_default_instance_;
}  // namespace S2S
static ::_pb::Metadata file_level_metadata_db_5fplayer_2eproto[2];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_db_5fplayer_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_db_5fplayer_2eproto = nullptr;

const uint32_t TableStruct_db_5fplayer_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::S2S::DBPlayerData, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::S2S::DBPlayerData, _impl_.player_base_info_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::S2S::DBPlayerBaseInfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::S2S::DBPlayerBaseInfo, _impl_.uid_),
  PROTOBUF_FIELD_OFFSET(::S2S::DBPlayerBaseInfo, _impl_.openid_),
  PROTOBUF_FIELD_OFFSET(::S2S::DBPlayerBaseInfo, _impl_.nickname_),
  PROTOBUF_FIELD_OFFSET(::S2S::DBPlayerBaseInfo, _impl_.ip_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::S2S::DBPlayerData)},
  { 7, -1, -1, sizeof(::S2S::DBPlayerBaseInfo)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::S2S::_DBPlayerData_default_instance_._instance,
  &::S2S::_DBPlayerBaseInfo_default_instance_._instance,
};

const char descriptor_table_protodef_db_5fplayer_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\017db_player.proto\022\003S2S\"\?\n\014DBPlayerData\022/"
  "\n\020player_base_info\030\001 \001(\0132\025.S2S.DBPlayerB"
  "aseInfo\"M\n\020DBPlayerBaseInfo\022\013\n\003uid\030\001 \001(\003"
  "\022\016\n\006openid\030\002 \001(\003\022\020\n\010nickname\030\003 \001(\t\022\n\n\002ip"
  "\030\004 \001(\tb\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_db_5fplayer_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_db_5fplayer_2eproto = {
    false, false, 174, descriptor_table_protodef_db_5fplayer_2eproto,
    "db_player.proto",
    &descriptor_table_db_5fplayer_2eproto_once, nullptr, 0, 2,
    schemas, file_default_instances, TableStruct_db_5fplayer_2eproto::offsets,
    file_level_metadata_db_5fplayer_2eproto, file_level_enum_descriptors_db_5fplayer_2eproto,
    file_level_service_descriptors_db_5fplayer_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_db_5fplayer_2eproto_getter() {
  return &descriptor_table_db_5fplayer_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_db_5fplayer_2eproto(&descriptor_table_db_5fplayer_2eproto);
namespace S2S {

// ===================================================================

class DBPlayerData::_Internal {
 public:
  static const ::S2S::DBPlayerBaseInfo& player_base_info(const DBPlayerData* msg);
};

const ::S2S::DBPlayerBaseInfo&
DBPlayerData::_Internal::player_base_info(const DBPlayerData* msg) {
  return *msg->_impl_.player_base_info_;
}
DBPlayerData::DBPlayerData(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:S2S.DBPlayerData)
}
DBPlayerData::DBPlayerData(const DBPlayerData& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  DBPlayerData* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.player_base_info_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_player_base_info()) {
    _this->_impl_.player_base_info_ = new ::S2S::DBPlayerBaseInfo(*from._impl_.player_base_info_);
  }
  // @@protoc_insertion_point(copy_constructor:S2S.DBPlayerData)
}

inline void DBPlayerData::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.player_base_info_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

DBPlayerData::~DBPlayerData() {
  // @@protoc_insertion_point(destructor:S2S.DBPlayerData)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void DBPlayerData::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete _impl_.player_base_info_;
}

void DBPlayerData::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void DBPlayerData::Clear() {
// @@protoc_insertion_point(message_clear_start:S2S.DBPlayerData)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaForAllocation() == nullptr && _impl_.player_base_info_ != nullptr) {
    delete _impl_.player_base_info_;
  }
  _impl_.player_base_info_ = nullptr;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* DBPlayerData::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .S2S.DBPlayerBaseInfo player_base_info = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_player_base_info(), ptr);
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

uint8_t* DBPlayerData::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:S2S.DBPlayerData)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .S2S.DBPlayerBaseInfo player_base_info = 1;
  if (this->_internal_has_player_base_info()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, _Internal::player_base_info(this),
        _Internal::player_base_info(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:S2S.DBPlayerData)
  return target;
}

size_t DBPlayerData::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:S2S.DBPlayerData)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .S2S.DBPlayerBaseInfo player_base_info = 1;
  if (this->_internal_has_player_base_info()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.player_base_info_);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData DBPlayerData::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    DBPlayerData::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*DBPlayerData::GetClassData() const { return &_class_data_; }


void DBPlayerData::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<DBPlayerData*>(&to_msg);
  auto& from = static_cast<const DBPlayerData&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:S2S.DBPlayerData)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_player_base_info()) {
    _this->_internal_mutable_player_base_info()->::S2S::DBPlayerBaseInfo::MergeFrom(
        from._internal_player_base_info());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void DBPlayerData::CopyFrom(const DBPlayerData& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:S2S.DBPlayerData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DBPlayerData::IsInitialized() const {
  return true;
}

void DBPlayerData::InternalSwap(DBPlayerData* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.player_base_info_, other->_impl_.player_base_info_);
}

::PROTOBUF_NAMESPACE_ID::Metadata DBPlayerData::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_db_5fplayer_2eproto_getter, &descriptor_table_db_5fplayer_2eproto_once,
      file_level_metadata_db_5fplayer_2eproto[0]);
}

// ===================================================================

class DBPlayerBaseInfo::_Internal {
 public:
};

DBPlayerBaseInfo::DBPlayerBaseInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:S2S.DBPlayerBaseInfo)
}
DBPlayerBaseInfo::DBPlayerBaseInfo(const DBPlayerBaseInfo& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  DBPlayerBaseInfo* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.nickname_){}
    , decltype(_impl_.ip_){}
    , decltype(_impl_.uid_){}
    , decltype(_impl_.openid_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.nickname_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.nickname_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_nickname().empty()) {
    _this->_impl_.nickname_.Set(from._internal_nickname(), 
      _this->GetArenaForAllocation());
  }
  _impl_.ip_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.ip_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_ip().empty()) {
    _this->_impl_.ip_.Set(from._internal_ip(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.uid_, &from._impl_.uid_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.openid_) -
    reinterpret_cast<char*>(&_impl_.uid_)) + sizeof(_impl_.openid_));
  // @@protoc_insertion_point(copy_constructor:S2S.DBPlayerBaseInfo)
}

inline void DBPlayerBaseInfo::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.nickname_){}
    , decltype(_impl_.ip_){}
    , decltype(_impl_.uid_){int64_t{0}}
    , decltype(_impl_.openid_){int64_t{0}}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.nickname_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.nickname_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.ip_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.ip_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

DBPlayerBaseInfo::~DBPlayerBaseInfo() {
  // @@protoc_insertion_point(destructor:S2S.DBPlayerBaseInfo)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void DBPlayerBaseInfo::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.nickname_.Destroy();
  _impl_.ip_.Destroy();
}

void DBPlayerBaseInfo::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void DBPlayerBaseInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:S2S.DBPlayerBaseInfo)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.nickname_.ClearToEmpty();
  _impl_.ip_.ClearToEmpty();
  ::memset(&_impl_.uid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.openid_) -
      reinterpret_cast<char*>(&_impl_.uid_)) + sizeof(_impl_.openid_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* DBPlayerBaseInfo::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int64 uid = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.uid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 openid = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _impl_.openid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string nickname = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          auto str = _internal_mutable_nickname();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "S2S.DBPlayerBaseInfo.nickname"));
        } else
          goto handle_unusual;
        continue;
      // string ip = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 34)) {
          auto str = _internal_mutable_ip();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "S2S.DBPlayerBaseInfo.ip"));
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

uint8_t* DBPlayerBaseInfo::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:S2S.DBPlayerBaseInfo)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 uid = 1;
  if (this->_internal_uid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(1, this->_internal_uid(), target);
  }

  // int64 openid = 2;
  if (this->_internal_openid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(2, this->_internal_openid(), target);
  }

  // string nickname = 3;
  if (!this->_internal_nickname().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_nickname().data(), static_cast<int>(this->_internal_nickname().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "S2S.DBPlayerBaseInfo.nickname");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_nickname(), target);
  }

  // string ip = 4;
  if (!this->_internal_ip().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_ip().data(), static_cast<int>(this->_internal_ip().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "S2S.DBPlayerBaseInfo.ip");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_ip(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:S2S.DBPlayerBaseInfo)
  return target;
}

size_t DBPlayerBaseInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:S2S.DBPlayerBaseInfo)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string nickname = 3;
  if (!this->_internal_nickname().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_nickname());
  }

  // string ip = 4;
  if (!this->_internal_ip().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_ip());
  }

  // int64 uid = 1;
  if (this->_internal_uid() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_uid());
  }

  // int64 openid = 2;
  if (this->_internal_openid() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_openid());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData DBPlayerBaseInfo::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    DBPlayerBaseInfo::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*DBPlayerBaseInfo::GetClassData() const { return &_class_data_; }


void DBPlayerBaseInfo::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<DBPlayerBaseInfo*>(&to_msg);
  auto& from = static_cast<const DBPlayerBaseInfo&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:S2S.DBPlayerBaseInfo)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_nickname().empty()) {
    _this->_internal_set_nickname(from._internal_nickname());
  }
  if (!from._internal_ip().empty()) {
    _this->_internal_set_ip(from._internal_ip());
  }
  if (from._internal_uid() != 0) {
    _this->_internal_set_uid(from._internal_uid());
  }
  if (from._internal_openid() != 0) {
    _this->_internal_set_openid(from._internal_openid());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void DBPlayerBaseInfo::CopyFrom(const DBPlayerBaseInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:S2S.DBPlayerBaseInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DBPlayerBaseInfo::IsInitialized() const {
  return true;
}

void DBPlayerBaseInfo::InternalSwap(DBPlayerBaseInfo* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.nickname_, lhs_arena,
      &other->_impl_.nickname_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.ip_, lhs_arena,
      &other->_impl_.ip_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(DBPlayerBaseInfo, _impl_.openid_)
      + sizeof(DBPlayerBaseInfo::_impl_.openid_)
      - PROTOBUF_FIELD_OFFSET(DBPlayerBaseInfo, _impl_.uid_)>(
          reinterpret_cast<char*>(&_impl_.uid_),
          reinterpret_cast<char*>(&other->_impl_.uid_));
}

::PROTOBUF_NAMESPACE_ID::Metadata DBPlayerBaseInfo::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_db_5fplayer_2eproto_getter, &descriptor_table_db_5fplayer_2eproto_once,
      file_level_metadata_db_5fplayer_2eproto[1]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace S2S
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::S2S::DBPlayerData*
Arena::CreateMaybeMessage< ::S2S::DBPlayerData >(Arena* arena) {
  return Arena::CreateMessageInternal< ::S2S::DBPlayerData >(arena);
}
template<> PROTOBUF_NOINLINE ::S2S::DBPlayerBaseInfo*
Arena::CreateMaybeMessage< ::S2S::DBPlayerBaseInfo >(Arena* arena) {
  return Arena::CreateMessageInternal< ::S2S::DBPlayerBaseInfo >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
