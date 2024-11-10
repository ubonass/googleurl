# Windows
############################################################################################################################################################################
# cmake -G  "Visual Studio 16 2019" -T ClangCL -DCMAKE_BUILD_TYPE:STRING="Debug" -DGURL_FETCH_ABSL=ON -DGURL_FETCH_DOUBLE_CONVERSION=ON -DGURL_BUILD_TEST=OFF ../
############################################################################################################################################################################
# cmake -G  "Visual Studio 16 2019" -T ClangCL -DCMAKE_BUILD_TYPE:STRING="Release" -DGURL_FETCH_ABSL=ON -DGURL_FETCH_DOUBLE_CONVERSION=ON -DGURL_BUILD_TEST=ON ../
############################################################################################################################################################################

#Linux
############################################################################################################################################################################
# cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE:STRING="Debug" -DGURL_FETCH_ABSL=ON -DGURL_FETCH_DOUBLE_CONVERSION=ON -DGURL_BUILD_TEST=OFF ../
############################################################################################################################################################################
# cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE:STRING="Release" -DGURL_FETCH_ABSL=ON -DGURL_FETCH_DOUBLE_CONVERSION=ON -DGURL_BUILD_TEST=ON ../
############################################################################################################################################################################

# git clone https://quiche.googlesource.com/googleurl

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.)
endif()

#
# Project configuration
#
set(GURL_VERSION_MAJOR 1)
set(GURL_VERSION_MINOR 0)
set(GURL_VERSION_PATCH 0)

project(
  GURL
  VERSION ${GURL_VERSION_MAJOR}.${GURL_VERSION_MINOR}.${GURL_VERSION_PATCH}
  DESCRIPTION GURL
  HOMEPAGE_URL https://gitee.com/ubonass/google_url.git
  LANGUAGES C CXX
)

# get rid of the extra default configurations
# what? why would you get id of other useful build types? - Ellzey
# set(CMAKE_CONFIGURATION_TYPES Debug;Release CACHE STRING Limited configurations FORCE)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})

# must using CXX20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GURL_FETCH_ABSL "Google ULR FETCH ABSL " OFF)

option(GURL_FETCH_DOUBLE_CONVERSION "Google ULR FETCH double-conversion " OFF)

option(GURL_BUILD_TEST "Build test programs" OFF)

if(GURL_FETCH_ABSL)
  include(abseil-cpp)
endif()

if(GURL_FETCH_DOUBLE_CONVERSION)
  include(double-conversion)
endif()

list(APPEND BASE_DEBUG_SRCS
  base/debug/crash_logging.cc
)

list(APPEND BASE_STRINGS_SRCS
  base/strings/abseil_string_number_conversions.cc
  base/strings/escape.cc
  base/strings/latin1_string_conversions.cc
  base/strings/pattern.cc
  base/strings/safe_sprintf.cc
  base/strings/strcat.cc
  base/strings/stringprintf.cc
  base/strings/string_number_conversions.cc
  base/strings/string_split.cc
  base/strings/string_util.cc
  base/strings/string_util_constants.cc
  base/strings/utf_offset_string_conversions.cc
  base/strings/utf_ostream_operators.cc
  base/strings/utf_string_conversions.cc
  base/strings/utf_string_conversion_utils.cc
)

if(WIN32)
  list(APPEND BASE_STRINGS_SRCS 
    base/strings/strcat_win.cc
    base/strings/string_number_conversions_win.cc
    base/strings/string_split_win.cc
    base/strings/string_util_win.cc
    base/strings/sys_string_conversions_win.cc
    base/scoped_clear_last_error_win.cc)
else()
  list(APPEND BASE_STRINGS_SRCS 
    base/strings/sys_string_conversions_posix.cc)
endif()

set(BASE_SRCS 
  ${BASE_STRINGS_SRCS}
  ${BASE_DEBUG_SRCS}
)

set(URL_SRCS
  url/gurl.cc
  # url/origin.cc
  # url/scheme_host_port.cc
  url/third_party/mozilla/url_parse.cc
  url/url_canon.cc
  url/url_canon_etc.cc
  url/url_canon_filesystemurl.cc
  url/url_canon_fileurl.cc
  url/url_canon_host.cc
  # url/url_canon_icu.cc
  url/url_canon_internal.cc
  url/url_canon_ip.cc
  url/url_canon_mailtourl.cc
  url/url_canon_path.cc
  url/url_canon_pathurl.cc
  url/url_canon_query.cc
  url/url_canon_relative.cc
  url/url_canon_stdstring.cc
  url/url_canon_stdurl.cc
  url/url_constants.cc
  url/url_features.cc
  url/url_idna_ascii_only.cc
  # url/url_idna_icu.cc
  url/url_parse_file.cc
  url/url_util.cc
)

add_library(google_url
  ${BASE_SRCS}
  ${URL_SRCS}
)

target_link_libraries(google_url
  $<$<TARGET_EXISTS:absl::base>:absl::base>
  $<$<TARGET_EXISTS:absl::algorithm>:absl::algorithm>
  $<$<TARGET_EXISTS:absl::strings>:absl::strings>
  $<$<TARGET_EXISTS:double-conversion::double-conversion>:double-conversion::double-conversion>
)

target_include_directories( google_url PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/polyfills
)

add_library(google_url::google_url ALIAS google_url)

if (GURL_BUILD_TEST)
  add_executable(google_url_test
    ${CMAKE_CURRENT_SOURCE_DIR}/test/basic_test.cc)
  
  target_link_libraries(google_url_test google_url::google_url)
endif()

