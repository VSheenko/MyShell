//
// Created by vovan on 15.09.2024.
//

#ifndef MYSHELL_XMLWORKER_H
#define MYSHELL_XMLWORKER_H

#include <string>
#include <filesystem>
#include "tinyxml2.h"

namespace fs = std::filesystem;
namespace xml = tinyxml2;

class XmlWorker {
public:
    explicit XmlWorker(const std::string&);
    bool SetAppsettingsPath(const std::string&);
    std::string GetValue(const std::string&);
private:
    xml::XMLDocument config;
    fs::path file_path;
};


#endif //MYSHELL_XMLWORKER_H
