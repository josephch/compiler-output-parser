#include <cstring>
#include <fstream>
#include <vector>
#include "compiler_output_parser.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage %s <log file>\n", argv[0]);
        return -1;
    }
    std::ifstream stream(argv[1]);
    if (!stream.good())
    {
        fprintf(stderr, "Error opening file  %s : %s\n", argv[0], strerror(errno));
        return -1;
    }

    std::vector<CompilerOutputLineInfo> compilerOutputLineInfos;
    std::string line;
    while (std::getline(stream, line))
    {
        CompilerOutputLineInfo compilerOutputLineInfo = GetCompilerOutputLineInfo(line);
        if (compilerOutputLineInfo.type != CompilerOutputLineType::normal)
        {
            compilerOutputLineInfos.push_back(std::move(compilerOutputLineInfo));
        }
    }
    stream.close();
    for (const CompilerOutputLineInfo& compilerOutputLineInfo : compilerOutputLineInfos)
    {
        const char* type;
        switch (compilerOutputLineInfo.type)
        {
            case CompilerOutputLineType::warning:
                type = "WARNING :";
                break;
            case CompilerOutputLineType::error:
                type = "ERROR :";
                break;
            case CompilerOutputLineType::info:
                type = "INFO :";
                break;
            default:
                type = "UNKNOWN :";
                break;
        }
        printf("%s ", type);
        if (!compilerOutputLineInfo.fileName.empty())
        {
            if (compilerOutputLineInfo.fileName.at(0) == '/')
            {
                printf("file://%s ", compilerOutputLineInfo.fileName.c_str());
            }
            else
            {
                printf("%s", compilerOutputLineInfo.fileName.c_str());
            }
            if (!compilerOutputLineInfo.line.empty())
            {
                printf(":%s ", compilerOutputLineInfo.line.c_str());
            }
        }
        if (!compilerOutputLineInfo.message.empty())
        {
            printf("%s\n", compilerOutputLineInfo.message.c_str());
        }
    }
}
