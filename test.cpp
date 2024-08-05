/*
 * Compiler Output Parser: Parse error, warning and messages
 * Copyright (C) 2024  Christo Joseph
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "compiler_output_parser.hpp"

#define USE_GOOGLE_TESTS

#ifdef USE_GOOGLE_TESTS
#include <gtest/gtest.h>

TEST(ErrorLine, Preprocessor_error)
{
    std::string testLine = "/home/test/file/path/test.cpp:3:10: fatal error: test.h: No such file or directory";
    CompilerOutputLineInfo expected = {.type = CompilerOutputLineType::error,
                                       .fileName = "/home/test/file/path/test.cpp",
                                       .line = "3",
                                       .message = "fatal error: test.h: No such file or directory"};
    CompilerOutputLineInfo info = GetCompilerOutputLineInfo(testLine);
    EXPECT_EQ(info.type, expected.type);
    EXPECT_EQ(info.fileName, expected.fileName);
    EXPECT_EQ(info.line, expected.line);
    EXPECT_EQ(info.message, expected.message);
}

TEST(Sanity, Handle_Large_String)
{
    std::string testLine =
        "[4/265] : && "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/x86_64-linux/usr/bin/arm-abc-linux-gnueabi/arm-abc-linux-gnueabi-g++ "
        "--sysroot=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000 -fPIC -march=armv7ve -mthumb -mfpu=neon  "
        "-mfloat-abi=hard -mcpu=cortex-a15 -fno-omit-frame-pointer -fno-optimize-sibling-calls  "
        "--sysroot=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000  -Os -pipe -g "
        "-feliminate-unused-debug-types "
        "-fdebug-prefix-map=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/cortexa15t2hf-neon-abc-linux-gnueabi/ABCFramework-plugins/"
        "3.0+gitrnuuday-r1=/usr/src/debug/ABCFramework-plugins/3.0+gitrnuuday-r1 "
        "-fdebug-prefix-map=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/x86_64-linux= "
        "-fdebug-prefix-map=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000=  -D_TRACE_LEVEL=0  -DTELEMETRY "
        "-fvisibility-inlines-hidden  -march=armv7ve -mthumb -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a15 -fno-omit-frame-pointer "
        "-fno-optimize-sibling-calls  --sysroot=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000 -DNDEBUG  "
        "-Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed -shared -Wl,-soname,libABCFrameworkToken.so -o Token/libABCFrameworkToken.so "
        "Token/CMakeFiles/ABCFrameworkToken.dir/Token.cpp.o Token/CMakeFiles/ABCFrameworkToken.dir/TokenJsonRpc.cpp.o "
        "Token/CMakeFiles/ABCFrameworkToken.dir/Module.cpp.o  "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libboost_program_options-mt.so "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libboost_system-mt.so "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libboost_filesystem-mt.so "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libboost_system-mt.so "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libboost_filesystem-mt.so "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libABCMfrLib.so "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libABCFrameworkProtocols.so.1.0.0 "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libABCFrameworkCryptalgo.so.1.0.0 "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libABCFrameworkTracing.so.1.0.0 "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000/usr/lib/libABCFrameworkCore.so.1.0.0 -pthread && :";
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).type, CompilerOutputLineType::normal);
}

TEST(Sanity, Handle_Large_String2)
{
    std::string testLine =
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "recipe-sysroot-native/usr/bin/arm-abcmllib32-linux-gnueabi/arm-abcmllib32-linux-gnueabi-g++  -Daaab_EXPORTS -Istaging "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "PRIVATE "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/WPEFramework "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/wpe-webkit-1.0 "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "drm "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "downloader "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "drm/aes "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "drm/helper "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "subtitle "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "isobmff "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/gstreamer-1.0 "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/glib-2.0 "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/lib/glib-2.0/include "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/libdash "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/libdash/mpd "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/libxml2 "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/opencdm "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "closedcaptions "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "subtec/subtecparser "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "subtec/libsubtec -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard   "
        "--sysroot=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/"
        "abcv-2023a2-dunfell-r0/lib32-recipe-sysroot  -Os -pipe -g -feliminate-unused-debug-types "
        "-fmacro-prefix-map=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/"
        "abcv-2023a2-dunfell-r0=/usr/src/debug/lib32-aaab/abcv-2023a2-dunfell-r0                      "
        "-fdebug-prefix-map=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/"
        "abcv-2023a2-dunfell-r0=/usr/src/debug/lib32-aaab/abcv-2023a2-dunfell-r0                      "
        "-fdebug-prefix-map=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/"
        "abcv-2023a2-dunfell-r0/lib32-recipe-sysroot=                      "
        "-fdebug-prefix-map=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/"
        "abcv-2023a2-dunfell-r0/recipe-sysroot-native=   -fvisibility-inlines-hidden  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard   "
        "--sysroot=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/"
        "abcv-2023a2-dunfell-r0/lib32-recipe-sysroot  -DAAAB_BUILD_INFO=abcv-2023a2-dunfell -DNO_NATIVE_AV -DPLAYBINTEST_WESTEROSSINK "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/abc/ds "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/abc/ds-hal "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/abc/iarmbus "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/abc/ds-rpc "
        "-I/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/"
        "lib32-recipe-sysroot/usr/include/abc/iarmmgrs/ir   -I=/usr/include/abc/iarmbus/ -I=/usr/include/abc/iarmmgrs-hal -Wno-multichar -std=c++11 "
        "-Wno-non-virtual-dtor -Wno-psabi -g -fPIC    -DAAAB_VANILLA_AES_SUPPORT -DUSE_MAC_FOR_RANDOM_GEN -DAAAB_BUILD_INFO='GNU/Linux-x86_64' "
        "-DNO_NATIVE_AV=1 -DAMLOGIC -DCONTENT_4K_SUPPORTED -DSUPPORT_JS_EVENTS -DIARM_MGR=1 -DUSE_THUNDER_OCDM_API_0_2 -DAAAB_MPD_DRM=1 "
        "-DUSE_OPENCDM -DAAAB_HLS_DRM=1 -DDRM_BUILD_PROFILE=DRM_BUILD_PROFILE_OEM -DTARGET_LITTLE_ENDIAN=1 "
        "-DTARGET_SUPPORTS_UNALIGNED_DWORD_POINTERS=0 -DUSE_CLEARKEY -DUSE_OPENCDM_ADAPTER -DUSE_OPENCDM -DAAAB_CC_ENABLED -DAAAB_RFC_ENABLED "
        "-DAAAB_SUBTEC_CC_ENABLED  -DUSE_SYSTEMD_JOURNAL_PRINT=1 -DSD_JOURNAL_SUPPRESS_LOCATION=1 -DUSE_ICX_TELEMETRY -DAAAB_WPEWEBKIT_JSBINDINGS "
        "-DUSE_CPP_THUNDER_PLUGIN_ACCESS -MD -MT CMakeFiles/aaab.dir/priv_aaab.cpp.o -MF CMakeFiles/aaab.dir/priv_aaab.cpp.o.d -o "
        "CMakeFiles/aaab.dir/priv_aaab.cpp.o -c "
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/armv7at2hf-neon-abcmllib32-linux-gnueabi/lib32-aaab/abcv-2023a2-dunfell-r0/git/"
        "priv_aaab.cpp";
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).type, CompilerOutputLineType::normal);
}

TEST(ErrorLine, Linker_error_lib_not_found)
{
    std::string testLine = "/usr/bin/ld: cannot find -lmagic";
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).type, CompilerOutputLineType::error);
}

TEST(WarningLine, General_warning)
{
    std::string testLine = "cc1: warning: command line option '-std=c++11' is valid for C++/ObjC++ but not for C";
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).type, CompilerOutputLineType::warning);
}

TEST(NoteLine, General_note)
{
    std::string testLine = "<command-line>:0:0: note: this is the location of the previous definition";
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).type, CompilerOutputLineType::info);
}

TEST(WarningLine, General_warning_2)
{
    std::string testLine = "<command-line>:0:0: warning: \"_TRACE_LEVEL\" redefined";
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).type, CompilerOutputLineType::warning);
}

TEST(WarningLine, Preprocessor_warning)
{
    std::string testLine =
        "/home/user123/codeblocks/codeblocks-dev/git/src/include/compiler.h:150:22: warning:   ‘wxString1 RegExStruct::token’ [-Wreorder]";
    CompilerOutputLineInfo expected = {.type = CompilerOutputLineType::warning,
                                       .fileName = "/home/user123/codeblocks/codeblocks-dev/git/src/include/compiler.h",
                                       .line = "150",
                                       .message = "warning:   ‘wxString1 RegExStruct::token’ [-Wreorder]"};
    CompilerOutputLineInfo info = GetCompilerOutputLineInfo(testLine);
    std::cout << " info.message : [" << info.message << "]\n";
    EXPECT_EQ(info.type, expected.type);
    EXPECT_EQ(info.fileName, expected.fileName);
    EXPECT_EQ(info.line, expected.line);
    EXPECT_EQ(info.message, expected.message);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else

int main(int argc, char **argv)
{
    std::string testLine = "/usr/bin/ld: cannot find -lmagic";
    if (auto type = GetCompilerOutputLineInfo(testLine).type != CompilerOutputLineType::error)
    {
        std::cout << "Test failed type : " << type << "\n";
    }
}

#endif
