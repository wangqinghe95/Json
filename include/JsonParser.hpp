#ifndef JSON_PARSER_HPP__
#define JSON_PARSER_HPP__

#include "JsonValue.hpp"

class JsonParser
{
public:
    static JsonValue toJsonValue(const string& strJson);
    static string toJsonString(const JsonValue& jsonValue);
};

#endif