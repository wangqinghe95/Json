#include "JsonParser.hpp"
#include "Logger.hpp"


enum class ParserState
{
    BEGIN = 0,
    OBJECT_OR_ARRAY_BEGIN,  // '[' or '{'
    STRING_BEGIN,
    STRING,
    ERROR,
    END,

    ESPACE  // '\' such as '\r', '\n'
};

JsonValue JsonParser::toJsonValue(const string& strJson)
{
    auto p = strJson.begin();
    ParserState state = ParserState::BEGIN;

    list<JsonToken> tokens;

    while (state != ParserState::ERROR && strJson.end() != p)
    {
        char ch = *p;
        auto p_cur = p++;
        switch (state)
        {
        case ParserState::BEGIN:
        {
            if(isSpace(ch)) {
                // nothing to do
            }
            else if(isBeginOfValue(ch)) {
                pushBackTokens(tokens, ch, p_cur);
                state = ParserState::OBJECT_OR_ARRAY_BEGIN;
            }
            else if('"' == ch) {
                state = ParserState::STRING_BEGIN;
            }
            else {
                state = ParserState::ERROR;
            }
            break;
        }
        case ParserState::OBJECT_OR_ARRAY_BEGIN:
        {
            if(isSpace(ch)) {
                // nothing to do
            }
            else if(isEndOfValue(ch)) {
                pushBackTokens(tokens, ch, p_cur);
                state = ParserState::END;
            }
            else {
                p--;
                state = ParserState::BEGIN;
            }
            break;
        }
        case ParserState::STRING_BEGIN:
        {
            tokens.emplace_back(TokenType::String, p_cur);
            state = ParserState::STRING;
            break;
        }
        case ParserState::STRING:
        {
            if('"' == ch) {
                tokens.back().m_end = p_cur;
                state = ParserState::END;
            }
            else if('\\' == ch) {
                state = ParserState::ESPACE;
            }
            break;
        }
        case ParserState::ESPACE:
        {
            state = ParserState::STRING;
            break;
        }
        case ParserState::END:
        {
            bool isEnd = isSpace(ch) || isSepartor(ch) || isEndOfValue(ch);
            if(isEnd) {
                pushBackTokens(tokens, ch, p_cur);
            }
            else {
                p--;
                state = ParserState::BEGIN;
            }
            break;
        }
        default:
            break;
        }
    }

    if(ParserState::ERROR == state) {
        return JsonValue();
    }

    return generateJsonValueViaTokens(tokens);
    // JsonValue res = generateJsonValueViaTokens(tokens);
    // DEBUG("type ", static_cast<std::underlying_type<JsonValueType>::type>(res.getJsonValueType()));
    // return res;
}

string JsonParser::toJsonString(const JsonValue& jsonValue)
{
    string strJson;
    switch (jsonValue.getJsonValueType())
    {
    case JsonValueType::Null: {
        strJson.append("null");
        break;
    }
    case JsonValueType::Boolean: {
        if(jsonValue.toBoolean()) {
            strJson.append("true");
        }
        else {
            strJson.append("false");
        }
        break;
    }
    case JsonValueType::Integer: {
        strJson.append(std::to_string(jsonValue.toLongLong()));
        break;
    }
    case JsonValueType::Double: {
        strJson.append(std::to_string(jsonValue.toLongLong()));
        break;
    }
    case JsonValueType::String: {
        strJson.push_back('"');
        strJson.append(jsonValue.toString());
        strJson.push_back('"');
        break;
    }
    case JsonValueType::Object: {
        strJson.push_back('{');
        bool isFirst = true;
        JsonObject jsonObject = jsonValue.toObject();
        for(const auto &kv : jsonObject)
        {
            if(isFirst) {
                isFirst = false;
            }
            else {
                strJson.push_back(',');
            }

            strJson.append(toJsonString(kv.first));
            strJson.push_back(',');
            strJson.append(toJsonString(kv.second));
        }
        strJson.push_back('}');
        break;
    }
    case JsonValueType::Array:
    
    default:
        break;
    }
}

bool JsonParser::isSpace(char ch)
{
    return (' ' == ch || '\t' == ch || '\r' == ch || '\n' == ch);
}

bool JsonParser::isBeginOfValue(char ch)
{
    return ('[' == ch || '{' == ch);
}

bool JsonParser::isEndOfValue(char ch)
{
    return (']' == ch || '}' == ch);
}

bool JsonParser::isSepartor(char ch)
{
    return (':' == ch || ',' == ch );
}

void JsonParser::pushBackTokens(std::list<JsonToken> &tokens, char ch, const string::const_iterator &pos)
{
    switch (ch)
    {
    case '{':
        tokens.emplace_back(TokenType::ObjectBegin, pos);
        break;
    case '}':
        tokens.emplace_back(TokenType::ObjectEnd, pos);
        break;
    case '[':
        tokens.emplace_back(TokenType::ArrayBegin, pos);
        break;
    case ']':
        tokens.emplace_back(TokenType::ArrayEnd, pos);
        break;
    case ':':
        tokens.emplace_back(TokenType::ObjectKeyValueSeparator, pos);
        break;
    case ',':
        tokens.emplace_back(TokenType::MemberSeparaotr, pos);
        break;
    default:
        break;
    }
}

JsonValue JsonParser::generateJsonValueViaTokens(list<JsonToken>& tokens)
{
    JsonToken token = tokens.front();
    tokens.pop_front();
    
    if(TokenType::ObjectBegin == token.m_token){
        return generateJsonObjectViaTokens(tokens);
        // JsonValue res = generateJsonObjectViaTokens(tokens);
        
        // DEBUG("type ", static_cast<std::underlying_type<JsonValueType>::type>(res.getJsonValueType()));

        // return res;
    }

    if(TokenType::String == token.m_token){
        return JsonValue(token.m_start, token.m_end);
    }
    else {
        // nothing
    }

    // return JsonValue();
}

JsonValue JsonParser::generateJsonObjectViaTokens(list<JsonToken>& tokens)
{
    JsonObject jsonObject;
    while (TokenType::ObjectEnd != tokens.front().m_token)
    {
        JsonToken token = tokens.front();
        tokens.pop_front();

        if(tokens.front().m_token != TokenType::ObjectKeyValueSeparator) {
            return false;
        }

        if(tokens.empty()) {
            return JsonValue();
        }

        tokens.pop_front();

        string key = string(token.m_start, token.m_end);
        JsonValue value = generateJsonValueViaTokens(tokens);
        jsonObject.insert(key,value);
        DEBUG("string: ", value.toString());

        if(tokens.empty()) {
            return JsonValue();
        }

        if(TokenType::MemberSeparaotr == tokens.front().m_token) {
            tokens.pop_front();
        }
        else if(TokenType::ObjectEnd == tokens.front().m_token) {
            // do nothing
        }
        else {
            return JsonValue();
        }

        tokens.pop_front();
        return JsonValue(jsonObject);        
    }
    
}