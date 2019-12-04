##
# Copyright (C) 2014 Raster Software Vigo
# Autov is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Autov is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Copyright (C) 2013, vma development team
#
# Copyright 2009-2010 Jakob Westhoff All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#	1. Redistributions of source code must retain the above copyright notice,
#	   this list of conditions and the following disclaimer.
# 
#	2. Redistributions in binary form must reproduce the above copyright notice,
#	   this list of conditions and the following disclaimer in the documentation
#	   and/or other materials provided with the distribution.
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

include(ParseArguments)
find_package(V REQUIRED)

##
# Ensure a certain v version is available
#
# The initial argument is the version to check for
# 
# It may be followed by a optional parameter to specifiy a version range. The
# following options are valid:
# 
# EXACT
#   v needs to be available in the exact version given
# 
# MINIMUM
#   The provided version is the minimum version. Therefore v needs to be
#   available in the given version or any higher version
#
# MAXIMUM
#   The provided version is the maximum. Therefore v needs to be available
#   in the given version or any version older than this
#
# If no option is specified the version will be treated as a minimal version.
##
macro(ensure_v_version ensure_version)

	if (USE_V_BINARY)
		message(STATUS "Forced use of v binary ${USE_V_BINARY}")
		set(V_EXECUTABLE ${USE_V_BINARY})
	else (USE_V_BINARY)

		message(STATUS "checking for V version of v ${ensure_version}")
	
		unset(version_accepted)
		execute_process(COMMAND "v ${ensure_version}" "--version"
			OUTPUT_VARIABLE "V_VERSION" OUTPUT_STRIP_TRAILING_WHITESPACE)
		if (NOT V_VERSION)
	
			find_program(V_EXECUTABLE "v")
	
			# If the specific version asked for this project is not found,
			# try to use the one pointed by the v link
	
			parse_arguments(ARGS "" "MINIMUM;MAXIMUM;EXACT" ${ARGN})
			set(compare_message "")
			set(error_message "")
			if(ARGS_MINIMUM)
				set(compare_message "a minimum ")
				set(error_message "or greater ")
			elseif(ARGS_MAXIMUM)
				set(compare_message "a maximum ")
				set(error_message "or less ")
			endif(ARGS_MINIMUM)
	
			message(STATUS "checking for ${compare_message}V version of ${ensure_version}" )
	
			execute_process(COMMAND ${V_EXECUTABLE} "--version"
				OUTPUT_VARIABLE "V_VERSION" OUTPUT_STRIP_TRAILING_WHITESPACE)
		else(NOT V_VERSION)
			set(V_EXECUTABLE "v ${ensure_version}")
		endif(NOT V_VERSION)

		# this code allows to identify development versions as the right version
		# e.g. 0.19.1 -> 0.20 ; 0.20.1 -> 0.20 ;
		# But this code seems to not be fine 0.24.0.2-2235 -> 0.26
		# Thanks to Yannick Inizan
		string(REPLACE "V" "" "V_VERSION" ${V_VERSION})
		string(STRIP ${V_VERSION} "V_VERSION")

		string(REPLACE "." ";" V_LIST "${V_VERSION}")
		list(GET V_LIST 0 maj_ver)
		list(GET V_LIST 1 min_ver)
		list(GET V_LIST 2 rev_ver)
		math(EXPR is_odd "${min_ver} % 2")
		list(LENGTH V_LIST len)
		#if((${is_odd} EQUAL 1))
		#	math(EXPR min_ver "${min_ver} + 1")
		#elseif(len GREATER 3)
		#	math(EXPR min_ver "${min_ver} + 2")
		#endif()

		set(V_SVERSION "${maj_ver}.${min_ver}" CACHE INTERNAL "")

		# MINIMUM is the default if no option is specified
		if(ARGS_EXACT)
			if(${V_SVERSION} VERSION_EQUAL ${ensure_version} )
				set(version_accepted TRUE)
			endif(${V_SVERSION} VERSION_EQUAL ${ensure_version})
		elseif(ARGS_MAXIMUM)
			if(${V_SVERSION} VERSION_LESS ${ensure_version} OR ${V_SVERSION} VERSION_EQUAL ${ensure_version})
				set(version_accepted TRUE)
			endif(${V_SVERSION} VERSION_LESS ${ensure_version} OR ${V_SVERSION} VERSION_EQUAL ${ensure_version})
		else(ARGS_MAXIMUM)
			if(${V_SVERSION} VERSION_GREATER ${ensure_version} OR ${V_SVERSION} VERSION_EQUAL ${ensure_version})
				set(version_accepted TRUE)
			endif(${V_SVERSION} VERSION_GREATER ${ensure_version} OR ${V_SVERSION} VERSION_EQUAL ${ensure_version})
		endif(ARGS_EXACT)
	
		if (NOT version_accepted)
			message(FATAL_ERROR 
				"V version ${ensure_version} ${error_message}is required."
			)
		endif(NOT version_accepted)
	
		message(STATUS
			"  found V, version ${V_SVERSION}"
		)
	endif(USE_V_BINARY)
endmacro(ensure_v_version)
