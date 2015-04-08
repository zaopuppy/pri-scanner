// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: comm_server.proto

#ifndef PROTOBUF_comm_5fserver_2eproto__INCLUDED
#define PROTOBUF_comm_5fserver_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace letsmidi {
namespace monsys {
namespace protocol {
namespace commserver {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_comm_5fserver_2eproto();
void protobuf_AssignDesc_comm_5fserver_2eproto();
void protobuf_ShutdownFile_comm_5fserver_2eproto();

class CommServerMsg;
class Register;
class RegisterRsp;
class RequestToken;
class RequestTokenRsp;

enum MsgType {
  HEARTBEAT = 1,
  REGISTER = 3,
  REGISTER_RSP = 4,
  REQUEST_TOKEN = 5,
  REQUEST_TOKEN_RSP = 6
};
bool MsgType_IsValid(int value);
const MsgType MsgType_MIN = HEARTBEAT;
const MsgType MsgType_MAX = REQUEST_TOKEN_RSP;
const int MsgType_ARRAYSIZE = MsgType_MAX + 1;

const ::google::protobuf::EnumDescriptor* MsgType_descriptor();
inline const ::std::string& MsgType_Name(MsgType value) {
  return ::google::protobuf::internal::NameOfEnum(
    MsgType_descriptor(), value);
}
inline bool MsgType_Parse(
    const ::std::string& name, MsgType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<MsgType>(
    MsgType_descriptor(), name, value);
}
// ===================================================================

class CommServerMsg : public ::google::protobuf::Message {
 public:
  CommServerMsg();
  virtual ~CommServerMsg();

  CommServerMsg(const CommServerMsg& from);

