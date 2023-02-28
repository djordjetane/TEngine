#pragma once

namespace File {
    class Manager
    {
    public:
        inline static bool Exists(const Path& path) { return std::filesystem::exists(path); }
        static TVector<StringView> GetDirectoryContent(const Path& path);
        inline static bool IsRegular(const Path& path) { return std::filesystem::is_regular_file(path); }
        inline static bool IsDirectory(const Path& path) { return std::filesystem::is_directory(path); }
        inline static bool HasExtension(const Path& path, const StringView& ext)
        {            
            return path.extension().compare(ext) == 0;
        }
        inline static bool HasExtension(const Path &path)
        {
	        return path.has_extension();
        }
        inline static Path Join(const Path &parent, const Path &child)
        {
            return (IsDirectory(parent) ? Path{} : (parent / child));
        }
    private:
    };
    
}
