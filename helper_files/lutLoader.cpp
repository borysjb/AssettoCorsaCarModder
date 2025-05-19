#include<bits/stdc++.h>

void lutLoader(const std::filesystem::path &lutpath, std::map<int,int> &map) {
    std::string line;
    std::ifstream file;
    try{
        std::ifstream file(lutpath);
    } catch (const std::exception &e) {
        throw std::runtime_error("Could not open power.lut file");
    }


    while(getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == ';' || line[0] == '[') {
            continue;
        }

        std::size_t found = line.find("|");
        if (found == std::string::npos) {
            continue;
        }

        // Remove whitespace and comments after values
        std::string key = line.substr(0, found);
        std::string value = line.substr(found + 1);
        size_t commentPos = value.find(";");
        if (commentPos != std::string::npos) {
            value = value.substr(0, commentPos);
        }
        
        // Trim whitespace
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Parse the values
        map[std::stoi(key)] = std::stoi(value);
    }
    file.close();
}