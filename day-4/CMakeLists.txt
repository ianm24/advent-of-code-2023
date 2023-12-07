cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("scratchcard_winnings")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(scratchcard_winnings "scratchcard_winnings.cpp")

if(TESTS_ENABLED)	
	set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
	add_subdirectory(${TEST_DIR})

	# Add CTests for the unit and behavioral executables
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET scratchcard_winnings PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.