cmake_minimum_required(VERSION 3.12)
project(svh_authentication_service CXX)

# Adding userver dependency
find_package(userver COMPONENTS core postgresql QUIET)

if(NOT userver_FOUND) # Fallback to subdirectory usage
    # Enable userver libraries that are needed in this project
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

    if(EXISTS third_party/userver)
        message(STATUS "Using userver framework from third_party/userver")
        add_subdirectory(third_party/userver)
    else()
        message(FATAL_ERROR "Either install the userver or provide a path to it")
    endif()
endif()

userver_setup_environment()

find_package(jwt-cpp REQUIRED)

add_library(${PROJECT_NAME}_model_signin OBJECT
    src/model/signin/signin_credentials.hpp
    src/model/signin/io.hpp
)
target_link_libraries(${PROJECT_NAME}_model_signin PUBLIC userver::postgresql)

# Common sources
add_library(${PROJECT_NAME}_signin OBJECT
    src/components/http/post/signin/SigninHandler.hpp
    src/components/http/post/signin/fwd.hpp
    src/components/http/post/signin/SigninHandler.cpp
)
target_link_libraries(${PROJECT_NAME}_signin PUBLIC userver::postgresql)
target_include_directories(${PROJECT_NAME}_signin PRIVATE src/model/)

add_library(${PROJECT_NAME}_model_signup OBJECT
    src/model/signup/signup_credentials.hpp
    src/model/signup/io.hpp
)
target_link_libraries(${PROJECT_NAME}_model_signup PUBLIC userver::postgresql)

add_library(${PROJECT_NAME}_signup OBJECT
    src/components/http/post/signup/SignupHandler.hpp
    src/components/http/post/signup/fwd.hpp
    src/components/http/post/signup/SignupHandler.cpp
)
target_link_libraries(${PROJECT_NAME}_signup PUBLIC userver::postgresql)
target_include_directories(${PROJECT_NAME}_signup PRIVATE src/model/)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_signin)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_signup)

target_link_libraries(${PROJECT_NAME} PUBLIC jwt-cpp::jwt-cpp)

# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple()

# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
