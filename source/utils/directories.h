#ifndef DIRECTORY_HEADER_FILE
#define DIRECTORY_HEADER_FILE
#include <vector>
#include <unistd.h>
#include <dirent.h>


inline std::vector<std::string> LS(std::string const& path,std::string const& filter="*"){
    std::vector<std::string> a;
    DIR* dir;
    dirent * ent;
    std::string tmp;
    dir=opendir(path.c_str());
    while((ent=readdir(dir)))
        if(ent->d_name[0]!='.'){
	    tmp=ent->d_name;
	    if(filter=="*")
		a.push_back(tmp);
	    else
		if (tmp.find(filter) != std::string::npos)
            	    a.push_back(tmp);
	}
    closedir(dir);
    return a;
}

inline void CD(std::string const& path){
    chdir(path.c_str());
}

#endif
