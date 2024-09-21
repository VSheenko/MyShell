#ifndef MYSHELL_XMLCONFIGWORKER_H
#define MYSHELL_XMLCONFIGWORKER_H

#include <string>
#include <filesystem>
#include "tinyxml2.h"
#include <vector>
#include "Logger.h"
#include "UtilsMini.h"

namespace fs = std::filesystem;
namespace xml = tinyxml2;

class XmlConfigWorker {
public:
    explicit XmlConfigWorker(const std::string&);
    bool SetAppsettingsPath(const std::string&);
    std::vector<std::string> GetValues(const std::string&);
private:
    xml::XMLDocument config;
    fs::path file_path;

    std::vector<xml::XMLElement*> GetElements(xml::XMLElement*, std::string&);
};


#endif //MYSHELL_XMLCONFIGWORKER_H