  inline CommServerMsg& operator=(const CommServerMsg& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CommServerMsg& default_instance();

  void Swap(CommServerMsg* other);

  // implements Message ----------------------------------------------

  CommServerMsg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CommServerMsg& from);
  void MergeFrom(const CommServerMsg& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 version = 1;
  inline bool has_version() const;
  inline void clear_version();
  static const int kVersionFieldNumber = 1;
  inline ::google::protobuf::uint32 version() const;
  inline void set_version(::google::protobuf::uint32 value);

  // required .com.letsmidi.monsys.protocol.commserver.MsgType type = 2;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 2;
  inline ::com::letsmidi::monsys::protocol::commserver::MsgType type() const;
  inline void set_type(::com::letsmidi::monsys::protocol::commserver::MsgType value);

  // required uint32 sequence = 3;
  inline bool has_sequence() const;
  inline void clear_sequence();
  static const int kSequenceFieldNumber = 3;
  inline ::google::protobuf::uint32 sequence() const;
  inline void set_sequence(::google::protobuf::uint32 value);

  // optional .com.letsmidi.monsys.protocol.commserver.Register register = 5;
  inline bool has_register_() const;
  inline void clear_register_();
  static const int kRegisterFieldNumber = 5;
  inline const ::com::letsmidi::monsys::protocol::commserver::Register& register_() const;
  inline ::com::letsmidi::monsys::protocol::commserver::Register* mutable_register_();
  inline ::com::letsmidi::monsys::protocol::commserver::Register* release_register_();
  inline void set_allocated_register_(::com::letsmidi::monsys::protocol::commserver::Register* register_);

  // optional .com.letsmidi.monsys.protocol.commserver.RegisterRsp register_rsp = 6;
  inline bool has_register_rsp() const;
  inline void clear_register_rsp();
  static const int kRegisterRspFieldNumber = 6;
  inline const ::com::letsmidi::monsys::protocol::commserver::RegisterRsp& register_rsp() const;
  inline ::com::letsmidi::monsys::protocol::commserver::RegisterRsp* mutable_register_rsp();
  inline ::com::letsmidi::monsys::protocol::commserver::RegisterRsp* release_register_rsp();
  inline void set_allocated_register_rsp(::com::letsmidi::monsys::protocol::commserver::RegisterRsp* register_rsp);

  // optional .com.letsmidi.monsys.protocol.commserver.RequestToken request_token = 7;
  inline bool has_request_token() const;
  inline void clear_request_token();
  static const int kRequestTokenFieldNumber = 7;
  inline const ::com::letsmidi::monsys::protocol::commserver::RequestToken& request_token() const;
  inline ::com::letsmidi::monsys::protocol::commserver::RequestToken* mutable_request_token();
  inline ::com::letsmidi::monsys::protocol::commserver::RequestToken* release_request_token();
  inline void set_allocated_request_token(::com::letsmidi::monsys::protocol::commserver::RequestToken* request_token);

  // optional .com.letsmidi.monsys.protocol.commserver.RequestTokenRsp request_token_rsp = 8;
  inline bool has_request_token_rsp() const;
  inline void clear_request_token_rsp();
  static const int kRequestTokenRspFieldNumber = 8;
  inline const ::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp& request_token_rsp() const;
  inline ::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp* mutable_request_token_rsp();
  inline ::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp* release_request_token_rsp();
  inline void set_allocated_request_token_rsp(::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp* request_token_rsp);

  // @@protoc_insertion_point(class_scope:com.letsmidi.monsys.protocol.commserver.CommServerMsg)
 private:
  inline void set_has_version();
  inline void clear_has_version();
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_sequence();
  inline void clear_has_sequence();
  inline void set_has_register_();
  inline void clear_has_register_();
  inline void set_has_register_rsp();
  inline void clear_has_register_rsp();
  inline void set_has_request_token();
  inline void clear_has_request_token();
  inline void set_has_request_token_rsp();
  inline void clear_has_request_token_rsp();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint32 version_;
  int type_;
  ::com::letsmidi::monsys::protocol::commserver::Register* register__;
  ::com::letsmidi::monsys::protocol::commserver::RegisterRsp* register_rsp_;
  ::com::letsmidi::monsys::protocol::commserver::RequestToken* request_token_;
  ::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp* request_token_rsp_;
  ::google::protobuf::uint32 sequence_;
  friend void  protobuf_AddDesc_comm_5fserver_2eproto();
  friend void protobuf_AssignDesc_comm_5fserver_2eproto();
  friend void protobuf_ShutdownFile_comm_5fserver_2eproto();

  void InitAsDefaultInstance();
  static CommServerMsg* default_instance_;
};
// -------------------------------------------------------------------

class Register : public ::google::protobuf::Message {
 public:
  Register();
  virtual ~Register();

  Register(const Register& from);

  inline Register& operator=(const Register& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Register& default_instance();

  void Swap(Register* other);

  // implements Message ----------------------------------------------

  Register* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Register& from);
  void MergeFrom(const Register& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string ip_v4_addr = 1;
  inline bool has_ip_v4_addr() const;
  inline void clear_ip_v4_addr();
  static const int kIpV4AddrFieldNumber = 1;
  inline const ::std::string& ip_v4_addr() const;
  inline void set_ip_v4_addr(const ::std::string& value);
  inline void set_ip_v4_addr(const char* value);
  inline void set_ip_v4_addr(const char* value, size_t size);
  inline ::std::string* mutable_ip_v4_addr();
  inline ::std::string* release_ip_v4_addr();
  inline void set_allocated_ip_v4_addr(::std::string* ip_v4_addr);

  // required uint32 port = 2;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 2;
  inline ::google::protobuf::uint32 port() const;
  inline void set_port(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:com.letsmidi.monsys.protocol.commserver.Register)
 private:
  inline void set_has_ip_v4_addr();
  inline void clear_has_ip_v4_addr();
  inline void set_has_port();
  inline void clear_has_port();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* ip_v4_addr_;
  ::google::protobuf::uint32 port_;
  friend void  protobuf_AddDesc_comm_5fserver_2eproto();
  friend void protobuf_AssignDesc_comm_5fserver_2eproto();
  friend void protobuf_ShutdownFile_comm_5fserver_2eproto();

  void InitAsDefaultInstance();
  static Register* default_instance_;
};
// -------------------------------------------------------------------

class RegisterRsp : public ::google::protobuf::Message {
 public:
  RegisterRsp();
  virtual ~RegisterRsp();

