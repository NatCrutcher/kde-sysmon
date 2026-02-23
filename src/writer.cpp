#include "writer.h"
#include <cstdio>
#include <unistd.h>

void atomic_write(const std::string& path, const std::string& content) {
    std::string tmp = path + ".tmp";
    FILE* f = fopen(tmp.c_str(), "w");
    if (!f) return;
    fwrite(content.data(), 1, content.size(), f);
    fclose(f);
    rename(tmp.c_str(), path.c_str());
}
