#ifndef COMPILER_OUTPUT_PARSER_HPP_INCLUDED
#define COMPILER_OUTPUT_PARSER_HPP_INCLUDED

#include <iostream>
#include "ctre.hpp"

enum class CompilerOutputLineType
{
    normal,
    warning,
    error,
    info
};

struct CompilerOutputLineInfo
{
    CompilerOutputLineType type;
    std::string fileName;
    std::string line;
    std::string message;
    auto operator<=>(const CompilerOutputLineInfo&) const = default;
};

struct CompilerRegexInfo
{
    const char* name{nullptr};
    CompilerOutputLineType type;
    size_t fileNameIdx;
    size_t lineIdx;
    size_t messageIdx;
};

#define POPULATE_INFO(info, match)                                              \
    {                                                                           \
        info.type = compilerRegexInfo.type;                                     \
        info.fileName = match.get<compilerRegexInfo.fileNameIdx>().to_string(); \
        info.line = match.get<compilerRegexInfo.lineIdx>().to_string();         \
        info.message = match.get<compilerRegexInfo.messageIdx>().to_string();   \
    }

inline CompilerOutputLineInfo GetCompilerOutputLineInfo(std::string_view line)
{
    CompilerOutputLineInfo ret;
    if (auto m = ctre::match<"([{}()[:blank:]#%$~[:alnum:]!&_:+/\\\\\\.\\-]+):([0-9]+):[[:blank:]]([Ww]arning:[[:blank:]].*)">(line))
    {
        static constexpr CompilerRegexInfo compilerRegexInfo = {
            .name = "Compiler warning", .type = CompilerOutputLineType::warning, .fileNameIdx = 1, .lineIdx = 2, .messageIdx = 3};
        POPULATE_INFO(ret, m);
    }
    else if (auto m = ctre::match<"([{}()[:blank:]#%$~[:alnum:]!&_:+/\\\\\\.\\-]+):([0-9]+):[0-9]+:[[:blank:]](.*)">(line))
    {
        static constexpr CompilerRegexInfo compilerRegexInfo = {
            .name = "Compiler error (2)", .type = CompilerOutputLineType::error, .fileNameIdx = 1, .lineIdx = 2, .messageIdx = 3};
        POPULATE_INFO(ret, m);
    }
    else if (auto m = ctre::match<"([{}()[:blank:]#%$~[:alnum:]!&_:+/\\\\\\.\\-]+):([0-9]+):[[:blank:]](.*)">(line))
    {
        static constexpr CompilerRegexInfo compilerRegexInfo = {
            .name = "Compiler error", .type = CompilerOutputLineType::error, .fileNameIdx = 1, .lineIdx = 2, .messageIdx = 3};
        POPULATE_INFO(ret, m);
    }
    else
    {
        std::cout << "Match failed!\n";
    }
    return ret;
}

#endif  // COMPILER_OUTPUT_PARSER_HPP_INCLUDED
