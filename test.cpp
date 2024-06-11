#include <gtest/gtest.h>
#include "compiler_output_parser.hpp"

using namespace std;

TEST(ErrorTest, HandlesError1)
{
    std::string testLine = "/home/test/file/path/test.cpp:3:10: fatal error: test.h: No such file or directory";
    CompilerOutputLineInfo expected = {.type = CompilerOutputLineType::error,
                                       .fileName = "/home/test/file/path/test.cpp",
                                       .line = "3",
                                       .message = "fatal error: test.h: No such file or directory"};
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).type, expected.type);
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).fileName, expected.fileName);
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).line, expected.line);
    EXPECT_EQ(GetCompilerOutputLineInfo(testLine).message, expected.message);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