  RegisterRsp(const RegisterRsp& from);

  inline RegisterRsp& operator=(const RegisterRsp& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RegisterRsp& default_instance();

  void Swap(RegisterRsp* other);

  // implements Message ----------------------------------------------

  RegisterRsp* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RegisterRsp& from);
  void MergeFrom(const RegisterRsp& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 code = 1;
  inline bool has_code() const;
  inline void clear_code();
  static const int kCodeFieldNumber = 1;
  inline ::google::protobuf::int32 code() const;
  inline void set_code(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:com.letsmidi.monsys.protocol.commserver.RegisterRsp)
 private:
  inline void set_has_code();
  inline void clear_has_code();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 code_;
  friend void  protobuf_AddDesc_comm_5fserver_2eproto();
  friend void protobuf_AssignDesc_comm_5fserver_2eproto();
  friend void protobuf_ShutdownFile_comm_5fserver_2eproto();

  void InitAsDefaultInstance();
  static RegisterRsp* default_instance_;
};
// -------------------------------------------------------------------

class RequestToken : public ::google::protobuf::Message {
 public:
  RequestToken();
  virtual ~RequestToken();

  RequestToken(const RequestToken& from);

  inline RequestToken& operator=(const RequestToken& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RequestToken& default_instance();

  void Swap(RequestToken* other);

  // implements Message ----------------------------------------------

  RequestToken* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RequestToken& from);
  void MergeFrom(const RequestToken& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:com.letsmidi.monsys.protocol.commserver.RequestToken)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_comm_5fserver_2eproto();
  friend void protobuf_AssignDesc_comm_5fserver_2eproto();
  friend void protobuf_ShutdownFile_comm_5fserver_2eproto();

  void InitAsDefaultInstance();
  static RequestToken* default_instance_;
};
// -------------------------------------------------------------------

class RequestTokenRsp : public ::google::protobuf::Message {
 public:
  RequestTokenRsp();
  virtual ~RequestTokenRsp();

  RequestTokenRsp(const RequestTokenRsp& from);

