##
##  This is the cmake intro
cmake_minimum_required(VERSION 3.10)
##
##  Abort if in source build
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

project(parsec_server CXX)
##
##  Load values from the os-release information.
if (EXISTS "/etc/os-release")
    file(STRINGS "/etc/os-release" OSINFO)
    foreach(NameAndValue ${OSINFO})
        # Strip leading spaces
        string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})
        # Find variable name
        string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
        # Find the value
        string(REPLACE "${Name}=" "" Value ${NameAndValue})
        string(REPLACE "\"" "" Value ${Value})
        # Set the variable
        set(${Name} "${Value}")
    endforeach()
else()
    message(WARNING "os-release info not found. Assuming a windows build.")
endif()
##
##  Here we look for the OpenGL support.
set(OpenGL_GL_PREFERENCE GLVND)
find_package (OpenGL COMPONENTS OpenGL REQUIRED)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(OPENPARSEC_GL_LIB ${OPENGL_gl_LIBRARY})
    set(OPENPARSEC_GLU_LIB ${OPENGL_glu_LIBRARY})
else()
    set(OPENPARSEC_TARGETS OpenGL::OpenGL OpenGL::GLU Freetype::Freetype)
endif()

##
##  Here we try to find the freetype library support.

set(OPENPARSEC_FREETYPE_LIB "freetype")

find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(
    CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
)
##
##  This is the application install dir. Set fixed for now. Should be changed.
set (XDG_APPS_INSTALL_DIR /usr/local/bin)


set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(FeatureSummary)
include(GNUInstallDirs)


add_feature_info(FREETYPE_FOUND "libfreetype" "FreeType lib is needed to build the openparsec project" "http://freetype.sf.net" FALSE "" "")
##
##  
include_directories(${FREETYPE_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})

add_compile_options (-O0 -g -Wall -std=c++14 -fPIC -DPARSEC_SERVER -DDEBUG -DPARSEC_DEBUG)
##
##  To get rid of the anoying warning about offsetof we add that until some other way to handle the object properties
##  have been introduced.
add_compile_options (-Wno-invalid-offsetof)
link_libraries(SDL2_mixer)

include_directories(${CMAKE_SOURCE_DIR} ../libparsec/include include ../libraries)
##  Static linkage to the povmodeler sub-systems.
##  Using --start-group/--end-group to have a recursive symbol resolution.

add_executable(${PROJECT_NAME}
        con_act_sv.cpp
        con_aux_sv.cpp
        con_com_sv.cpp
        con_ext_sv.cpp
        con_info_sv.cpp
        con_int_sv.cpp
        con_list_sv.cpp
        con_load_sv.cpp
        con_main_sv.cpp
        con_std_sv.cpp
        e_colldet_sv.cpp
        e_connmanager.cpp
        e_gameserver.cpp
        e_global_sv.cpp
        e_packethandler.cpp
        e_simnetinput.cpp
        e_simnetoutput.cpp
        e_simplayerinfo.cpp
        e_simulator.cpp
        e_world.cpp
        g_main_sv.cpp
        g_player.cpp
        g_stgate_sv.cpp
        inp_main_sv.cpp
        MasterServer.cpp
        MasterServerItem.cpp
        net_packetdriver.cpp
        net_util_sv.cpp
        nl_udpdriver.cpp
        sl_util_sv.cpp
        sv_main.cpp
        sys_err_sv.cpp
        sys_msg_sv.cpp
        sys_util_sv.cpp

    ../libparsec/net_stream.cpp
    ../libparsec/con_arg.cpp
    ../libparsec/g_extra.cpp
    ../libparsec/utl_bsp.cpp
    ../libparsec/e_relist.cpp
    ../libparsec/obj_cust.cpp
    ../libparsec/g_emp.cpp
    ../libparsec/g_telep.cpp
    ../libparsec/sl_path.cpp
    ../libparsec/utl_cull.cpp
    ../libparsec/net_swap.cpp
    ../libparsec/utl_clpo.cpp
    ../libparsec/net_pckt_gmsv.cpp
    ../libparsec/utl_math2.cpp
    ../libparsec/obj_type.cpp
    ../libparsec/con_tab.cpp
    ../libparsec/con_vald.cpp
    ../libparsec/obj_creg.cpp
    ../libparsec/obj_odt.cpp
    ../libparsec/sys_date.cpp
    ../libparsec/debug.cpp
    ../libparsec/sys_swap.cpp
    ../libparsec/net_util.cpp
    ../libparsec/sw_path.cpp
    ../libparsec/sys_file.cpp
    ../libparsec/e_modulemanager.cpp
    ../libparsec/utl_clip.cpp
    ../libparsec/net_wrap.cpp
    ../libparsec/utl_logfile.cpp
    ../libparsec/sw_timer.cpp
    ../libparsec/obj_clas.cpp
    ../libparsec/sl_timer.cpp
    ../libparsec/utl_math.cpp
    )

