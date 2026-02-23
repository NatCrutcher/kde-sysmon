#pragma once
#include <string>

// Atomically write content to path via tmp+rename
void atomic_write(const std::string& path, const std::string& content);