  inline RequestTokenRsp& operator=(const RequestTokenRsp& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RequestTokenRsp& default_instance();

  void Swap(RequestTokenRsp* other);

  // implements Message ----------------------------------------------

  RequestTokenRsp* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RequestTokenRsp& from);
  void MergeFrom(const RequestTokenRsp& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 code = 1;
  inline bool has_code() const;
  inline void clear_code();
  static const int kCodeFieldNumber = 1;
  inline ::google::protobuf::int32 code() const;
  inline void set_code(::google::protobuf::int32 value);

  // required string token = 2;
  inline bool has_token() const;
  inline void clear_token();
  static const int kTokenFieldNumber = 2;
  inline const ::std::string& token() const;
  inline void set_token(const ::std::string& value);
  inline void set_token(const char* value);
  inline void set_token(const char* value, size_t size);
  inline ::std::string* mutable_token();
  inline ::std::string* release_token();
  inline void set_allocated_token(::std::string* token);

  // @@protoc_insertion_point(class_scope:com.letsmidi.monsys.protocol.commserver.RequestTokenRsp)
 private:
  inline void set_has_code();
  inline void clear_has_code();
  inline void set_has_token();
  inline void clear_has_token();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* token_;
  ::google::protobuf::int32 code_;
  friend void  protobuf_AddDesc_comm_5fserver_2eproto();
  friend void protobuf_AssignDesc_comm_5fserver_2eproto();
  friend void protobuf_ShutdownFile_comm_5fserver_2eproto();

  void InitAsDefaultInstance();
  static RequestTokenRsp* default_instance_;
};
// ===================================================================


// ===================================================================

// CommServerMsg

// required uint32 version = 1;
inline bool CommServerMsg::has_version() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CommServerMsg::set_has_version() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CommServerMsg::clear_has_version() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CommServerMsg::clear_version() {
  version_ = 0u;
  clear_has_version();
}
inline ::google::protobuf::uint32 CommServerMsg::version() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.CommServerMsg.version)
  return version_;
}
inline void CommServerMsg::set_version(::google::protobuf::uint32 value) {
  set_has_version();
  version_ = value;
  // @@protoc_insertion_point(field_set:com.letsmidi.monsys.protocol.commserver.CommServerMsg.version)
}

// required .com.letsmidi.monsys.protocol.commserver.MsgType type = 2;
inline bool CommServerMsg::has_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CommServerMsg::set_has_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CommServerMsg::clear_has_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CommServerMsg::clear_type() {
  type_ = 1;
  clear_has_type();
}
inline ::com::letsmidi::monsys::protocol::commserver::MsgType CommServerMsg::type() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.CommServerMsg.type)
  return static_cast< ::com::letsmidi::monsys::protocol::commserver::MsgType >(type_);
}
inline void CommServerMsg::set_type(::com::letsmidi::monsys::protocol::commserver::MsgType value) {
  assert(::com::letsmidi::monsys::protocol::commserver::MsgType_IsValid(value));
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:com.letsmidi.monsys.protocol.commserver.CommServerMsg.type)
}

// required uint32 sequence = 3;
inline bool CommServerMsg::has_sequence() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void CommServerMsg::set_has_sequence() {
  _has_bits_[0] |= 0x00000004u;
}
inline void CommServerMsg::clear_has_sequence() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void CommServerMsg::clear_sequence() {
  sequence_ = 0u;
  clear_has_sequence();
}
inline ::google::protobuf::uint32 CommServerMsg::sequence() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.CommServerMsg.sequence)
  return sequence_;
}
inline void CommServerMsg::set_sequence(::google::protobuf::uint32 value) {
  set_has_sequence();
  sequence_ = value;
  // @@protoc_insertion_point(field_set:com.letsmidi.monsys.protocol.commserver.CommServerMsg.sequence)
}

// optional .com.letsmidi.monsys.protocol.commserver.Register register = 5;
inline bool CommServerMsg::has_register_() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void CommServerMsg::set_has_register_() {
  _has_bits_[0] |= 0x00000008u;
}
inline void CommServerMsg::clear_has_register_() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void CommServerMsg::clear_register_() {
  if (register__ != NULL) register__->::com::letsmidi::monsys::protocol::commserver::Register::Clear();
  clear_has_register_();
}
inline const ::com::letsmidi::monsys::protocol::commserver::Register& CommServerMsg::register_() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.CommServerMsg.register)
  return register__ != NULL ? *register__ : *default_instance_->register__;
}
inline ::com::letsmidi::monsys::protocol::commserver::Register* CommServerMsg::mutable_register_() {
  set_has_register_();
  if (register__ == NULL) register__ = new ::com::letsmidi::monsys::protocol::commserver::Register;
  // @@protoc_insertion_point(field_mutable:com.letsmidi.monsys.protocol.commserver.CommServerMsg.register)
  return register__;
}
inline ::com::letsmidi::monsys::protocol::commserver::Register* CommServerMsg::release_register_() {
  clear_has_register_();
  ::com::letsmidi::monsys::protocol::commserver::Register* temp = register__;
  register__ = NULL;
  return temp;
}
inline void CommServerMsg::set_allocated_register_(::com::letsmidi::monsys::protocol::commserver::Register* register_) {
  delete register__;
  register__ = register_;
  if (register_) {
    set_has_register_();
  } else {
    clear_has_register_();
  }
  // @@protoc_insertion_point(field_set_allocated:com.letsmidi.monsys.protocol.commserver.CommServerMsg.register)
}

