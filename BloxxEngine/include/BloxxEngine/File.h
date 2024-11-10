// Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
// All rights reserved.

#pragma once

#include <filesystem>
#include <sstream>
#include <string>


namespace BloxxEngine
{
static std::string GetAbsolutePathToFile(const std::string &filePath)
{
    std::string path = filePath;
    if (filePath.substr(0, 2) == "./" || filePath.substr(0, 2) == ".\\")
    {
        path = filePath.substr(2);
    }

    return std::filesystem::absolute(path).string();
}
} // namespace BloxxEngine