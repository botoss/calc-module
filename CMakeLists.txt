cmake_minimum_required(VERSION 3.0)
project(calc-module)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(Boost_USE_STATIC_LIBS   ON)

find_package(Threads)
find_package(Boost COMPONENTS system log log_setup thread REQUIRED)

set(SRC_DIR "./src")

include_directories(
	${SRC_DIR}
	${Threads_INCLUDE_DIR}
	SYSTEM
	${RDKAFKA_INCLUDE_DIR}
	${CPPKAFKA_DIR}/include
	${Boost_INCLUDE_DIRS}
)

set(SOURCES
	"${SRC_DIR}/main.cpp"
	"${SRC_DIR}/kafka_connector.cpp"
	"${SRC_DIR}/message.cpp"	
)

add_executable(calc-module "${SOURCES}")

set_target_properties(calc-module PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(
	calc-module

	${CPPKAFKA_DIR}/build/lib/libcppkafka.a
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	-lpthread
	-lrdkafka
)

