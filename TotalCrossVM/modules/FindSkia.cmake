# Copyright (C) 2000-2013 SuperWaba Ltda.
# Copyright (C) 2014-2020 TotalCross Global Mobile Platform Ltda.
#
# SPDX-License-Identifier: LGPL-2.1-only

# Skia as graphics backend.
# Together with its dependencies.
# This should be a custom build from source, put into /opt/build/skia
SET ( SKIA_DIR ${CMAKE_MODULE_PATH}/skia CACHE PATH "Skia directory")

find_path(
  SKIA_CONFIG_INCLUDE_DIR 
  SkUserConfig.h 
  HINTS "${SKIA_DIR}/include/config"
  NO_CMAKE_FIND_ROOT_PATH #workaround needed for Android build
  )
find_path(
  SKIA_CORE_INCLUDE_DIR 
  SkCanvas.h 
  HINTS "${SKIA_DIR}/include/core"
  NO_CMAKE_FIND_ROOT_PATH #workaround needed for Android build
  )
find_path(
  SKIA_UTILS_INCLUDE_DIR 
  SkRandom.h 
  HINTS "${SKIA_DIR}/include/utils" 
  NO_CMAKE_FIND_ROOT_PATH #workaround needed for Android build
  )
find_path(
  SKIA_EFFECTS_INCLUDE_DIR 
  SkImageSource.h 
  HINTS "${SKIA_DIR}/include/effects" 
  NO_CMAKE_FIND_ROOT_PATH #workaround needed for Android build
  )
find_path(
  SKIA_GPU_INCLUDE_DIR 
  GrContext.h 
  HINTS "${SKIA_DIR}/include/gpu" 
  NO_CMAKE_FIND_ROOT_PATH #workaround needed for Android build
  )
find_path(
  SKIA_GPU2_INCLUDE_DIR 
  gl/GrGLDefines.h 
  HINTS "${SKIA_DIR}/src/gpu" 
  NO_CMAKE_FIND_ROOT_PATH #workaround needed for Android build
  )

SET ( SKIA_INCLUDE_DIRS
  "${SKIA_CONFIG_INCLUDE_DIR}"
  "${SKIA_CORE_INCLUDE_DIR}"
  "${SKIA_UTILS_INCLUDE_DIR}"
  "${SKIA_EFFECTS_INCLUDE_DIR}"
  "${SKIA_GPU_INCLUDE_DIR}"
  "${SKIA_GPU2_INCLUDE_DIR}"
)

IF (DEFINED ANDROID_ABI)
  SET ( SKIA_LIBRARY_DIRS "${ANDROID_LIBS_DIR}" CACHE PATH "")
  SET ( SKIA_LIBRARIES "${SKIA_LIBRARY_DIRS}/${ANDROID_ABI}/libskia.so" CACHE PATH "")
ELSEIF (APPLE)
  IF(CMAKE_GENERATOR STREQUAL Xcode)
    SET ( SKIA_LIBRARY_DIRS "${SKIA_DIR}/out/Release/ios" CACHE PATH "")
  ELSE()
    SET ( SKIA_LIBRARY_DIRS "${SKIA_DIR}/out/Release/macos" CACHE PATH "")
  ENDIF(CMAKE_GENERATOR STREQUAL Xcode)
ELSEIF (UNIX AND (CMAKE_SYSTEM_NAME STREQUAL "Linux"))
  SET ( SKIA_LIBRARY_DIRS "${SKIA_DIR}/out/Release/linux/${CMAKE_SYSTEM_PROCESSOR}" CACHE PATH "")
ENDIF(DEFINED ANDROID_ABI)

IF (NOT DEFINED SKIA_LIBRARIES)
  SET ( SKIA_LIBRARIES
    "${SKIA_LIBRARY_DIRS}/libskia.a" CACHE PATH ""
  #  "${SKIA_LIBRARY_DIRS}/libskia_pdf.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_core.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_effects.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_images.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_utils.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_opts.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_opts_ssse3.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_opts_sse4.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_ports.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_sfnt.a"
  #  "${SKIA_LIBRARY_DIRS}/libskia_skgpu.a"
  )
ENDIF (NOT DEFINED SKIA_LIBRARIES)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    FIND_LIBRARY(ApplicationServices_LIBRARY ApplicationServices )
    FIND_LIBRARY(AGL_LIBRARY AGL )
    FIND_PACKAGE ( ZLIB )
#    LIST ( APPEND SKIA_LIBRARIES
#        "${SKIA_LIBRARY_DIRS}/libetc1.a"
#        "${SKIA_LIBRARY_DIRS}/libSkKTX.a"
#        "${SKIA_LIBRARY_DIRS}/libskflate.a"
#        "${SKIA_LIBRARY_DIRS}/libjpeg.a"
#        "${SKIA_LIBRARY_DIRS}/libwebp_enc.a"
#        "${SKIA_LIBRARY_DIRS}/libwebp_dec.a"
#        "${SKIA_LIBRARY_DIRS}/libwebp_utils.a"
#        "${SKIA_LIBRARY_DIRS}/libwebp_dsp.a"
#        "${SKIA_LIBRARY_DIRS}/libwebp_demux.a"
#        ${ZLIB_LIBRARIES}
#        ${ApplicationServices_LIBRARY}
#        ${AGL_LIBRARY}
#    )
  ENDIF ( )

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # PKG_CHECK_MODULES ( FreeType freetype2 )
    # PKG_CHECK_MODULES ( FontConfig fontconfig )
    # FIND_PACKAGE ( OpenGL )
    # FIND_PACKAGE ( Threads )
    # FIND_PACKAGE ( PNG )
    # FIND_PACKAGE ( ZLIB )
    # FIND_PACKAGE ( GIF )
    # LIST ( APPEND SKIA_LIBRARIES
        # "${SKIA_LIBRARY_DIRS}/obj/gyp/libetc1.a"
        # "${SKIA_LIBRARY_DIRS}/obj/gyp/libSkKTX.a"
        # "${SKIA_LIBRARY_DIRS}/obj/gyp/libskflate.a"
        # "${SKIA_LIBRARY_DIRS}/obj/gyp/libjpeg.a"
        # "${SKIA_LIBRARY_DIRS}/obj/gyp/libwebp_enc.a"
        # "${SKIA_LIBRARY_DIRS}/obj/gyp/libwebp_dec.a"
        # "${SKIA_LIBRARY_DIRS}/obj/gyp/libwebp_utils.a"
        # "${SKIA_LIBRARY_DIRS}/obj/gyp/libwebp_dsp.a"
        # "${SKIA_LIBRARY_DIRS}/obj/gyp/libwebp_demux.a"
        # ${FreeType_LIBRARIES}
        # ${FontConfig_LIBRARIES}
        # ${OPENGL_LIBRARIES}
        # ${PNG_LIBRARIES}
        # ${GIF_LIBRARIES}
        # ${ZLIB_LIBRARIES}
        # ${CMAKE_THREAD_LIBS_INIT}
    # )
    # In Linux, to avoid circular dependency problem, add start-group and end-group for Skia static files.
    SET ( SKIA_LIBRARIES -Wl,--start-group ${SKIA_LIBRARIES} -Wl,--end-group )
  ENDIF ( )

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Skia REQUIRED_VARS SKIA_LIBRARIES SKIA_INCLUDE_DIRS)