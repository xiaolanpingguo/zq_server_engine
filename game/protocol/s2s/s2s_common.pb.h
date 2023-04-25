// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: s2s_common.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_s2s_5fcommon_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_s2s_5fcommon_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_s2s_5fcommon_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_s2s_5fcommon_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_s2s_5fcommon_2eproto;
namespace S2SMsg {
class S2SHeartBeat;
struct S2SHeartBeatDefaultTypeInternal;
extern S2SHeartBeatDefaultTypeInternal _S2SHeartBeat_default_instance_;
class S2SServerRegisterReq;
struct S2SServerRegisterReqDefaultTypeInternal;
extern S2SServerRegisterReqDefaultTypeInternal _S2SServerRegisterReq_default_instance_;
class S2SServerRegisterRes;
struct S2SServerRegisterResDefaultTypeInternal;
extern S2SServerRegisterResDefaultTypeInternal _S2SServerRegisterRes_default_instance_;
class ServerInfo;
struct ServerInfoDefaultTypeInternal;
extern ServerInfoDefaultTypeInternal _ServerInfo_default_instance_;
}  // namespace S2SMsg
PROTOBUF_NAMESPACE_OPEN
template<> ::S2SMsg::S2SHeartBeat* Arena::CreateMaybeMessage<::S2SMsg::S2SHeartBeat>(Arena*);
template<> ::S2SMsg::S2SServerRegisterReq* Arena::CreateMaybeMessage<::S2SMsg::S2SServerRegisterReq>(Arena*);
template<> ::S2SMsg::S2SServerRegisterRes* Arena::CreateMaybeMessage<::S2SMsg::S2SServerRegisterRes>(Arena*);
template<> ::S2SMsg::ServerInfo* Arena::CreateMaybeMessage<::S2SMsg::ServerInfo>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace S2SMsg {

enum S2S_MSG_ID : int {
  S2S_ID_NONE = 0,
  S2S_ID_HEARTBEAT = 101,
  S2S_ID_SERVER_REGSTER_REQ = 102,
  S2S_ID_SERVER_REGSTER_RES = 103,
  S2S_MSG_ID_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  S2S_MSG_ID_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool S2S_MSG_ID_IsValid(int value);
constexpr S2S_MSG_ID S2S_MSG_ID_MIN = S2S_ID_NONE;
constexpr S2S_MSG_ID S2S_MSG_ID_MAX = S2S_ID_SERVER_REGSTER_RES;
constexpr int S2S_MSG_ID_ARRAYSIZE = S2S_MSG_ID_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* S2S_MSG_ID_descriptor();
template<typename T>
inline const std::string& S2S_MSG_ID_Name(T enum_t_value) {
  static_assert(::std::is_same<T, S2S_MSG_ID>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function S2S_MSG_ID_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    S2S_MSG_ID_descriptor(), enum_t_value);
}
inline bool S2S_MSG_ID_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, S2S_MSG_ID* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<S2S_MSG_ID>(
    S2S_MSG_ID_descriptor(), name, value);
}
// ===================================================================

class S2SHeartBeat final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:S2SMsg.S2SHeartBeat) */ {
 public:
  inline S2SHeartBeat() : S2SHeartBeat(nullptr) {}
  ~S2SHeartBeat() override;
  explicit PROTOBUF_CONSTEXPR S2SHeartBeat(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  S2SHeartBeat(const S2SHeartBeat& from);
  S2SHeartBeat(S2SHeartBeat&& from) noexcept
    : S2SHeartBeat() {
    *this = ::std::move(from);
  }

  inline S2SHeartBeat& operator=(const S2SHeartBeat& from) {
    CopyFrom(from);
    return *this;
  }
  inline S2SHeartBeat& operator=(S2SHeartBeat&& from) noexcept {
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
  static const S2SHeartBeat& default_instance() {
    return *internal_default_instance();
  }
  static inline const S2SHeartBeat* internal_default_instance() {
    return reinterpret_cast<const S2SHeartBeat*>(
               &_S2SHeartBeat_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(S2SHeartBeat& a, S2SHeartBeat& b) {
    a.Swap(&b);
  }
  inline void Swap(S2SHeartBeat* other) {
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
  void UnsafeArenaSwap(S2SHeartBeat* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  S2SHeartBeat* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<S2SHeartBeat>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const S2SHeartBeat& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const S2SHeartBeat& from) {
    S2SHeartBeat::MergeImpl(*this, from);
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
  void InternalSwap(S2SHeartBeat* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "S2SMsg.S2SHeartBeat";
  }
  protected:
  explicit S2SHeartBeat(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kServerTypeFieldNumber = 1,
    kServerIdFieldNumber = 2,
    kStateFieldNumber = 3,
  };
  // int32 server_type = 1;
  void clear_server_type();
  int32_t server_type() const;
  void set_server_type(int32_t value);
  private:
  int32_t _internal_server_type() const;
  void _internal_set_server_type(int32_t value);
  public:

  // int32 server_id = 2;
  void clear_server_id();
  int32_t server_id() const;
  void set_server_id(int32_t value);
  private:
  int32_t _internal_server_id() const;
  void _internal_set_server_id(int32_t value);
  public:

  // int32 state = 3;
  void clear_state();
  int32_t state() const;
  void set_state(int32_t value);
  private:
  int32_t _internal_state() const;
  void _internal_set_state(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:S2SMsg.S2SHeartBeat)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    int32_t server_type_;
    int32_t server_id_;
    int32_t state_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_s2s_5fcommon_2eproto;
};
// -------------------------------------------------------------------

class ServerInfo final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:S2SMsg.ServerInfo) */ {
 public:
  inline ServerInfo() : ServerInfo(nullptr) {}
  ~ServerInfo() override;
  explicit PROTOBUF_CONSTEXPR ServerInfo(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ServerInfo(const ServerInfo& from);
  ServerInfo(ServerInfo&& from) noexcept
    : ServerInfo() {
    *this = ::std::move(from);
  }

  inline ServerInfo& operator=(const ServerInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline ServerInfo& operator=(ServerInfo&& from) noexcept {
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
  static const ServerInfo& default_instance() {
    return *internal_default_instance();
  }
  static inline const ServerInfo* internal_default_instance() {
    return reinterpret_cast<const ServerInfo*>(
               &_ServerInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(ServerInfo& a, ServerInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(ServerInfo* other) {
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
  void UnsafeArenaSwap(ServerInfo* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ServerInfo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ServerInfo>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ServerInfo& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const ServerInfo& from) {
    ServerInfo::MergeImpl(*this, from);
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
  void InternalSwap(ServerInfo* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "S2SMsg.ServerInfo";
  }
  protected:
  explicit ServerInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kIpFieldNumber = 4,
    kServerTypeFieldNumber = 1,
    kServerIdFieldNumber = 2,
    kPortFieldNumber = 3,
  };
  // string ip = 4;
  void clear_ip();
  const std::string& ip() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_ip(ArgT0&& arg0, ArgT... args);
  std::string* mutable_ip();
  PROTOBUF_NODISCARD std::string* release_ip();
  void set_allocated_ip(std::string* ip);
  private:
  const std::string& _internal_ip() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_ip(const std::string& value);
  std::string* _internal_mutable_ip();
  public:

  // int32 server_type = 1;
  void clear_server_type();
  int32_t server_type() const;
  void set_server_type(int32_t value);
  private:
  int32_t _internal_server_type() const;
  void _internal_set_server_type(int32_t value);
  public:

  // int32 server_id = 2;
  void clear_server_id();
  int32_t server_id() const;
  void set_server_id(int32_t value);
  private:
  int32_t _internal_server_id() const;
  void _internal_set_server_id(int32_t value);
  public:

  // int32 port = 3;
  void clear_port();
  int32_t port() const;
  void set_port(int32_t value);
  private:
  int32_t _internal_port() const;
  void _internal_set_port(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:S2SMsg.ServerInfo)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr ip_;
    int32_t server_type_;
    int32_t server_id_;
    int32_t port_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_s2s_5fcommon_2eproto;
};
// -------------------------------------------------------------------

class S2SServerRegisterReq final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:S2SMsg.S2SServerRegisterReq) */ {
 public:
  inline S2SServerRegisterReq() : S2SServerRegisterReq(nullptr) {}
  ~S2SServerRegisterReq() override;
  explicit PROTOBUF_CONSTEXPR S2SServerRegisterReq(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  S2SServerRegisterReq(const S2SServerRegisterReq& from);
  S2SServerRegisterReq(S2SServerRegisterReq&& from) noexcept
    : S2SServerRegisterReq() {
    *this = ::std::move(from);
  }

  inline S2SServerRegisterReq& operator=(const S2SServerRegisterReq& from) {
    CopyFrom(from);
    return *this;
  }
  inline S2SServerRegisterReq& operator=(S2SServerRegisterReq&& from) noexcept {
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
  static const S2SServerRegisterReq& default_instance() {
    return *internal_default_instance();
  }
  static inline const S2SServerRegisterReq* internal_default_instance() {
    return reinterpret_cast<const S2SServerRegisterReq*>(
               &_S2SServerRegisterReq_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(S2SServerRegisterReq& a, S2SServerRegisterReq& b) {
    a.Swap(&b);
  }
  inline void Swap(S2SServerRegisterReq* other) {
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
  void UnsafeArenaSwap(S2SServerRegisterReq* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  S2SServerRegisterReq* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<S2SServerRegisterReq>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const S2SServerRegisterReq& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const S2SServerRegisterReq& from) {
    S2SServerRegisterReq::MergeImpl(*this, from);
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
  void InternalSwap(S2SServerRegisterReq* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "S2SMsg.S2SServerRegisterReq";
  }
  protected:
  explicit S2SServerRegisterReq(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kServerInfoFieldNumber = 1,
  };
  // .S2SMsg.ServerInfo server_info = 1;
  bool has_server_info() const;
  private:
  bool _internal_has_server_info() const;
  public:
  void clear_server_info();
  const ::S2SMsg::ServerInfo& server_info() const;
  PROTOBUF_NODISCARD ::S2SMsg::ServerInfo* release_server_info();
  ::S2SMsg::ServerInfo* mutable_server_info();
  void set_allocated_server_info(::S2SMsg::ServerInfo* server_info);
  private:
  const ::S2SMsg::ServerInfo& _internal_server_info() const;
  ::S2SMsg::ServerInfo* _internal_mutable_server_info();
  public:
  void unsafe_arena_set_allocated_server_info(
      ::S2SMsg::ServerInfo* server_info);
  ::S2SMsg::ServerInfo* unsafe_arena_release_server_info();

  // @@protoc_insertion_point(class_scope:S2SMsg.S2SServerRegisterReq)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::S2SMsg::ServerInfo* server_info_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_s2s_5fcommon_2eproto;
};
// -------------------------------------------------------------------

class S2SServerRegisterRes final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:S2SMsg.S2SServerRegisterRes) */ {
 public:
  inline S2SServerRegisterRes() : S2SServerRegisterRes(nullptr) {}
  ~S2SServerRegisterRes() override;
  explicit PROTOBUF_CONSTEXPR S2SServerRegisterRes(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  S2SServerRegisterRes(const S2SServerRegisterRes& from);
  S2SServerRegisterRes(S2SServerRegisterRes&& from) noexcept
    : S2SServerRegisterRes() {
    *this = ::std::move(from);
  }

  inline S2SServerRegisterRes& operator=(const S2SServerRegisterRes& from) {
    CopyFrom(from);
    return *this;
  }
  inline S2SServerRegisterRes& operator=(S2SServerRegisterRes&& from) noexcept {
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
  static const S2SServerRegisterRes& default_instance() {
    return *internal_default_instance();
  }
  static inline const S2SServerRegisterRes* internal_default_instance() {
    return reinterpret_cast<const S2SServerRegisterRes*>(
               &_S2SServerRegisterRes_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  friend void swap(S2SServerRegisterRes& a, S2SServerRegisterRes& b) {
    a.Swap(&b);
  }
  inline void Swap(S2SServerRegisterRes* other) {
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
  void UnsafeArenaSwap(S2SServerRegisterRes* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  S2SServerRegisterRes* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<S2SServerRegisterRes>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const S2SServerRegisterRes& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const S2SServerRegisterRes& from) {
    S2SServerRegisterRes::MergeImpl(*this, from);
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
  void InternalSwap(S2SServerRegisterRes* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "S2SMsg.S2SServerRegisterRes";
  }
  protected:
  explicit S2SServerRegisterRes(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kErrorMsgFieldNumber = 2,
    kSuccessFieldNumber = 1,
  };
  // string error_msg = 2;
  void clear_error_msg();
  const std::string& error_msg() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_error_msg(ArgT0&& arg0, ArgT... args);
  std::string* mutable_error_msg();
  PROTOBUF_NODISCARD std::string* release_error_msg();
  void set_allocated_error_msg(std::string* error_msg);
  private:
  const std::string& _internal_error_msg() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_error_msg(const std::string& value);
  std::string* _internal_mutable_error_msg();
  public:

  // bool success = 1;
  void clear_success();
  bool success() const;
  void set_success(bool value);
  private:
  bool _internal_success() const;
  void _internal_set_success(bool value);
  public:

  // @@protoc_insertion_point(class_scope:S2SMsg.S2SServerRegisterRes)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr error_msg_;
    bool success_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_s2s_5fcommon_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// S2SHeartBeat

// int32 server_type = 1;
inline void S2SHeartBeat::clear_server_type() {
  _impl_.server_type_ = 0;
}
inline int32_t S2SHeartBeat::_internal_server_type() const {
  return _impl_.server_type_;
}
inline int32_t S2SHeartBeat::server_type() const {
  // @@protoc_insertion_point(field_get:S2SMsg.S2SHeartBeat.server_type)
  return _internal_server_type();
}
inline void S2SHeartBeat::_internal_set_server_type(int32_t value) {
  
  _impl_.server_type_ = value;
}
inline void S2SHeartBeat::set_server_type(int32_t value) {
  _internal_set_server_type(value);
  // @@protoc_insertion_point(field_set:S2SMsg.S2SHeartBeat.server_type)
}

// int32 server_id = 2;
inline void S2SHeartBeat::clear_server_id() {
  _impl_.server_id_ = 0;
}
inline int32_t S2SHeartBeat::_internal_server_id() const {
  return _impl_.server_id_;
}
inline int32_t S2SHeartBeat::server_id() const {
  // @@protoc_insertion_point(field_get:S2SMsg.S2SHeartBeat.server_id)
  return _internal_server_id();
}
inline void S2SHeartBeat::_internal_set_server_id(int32_t value) {
  
  _impl_.server_id_ = value;
}
inline void S2SHeartBeat::set_server_id(int32_t value) {
  _internal_set_server_id(value);
  // @@protoc_insertion_point(field_set:S2SMsg.S2SHeartBeat.server_id)
}

// int32 state = 3;
inline void S2SHeartBeat::clear_state() {
  _impl_.state_ = 0;
}
inline int32_t S2SHeartBeat::_internal_state() const {
  return _impl_.state_;
}
inline int32_t S2SHeartBeat::state() const {
  // @@protoc_insertion_point(field_get:S2SMsg.S2SHeartBeat.state)
  return _internal_state();
}
inline void S2SHeartBeat::_internal_set_state(int32_t value) {
  
  _impl_.state_ = value;
}
inline void S2SHeartBeat::set_state(int32_t value) {
  _internal_set_state(value);
  // @@protoc_insertion_point(field_set:S2SMsg.S2SHeartBeat.state)
}

// -------------------------------------------------------------------

// ServerInfo

// int32 server_type = 1;
inline void ServerInfo::clear_server_type() {
  _impl_.server_type_ = 0;
}
inline int32_t ServerInfo::_internal_server_type() const {
  return _impl_.server_type_;
}
inline int32_t ServerInfo::server_type() const {
  // @@protoc_insertion_point(field_get:S2SMsg.ServerInfo.server_type)
  return _internal_server_type();
}
inline void ServerInfo::_internal_set_server_type(int32_t value) {
  
  _impl_.server_type_ = value;
}
inline void ServerInfo::set_server_type(int32_t value) {
  _internal_set_server_type(value);
  // @@protoc_insertion_point(field_set:S2SMsg.ServerInfo.server_type)
}

// int32 server_id = 2;
inline void ServerInfo::clear_server_id() {
  _impl_.server_id_ = 0;
}
inline int32_t ServerInfo::_internal_server_id() const {
  return _impl_.server_id_;
}
inline int32_t ServerInfo::server_id() const {
  // @@protoc_insertion_point(field_get:S2SMsg.ServerInfo.server_id)
  return _internal_server_id();
}
inline void ServerInfo::_internal_set_server_id(int32_t value) {
  
  _impl_.server_id_ = value;
}
inline void ServerInfo::set_server_id(int32_t value) {
  _internal_set_server_id(value);
  // @@protoc_insertion_point(field_set:S2SMsg.ServerInfo.server_id)
}

// int32 port = 3;
inline void ServerInfo::clear_port() {
  _impl_.port_ = 0;
}
inline int32_t ServerInfo::_internal_port() const {
  return _impl_.port_;
}
inline int32_t ServerInfo::port() const {
  // @@protoc_insertion_point(field_get:S2SMsg.ServerInfo.port)
  return _internal_port();
}
inline void ServerInfo::_internal_set_port(int32_t value) {
  
  _impl_.port_ = value;
}
inline void ServerInfo::set_port(int32_t value) {
  _internal_set_port(value);
  // @@protoc_insertion_point(field_set:S2SMsg.ServerInfo.port)
}

// string ip = 4;
inline void ServerInfo::clear_ip() {
  _impl_.ip_.ClearToEmpty();
}
inline const std::string& ServerInfo::ip() const {
  // @@protoc_insertion_point(field_get:S2SMsg.ServerInfo.ip)
  return _internal_ip();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ServerInfo::set_ip(ArgT0&& arg0, ArgT... args) {
 
 _impl_.ip_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:S2SMsg.ServerInfo.ip)
}
inline std::string* ServerInfo::mutable_ip() {
  std::string* _s = _internal_mutable_ip();
  // @@protoc_insertion_point(field_mutable:S2SMsg.ServerInfo.ip)
  return _s;
}
inline const std::string& ServerInfo::_internal_ip() const {
  return _impl_.ip_.Get();
}
inline void ServerInfo::_internal_set_ip(const std::string& value) {
  
  _impl_.ip_.Set(value, GetArenaForAllocation());
}
inline std::string* ServerInfo::_internal_mutable_ip() {
  
  return _impl_.ip_.Mutable(GetArenaForAllocation());
}
inline std::string* ServerInfo::release_ip() {
  // @@protoc_insertion_point(field_release:S2SMsg.ServerInfo.ip)
  return _impl_.ip_.Release();
}
inline void ServerInfo::set_allocated_ip(std::string* ip) {
  if (ip != nullptr) {
    
  } else {
    
  }
  _impl_.ip_.SetAllocated(ip, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.ip_.IsDefault()) {
    _impl_.ip_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:S2SMsg.ServerInfo.ip)
}

// -------------------------------------------------------------------

// S2SServerRegisterReq

// .S2SMsg.ServerInfo server_info = 1;
inline bool S2SServerRegisterReq::_internal_has_server_info() const {
  return this != internal_default_instance() && _impl_.server_info_ != nullptr;
}
inline bool S2SServerRegisterReq::has_server_info() const {
  return _internal_has_server_info();
}
inline void S2SServerRegisterReq::clear_server_info() {
  if (GetArenaForAllocation() == nullptr && _impl_.server_info_ != nullptr) {
    delete _impl_.server_info_;
  }
  _impl_.server_info_ = nullptr;
}
inline const ::S2SMsg::ServerInfo& S2SServerRegisterReq::_internal_server_info() const {
  const ::S2SMsg::ServerInfo* p = _impl_.server_info_;
  return p != nullptr ? *p : reinterpret_cast<const ::S2SMsg::ServerInfo&>(
      ::S2SMsg::_ServerInfo_default_instance_);
}
inline const ::S2SMsg::ServerInfo& S2SServerRegisterReq::server_info() const {
  // @@protoc_insertion_point(field_get:S2SMsg.S2SServerRegisterReq.server_info)
  return _internal_server_info();
}
inline void S2SServerRegisterReq::unsafe_arena_set_allocated_server_info(
    ::S2SMsg::ServerInfo* server_info) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.server_info_);
  }
  _impl_.server_info_ = server_info;
  if (server_info) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:S2SMsg.S2SServerRegisterReq.server_info)
}
inline ::S2SMsg::ServerInfo* S2SServerRegisterReq::release_server_info() {
  
  ::S2SMsg::ServerInfo* temp = _impl_.server_info_;
  _impl_.server_info_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::S2SMsg::ServerInfo* S2SServerRegisterReq::unsafe_arena_release_server_info() {
  // @@protoc_insertion_point(field_release:S2SMsg.S2SServerRegisterReq.server_info)
  
  ::S2SMsg::ServerInfo* temp = _impl_.server_info_;
  _impl_.server_info_ = nullptr;
  return temp;
}
inline ::S2SMsg::ServerInfo* S2SServerRegisterReq::_internal_mutable_server_info() {
  
  if (_impl_.server_info_ == nullptr) {
    auto* p = CreateMaybeMessage<::S2SMsg::ServerInfo>(GetArenaForAllocation());
    _impl_.server_info_ = p;
  }
  return _impl_.server_info_;
}
inline ::S2SMsg::ServerInfo* S2SServerRegisterReq::mutable_server_info() {
  ::S2SMsg::ServerInfo* _msg = _internal_mutable_server_info();
  // @@protoc_insertion_point(field_mutable:S2SMsg.S2SServerRegisterReq.server_info)
  return _msg;
}
inline void S2SServerRegisterReq::set_allocated_server_info(::S2SMsg::ServerInfo* server_info) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete _impl_.server_info_;
  }
  if (server_info) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(server_info);
    if (message_arena != submessage_arena) {
      server_info = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, server_info, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.server_info_ = server_info;
  // @@protoc_insertion_point(field_set_allocated:S2SMsg.S2SServerRegisterReq.server_info)
}

// -------------------------------------------------------------------

// S2SServerRegisterRes

// bool success = 1;
inline void S2SServerRegisterRes::clear_success() {
  _impl_.success_ = false;
}
inline bool S2SServerRegisterRes::_internal_success() const {
  return _impl_.success_;
}
inline bool S2SServerRegisterRes::success() const {
  // @@protoc_insertion_point(field_get:S2SMsg.S2SServerRegisterRes.success)
  return _internal_success();
}
inline void S2SServerRegisterRes::_internal_set_success(bool value) {
  
  _impl_.success_ = value;
}
inline void S2SServerRegisterRes::set_success(bool value) {
  _internal_set_success(value);
  // @@protoc_insertion_point(field_set:S2SMsg.S2SServerRegisterRes.success)
}

// string error_msg = 2;
inline void S2SServerRegisterRes::clear_error_msg() {
  _impl_.error_msg_.ClearToEmpty();
}
inline const std::string& S2SServerRegisterRes::error_msg() const {
  // @@protoc_insertion_point(field_get:S2SMsg.S2SServerRegisterRes.error_msg)
  return _internal_error_msg();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void S2SServerRegisterRes::set_error_msg(ArgT0&& arg0, ArgT... args) {
 
 _impl_.error_msg_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:S2SMsg.S2SServerRegisterRes.error_msg)
}
inline std::string* S2SServerRegisterRes::mutable_error_msg() {
  std::string* _s = _internal_mutable_error_msg();
  // @@protoc_insertion_point(field_mutable:S2SMsg.S2SServerRegisterRes.error_msg)
  return _s;
}
inline const std::string& S2SServerRegisterRes::_internal_error_msg() const {
  return _impl_.error_msg_.Get();
}
inline void S2SServerRegisterRes::_internal_set_error_msg(const std::string& value) {
  
  _impl_.error_msg_.Set(value, GetArenaForAllocation());
}
inline std::string* S2SServerRegisterRes::_internal_mutable_error_msg() {
  
  return _impl_.error_msg_.Mutable(GetArenaForAllocation());
}
inline std::string* S2SServerRegisterRes::release_error_msg() {
  // @@protoc_insertion_point(field_release:S2SMsg.S2SServerRegisterRes.error_msg)
  return _impl_.error_msg_.Release();
}
inline void S2SServerRegisterRes::set_allocated_error_msg(std::string* error_msg) {
  if (error_msg != nullptr) {
    
  } else {
    
  }
  _impl_.error_msg_.SetAllocated(error_msg, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.error_msg_.IsDefault()) {
    _impl_.error_msg_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:S2SMsg.S2SServerRegisterRes.error_msg)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace S2SMsg

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::S2SMsg::S2S_MSG_ID> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::S2SMsg::S2S_MSG_ID>() {
  return ::S2SMsg::S2S_MSG_ID_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_s2s_5fcommon_2eproto
