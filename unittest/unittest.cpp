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

    JsonValue root = JsonParser::toJsonValue(str_json);
    
    JsonObject result = root.toObject();
    int number = result["Integer"].toLongLong();
    double d_number = result["Double"].toDouble();
    string string_mess = result["String"].toString();
    bool b_true = result["True"].toBoolean();
    bool b_false = result["Fasle"].toBoolean();

    DEBUG("Integer: ", number);
    DEBUG("Double: ", d_number);
    DEBUG("String: ", string_mess);
    DEBUG("Bool: ", b_true);

    JsonObject object = result["object"].toObject();
    JsonArray array = result["Array"].toArray();

    return 0;
}