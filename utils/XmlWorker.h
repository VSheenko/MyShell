#ifndef MYSHELL_XMLWORKER_H
#define MYSHELL_XMLWORKER_H

#include <string>
#include <filesystem>
#include "tinyxml2.h"
#include <vector>

namespace fs = std::filesystem;
namespace xml = tinyxml2;

class XmlWorker {
public:
    explicit XmlWorker(const std::string&);
    bool SetAppsettingsPath(const std::string&);
    std::vector<std::string> GetValues(const std::string&);
private:
    xml::XMLDocument config;
    fs::path file_path;

    std::vector<std::string> Split(std::string, const std::string&);
    std::vector<xml::XMLElement*> GetElements(xml::XMLElement*, std::string&);
};


#endif //MYSHELL_XMLWORKER_H
