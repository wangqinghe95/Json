#include "JsonParser.hpp"
// #include "../FilesSystem/include/FileOperation.hpp"
// #include "../Logger/include/Logger.hpp"
#include "FileOperation.hpp"
#include "Logger.hpp"

#include <type_traits>

const char* JSONFILENAME = "./unittest.json";
// const std::string config_file("/home/user/Desktop/Github/Json/unittest/unittest.json");
const std::string config_file("./unittest/unittest.json");


string readJsonFile(const string file_name)
{
    FileOperation CFiles;
    string json_string;
    if(CFiles.isFileExist(file_name)) {
        CFiles.readContentFromFile(json_string, file_name);
    }
    else {
        WARN(config_file, "is not exist");
    }
    return json_string;
}

int main()
{
    string str_json = readJsonFile(config_file);
    DEBUG("str_jsong:size", str_json.size());

    JsonValue root = JsonParser::toJsonValue(str_json);
    
    // DEBUG("type ", static_cast<std::underlying_type<JsonValueType>::type>(type));
    DEBUG(JsonParser::toJsonString(root));

    return 0;
}