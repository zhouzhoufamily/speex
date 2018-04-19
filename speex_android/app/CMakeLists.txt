# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


set(JNI_PATH_TEST jni/include)

set(JNI_PATH jni)

set(JNI_SPEEX_INCLUDE ${JNI_PATH}/include)
set(JNI_SPEEX_LIB ${JNI_PATH}/libspeex)

# add header
include_directories(
    ${JNI_SPEEX_INCLUDE}
)

set(SPEEX_SRC ${JNI_PATH}/speex_jni.cpp
    ${JNI_SPEEX_LIB}/bits.c
    ${JNI_SPEEX_LIB}/cb_search.c
    ${JNI_SPEEX_LIB}/exc_10_16_table.c
    ${JNI_SPEEX_LIB}/exc_10_32_table.c
    ${JNI_SPEEX_LIB}/exc_20_32_table.c
    ${JNI_SPEEX_LIB}/exc_5_256_table.c
    ${JNI_SPEEX_LIB}/exc_5_64_table.c
    ${JNI_SPEEX_LIB}/exc_8_128_table.c
    ${JNI_SPEEX_LIB}/filters.c
    ${JNI_SPEEX_LIB}/gain_table.c
    ${JNI_SPEEX_LIB}/gain_table_lbr.c
    ${JNI_SPEEX_LIB}/hexc_10_32_table.c
    ${JNI_SPEEX_LIB}/hexc_table.c
    ${JNI_SPEEX_LIB}/high_lsp_tables.c
    ${JNI_SPEEX_LIB}/kiss_fft.c
    ${JNI_SPEEX_LIB}/kiss_fftr.c
    ${JNI_SPEEX_LIB}/lpc.c
    ${JNI_SPEEX_LIB}/lsp.c
    ${JNI_SPEEX_LIB}/lsp_tables_nb.c
    ${JNI_SPEEX_LIB}/ltp.c
    ${JNI_SPEEX_LIB}/modes.c
    ${JNI_SPEEX_LIB}/modes_wb.c
    ${JNI_SPEEX_LIB}/nb_celp.c
    ${JNI_SPEEX_LIB}/quant_lsp.c
    ${JNI_SPEEX_LIB}/sb_celp.c
    ${JNI_SPEEX_LIB}/smallft.c
    ${JNI_SPEEX_LIB}/speex.c
    ${JNI_SPEEX_LIB}/speex_callbacks.c
    ${JNI_SPEEX_LIB}/speex_header.c
    ${JNI_SPEEX_LIB}/stereo.c
    ${JNI_SPEEX_LIB}/vbr.c
    ${JNI_SPEEX_LIB}/vorbis_psy.c
    ${JNI_SPEEX_LIB}/vq.c
    ${JNI_SPEEX_LIB}/window.c
)

add_library( # Sets the name of the library.
             speex

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${SPEEX_SRC} )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       speex

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )