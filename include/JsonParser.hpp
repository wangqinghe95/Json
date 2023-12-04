#ifndef JSON_PARSER_HPP__
#define JSON_PARSER_HPP__

#include "JsonValue.hpp"

#include <list>
using std::list;

enum class TokenType
{
    ObjectBegin = 0,
    ObjectEnd,
    ObjectKeyValueSeparator,    // ':'

    ArrayBegin,
    ArrayEnd,

    MemberSeparaotr,

    String,
    Double,
    LongLong,
    True,
    False,
};

class JsonToken
{
public:
    TokenType m_token;
    string::const_iterator m_start;
    string::const_iterator m_end;
public:

    JsonToken(TokenType type, const string::const_iterator &start) : m_token(type)
                                                                    , m_start(start)
                                                                    , m_end(start)
    {}
};

class JsonParser
{
public:
    static JsonValue toJsonValue(const string& strJson);
    static string toJsonString(const JsonValue& jsonValue);
private:
    static bool isSpace(char ch);
    static bool isBeginOfValue(char ch);
    static bool isEndOfValue(char ch);
    static bool isSepartor(char ch);
    static void pushBackTokens(list<JsonToken> &tokens, char ch, const string::const_iterator &pos);

    static JsonValue generateJsonValueViaTokens(list<JsonToken>& tokens);
    static JsonValue generateJsonObjectViaTokens(list<JsonToken>& tokens);
};

#endif