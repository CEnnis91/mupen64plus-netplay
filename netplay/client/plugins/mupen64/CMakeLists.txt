# ------------------------------------------------------------------------------
# Component libraries

ADD_LIBRARY (Coder coder.cc)
ADD_LIBRARY (ConfigHandler config-handler.cc)
ADD_LIBRARY (OsalDynamicLib osal_dynamiclib_unix.cc)
ADD_LIBRARY (PluginImpl plugin-impl.cc)
ADD_LIBRARY (Util util.cc)

SET (
  M64_PLUGIN_LIBS 
  Coder
  PluginImpl
  ConfigHandler
  OsalDynamicLib
  Util
  ${CMAKE_DL_LIBS})

# ------------------------------------------------------------------------------
# Tests

SET (GTEST_ARGS "--gtest_color=yes")

ADD_EXECUTABLE (PluginImpl_test plugin-impl_test.cc)
TARGET_LINK_LIBRARIES (
  PluginImpl_test 
  ${M64_PLUGIN_LIBS}
  ${NETPLAY_TEST_LIBS})
GTEST_ADD_TESTS (PluginImpl_test ${GTEST_ARGS} plugin-impl_test.cc)

ADD_EXECUTABLE (Util_test util_test.cc)
TARGET_LINK_LIBRARIES (
  Util_test 
  ${NETPLAY_TEST_LIBS}
  ${M64_PLUGIN_LIBS})
GTEST_ADD_TESTS (Util_test ${GTEST_ARGS} util_test.cc)

# ------------------------------------------------------------------------------
# Mupen64Plugin library

ADD_LIBRARY (mupen64plus-netplay SHARED mupen64.cc)
SET_TARGET_PROPERTIES (mupen64plus-netplay PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES (
  mupen64plus-netplay
  ${M64_PLUGIN_LIBS}
  ${NETPLAY_LIBS})
