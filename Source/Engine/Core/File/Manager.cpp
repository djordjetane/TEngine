#include <precomp.h>
#include "Manager.h"


namespace File {

    namespace fs = std::filesystem;

    TVector<StringView> Manager::GetDirectoryContent(const Path& path)
    {
        if(!Exists(path) && fs::is_directory(path)) 
            return {};

        TVector<StringView> content{2};
        for(auto& It : fs::directory_iterator(path))
        {
            if(!fs::is_regular_file(It.path()))
                continue;
            content.emplace_back(fs::absolute(It.path()).string());
        }
        return content;
    }
}
