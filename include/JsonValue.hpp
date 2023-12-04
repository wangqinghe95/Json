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
    JsonValue(long long value);
    JsonValue(double value);
    JsonValue(const JsonObject& value);
    JsonValue(const JsonValue& other);
    JsonValue(const JsonArray& value);
    JsonValue& operator=(const JsonValue &other);
    ~JsonValue();

    bool toBoolean() const;

    const JsonValue& operator[](const string& key) const;

    double toDouble() const;

    JsonObject toObject() const;
    JsonArray toArray() const;
    JsonValueType getJsonValueType() const;

    long long toLongLong() const;

    string toString() const;

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

    bool contains(const string& key) const;

    const JsonValue &operator[](const string& key) const;

    map<string, JsonValue>::const_iterator begin() const;
    map<string, JsonValue>::const_iterator end() const;

    void insert(const string& key, const JsonValue& value);

private:
    map<string,JsonValue> m_JsonObject;
};

class JsonArray
{
public:
    JsonArray();

    const JsonValue &operator[](size_t index) const;

    void append(const JsonValue &value);
    vector<JsonValue>::const_iterator begin() const;
    vector<JsonValue>::const_iterator end() const;

private:
    vector<JsonValue> m_JsonArray;
};

#endif