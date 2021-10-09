#pragma once

namespace File {
    using Path = std::filesystem::path;
    using Dirent = std::filesystem::directory_entry;    
    using DirIt  = std::filesystem::directory_iterator;

    class Manager
    {
    public:
        inline static bool Exists(const Path& path) { return std::filesystem::exists(path); }
        static TVector<StringView> GetDirectoryContent(const Path& path);
        inline static bool IsRegular(const Path& path) { return std::filesystem::is_regular_file(path); }
        inline static bool HasExtension(const Path& path, const StringView& ext)
        {            
            return path.extension().compare(ext) == 0;
        }
    private:
    };
    
}
