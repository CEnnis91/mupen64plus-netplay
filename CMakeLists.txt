# ------------------------------------------------------------------------------
# mupen64plus-netplay Global CMake

cmake_minimum_required (VERSION 3.2)
include (ExternalProject)

# ------------------------------------------------------------------------------
# Global Variables

SET (GLOBAL_BUILD_TYPE Debug)

SET (MUPEN64PLUS_PREFIX ${CMAKE_SOURCE_DIR}/mupen64plus)
SET (NETPLAY_PREFIX ${CMAKE_SOURCE_DIR}/netplay)
SET (STATIC_FILES_PREFIX ${CMAKE_SOURCE_DIR}/static-files)
SET (THIRD_PARTY_PREFIX ${CMAKE_SOURCE_DIR}/third-party)

# mupen64plus holds project files in different directories
if (WIN32)
    SET (MUPEN64PLUS_PROJECT_DIR projects/VisualStudio2013)
    SET (OUTPUT_PREFIX ${CMAKE_SOURCE_DIR}/output)
ELSE ()
    SET (MUPEN64PLUS_PROJECT_DIR projects/unix)
    SET (OUTPUT_PREFIX ${CMAKE_SOURCE_DIR}/usr)
ENDIF ()

# ------------------------------------------------------------------------------
# GRPC C/C++

SET (GRPC_PREFIX ${THIRD_PARTY_PREFIX}/grpc)

IF (WIN32)
SET (GRPC_BORINGSSL_PREFIX ${GRPC_PREFIX}/third_party/boringssl)
SET (GRPC_BUILD_TYPE Release)
SET (GRPC_YASM_PATH C:/ProgramData/chocolatey/lib/yasm/tools/yasm.exe)

ExternalProject_Add ( GRPC
    PREFIX ${GRPC_PREFIX}
    SOURCE_DIR ${GRPC_PREFIX}

    # TODO(cennis91): carry our own repo of BoringSSL and specify it
    # with BORINGSSL_ROOT_DIR, from GRPC to point to master (or more recent)
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX}
    CONFIGURE_COMMAND ${CMAKE_COMMAND}
        -GNinja
        -DCMAKE_BUILD_TYPE=${GRPC_BUILD_TYPE}
        -DBORINGSSL_ROOT_DIR=${GRPC_BORINGSSL_PREFIX}
        -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX}
	-DCMAKE_CXX_FLAGS:string="${CMAKE_CXX_FLAGS} /D_WIN32_WINNT=0x0A00 /DWINVER=0x0A00"
        # TODO(cennis91): detection of YASM seems to be working fine
        # unsure if it will work everywhere, it can be set this way
        #-DCMAKE_ASM_NASM_COMPILER="${GRPC_YASM_PATH}"
   
    BINARY_DIR ${GRPC_PREFIX}
    BUILD_COMMAND ninja
        
    INSTALL_DIR ${OUTPUT_PREFIX}
    #INSTALL_COMMAND ""
)
ELSE ()
SET (GRPC_MAKE_ARGS prefix=${OUTPUT_PREFIX} REQUIRE_CUSTOM_LIBRARIES_opt=true)

ExternalProject_Add ( GRPC
    PREFIX ${GRPC_PREFIX}
    SOURCE_DIR ${GRPC_PREFIX}
        
    CONFIGURE_COMMAND ""
        
    BINARY_DIR ${GRPC_PREFIX}
    BUILD_COMMAND make ${GRPC_MAKE_ARGS}
        
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND make install ${GRPC_MAKE_ARGS}
)
ENDIF ()

# ------------------------------------------------------------------------------
# Protobuf3

SET (PROTOBUF3_PREFIX ${GRPC_PREFIX}/third_party/protobuf)
SET (PROTOBUF3_BUILD_TYPE ${GLOBAL_BUILD_TYPE})

