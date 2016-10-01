cmake_minimum_required(VERSION 3.4)
project(s3tp)

# packages
find_package(moveio REQUIRED)
include_directories(${MOVEIO_INCLUDE_DIRS})

find_package(trctrl REQUIRED)
include_directories(${TRCTRL_INCLUDE_DIRS})

find_package(s3tp REQUIRED)
include_directories(${S3TP_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#core
set(S3TP_CORE_FILES
        ../core/Constants.h
        ../core/PriorityQueue.h
        ../core/CommonTypes.h
        ../core/TxModule.cpp
        ../core/TxModule.h
        ../core/RxModule.cpp
        ../core/RxModule.h
        ../core/Buffer.cpp
        ../core/Buffer.h
        ../core/S3TP.cpp
        ../core/S3TP.h
        ../core/SimpleQueue.h
        ../core/TransportDaemon.cpp
        ../core/TransportDaemon.h
        ../core/utilities.h
        ../core/utilities.cpp
        ../core/Client.cpp
        ../core/Client.h
        ../core/ClientInterface.h
        ../core/StatusInterface.h
        ../core/PolicyActor.h
        s3tp_start.cpp)

set(S3TP_CONTROL_FILES
        ../control/SimpleTransceiverControl.cpp
        ../control/SimpleTransceiverControl.h
        )

add_executable(s3tpd ${S3TP_CORE_FILES} ${S3TP_CONTROL_FILES})

target_link_libraries(s3tpd ${MOVEIO_LIBRARY})
target_link_libraries(s3tpd ${TRCTRL_LIBRARY})
target_link_libraries(s3tpd ${S3TP_LIBRARY})
target_link_libraries(s3tpd pthread)

install(TARGETS s3tpd RUNTIME DESTINATION bin)

#connector
set(CONNECTOR_SRC_FILES
        connector_main.cpp)

add_executable(s3tp_conn ${CONNECTOR_SRC_FILES})

target_link_libraries(s3tp_conn ${S3TP_LIBRARY})
target_link_libraries(s3tp_conn pthread)

install(TARGETS s3tp_conn RUNTIME DESTINATION bin)

#transceiver mod
set(TRMOD_SRC_FILES
        trmod.cpp
        )
add_executable(trmod ${TRMOD_SRC_FILES})

target_link_libraries(trmod ${MOVEIO_LIBRARY})
target_link_libraries(trmod ${TRCTRL_LIBRARY})
target_link_libraries(trmod ${S3TP_LIBRARY})
target_link_libraries(trmod pthread)

install(TARGETS trmod RUNTIME DESTINATION bin)

#dummy control
set(DUMMY_CONTROL_SRC_FILES
        dummy_control.cpp
        )
add_executable(dummy_control ${DUMMY_CONTROL_SRC_FILES} ${S3TP_CONTROL_FILES})

target_link_libraries(dummy_control ${MOVEIO_LIBRARY})
target_link_libraries(dummy_control ${TRCTRL_LIBRARY})
target_link_libraries(dummy_control ${S3TP_LIBRARY})
target_link_libraries(dummy_control pthread)

install(TARGETS dummy_control RUNTIME DESTINATION bin)
