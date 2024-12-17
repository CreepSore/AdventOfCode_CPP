#pragma once
#include <map>
#include <string>
#include <vector>

struct MatcherResult
{
    int type;
    bool valid;
    size_t offset;
    size_t length;

    MatcherResult();
    MatcherResult(size_t offset, size_t length);
};

typedef MatcherResult(*Matcher)(const std::string& data, size_t offset);

class LexerLayer
{
private:
    std::map<int, Matcher> matchers;
   
public:
    LexerLayer addMatcher(int type, Matcher matcher);
    MatcherResult match(int expectedToken, const std::string& data, size_t offset);
};

class Lexer
{
    std::string* data;

public:
    int offset = 0;
    LexerLayer layer;

    explicit Lexer(std::string* data);

    Lexer reset();
    MatcherResult expect(const std::vector<int>& expectedTokens);
    MatcherResult expect(int expectedToken);
    bool hasData() const;
};