IF (WIN32)
ExternalProject_Add (Protobuf3
    PREFIX ${PROTOBUF3_PREFIX}
    DEPENDS GRPC
    SOURCE_DIR ${PROTOBUF3_PREFIX}

    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX}
    CONFIGURE_COMMAND ""
        
    BINARY_DIR ${PROTOBUF3_PREFIX}
    BUILD_COMMAND ""
        
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND echo "No need to install Protobuf3"
)
ELSE ()
ExternalProject_Add (Protobuf3
    PREFIX ${PROTOBUF3_PREFIX}
    DEPENDS GRPC
    SOURCE_DIR ${PROTOBUF3_PREFIX}

    CONFIGURE_COMMAND ""
        
    BINARY_DIR ${PROTOBUF3_PREFIX}
    BUILD_COMMAND ""
        
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND make install prefix=${OUTPUT_PREFIX}
)
ENDIF ()

# ------------------------------------------------------------------------------
# GoogleTest

SET (GOOGLE_TEST_PREFIX ${THIRD_PARTY_PREFIX}/googletest/googletest)
SET (GOOGLE_TEST_BUILD_TYPE ${GLOBAL_BUILD_TYPE})

IF (WIN32)
ExternalProject_Add (GoogleTest
    PREFIX ${GOOGLE_TEST_PREFIX}
    SOURCE_DIR ${GOOGLE_TEST_PREFIX}

    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX}
    CONFIGURE_COMMAND ${CMAKE_COMMAND}
        -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX} .
    
    BINARY_DIR ${GOOGLE_TEST_PREFIX}
    BUILD_COMMAND ${CMAKE_COMMAND} --build .
    
    INSTALL_DIR ${OUTPUT_PREFIX}
    #INSTALL_COMMAND ""
)
ELSE ()
ExternalProject_Add (GoogleTest
    PREFIX ${GOOGLE_TEST_PREFIX}
    SOURCE_DIR ${GOOGLE_TEST_PREFIX}
    
    BINARY_DIR ${GOOGLE_TEST_PREFIX}
    BUILD_COMMAND make
    
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND ""
)
ENDIF ()

# ------------------------------------------------------------------------------
# GoogleMock

SET (GOOGLE_MOCK_PREFIX ${THIRD_PARTY_PREFIX}/googletest/googlemock)
SET (GOOGLE_MOCK_BUILD_TYPE ${GLOBAL_BUILD_TYPE})

IF (WIN32)
ExternalProject_Add (GoogleMock
    PREFIX ${GOOGLE_MOCK_PREFIX}
    SOURCE_DIR ${GOOGLE_MOCK_PREFIX}

    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX}
    CONFIGURE_COMMAND ${CMAKE_COMMAND}
         -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX} .

    BINARY_DIR ${GOOGLE_MOCK_PREFIX}
    BUILD_COMMAND ${CMAKE_COMMAND} --build .

    INSTALL_DIR ${OUTPUT_PREFIX}
    #INSTALL_COMMAND ""
)
ELSE ()
ExternalProject_Add (GoogleMock
    PREFIX ${GOOGLE_MOCK_PREFIX}
    SOURCE_DIR ${GOOGLE_MOCK_PREFIX}
    
    BINARY_DIR ${GOOGLE_MOCK_PREFIX}
    BUILD_COMMAND make
    
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND ""
)
ENDIF ()

# ------------------------------------------------------------------------------
# GLog

SET (GLOG_PREFIX ${THIRD_PARTY_PREFIX}/glog)
SET (GLOG_BUILD_TYPE ${GLOBAL_BUILD_TYPE})

IF (WIN32)
ExternalProject_Add (GLog
    PREFIX ${GLOG_PREFIX}
    SOURCE_DIR ${GLOG_PREFIX}

    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX}
    CONFIGURE_COMMAND ${CMAKE_COMMAND}
         -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX} .

    BINARY_DIR ${GLOG_PREFIX}
    BUILD_COMMAND ${CMAKE_COMMAND} --build .

    INSTALL_DIR ${OUTPUT_PREFIX}
    #INSTALL_COMMAND ""
)
ELSE ()
ExternalProject_Add (GLog
    PREFIX ${GLOG_PREFIX}
    SOURCE_DIR ${GLOG_PREFIX}

    CONFIGURE_COMMAND cd ${GLOG_PREFIX} && autoreconf --force --install
        COMMAND ${GLOG_PREFIX}/configure --prefix=${OUTPUT_PREFIX}

    BINARY_DIR ${GLOG_PREFIX}
    BUILD_COMMAND ""

    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND ""
)
ENDIF ()

