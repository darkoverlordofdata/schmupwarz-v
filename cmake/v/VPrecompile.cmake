##
# Copyright 2009-2010 Jakob Westhoff. All rights reserved.
# Copyright 2012 elementary.
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

include(ParseArguments)
find_package(V REQUIRED)

##
# Compile v files to their c equivalents for further processing.
#
# The "v_precompile" macro takes care of calling the vc executable on the
# given source to produce c files which can then be processed further using
# default cmake functions.
#
# The first parameter provided is a variable, which will be filled with a list
# of c files outputted by the v compiler. This list can than be used in
# conjuction with functions like "add_executable" or others to create the
# neccessary compile rules with CMake.
#
# The initial variable is followed by a list of .v files to be compiled.
# Please take care to add every v file belonging to the currently compiled
# project or library as V will otherwise not be able to resolve all
# dependencies.
#
# The following sections may be specified afterwards to provide certain options
# to the v compiler:
#
# PACKAGES
#   A list of v packages/libraries to be used during the compile cycle. The
#   package names are exactly the same, as they would be passed to the vc
#   "--pkg=" option.
#
# OPTIONS
#   A list of optional options to be passed to the vc executable. This can be
#   used to pass "--thread" for example to enable multi-threading support.
#
# CUSTOM_VAPIS
#   A list of custom vapi files to be included for compilation. This can be
#   useful to include freshly created v libraries without having to install
#   them in the system.
#
# GENERATE_VAPI
#   Pass all the needed flags to the compiler to create an internal vapi for
#   the compiled library. The provided name will be used for this and a
#   <provided_name>.vapi file will be created.
#
# GENERATE_HEADER
#   Let the compiler generate a header file for the compiled code. There will
#   be a header file as well as an internal header file being generated called
#   <provided_name>.h and <provided_name>_internal.h
#
# GENERATE_GIR
#   Have the compiler generate a GObject-Introspection repository file with
#   name: <provided_name>.gir. This can be later used to create a binary typelib
#   using the GI compiler.
#
# GENERATE_SYMBOLS
#   Output a <provided_name>.symbols file containing all the exported symbols.
#
# The following call is a simple example to the v_precompile macro showing
# an example to every of the optional sections:
#
#   v_precompile(V_C mytargetname
#       source1.v
#       source2.v
#       source3.v
#   PACKAGES
#       gtk+-2.0
#       gio-1.0
#       posix
#   DIRECTORY
#       gen
#   OPTIONS
#       --thread
#   GENERATE_HEADER
#       myheader
#   GENERATE_SYMBOLS
#       mysymbols
#   )
#
# Most important is the variable V_C which will contain all the generated c
# file names after the call.
##

macro(v_precompile output target_name)

    parse_arguments(ARGS "TARGET;PACKAGES;OPTIONS;DIRECTORY;GENERATE_GIR;GENERATE_SYMBOLS;GENERATE_HEADER;GENERATE_VAPI;CUSTOM_VAPIS" "" ${ARGN})

    if(ARGS_DIRECTORY)
        set(DIRECTORY ${ARGS_DIRECTORY})
    else(ARGS_DIRECTORY)
        set(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif(ARGS_DIRECTORY)
    include_directories(${DIRECTORY})
    set(v_pkg_opts "")
    foreach(pkg ${ARGS_PACKAGES})
        list(APPEND v_pkg_opts "--pkg=${pkg}")
    endforeach(pkg ${ARGS_PACKAGES})
    set(in_files "")
    set(out_files "")
    set(out_files_display "")
    set(${output} "")

    foreach(src ${ARGS_DEFAULT_ARGS})
        string(REGEX MATCH "^([a-zA-Z]:)?/" IS_MATCHED ${src})
        if(${IS_MATCHED} MATCHES "/")
            set(src_file_path ${src})
        else()
            set(src_file_path ${CMAKE_CURRENT_SOURCE_DIR}/${src})
        endif()
        list(APPEND in_files ${src_file_path})
        string(REPLACE ".v" ".c" src ${src})
        if(${IS_MATCHED} MATCHES "/")
            get_filename_component(V_FILE_NAME ${src} NAME)
            set(out_file "${DIRECTORY}/${V_FILE_NAME}")
            list(APPEND out_files "${DIRECTORY}/${V_FILE_NAME}")
        else()
            set(out_file "${DIRECTORY}/${src}")
            list(APPEND out_files "${DIRECTORY}/${src}")
        endif()
        list(APPEND ${output} ${out_file})
        list(APPEND out_files_display "${src}")
    endforeach(src ${ARGS_DEFAULT_ARGS})

    set(header_arguments "")
    if(ARGS_GENERATE_HEADER)
        list(APPEND out_files "${DIRECTORY}/${ARGS_GENERATE_HEADER}.h")
        list(APPEND out_files_display "${ARGS_GENERATE_HEADER}.h")
        list(APPEND header_arguments "--header=${ARGS_GENERATE_HEADER}.h")
    endif(ARGS_GENERATE_HEADER)

    set(symbols_arguments "")
    if(ARGS_GENERATE_SYMBOLS)
        list(APPEND out_files "${DIRECTORY}/${ARGS_GENERATE_SYMBOLS}.symbols")
        list(APPEND out_files_display "${ARGS_GENERATE_SYMBOLS}.symbols")
        set(symbols_arguments "--symbols=${ARGS_GENERATE_SYMBOLS}.symbols")
    endif(ARGS_GENERATE_SYMBOLS)

    set(os_defines "")
    if(WIN32)
        list(APPEND os_defines "-D" "\"G_OS_WIN32\"")
    endif(WIN32)

    # Workaround for a bug that would make vc run twice. This file is written
    # after the v compiler generates C source code.
    set (extra_name ${extra_name}_ )
    set(OUTPUT_STAMP ${CMAKE_CURRENT_BINARY_DIR}/${target_name}${extra_name}v.stamp)

    if (WIN32)
        add_custom_command(
            OUTPUT
                ${OUTPUT_STAMP}
            COMMAND
                ${V_EXECUTABLE}
            ARGS
            "-o"
            ${out_files}
            ${ARGS_OPTIONS}
            ${in_files}
        COMMAND
                c:\\msys64\\usr\\bin\\touch.exe
            ARGS
                ${OUTPUT_STAMP}
            DEPENDS
                ${in_files}
            COMMENT
                "Generating ${out_files_display}"
        )
    else ()
        add_custom_command(
            OUTPUT
                ${OUTPUT_STAMP}
            COMMAND
                ${V_EXECUTABLE}
            ARGS
                "-o"
                ${out_files}
                ${ARGS_OPTIONS}
                ${in_files}
            COMMAND
                touch
            ARGS
                ${OUTPUT_STAMP}
            DEPENDS
                ${in_files}
            COMMENT
                "Generating ${out_files_display}"
            )
    endif ()
    # This command will be run twice for some reason (pass a non-empty string to COMMENT
    # in order to see it). Since vc is not executed from here, this won't be a problem.
    add_custom_command(OUTPUT ${out_files} DEPENDS ${OUTPUT_STAMP} COMMENT "")
endmacro(v_precompile)
