cmake_minimum_required(VERSION 3.1)
project(pri_scanner)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(libframework)

include_directories(libs/include)
include_directories(.)

aux_source_directory(libframework LIBFRAMEWORK_SOURCE_FILES)
aux_source_directory(gw GATEWAY_SOURCE_FILES)

add_executable(pri_scanner ${LIBFRAMEWORK_SOURCE_FILES} ${GATEWAY_SOURCE_FILES})

# set(SOURCE_FILES LIBFRAMEWORK_SOURCEFILES GATEWAY_SOURCE_FILES)

#set(SOURCE_FILES
#    gw/center.cc
#    gw/center.pb.cc
#    gw/center.pb.h
#    gw/client.pb.cc
#    gw/client.pb.h
#    gw/comm_server.pb.cc
#    gw/comm_server.pb.h
#    gw/database.h
#    gw/echo_server.cc
#    gw/echo_server.h
#    gw/exchange.pb.cc
#    gw/exchange.pb.h
#    gw/fgw.cc
#    gw/fgw_client.cc
#    gw/fgw_client.h
#    gw/fgw_client_handler.cc
#    gw/fgw_client_handler.h
#    gw/fgw_client_session.h
#    gw/fgw_manager.cc
#    gw/fgw_manager.h
#    gw/fgw_server.cc
#    gw/fgw_server.h
#    gw/fgw_server_handler.cc
#    gw/fgw_server_handler.h
#    gw/fgw_session.h
#    gw/module.cc
#    gw/module.h
#    gw/msg_factory.cc
#    gw/msg_factory.h
#    gw/protobuf_convert.cc
#    gw/protobuf_convert.h
#    gw/protobuf_helper.cc
#    gw/protobuf_helper.h
#    gw/push.pb.cc
#    gw/push.pb.h
#    gw/push_codec.h
#    gw/push_msg.cc
#    gw/push_msg.h
#    gw/route.pb.cc
#    gw/route.pb.h
#    gw/syn_byte_matcher.h
#    gw/test.cc
#    gw/test_push_msg.cc
#    gw/test_zbstream.cc
#    gw/test_zigbee_message.cc
#    gw/unittest.cc
#    gw/unittest_main.cc
#    gw/unittest_main.h
#    gw/webapi_msg.cc
#    gw/webapi_msg.h
#    gw/zb_stream.cc
#    gw/zb_stream.h
#    gw/zb_transfer_layer.h
#    gw/zbdefines.h
#    gw/zbdevice_manager.cc
#    gw/zbdevice_manager.h
#    gw/zconfig.cc
#    gw/zconfig.h
#    gw/zformatter.cc
#    gw/zformatter.h
#    gw/zgenerator.h
#    gw/zinner_message_ex.cc
#    gw/zinner_message_ex.h
#    gw/zjson_codec.h
#    gw/zroute.cc
#    gw/zroute.h
#    gw/zserial.cc
#    gw/zserial.h
#    gw/zstate.h
#    gw/zwebapi_handler.cc
#    gw/zwebapi_handler.h
#    gw/zwebapi_msg.cc
#    gw/zwebapi_msg.h
#    gw/zwebapi_server.cc
#    gw/zwebapi_server.h
#    gw/zwebapi_session.cc
#    gw/zwebapi_session.h
#    gw/zzigbee_codec.h
#    gw/zzigbee_handler.cc
#    gw/zzigbee_handler.h
#    gw/zzigbee_message.cc
#    gw/zzigbee_message.h
#    gw/zzigbee_session.h
#    gw/errno_ex.h
#    libframework/algorithm/base64.cc
#    libframework/algorithm/prac.cc
#    libframework/algorithm/prac.h
#    libframework/algorithm/zstring_search.cc
#    libframework/algorithm/zstring_search.h
#    libframework/main.cc
#    libframework/thread.cc
#    libframework/thread.h
#    libframework/zbyte_buffer.cc
#    libframework/zbyte_buffer.h
#    libframework/zclient.cc
#    libframework/zclient.h
#    libframework/zclient_handler.h
#    libframework/zcodec.c
#    libframework/zcodec.h
#    libframework/zcodecpp.h
#    libframework/zdata_buffer.h
#    libframework/zdispatcher.cc
#    libframework/zdispatcher.h
#    libframework/zerrno.h
#    libframework/zevent_proxy.h
#    libframework/zframework.h
#    libframework/zhandler.h
#    libframework/zinner_message.h
#    libframework/zlog_wrapper.cc
#    libframework/zlog_wrapper.h
#    libframework/zmodule.h
#    libframework/zserver.cc
#    libframework/zserver.h
#    libframework/zserver_handler.cc
#    libframework/zserver_handler.h
#    libframework/zsession.h
#    libframework/zsession_ctrl.h
#    libframework/zstream.h
#    libframework/ztime.h
#    libframework/ztimer.h
#    libframework/ztypes.h
#    libframework/zutil.cc
#    libframework/zutil.h
#    libframework/zhttp_client.cc
#    libframework/zhttp_client.h)