# ------------------------------------------------------------------------------
# GFlags

SET (GFLAGS_PREFIX ${THIRD_PARTY_PREFIX}/gflags)

ExternalProject_Add (GFlags
    PREFIX ${GFLAGS_PREFIX}
    SOURCE_DIR ${GFLAGS_PREFIX}

    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${OUTPUT_PREFIX}
        -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=True
)

# ------------------------------------------------------------------------------
# mupen64plus-core

SET (MUPEN64PLUS_CORE_PREFIX ${MUPEN64PLUS_PREFIX}/mupen64plus-core)
SET (MUPEN64PLUS_CORE_PROJECT_PREFIX ${MUPEN64PLUS_CORE_PREFIX}/${MUPEN64PLUS_PROJECT_DIR})

IF (WIN32)
ExternalProject_Add (Mupen64Plus_Core
    PREFIX ${MUPEN64PLUS_CORE_PROJECT_PREFIX}
    SOURCE_DIR ${MUPEN64PLUS_CORE_PROJECT_PREFIX}

    CONFIGURE_COMMAND ""
    
    BINARY_DIR ${MUPEN64PLUS_CORE_PROJECT_PREFIX}
    BUILD_COMMAND ""
    
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND ""
)
ELSE ()
ExternalProject_Add (Mupen64Plus_Core
    PREFIX ${MUPEN64PLUS_CORE_PROJECT_PREFIX}
    SOURCE_DIR ${MUPEN64PLUS_CORE_PROJECT_PREFIX}
    
    CONFIGURE_COMMAND ""
    
    BINARY_DIR ${MUPEN64PLUS_CORE_PROJECT_PREFIX}
    BUILD_COMMAND make all DEBUG=1
    
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND ""
)
ENDIF ()

ExternalProject_Add_Step (Mupen64Plus_Core Force_Configure
    COMMAND ${CMAKE_COMMAND} -E echo "Force configure of Mupen64Plus_Core"
    DEPENDEES update
    DEPENDERS configure
    ALWAYS 1
)

# ------------------------------------------------------------------------------
# mupen64plus-ui-console

SET (MUPEN64PLUS_UI_CONSOLE_PREFIX ${MUPEN64PLUS_PREFIX}/mupen64plus-ui-console)
SET (MUPEN64PLUS_UI_CONSOLE_PROJECT_PREFIX ${MUPEN64PLUS_UI_CONSOLE_PREFIX}/${MUPEN64PLUS_PROJECT_DIR})
SET (MSVC_2013_PREFIX C:\\PROGRA~2\\MICROS~2.0\\Common7\\Tools)

IF (WIN32)
ExternalProject_Add (Mupen64Plus_Ui_Console
    PREFIX ${MUPEN64PLUS_UI_CONSOLE_PROJECT_PREFIX}
    SOURCE_DIR ${MUPEN64PLUS_UI_CONSOLE_PROJECT_PREFIX}

    CONFIGURE_COMMAND ""
    
    BINARY_DIR ${MUPEN64PLUS_UI_CONSOLE_PROJECT_PREFIX}
    BUILD_COMMAND msbuild ${MUPEN64PLUS_UI_CONSOLE_PROJECT_PREFIX}/mupen64plus-ui-console.vcxproj
    
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND ""
)
ELSE ()
ExternalProject_Add (Mupen64Plus_Ui_Console
    PREFIX ${MUPEN64PLUS_UI_CONSOLE_PROJECT_PREFIX}
    SOURCE_DIR ${MUPEN64PLUS_UI_CONSOLE_PROJECT_PREFIX}

    CONFIGURE_COMMAND ""
    
    BINARY_DIR ${MUPEN64PLUS_UI_CONSOLE_PROJECT_PREFIX}
    BUILD_COMMAND make PIE=1 all
    
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND ""
)
ENDIF ()

ExternalProject_Add_Step (Mupen64Plus_Ui_Console Force_Configure
    COMMAND ${CMAKE_COMMAND} -E echo "Force configure of Mupen64Plus_Ui_Console"
    DEPENDEES update
    DEPENDERS configure
    ALWAYS 1
)

