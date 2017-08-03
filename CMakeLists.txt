cmake_minimum_required(VERSION 3.7)
project(calc-module)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads)
find_package(Boost COMPONENTS log log_setup thread REQUIRED)

include_directories(
	.
	${Threads_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	SYSTEM
	${RDKAFKA_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../external/cppkafka/
	${CMAKE_CURRENT_SOURCE_DIR}/../external/boost/
)

add_executable(calc-module message.cpp kafka_connector.cpp main.cpp)

target_link_libraries(
	calc-module

	${CMAKE_CURRENT_SOURCE_DIR}/../external/cppkafka/lib/libcppkafka.a
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	-lpthread
	-lrdkafka
)