// optional .com.letsmidi.monsys.protocol.commserver.RegisterRsp register_rsp = 6;
inline bool CommServerMsg::has_register_rsp() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void CommServerMsg::set_has_register_rsp() {
  _has_bits_[0] |= 0x00000010u;
}
inline void CommServerMsg::clear_has_register_rsp() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void CommServerMsg::clear_register_rsp() {
  if (register_rsp_ != NULL) register_rsp_->::com::letsmidi::monsys::protocol::commserver::RegisterRsp::Clear();
  clear_has_register_rsp();
}
inline const ::com::letsmidi::monsys::protocol::commserver::RegisterRsp& CommServerMsg::register_rsp() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.CommServerMsg.register_rsp)
  return register_rsp_ != NULL ? *register_rsp_ : *default_instance_->register_rsp_;
}
inline ::com::letsmidi::monsys::protocol::commserver::RegisterRsp* CommServerMsg::mutable_register_rsp() {
  set_has_register_rsp();
  if (register_rsp_ == NULL) register_rsp_ = new ::com::letsmidi::monsys::protocol::commserver::RegisterRsp;
  // @@protoc_insertion_point(field_mutable:com.letsmidi.monsys.protocol.commserver.CommServerMsg.register_rsp)
  return register_rsp_;
}
inline ::com::letsmidi::monsys::protocol::commserver::RegisterRsp* CommServerMsg::release_register_rsp() {
  clear_has_register_rsp();
  ::com::letsmidi::monsys::protocol::commserver::RegisterRsp* temp = register_rsp_;
  register_rsp_ = NULL;
  return temp;
}
inline void CommServerMsg::set_allocated_register_rsp(::com::letsmidi::monsys::protocol::commserver::RegisterRsp* register_rsp) {
  delete register_rsp_;
  register_rsp_ = register_rsp;
  if (register_rsp) {
    set_has_register_rsp();
  } else {
    clear_has_register_rsp();
  }
  // @@protoc_insertion_point(field_set_allocated:com.letsmidi.monsys.protocol.commserver.CommServerMsg.register_rsp)
}

// optional .com.letsmidi.monsys.protocol.commserver.RequestToken request_token = 7;
inline bool CommServerMsg::has_request_token() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void CommServerMsg::set_has_request_token() {
  _has_bits_[0] |= 0x00000020u;
}
inline void CommServerMsg::clear_has_request_token() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void CommServerMsg::clear_request_token() {
  if (request_token_ != NULL) request_token_->::com::letsmidi::monsys::protocol::commserver::RequestToken::Clear();
  clear_has_request_token();
}
inline const ::com::letsmidi::monsys::protocol::commserver::RequestToken& CommServerMsg::request_token() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.CommServerMsg.request_token)
  return request_token_ != NULL ? *request_token_ : *default_instance_->request_token_;
}
inline ::com::letsmidi::monsys::protocol::commserver::RequestToken* CommServerMsg::mutable_request_token() {
  set_has_request_token();
  if (request_token_ == NULL) request_token_ = new ::com::letsmidi::monsys::protocol::commserver::RequestToken;
  // @@protoc_insertion_point(field_mutable:com.letsmidi.monsys.protocol.commserver.CommServerMsg.request_token)
  return request_token_;
}
inline ::com::letsmidi::monsys::protocol::commserver::RequestToken* CommServerMsg::release_request_token() {
  clear_has_request_token();
  ::com::letsmidi::monsys::protocol::commserver::RequestToken* temp = request_token_;
  request_token_ = NULL;
  return temp;
}
inline void CommServerMsg::set_allocated_request_token(::com::letsmidi::monsys::protocol::commserver::RequestToken* request_token) {
  delete request_token_;
  request_token_ = request_token;
  if (request_token) {
    set_has_request_token();
  } else {
    clear_has_request_token();
  }
  // @@protoc_insertion_point(field_set_allocated:com.letsmidi.monsys.protocol.commserver.CommServerMsg.request_token)
}

