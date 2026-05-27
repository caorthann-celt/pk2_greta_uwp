#pragma once

#include <string>

void pk2_set_paths(const std::string& assets_path, const std::string& data_path);
void pk2_init();
void pk2_main(bool dev_mode, bool show_fps, bool test_level, const std::string& test_path);
