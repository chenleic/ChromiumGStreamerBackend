// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_CLIENT_CONTROL_DISPATCHER_H_
#define REMOTING_PROTOCOL_CLIENT_CONTROL_DISPATCHER_H_

#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "remoting/protocol/channel_dispatcher_base.h"
#include "remoting/protocol/clipboard_stub.h"
#include "remoting/protocol/cursor_shape_stub.h"
#include "remoting/protocol/host_stub.h"
#include "remoting/protocol/protobuf_message_parser.h"

namespace remoting {
namespace protocol {

class ClientStub;
class ControlMessage;
class Session;

// ClientControlDispatcher dispatches incoming messages on the control
// channel to ClientStub, ClipboardStub or CursorShapeStub.
// It also implements ClipboardStub and HostStub for outgoing messages.
class ClientControlDispatcher : public ChannelDispatcherBase,
                                public ClipboardStub,
                                public HostStub {
 public:
  ClientControlDispatcher();
  ~ClientControlDispatcher() override;

  // ClipboardStub implementation.
  void InjectClipboardEvent(const ClipboardEvent& event) override;

  // HostStub implementation.
  void NotifyClientResolution(const ClientResolution& resolution) override;
  void ControlVideo(const VideoControl& video_control) override;
  void ControlAudio(const AudioControl& audio_control) override;
  void SetCapabilities(const Capabilities& capabilities) override;
  void RequestPairing(const PairingRequest& pairing_request) override;
  void DeliverClientMessage(const ExtensionMessage& message) override;

  // Sets the ClientStub that will be called for each incoming control
  // message. |client_stub| must outlive this object.
  void set_client_stub(ClientStub* client_stub) { client_stub_ = client_stub; }

  // Sets the ClipboardStub that will be called for each incoming clipboard
  // message. |clipboard_stub| must outlive this object.
  void set_clipboard_stub(ClipboardStub* clipboard_stub) {
    clipboard_stub_ = clipboard_stub;
  }

 private:
  void OnMessageReceived(scoped_ptr<ControlMessage> message,
                         const base::Closure& done_task);

  ClientStub* client_stub_;
  ClipboardStub* clipboard_stub_;

  ProtobufMessageParser<ControlMessage> parser_;

  DISALLOW_COPY_AND_ASSIGN(ClientControlDispatcher);
};

}  // namespace protocol
}  // namespace remoting

#endif  // REMOTING_PROTOCOL_CLIENT_CONTROL_DISPATCHER_H_
