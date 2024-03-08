# Copyright 2024 Holger Kluck
# This file CMakeLists.txt is part of G4minWE.
#
# G4minWE is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# G4minWE is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with G4minWE. If not, see <http://www.gnu.org/licenses/>.

#-------------------------------------------------------------------------------
# Setup the project
#

# Set minimum required cmake version 
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

# Set name and version of the project, and which programming language is used
project(G4minWE VERSION 1.0.0 LANGUAGES CXX)

# Use GNU-style hierarchy for installing build products
include(GNUInstallDirs)

# Define the C++ Standard to use: require C++14 and allow no non-standard extension
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# If GCC is used add compiler options to get more strict checks
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# ... for all configurations
  	add_compile_options(-Wall -Wextra -Wpedantic)
  	# ... only for debug: get more debug information
  	add_compile_options($<$<CONFIG:Debug>:-g3>)
endif()


#-------------------------------------------------------------------------------
# Find Geant4 packages
#

# Because we want full GUI, need Geant4 with Qt, UI, and visualisation
find_package(Geant4 10.6.3 EXACT REQUIRED ui_all vis_all qt)
include(${Geant4_USE_FILE})


#-------------------------------------------------------------------------------
# Locate sources and headers for this project
#

# For headers
include_directories(${CMAKE_SOURCE_DIR}/include)
# For source files
file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/ *.cc)


#------------------------------------------------------------------------------
# Install 
#

# Executable depends on the main files and all other source and header files
add_executable(${PROJECT_NAME} main.cc ${sources})# ${headers})
# ... and it depends also on the Geant4 libraries
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES})

# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Install documentation like files to the default GNU doc directory
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE.md
              DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES ${CMAKE_SOURCE_DIR}/README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES ${CMAKE_SOURCE_DIR}/CHANGELOG.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
