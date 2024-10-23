/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include <string>
#include <vector>

struct kevent;

namespace BloxxEngine
{

class DirWatcher
{
  public:
    explicit DirWatcher(std::string dir);

    std::vector<std::string> PollFiles();

    DirWatcher(DirWatcher &) = delete;
    DirWatcher(DirWatcher &&) = delete;
    DirWatcher &operator=(DirWatcher &) = delete;
    DirWatcher &operator=(DirWatcher &&) = delete;

  private:
    struct File
    {
        std::string Path;
        std::string FileName;
        time_t MTime;
        bool IsDir;
        std::vector<File> Children;
        int FD = -1;
    };

    void Create();
    void AddDir(std::string dir);
    void Destroy();

    void AddDir(File& file);
    void PollFilesIn(std::vector<std::string>& changes, File& dir);
    void Rescan(std::vector<std::string>& changes, File& dir);

    void AddKEvent(File& file);

    void CloseFile(File& file);

    const std::string m_Dir;
    bool m_Recursive = true;
    bool m_CheckFiles = true;
    std::vector<File> m_Dirs;
    std::vector<kevent> m_Events;
    int m_KQueue;

};

} // namespace BloxxEngine
