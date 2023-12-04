#include "JsonParser.hpp"
#include "Logger.hpp"


enum class ParserState
{
    BEGIN = 0,
    OBJECT_OR_ARRAY_BEGIN,  // '[' or '{'
    STRING_BEGIN,
    STRING,

    // number, integer or double
    NUMBER_BEGIN,
    NUMBER_POINT,
    NUMBER_BEFORE_POINT,
    NUMBER_AFTER_POINT,

    // True
    T_IN_TRUE,
    TR_IN_TRUE,
    TRU_IN_TRUE,

    // False
    F_IN_FALSE,
    FA_IN_FALSE,
    FAL_IN_FALSE,
    FALS_IN_FALSE,

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
            else if(ch >= '0' && ch <= '9') {
                tokens.emplace_back(TokenType::Double, p_cur);
                p--;
                state = ParserState::NUMBER_BEGIN;
            }
            else if(ch == '-') {
                tokens.emplace_back(TokenType::Double, p_cur);
                state = ParserState::NUMBER_BEGIN;
            }
            else if(ch == 't') {
                tokens.emplace_back(TokenType::True, p_cur);
                state = ParserState::T_IN_TRUE;
            }
            else if (ch == 'f') {
                tokens.emplace_back(TokenType::False, p_cur);
                state = ParserState::F_IN_FALSE;
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
        case ParserState::NUMBER_BEGIN:
        {
            if(ch == '0') {
                state = ParserState::NUMBER_POINT;
            }
            else if(ch >= '1' && ch <= '9') {
                state = ParserState::NUMBER_BEFORE_POINT;
            }
            else {
                state = ParserState::ERROR;
            }
            break;
        }
        case ParserState::NUMBER_POINT:
        {
            if (ch == '.') {
                state = ParserState::NUMBER_AFTER_POINT;
            }
            else if (ch == ',' || isSpace(ch) || isEndOfValue(ch)) {
                tokens.back().m_token = TokenType::LongLong;
                tokens.back().m_end = p_cur;
                p--;
                state = ParserState::END;
            }
            else {
                state = ParserState::ERROR;
            }
            break;
        }
        case ParserState::NUMBER_BEFORE_POINT:
        {
            if(ch >= '0' && ch <= '9') {
                // do nothing
            }
            else {
                p--;
                state = ParserState::NUMBER_POINT;
            }
            break;
        }
        case ParserState::NUMBER_AFTER_POINT:
        {
            if(ch >= '0' && ch <= '9') {
                // nothing to do
            }
            else if (ch == ',' || isSpace(ch) || isEndOfValue(ch)) {
                tokens.back().m_end = p_cur;
                p--;
                state = ParserState::END;
            }
            else {
                state = ParserState::ERROR;
            }
            break;
        }
        case ParserState::T_IN_TRUE:
        {
            state = (ch == 'r' ? ParserState::TR_IN_TRUE : ParserState::ERROR);
            break;
        }
        case ParserState::TR_IN_TRUE:
        {
            state = (ch == 'u' ? ParserState::TRU_IN_TRUE : ParserState::ERROR);
            break;
        }
        case ParserState::TRU_IN_TRUE:
        {
            if(ch == 'e') {
                tokens.back().m_end = p_cur;
                state = ParserState::END;
            }
            else {
                state = ParserState::ERROR;
            }
            break;
        }
        case ParserState::F_IN_FALSE:
        {
            state = (ch == 'a' ? ParserState::FA_IN_FALSE : ParserState::ERROR);
            break;
        }
        case ParserState::FA_IN_FALSE:
        {
            state = (ch == 'l' ? ParserState::FAL_IN_FALSE : ParserState::ERROR);
            break;
        }
        case ParserState::FAL_IN_FALSE:
        {
            state = (ch == 's' ? ParserState::FALS_IN_FALSE : ParserState::ERROR);
            break;
        }
        case ParserState::FALS_IN_FALSE:
        {
            if(ch == 'e') {
                tokens.back().m_end = p_cur;
                state = ParserState::END;
            }
            else {
                state = ParserState::ERROR;
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
}

string JsonParser::toJsonString(const JsonValue& jsonValue)
{
    // DEBUG("toJsonString");
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
        strJson.append(std::to_string(jsonValue.toDouble()));
        break;
    }
    case JsonValueType::String: {
        strJson.push_back('"');
        for(char ch : jsonValue.toString()) {
            switch (ch)
            {
            case '\"':
                strJson.append({'\\', '\"'});
                break;
            case '\\':
                strJson.append({'\\', '\\'});
                break;
            case '\b':
                strJson.append({'\\', 'b'});
                break;
            case '\f':
                strJson.append({'\\', 'f'});
                break;
            case '\n':
                strJson.append({'\\', '\n'});
                break;
            case '\r':
                strJson.append({'\\', 'r'});
                break;
            case '\t':
                strJson.append({'\\', 't'});
                break;
            default:
                strJson.push_back(ch);
                break;
            }
        }
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
            string key = toJsonString(kv.first);
            strJson.append(key);
            strJson.push_back(':');
            strJson.append(toJsonString(kv.second));
        }
        strJson.push_back('}');
        break;
    }
    case JsonValueType::Array:
    {
        break;
    }
    
    default:
        break;
    }
    return strJson;
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
    }

    if(TokenType::String == token.m_token){
        string s = string(token.m_start, token.m_end);
        return JsonValue(s);
    }
    else if (TokenType::True == token.m_token) {
        return JsonValue(true);
    }
    else if (TokenType::False == token.m_token) {
        return JsonValue(false);
    }
    else if(TokenType::Double == token.m_token) {
        double x = atof(string(token.m_start, token.m_end).c_str());
        return JsonValue(x);
        // return JsonValue(atof(string(token.m_start, token.m_end).data()));
    }
    else if (TokenType::LongLong == token.m_token) {
        long long l = atoll(string(token.m_start, token.m_end).c_str());
        return JsonValue(l);
        // return JsonValue(atoll(string(token.m_start, token.m_end).data()));
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

            
    }
    tokens.pop_front();
    return JsonValue(jsonObject);
}