#include "Lexer.h"

MatcherResult::MatcherResult()
{
    this->valid = false;
    this->offset = 0;
    this->length = 0;
}

MatcherResult::MatcherResult(const size_t offset, const size_t length)
{
    this->valid = true;
    this->offset = offset;
    this->length = length;
}

LexerLayer LexerLayer::addMatcher(const int type, Matcher matcher)
{
    this->matchers.insert_or_assign(type, matcher);
    return *this;
}

MatcherResult LexerLayer::match(const int expectedToken, const std::string& data, size_t offset)
{
    const auto matcher = this->matchers[expectedToken];
    const auto matched = matcher(data, offset);

    return matched;
}

Lexer::Lexer(std::string* data)
{
    this->data = data;
}

Lexer Lexer::reset()
{
    this->offset = 0;
    return *this;
}

MatcherResult Lexer::expect(const std::vector<int>& expectedTokens)
{
    for (int expectedToken : expectedTokens)
    {
        auto match = this->layer.match(expectedToken, *this->data, this->offset);

        if (match.valid)
        {
            this->offset = match.offset + match.length;
            match.type = expectedToken;
            return match;
        }
    }

    this->offset += 1;

    return {};
}

MatcherResult Lexer::expect(const int expectedToken)
{
    auto match = this->layer.match(expectedToken, *this->data, this->offset);

    if(match.valid)
    {
        this->offset = match.offset + match.length;
    }
    else
    {
        this->offset += 1;
    }

    match.type = expectedToken;

    return match;
}

bool Lexer::hasData() const
{
    return this->offset <= this->data->length();
}


