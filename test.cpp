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
        "/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/x86_64-linux/usr/bin/arm-rdk-linux-gnueabi/arm-rdk-linux-gnueabi-g++ "
        "--sysroot=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000 -fPIC -march=armv7ve -mthumb -mfpu=neon  "
        "-mfloat-abi=hard -mcpu=cortex-a15 -fno-omit-frame-pointer -fno-optimize-sibling-calls  "
        "--sysroot=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/sysroots/abcd123456ef-xy1000  -Os -pipe -g "
        "-feliminate-unused-debug-types "
        "-fdebug-prefix-map=/home/user123/build_linear/build-abcd123456ef-xy1000/tmp/work/cortexa15t2hf-neon-rdk-linux-gnueabi/ABCFramework-plugins/"
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
