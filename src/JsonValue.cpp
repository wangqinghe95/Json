#include "JsonValue.hpp"
#include "Logger.hpp"

JsonValue::JsonValue()
{
    setJsonType(JsonValueType::Null);
}

JsonValue::JsonValue(const string& value)
{
    setJsonType(JsonValueType::String);
    // m_value._string->assign(value);
    m_value._string = new string(value);
}

JsonValue::JsonValue(const string::const_iterator& begin, const string::const_iterator& end)
{
    setJsonType(JsonValueType::String);
    m_value._string = new string(begin,end);
}

JsonValue::JsonValue(bool value)
{
    setJsonType(JsonValueType::Boolean);
    m_value._bool = value;
}

JsonValue::JsonValue(long long value)
{
    setJsonType(JsonValueType::Integer);
    m_value._longlong = value;
}

JsonValue::JsonValue(double value)
{
    setJsonType(JsonValueType::Double);
    m_value._double = value;
}

JsonValue::JsonValue(const JsonObject& value)
{
    setJsonType(JsonValueType::Object);
    m_value._object = new JsonObject(value);
}

JsonValue::JsonValue(const JsonArray& value)
{
    setJsonType(JsonValueType::Array);
    m_value._array = new JsonArray(value);
}

JsonValue& JsonValue::operator=(const JsonValue &other)
{
    DEBUG("operator=");

    JsonValue(other).swap(*this);
    return *this;
}

void JsonValue::swap(JsonValue &other)
{
    std::swap(m_value, other.m_value);
    std::swap(m_type, other.m_type);
}

long long JsonValue::toLongLong() const
{
    if(JsonValueType::Integer == m_type) {
        return m_value._longlong;
    }
    return 0;
}

double JsonValue::toDouble() const
{
    if(JsonValueType::Double == m_type) {
        return m_value._double;
    }
    return 0.0;
}

JsonObject JsonValue::toObject() const
{
    if(JsonValueType::Object == m_type) {
        return *m_value._object;
    }
    return JsonObject();
}

JsonArray JsonValue::toArray() const
{
    if(JsonValueType::Array == m_type) {
        return *m_value._array;
    }
    return JsonArray();
}

bool JsonValue::toBoolean() const
{
    if(JsonValueType::Boolean == m_type) {
        return m_value._bool;
    }
    return false;
}

// JsonValue JsonValue::operator[](const string& key)
// {
//     return toObject()[key];
// }

JsonValue::JsonValue(const JsonValue& other)
{
    // DEBUG("const JsonValue& other");
    m_type = other.getJsonValueType();
    switch (m_type)
    {
    case JsonValueType::Null:
    case JsonValueType::Boolean:
    case JsonValueType::Integer:
    case JsonValueType::Double:
        m_value = other.m_value;
        break;
    case JsonValueType::String:
        m_value._string = new string(*other.m_value._string);
        break;
    case JsonValueType::Object:
        m_value._object = new JsonObject(*other.m_value._object);
        break;
    case JsonValueType::Array:
        m_value._array = new JsonArray(*other.m_value._array);
        break;
    default:
        break;
    }
}

JsonValue::~JsonValue()
{
    switch (m_type)
    {
    case JsonValueType::String:
        delete m_value._string;
        break;
    case JsonValueType::Object:
        delete m_value._object;
        break;
    case JsonValueType::Array:
        delete m_value._array;
        break;
    default:
        break;
    }
}

string JsonValue::toString() const
{
    if(JsonValueType::String == m_type) {
        return std::string(m_value._string->begin(), m_value._string->end());
    }
    return string();
}

JsonValueType JsonValue::getJsonValueType() const
{
    return m_type;
}

void JsonValue::setJsonType(JsonValueType type)
{
    m_type = type;
}

/************ JsonObject **********************/

JsonObject::JsonObject()
{

}

bool JsonObject::contains(const string& key) const
{
    return m_JsonObject.find(key) != m_JsonObject.end();
}

void JsonObject::insert(const string& key, const JsonValue& value)
{
    m_JsonObject.emplace(key,value);
}

const JsonValue& JsonObject::operator[](const string& key) const
{
    auto it = m_JsonObject.find(key);
    return (*it).second;
}

map<string, JsonValue>::const_iterator JsonObject::begin() const
{
    return m_JsonObject.begin();

}
map<string, JsonValue>::const_iterator JsonObject::end() const
{
    return m_JsonObject.end();
}


/************ JsonArray **********************/

JsonArray::JsonArray()
{

}

void JsonArray::append(const JsonValue &value)
{
    m_JsonArray.push_back(value);
}

const JsonValue& JsonArray::operator[](size_t index) const
{
    return m_JsonArray[index];
}

vector<JsonValue>::const_iterator JsonArray::begin() const
{
    return m_JsonArray.begin();
}
vector<JsonValue>::const_iterator JsonArray::end() const
{
    return m_JsonArray.end();
}