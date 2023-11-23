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
    JsonValue(const string& value);
    JsonValue(const string::const_iterator& begin, const string::const_iterator& end);
    JsonValue(bool value);
    JsonValue(const JsonObject& value);
    JsonValue(const JsonValue& other) ;
    JsonValue& operator=(const JsonValue &other);

    string toString() const;
    bool toBoolean() const;
    long long toLongLong() const;
    double toDouble() const;
    JsonObject toObject() const;

    JsonValueType getJsonValueType() const;
private:
    void setJsonType(JsonValueType type);
    void swap(JsonValue &other);
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

    void insert(const string& key, const JsonValue& value);
    const JsonValue &operator[](const string& key) const;

    map<string, JsonValue>::const_iterator begin() const;
    map<string, JsonValue>::const_iterator end() const;

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