# ------------------------------------------------------------------------------
# Netplay

SET (NETPLAY_BUILD_TYPE ${GLOBAL_BUILD_TYPE})

IF (WIN32)
ExternalProject_Add (Netplay
    PREFIX ${NETPLAY_PREFIX}
    SOURCE_DIR ${NETPLAY_PREFIX}

    DEPENDS
        GRPC
        Protobuf3
        GoogleTest
        GoogleMock
        GLog
        Mupen64Plus_Core
        Mupen64Plus_Ui_Console
        
    CMAKE_ARGS
        -DGRPC_ROOT:string=${GRPC_PREFIX}
        -DProtobuf_SRC_ROOT_FOLDER:string=${PROTOBUF3_PREFIX}
        -DCMAKE_PREFIX_PATH:string=${PROTOBUF3_PREFIX}
        -DGTEST_ROOT:string=${OUTPUT_PREFIX}
        -DGMOCK_ROOT:string=${GOOGLE_MOCK_PREFIX}
        -DGFLAGS_ROOT:string=${OUTPUT_PREFIX}/lib
        -DGFLAGS_LIBRARIES:string=${OUTPUT_PREFIX}/lib
        -DGLOG_ROOT:string=${OUTPUT_PREFIX}/lib
        -DMUPEN64_ROOT:string=${MUPEN64PLUS_CORE_PREFIX}/src/api
        -DCMAKE_PROGRAM_PATH:string=${OUTPUT_PREFIX}/bin
        -DCMAKE_LIBRARY_PATH:string=${OUTPUT_PREFIX}/lib
        -DCMAKE_INCLUDE_PATH:string=${OUTPUT_PREFIX}/include
        # TODO(alexgolec): LLVM 7.0.0 on OSX complains about missing overrides in
        # GRPC headers. Hopefully those headers will eventually be updated, so
        # remove this once that happens.
	-DCMAKE_CXX_FLAGS:string=${CMAKE_CXX_FLAGS}
        #-DCMAKE_CXX_FLAGS:string=-Wno-inconsistent-missing-override
        -DCMAKE_BUILD_TYPE:string=${NETPLAY_BUILD_TYPE}
    
    BINARY_DIR ${NETPLAY_PREFIX}
    BUILD_COMMAND ""
    
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND ""
)
ELSE ()
ExternalProject_Add (Netplay
    PREFIX ${NETPLAY_PREFIX}
    SOURCE_DIR ${NETPLAY_PREFIX}

    DEPENDS
        GRPC
        Protobuf3
        GoogleTest
        GoogleMock
        GLog
        Mupen64Plus_Core
        Mupen64Plus_Ui_Console
        
    CMAKE_ARGS
        -DGRPC_ROOT:string=${OUTPUT_PREFIX}
        -DGTEST_ROOT:string=${GOOGLE_TEST_PREFIX}
        -DGMOCK_ROOT:string=${GOOGLE_MOCK_PREFIX}
        -DMUPEN64_ROOT:string=${MUPEN64PLUS_CORE_PREFIX}/src/api
        -DCMAKE_PROGRAM_PATH:string=${OUTPUT_PREFIX}/bin
        -DCMAKE_LIBRARY_PATH:string=${OUTPUT_PREFIX}/lib
        -DCMAKE_INCLUDE_PATH:string=${OUTPUT_PREFIX}/include
        # TODO(alexgolec): LLVM 7.0.0 on OSX complains about missing overrides in
        # GRPC headers. Hopefully those headers will eventually be updated, so
        # remove this once that happens.
        -DCMAKE_CXX_FLAGS:string=-Wno-inconsistent-missing-override
        -DCMAKE_BUILD_TYPE:string=${NETPLAY_BUILD_TYPE}
    
    BINARY_DIR ${NETPLAY_PREFIX}
    BUILD_COMMAND make
    
    INSTALL_DIR ${OUTPUT_PREFIX}
    INSTALL_COMMAND ""
)
ENDIF ()

ExternalProject_Add_Step (Netplay Force_Configure
    COMMAND ${CMAKE_COMMAND} -E echo "Force configure of Netplay"
    DEPENDEES update
    DEPENDERS configure
    ALWAYS 1
)