include(FetchContent)
FetchContent_Declare(
  double-conversion
  GIT_REPOSITORY https://gitee.com/ubonass/double-conversion.git
  GIT_TAG        v3.3.0
  # SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/base/third_party/double_conversion
)

if(NOT double-conversion_POPULATED)
  FetchContent_Populate(double-conversion)
  add_subdirectory(${double-conversion_SOURCE_DIR} ${double-conversion_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_library(double-conversion::double-conversion ALIAS double-conversion)
target_include_directories(
	double-conversion 
		PUBLIC
			$<BUILD_INTERFACE:${double-conversion_SOURCE_DIR}>
)
