#include "JsonParser.hpp"



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
                state == ParserState::BEGIN;
            }
            break;
        }
        default:
            break;
        }
    }

    if(ParserState::ERROR == state) {
        
    }
    
}
string JsonParser::toJsonString(const JsonValue& jsonValue)
{
    
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