// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: c2s_common.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_c2s_5fcommon_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_c2s_5fcommon_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_c2s_5fcommon_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_c2s_5fcommon_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_c2s_5fcommon_2eproto;
namespace C2S {
class C2SHeartBeat;
struct C2SHeartBeatDefaultTypeInternal;
extern C2SHeartBeatDefaultTypeInternal _C2SHeartBeat_default_instance_;
}  // namespace C2S
PROTOBUF_NAMESPACE_OPEN
template<> ::C2S::C2SHeartBeat* Arena::CreateMaybeMessage<::C2S::C2SHeartBeat>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace C2S {

enum C2S_MSG_ID : int {
  MSG_ID_NONE = 0,
  MSG_ID_HEARTBEAT = 10001,
  MSG_ID_LOGIN_REQ = 10002,
  MSG_ID_LOGIN_RES = 10003,
  MSG_ID_LOGIN_ZONE_REQ = 10004,
  MSG_ID_LOGIN_ZONE_RES = 10005,
  C2S_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  C2S_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool C2S_MSG_ID_IsValid(int value);
constexpr C2S_MSG_ID C2S_MSG_ID_MIN = MSG_ID_NONE;
constexpr C2S_MSG_ID C2S_MSG_ID_MAX = MSG_ID_LOGIN_ZONE_RES;
constexpr int C2S_MSG_ID_ARRAYSIZE = C2S_MSG_ID_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* C2S_MSG_ID_descriptor();
template<typename T>
inline const std::string& C2S_MSG_ID_Name(T enum_t_value) {
  static_assert(::std::is_same<T, C2S_MSG_ID>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function C2S_MSG_ID_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    C2S_MSG_ID_descriptor(), enum_t_value);
}
inline bool C2S_MSG_ID_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, C2S_MSG_ID* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<C2S_MSG_ID>(
    C2S_MSG_ID_descriptor(), name, value);
}
enum C2S_ERROR_CODE : int {
  EC_SUCCESS = 0,
  EC_GENERRAL_ERROR = 101,
  EC_SERVER_INTERNAL_ERROR = 102,
  EC_SERVER_NOT_READY = 103,
  EC_INVALID_PARAMETER = 104,
  EC_SERVER_BUSY = 105,
  EC_LOGIN_IN_PROGRESS = 106,
  EC_LOGIN_SESSION_HAS_EXPIRED = 107,
  C2S_ERROR_CODE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  C2S_ERROR_CODE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool C2S_ERROR_CODE_IsValid(int value);
constexpr C2S_ERROR_CODE C2S_ERROR_CODE_MIN = EC_SUCCESS;
constexpr C2S_ERROR_CODE C2S_ERROR_CODE_MAX = EC_LOGIN_SESSION_HAS_EXPIRED;
constexpr int C2S_ERROR_CODE_ARRAYSIZE = C2S_ERROR_CODE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* C2S_ERROR_CODE_descriptor();
template<typename T>
inline const std::string& C2S_ERROR_CODE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, C2S_ERROR_CODE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function C2S_ERROR_CODE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    C2S_ERROR_CODE_descriptor(), enum_t_value);
}
inline bool C2S_ERROR_CODE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, C2S_ERROR_CODE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<C2S_ERROR_CODE>(
    C2S_ERROR_CODE_descriptor(), name, value);
}
// ===================================================================

class C2SHeartBeat final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:C2S.C2SHeartBeat) */ {
 public:
  inline C2SHeartBeat() : C2SHeartBeat(nullptr) {}
  ~C2SHeartBeat() override;
  explicit PROTOBUF_CONSTEXPR C2SHeartBeat(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  C2SHeartBeat(const C2SHeartBeat& from);
  C2SHeartBeat(C2SHeartBeat&& from) noexcept
    : C2SHeartBeat() {
    *this = ::std::move(from);
  }

  inline C2SHeartBeat& operator=(const C2SHeartBeat& from) {
    CopyFrom(from);
    return *this;
  }
  inline C2SHeartBeat& operator=(C2SHeartBeat&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const C2SHeartBeat& default_instance() {
    return *internal_default_instance();
  }
  static inline const C2SHeartBeat* internal_default_instance() {
    return reinterpret_cast<const C2SHeartBeat*>(
               &_C2SHeartBeat_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(C2SHeartBeat& a, C2SHeartBeat& b) {
    a.Swap(&b);
  }
  inline void Swap(C2SHeartBeat* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(C2SHeartBeat* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  C2SHeartBeat* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<C2SHeartBeat>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const C2SHeartBeat& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const C2SHeartBeat& from) {
    C2SHeartBeat::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(C2SHeartBeat* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "C2S.C2SHeartBeat";
  }
  protected:
  explicit C2SHeartBeat(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kStateFieldNumber = 1,
  };
  // int32 state = 1;
  void clear_state();
  int32_t state() const;
  void set_state(int32_t value);
  private:
  int32_t _internal_state() const;
  void _internal_set_state(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:C2S.C2SHeartBeat)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    int32_t state_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_c2s_5fcommon_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// C2SHeartBeat

// int32 state = 1;
inline void C2SHeartBeat::clear_state() {
  _impl_.state_ = 0;
}
inline int32_t C2SHeartBeat::_internal_state() const {
  return _impl_.state_;
}
inline int32_t C2SHeartBeat::state() const {
  // @@protoc_insertion_point(field_get:C2S.C2SHeartBeat.state)
  return _internal_state();
}
inline void C2SHeartBeat::_internal_set_state(int32_t value) {
  
  _impl_.state_ = value;
}
inline void C2SHeartBeat::set_state(int32_t value) {
  _internal_set_state(value);
  // @@protoc_insertion_point(field_set:C2S.C2SHeartBeat.state)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace C2S

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::C2S::C2S_MSG_ID> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::C2S::C2S_MSG_ID>() {
  return ::C2S::C2S_MSG_ID_descriptor();
}
template <> struct is_proto_enum< ::C2S::C2S_ERROR_CODE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::C2S::C2S_ERROR_CODE>() {
  return ::C2S::C2S_ERROR_CODE_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_c2s_5fcommon_2eproto
