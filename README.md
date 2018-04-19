# speex

# Android
IDE:					Android Studio
NDK Build tools:		CMake use CMakeList.txt

# Step
1. Download the speex codec from https://www.speex.org/downloads. [I use the Speex 1.2.0]
2. New an empty android project with cplusplus support. [Select the default config]
3. Copy the libspeex and include directories under the speex source directory and their subdirectory files to the $project/jni directory;
and remove the file speex_config_types.h.in suffix '.in' under the file include/speex folder;Change @size16@ to short and change @size32@ to int(change the mapping Java data type).
4. Create the package com.speex.util and new class SpeexUtil;Use javah build a jni cplusplus file such as speex_jni.cpp, and implement all function.Then, you can export the class SpeexUtil to the jar package like speex_1.2.0.jar.
5. Create the JNI wrapper class speex_jni.cpp, which is used to call the C code function in Speex.
6. Edit the content of CMakeLists.txt. [https://cmake.org/cmake-tutorial]
7. Edit the build.gradle in project.
	externalNativeBuild {
		cmake {
			cFlags "-DFIXED_POINT -DUSE_KISS_FFT -UHAVE_CONFIG_H" //don`t use -DEXPORT=\"\", will be escape error:souce $project/Nijna not found.(solution see issue 3.)
           	cppFlags ""
            abiFilters 'armeabi', 'armeabi-v7a'
            }
        }
		externalNativeBuild {
        cmake {
            path "CMakeLists.txt"
        }
    }
dependencies {
    ...
    api files('libs/speex_1.2.0.jar')
}
8. Build and test.

# issues:
1. Note that the jni package names are the same.[speex_1.2.0.jar and speex_jni.cpp, you can compile with my code.]
2. Some test files such as testenc.c/testenc_uwb.c/testenc_wb.c, don`t introduce builds when edit CMakeList.txt.
3. Some of the macros used in C code are defined in the NDK, like -DFIXED_POINT -DEXPORT="".It will be error when use the -DEXPORT=\"\"
in build.gradle, I guess this escape is in conflict with some commands.
#[If you use ndk build not CMakeLists.txt will funpass normally]
Error:(49, 1) error: unknown type name 'EXPORT'
Some modify:
	Change variable name EXPORT --> JNIEXPORT;
	#include "jni.h".
	
Speex codec resource url=https://www.speex.org/downloads
Reference documents url=https://blog.csdn.net/jianiuqi/article/details/53174155
Android Studio jni external tool: url=https://www.jianshu.com/p/e689d0196a17
[build .jar can be add in external too]

	
