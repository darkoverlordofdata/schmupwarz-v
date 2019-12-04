##
# Copyright 2009-2010 Jakob Westhoff, 2014 Raster Software Vigo
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#    1. Redistributions of source code must retain the above copyright notice,
#       this list of conditions and the following disclaimer.
# 
#    2. Redistributions in binary form must reproduce the above copyright notice,
#       this list of conditions and the following disclaimer in the documentation
#       and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY JAKOB WESTHOFF ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL JAKOB WESTHOFF OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# The views and conclusions contained in the software and documentation are those
# of the authors and should not be interpreted as representing official policies,
# either expressed or implied, of Jakob Westhoff
##

##
# Find module for the Vala compiler (v)
#
# This module determines wheter a v compiler is installed on the current
# system and where its executable is.
#
# Call the module using "find_package(Vala) from within your CMakeLists.txt.
#
# The following variables will be set after an invocation:
#
#  V_FOUND       Whether the vala compiler has been found or not
#  V_EXECUTABLE  Full path to the v executable if it has been found
#  V_VERSION     Version number of the available v
##


# Search for the v executable in the usual system paths.
find_program(V_EXECUTABLE NAMES v)

if (V_EXECUTABLE STREQUAL "V_EXECUTABLE-NOTFOUND")

# if v executable is not found, it can be that v is not installed, or
# that the OS is source-based (like gentoo), and doesn't do a link from
# v-X.YY to v. In that case, search for the specific v binary after

	if (NOT V_DEFERRING_COMPILER_SEARCH)
		message (STATUS "v not found. Deferring compiler search")
	endif(NOT V_DEFERRING_COMPILER_SEARCH)
	set(V_DEFERRING_COMPILER_SEARCH TRUE)
	unset(V_EXECUTABLE)
	unset(V_VERSION)

else(V_EXECUTABLE STREQUAL "V_EXECUTABLE-NOTFOUND")

# Handle the QUIETLY and REQUIRED arguments, which may be given to the find call.
# Furthermore set V_FOUND to TRUE if Vala has been found (aka.
# V_EXECUTABLE is set)

	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(V DEFAULT_MSG V_EXECUTABLE)

	mark_as_advanced(V_EXECUTABLE)

# Determine the v version
	if(V_FOUND)
	    execute_process(COMMAND ${V_EXECUTABLE} "--version" OUTPUT_VARIABLE "V_VERSION")
	    string(REPLACE "V" "" "V_VERSION" ${V_VERSION})
	    string(STRIP ${V_VERSION} "V_VERSION")
	endif(V_FOUND)
endif(V_EXECUTABLE STREQUAL "V_EXECUTABLE-NOTFOUND")
