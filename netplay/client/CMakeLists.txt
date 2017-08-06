INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR})

# ------------------------------------------------------------------------------
# Libs

ADD_LIBRARY (HostUtils host-utils.cc)

# ------------------------------------------------------------------------------
# Tests

SET (NETPLAY_LIBS
  HostUtils
  NetplayServiceProtos
  NetplayServiceGRPCCpp
  TimingsProtos
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARIES}
  ${PROTOBUF_LIBRARIES}
  ${GRPC_LIBRARY}
  ${GRPCPP_LIBRARY})

SET (NETPLAY_TEST_LIBS
  ${NETPLAY_LIBS}
  ${GMOCK_BOTH_LIBRARIES}
  ${GTEST_MAIN_LIBRARIES})

SET (GTEST_ARGS "--gtest_color=yes")

ADD_EXECUTABLE (Client_test client_test.cc)
TARGET_LINK_LIBRARIES (Client_test ${NETPLAY_TEST_LIBS})
IF (WIN32)
# TODO(cennis91): Google Test is not being found correctly at the moment
#GTEST_ADD_TESTS (Client_test ${GTEST_ARGS} client_test.cc)
ELSE ()
GTEST_ADD_TESTS (Client_test ${GTEST_ARGS} client_test.cc)
ENDIF ()

ADD_EXECUTABLE (EventStreamHandler_test event-stream-handler_test.cc)
TARGET_LINK_LIBRARIES (EventStreamHandler_test ${NETPLAY_TEST_LIBS})
IF (WIN32)
# TODO(cennis91): Google Test is not being found correctly at the moment
#GTEST_ADD_TESTS (
#  EventStreamHandler_test 
#  ${GTEST_ARGS}
#  event-stream-handler_test.cc)
ELSE ()
GTEST_ADD_TESTS (
  EventStreamHandler_test 
  ${GTEST_ARGS}
  event-stream-handler_test.cc)
ENDIF ()

ADD_EXECUTABLE (InputQueue_test input-queue_test.cc)
TARGET_LINK_LIBRARIES (InputQueue_test ${NETPLAY_TEST_LIBS})
IF (WIN32)
# TODO(cennis91): Google Test is not being found correctly at the moment
#GTEST_ADD_TESTS (InputQueue_test ${GTEST_ARGS} input-queue_test.cc)
ELSE ()
GTEST_ADD_TESTS (InputQueue_test ${GTEST_ARGS} input-queue_test.cc)
ENDIF ()

# ------------------------------------------------------------------------------
# Client library targets

ADD_SUBDIRECTORY (plugins/mupen64)

# ------------------------------------------------------------------------------
# Output variables

SET (NETPLAY_TEST_LIBS ${NETPLAY_TEST_LIBS} PARENT_SCOPE)
SET (NETPLAY_LIBS ${NETPLAY_LIBS} PARENT_SCOPE)