// optional .com.letsmidi.monsys.protocol.commserver.RequestTokenRsp request_token_rsp = 8;
inline bool CommServerMsg::has_request_token_rsp() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void CommServerMsg::set_has_request_token_rsp() {
  _has_bits_[0] |= 0x00000040u;
}
inline void CommServerMsg::clear_has_request_token_rsp() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void CommServerMsg::clear_request_token_rsp() {
  if (request_token_rsp_ != NULL) request_token_rsp_->::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp::Clear();
  clear_has_request_token_rsp();
}
inline const ::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp& CommServerMsg::request_token_rsp() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.CommServerMsg.request_token_rsp)
  return request_token_rsp_ != NULL ? *request_token_rsp_ : *default_instance_->request_token_rsp_;
}
inline ::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp* CommServerMsg::mutable_request_token_rsp() {
  set_has_request_token_rsp();
  if (request_token_rsp_ == NULL) request_token_rsp_ = new ::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp;
  // @@protoc_insertion_point(field_mutable:com.letsmidi.monsys.protocol.commserver.CommServerMsg.request_token_rsp)
  return request_token_rsp_;
}
inline ::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp* CommServerMsg::release_request_token_rsp() {
  clear_has_request_token_rsp();
  ::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp* temp = request_token_rsp_;
  request_token_rsp_ = NULL;
  return temp;
}
inline void CommServerMsg::set_allocated_request_token_rsp(::com::letsmidi::monsys::protocol::commserver::RequestTokenRsp* request_token_rsp) {
  delete request_token_rsp_;
  request_token_rsp_ = request_token_rsp;
  if (request_token_rsp) {
    set_has_request_token_rsp();
  } else {
    clear_has_request_token_rsp();
  }
  // @@protoc_insertion_point(field_set_allocated:com.letsmidi.monsys.protocol.commserver.CommServerMsg.request_token_rsp)
}

// -------------------------------------------------------------------

// Register

// required string ip_v4_addr = 1;
inline bool Register::has_ip_v4_addr() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Register::set_has_ip_v4_addr() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Register::clear_has_ip_v4_addr() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Register::clear_ip_v4_addr() {
  if (ip_v4_addr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_v4_addr_->clear();
  }
  clear_has_ip_v4_addr();
}
inline const ::std::string& Register::ip_v4_addr() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.Register.ip_v4_addr)
  return *ip_v4_addr_;
}
inline void Register::set_ip_v4_addr(const ::std::string& value) {
  set_has_ip_v4_addr();
  if (ip_v4_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_v4_addr_ = new ::std::string;
  }
  ip_v4_addr_->assign(value);
  // @@protoc_insertion_point(field_set:com.letsmidi.monsys.protocol.commserver.Register.ip_v4_addr)
}
inline void Register::set_ip_v4_addr(const char* value) {
  set_has_ip_v4_addr();
  if (ip_v4_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_v4_addr_ = new ::std::string;
  }
  ip_v4_addr_->assign(value);
  // @@protoc_insertion_point(field_set_char:com.letsmidi.monsys.protocol.commserver.Register.ip_v4_addr)
}
inline void Register::set_ip_v4_addr(const char* value, size_t size) {
  set_has_ip_v4_addr();
  if (ip_v4_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_v4_addr_ = new ::std::string;
  }
  ip_v4_addr_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:com.letsmidi.monsys.protocol.commserver.Register.ip_v4_addr)
}
inline ::std::string* Register::mutable_ip_v4_addr() {
  set_has_ip_v4_addr();
  if (ip_v4_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_v4_addr_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:com.letsmidi.monsys.protocol.commserver.Register.ip_v4_addr)
  return ip_v4_addr_;
}
inline ::std::string* Register::release_ip_v4_addr() {
  clear_has_ip_v4_addr();
  if (ip_v4_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = ip_v4_addr_;
    ip_v4_addr_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Register::set_allocated_ip_v4_addr(::std::string* ip_v4_addr) {
  if (ip_v4_addr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete ip_v4_addr_;
  }
  if (ip_v4_addr) {
    set_has_ip_v4_addr();
    ip_v4_addr_ = ip_v4_addr;
  } else {
    clear_has_ip_v4_addr();
    ip_v4_addr_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:com.letsmidi.monsys.protocol.commserver.Register.ip_v4_addr)
}

// required uint32 port = 2;
inline bool Register::has_port() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Register::set_has_port() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Register::clear_has_port() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Register::clear_port() {
  port_ = 0u;
  clear_has_port();
}
inline ::google::protobuf::uint32 Register::port() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.Register.port)
  return port_;
}
inline void Register::set_port(::google::protobuf::uint32 value) {
  set_has_port();
  port_ = value;
  // @@protoc_insertion_point(field_set:com.letsmidi.monsys.protocol.commserver.Register.port)
}