target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL GLEW::GLEW SDL2::SDL2 ncurses)


##
##
##  Here we describe the files and positions of
##  to be installed files.
## install( PROGRAMS ${CMAKE_BINARY_DIR}/povmodeler DESTINATION  ${CMAKE_INSTALL_BINDIR} )
##
##  desktop file
## install( FILES povmodeler.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
##
##  application specific files
## install( FILES povraydocmap.xml baseinsertrules.xml questionmark.png DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/povmodeler)
##
## Examples and libraries.
## install( DIRECTORY examples/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/povmodeler/examples )
## install( DIRECTORY library/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/povmodeler/library )
##
##  Icon files in various resolutions
## install( FILES hi48-app-povmodeler.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/48x48/apps/ RENAME povmodeler.png)
## install( FILES hi32-app-povmodeler.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/32x32/apps/ RENAME povmodeler.png)
## install( FILES hi22-app-povmodeler.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/22x22/apps/ RENAME povmodeler.png)
## install( FILES hi16-app-povmodeler.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/16x16/apps/ RENAME povmodeler.png)
##
##  Packaging setup.
##
##  Only supported on unix systems.
## IF (UNIX)
##
##  Debian specific switches
## SET (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
## SET (CPACK_DEBIAN_PACKAGE_DEBUG ON)
##
##  RPM specific switches
## set (CPACK_RPM_PACKAGE_DEBUG OFF)
## set (CPACK_RPM_PACKAGE_AUTOREQ ON)
##
##  Some systems do not like it to have the directories in the files to create
## set (CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/share/applications"
##                                                    "/usr/share/icons"
##                                                    "/usr/share/icons/hicolor"
##                                                    "/usr/share/icons/hicolor/16x16"
##                                                    "/usr/share/icons/hicolor/16x16/apps"
##                                                    "/usr/share/icons/hicolor/22x22"
##                                                    "/usr/share/icons/hicolor/22x22/apps"
##                                                    "/usr/share/icons/hicolor/32x32"
##                                                    "/usr/share/icons/hicolor/32x32/apps"
##                                                    "/usr/share/icons/hicolor/48x48"
##                                                   "/usr/share/icons/hicolor/48x48/apps"
##                                                    )

##
##
## SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
## SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This is the former KPovModeler now PovModeler because stripped off the KDE stuff")
## SET(CPACK_PACKAGE_VENDOR "Simulated-Universe")
## SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
## SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
## SET(CPACK_PACKAGE_VERSION_MAJOR ${POVMODELER_VERSION_MAJOR})
## SET(CPACK_PACKAGE_VERSION_MINOR ${POVMODELER_VERSION_MINOR})
## set(CPACK_PACKAGE_VERSION_PATCH ${POVMODELER_VERSION_PATCH})
## SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
## SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
## SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${ID}-${VERSION_ID}")
## SET(CPACK_PACKAGE_CONTACT "Hans-Juergen Lange <hjl@simulated-universe.de>")
## SET(CPACK_PACKAGE_SECTION "graphics")
## INCLUDE(CPack)
## ENDIF (UNIX)
