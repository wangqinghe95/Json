#ifndef JSON_VALUE_HPP__
#define JSON_VALUE_HPP__

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

enum class JsonValueType
{
    Null = 0,
    Boolean,
    Integer,
    Double,
    String,
    Object,
    Array
};

class JsonObject;
class JsonArray;

class JsonValue
{
public:
    JsonValue();

private:
    void setJsonType(JsonValueType type);

private:
    JsonValueType m_type;
    union 
    {
        bool _bool;
        long long _longlong;
        double _double;
        string* _string;
        JsonObject *_object;
        JsonArray *_array;
    }m_value;
    
};

class JsonObject
{
public:
    JsonObject();

private:
    map<string,JsonValue> m_JsonObject;
};

class JsonArray
{
public:
    JsonArray();

private:
    vector<JsonValue> m_JsonArray;
};



#endif