// -------------------------------------------------------------------

// RegisterRsp

// required int32 code = 1;
inline bool RegisterRsp::has_code() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RegisterRsp::set_has_code() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RegisterRsp::clear_has_code() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RegisterRsp::clear_code() {
  code_ = 0;
  clear_has_code();
}
inline ::google::protobuf::int32 RegisterRsp::code() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.RegisterRsp.code)
  return code_;
}
inline void RegisterRsp::set_code(::google::protobuf::int32 value) {
  set_has_code();
  code_ = value;
  // @@protoc_insertion_point(field_set:com.letsmidi.monsys.protocol.commserver.RegisterRsp.code)
}

// -------------------------------------------------------------------

// RequestToken

// -------------------------------------------------------------------

// RequestTokenRsp

// required int32 code = 1;
inline bool RequestTokenRsp::has_code() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RequestTokenRsp::set_has_code() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RequestTokenRsp::clear_has_code() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RequestTokenRsp::clear_code() {
  code_ = 0;
  clear_has_code();
}
inline ::google::protobuf::int32 RequestTokenRsp::code() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.RequestTokenRsp.code)
  return code_;
}
inline void RequestTokenRsp::set_code(::google::protobuf::int32 value) {
  set_has_code();
  code_ = value;
  // @@protoc_insertion_point(field_set:com.letsmidi.monsys.protocol.commserver.RequestTokenRsp.code)
}

// required string token = 2;
inline bool RequestTokenRsp::has_token() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RequestTokenRsp::set_has_token() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RequestTokenRsp::clear_has_token() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RequestTokenRsp::clear_token() {
  if (token_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    token_->clear();
  }
  clear_has_token();
}
inline const ::std::string& RequestTokenRsp::token() const {
  // @@protoc_insertion_point(field_get:com.letsmidi.monsys.protocol.commserver.RequestTokenRsp.token)
  return *token_;
}
inline void RequestTokenRsp::set_token(const ::std::string& value) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    token_ = new ::std::string;
  }
  token_->assign(value);
  // @@protoc_insertion_point(field_set:com.letsmidi.monsys.protocol.commserver.RequestTokenRsp.token)
}
inline void RequestTokenRsp::set_token(const char* value) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    token_ = new ::std::string;
  }
  token_->assign(value);
  // @@protoc_insertion_point(field_set_char:com.letsmidi.monsys.protocol.commserver.RequestTokenRsp.token)
}
inline void RequestTokenRsp::set_token(const char* value, size_t size) {
  set_has_token();
  if (token_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    token_ = new ::std::string;
  }
  token_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:com.letsmidi.monsys.protocol.commserver.RequestTokenRsp.token)
}
inline ::std::string* RequestTokenRsp::mutable_token() {
  set_has_token();
  if (token_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    token_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:com.letsmidi.monsys.protocol.commserver.RequestTokenRsp.token)
  return token_;
}
inline ::std::string* RequestTokenRsp::release_token() {
  clear_has_token();
  if (token_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = token_;
    token_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void RequestTokenRsp::set_allocated_token(::std::string* token) {
  if (token_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete token_;
  }
  if (token) {
    set_has_token();
    token_ = token;
  } else {
    clear_has_token();
    token_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:com.letsmidi.monsys.protocol.commserver.RequestTokenRsp.token)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace commserver
}  // namespace protocol
}  // namespace monsys
}  // namespace letsmidi
}  // namespace com

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::com::letsmidi::monsys::protocol::commserver::MsgType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::com::letsmidi::monsys::protocol::commserver::MsgType>() {
  return ::com::letsmidi::monsys::protocol::commserver::MsgType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_comm_5fserver_2eproto__INCLUDED
