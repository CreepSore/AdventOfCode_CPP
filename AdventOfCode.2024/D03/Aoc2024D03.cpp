#include "pch.h"
#include "Aoc2024D03.h"

#include "Lexer.h"

Aoc2024D03::Aoc2024D03()
{
    this->day = 3;
    this->year = 2024;
}

Aoc2024D03::~Aoc2024D03()
{
    delete this->data;
}

AocDayPartResult Aoc2024D03::runPart1Alt() const
{
    char expected = 'm';
    int num0 = -1;
    int result = 0;
    std::string buffer;


    for(int i = 0; i < this->data->length(); i++)
    {
        const char c = this->data->at(i);

        if(
            c != expected
            && !(expected == '0' && ((c >= '0' && c <= '9') || c == ','))
            && !(expected == '1' && ((c >= '0' && c <= '9') || c == ')'))
        )
        {
            buffer.clear();
            expected = 'm';
            continue;
        }

        if(expected == 'm')
        {
            expected = 'u';
        }
        else if(c == 'u')
        {
            expected = 'l';
        }
        else if(c == 'l')
        {
            expected = '(';
        }
        else if(c == '(')
        {
            expected = '0';
        }
        else if(expected == '0')
        {
            if (c == ',')
            {
                num0 = atoi(buffer.data());
                buffer.clear();
                expected = '1';
            }
            else
            {
                buffer.push_back(c);
            }
        }
        else if (expected == '1')
        {
            if (c == ')')
            {
                const int num1 = atoi(buffer.data());
                buffer.clear();

                result += num0 * num1;
                expected = 'm';
            }
            else
            {
                buffer.push_back(c);
            }
        }
    }

    return { result };
}

AocDayPartResult Aoc2024D03::runPart1() const
{
    enum class LexerType : uint8_t
    {
        NUM = 1,
        MUL = 10,
        DO = 11,
        DONT = 12,
        COMMA = 20,
        OPEN = 21,
        CLOSE = 22
    };

    enum class Step : uint8_t
    {
        NONE = 0,
        MUL = 1,
    };

    Lexer lexer = Lexer(this->data);
    lexer.layer = LexerLayer()
        .addMatcher(
            static_cast<int>(LexerType::MUL),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset + 3 > data.length())
                {
                    return {};
                }

                if (
                    data[offset] == 'm'
                    && data[offset + 1] == 'u'
                    && data[offset + 2] == 'l'
                    )
                {
                    return { offset, 3 };
                }

                return {};
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::NUM),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset >= data.length())
                {
                    return {};
                }

                size_t currentOffset = offset;

                while (currentOffset < data.length())
                {
                    if (data[currentOffset] < '0' || data[currentOffset] > '9')
                    {
                        break;
                    }

                    currentOffset += 1;
                }

                if (offset == currentOffset)
                {
                    return {};
                }

                return { offset, currentOffset - offset };
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::COMMA),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset >= data.length())
                {
                    return {};
                }

                if (data[offset] != ',')
                {
                    return {};
                }

                return { offset, 1 };
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::OPEN),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset >= data.length())
                {
                    return {};
                }

                if (data[offset] != '(')
                {
                    return {};
                }

                return { offset, 1 };
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::CLOSE),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset >= data.length())
                {
                    return {};
                }

                if (data[offset] != ')')
                {
                    return {};
                }

                return { offset, 1 };
            }
        );

    int result = 0;

    int num0 = -1;
    int num1 = -1;

    std::vector defaultExpect = { static_cast<int>(LexerType::MUL) };
    std::vector expect = defaultExpect;
    Step step = Step::NONE;

    while (lexer.hasData())
    {
        auto match = lexer.expect(expect);
        if (!match.valid)
        {
            step = Step::NONE;
            expect = defaultExpect;
            continue;
        }

        auto matchedType = static_cast<LexerType>(match.type);

        if (step == Step::NONE)
        {
            if (matchedType == LexerType::MUL)
            {
                num0 = -1;
                num1 = -1;
                expect = { static_cast<int>(LexerType::OPEN) };
                step = Step::MUL;
            }
        }
        else if (step == Step::MUL)
        {
            if (matchedType == LexerType::OPEN)
            {
                expect = { static_cast<int>(LexerType::NUM) };
            }
            else if (matchedType == LexerType::NUM)
            {
                if (num0 == -1)
                {
                    num0 = atoi(this->data->substr(match.offset, match.length).data());
                    expect = { static_cast<int>(LexerType::COMMA) };
                }
                else if (num1 == -1)
                {
                    num1 = atoi(this->data->substr(match.offset, match.length).data());
                    expect = { static_cast<int>(LexerType::CLOSE) };
                }
            }
            else if (matchedType == LexerType::COMMA)
            {
                expect = { static_cast<int>(LexerType::NUM) };
            }
            else if (matchedType == LexerType::CLOSE)
            {
                if (num0 != -1 && num1 != -1)
                {
                    result += num0 * num1;
                    num0 = -1;
                    num1 = -1;
                    step = Step::NONE;
                    expect = defaultExpect;
                }
            }
        }
    }

    return { result };
}

AocDayPartResult Aoc2024D03::runPart2() const
{
    enum class LexerType : uint8_t
    {
        NUM = 1,
        MUL = 10,
        DO = 11,
        DONT = 12,
        COMMA = 20,
        OPEN = 21,
        CLOSE = 22
    };

    enum class Step : uint8_t
    {
        NONE = 0,
        MUL = 1,
        DO = 2,
        DONT = 3
    };

    Lexer lexer = Lexer(this->data);
    lexer.layer = LexerLayer()
        .addMatcher(
            static_cast<int>(LexerType::MUL),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset + 3 > data.length())
                {
                    return {};
                }

                if (
                    data[offset] == 'm'
                    && data[offset + 1] == 'u'
                    && data[offset + 2] == 'l'
                    )
                {
                    return { offset, 3 };
                }

                return {};
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::NUM),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset >= data.length())
                {
                    return {};
                }

                size_t currentOffset = offset;

                while (currentOffset < data.length())
                {
                    if (data[currentOffset] < '0' || data[currentOffset] > '9')
                    {
                        break;
                    }

                    currentOffset += 1;
                }

                if (offset == currentOffset)
                {
                    return {};
                }

                return { offset, currentOffset - offset };
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::COMMA),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset >= data.length())
                {
                    return {};
                }

                if (data[offset] != ',')
                {
                    return {};
                }

                return { offset, 1 };
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::OPEN),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset >= data.length())
                {
                    return {};
                }

                if (data[offset] != '(')
                {
                    return {};
                }

                return { offset, 1 };
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::CLOSE),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset >= data.length())
                {
                    return {};
                }

                if (data[offset] != ')')
                {
                    return {};
                }

                return { offset, 1 };
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::DO),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset + 2 > data.length())
                {
                    return {};
                }

                if (
                    data[offset] == 'd'
                    && data[offset + 1] == 'o'
                )
                {
                    return { offset, 2 };
                }

                return {};
            }
        )
        .addMatcher(
            static_cast<int>(LexerType::DONT),
            [](const std::string& data, size_t offset)->MatcherResult
            {
                if (offset + 5 > data.length())
                {
                    return {};
                }

                if (
                    data[offset] == 'd'
                    && data[offset + 1] == 'o'
                    && data[offset + 2] == 'n'
                    && data[offset + 3] == '\''
                    && data[offset + 4] == 't'
                )
                {
                    return { offset, 5 };
                }

                return {};
            }
        );

    int result = 0;

    int num0 = -1;
    int num1 = -1;

    bool enabled = true;
    std::vector enabledExpect = { static_cast<int>(LexerType::MUL), static_cast<int>(LexerType::DONT) };
    std::vector disabledExpect = { static_cast<int>(LexerType::DO) };
    std::vector expect = enabledExpect;
    Step step = Step::NONE;

    while (lexer.hasData())
    {
        auto match = lexer.expect(expect);
        if (!match.valid)
        {
            expect = enabled ? enabledExpect : disabledExpect;

            step = Step::NONE;
            continue;
        }

        auto matchedType = static_cast<LexerType>(match.type);

        if(step == Step::NONE)
        {
            if (matchedType == LexerType::MUL)
            {
                num0 = -1;
                num1 = -1;
                expect = { static_cast<int>(LexerType::OPEN) };
                step = Step::MUL;
            }
            else if(matchedType == LexerType::DO)
            {
                expect = { static_cast<int>(LexerType::OPEN) };
                step = Step::DO;
            }
            else if (matchedType == LexerType::DONT)
            {
                expect = { static_cast<int>(LexerType::OPEN) };
                step = Step::DONT;
            }
        }
        else if(step == Step::MUL)
        {
            if (matchedType == LexerType::OPEN)
            {
                expect = { static_cast<int>(LexerType::NUM) };
            }
            else if (matchedType == LexerType::NUM)
            {
                if (num0 == -1)
                {
                    num0 = atoi(this->data->substr(match.offset, match.length).data());
                    expect = { static_cast<int>(LexerType::COMMA) };
                }
                else if (num1 == -1)
                {
                    num1 = atoi(this->data->substr(match.offset, match.length).data());
                    expect = { static_cast<int>(LexerType::CLOSE) };
                }
            }
            else if (matchedType == LexerType::COMMA)
            {
                expect = { static_cast<int>(LexerType::NUM) };
            }
            else if (matchedType == LexerType::CLOSE)
            {
                if (num0 != -1 && num1 != -1)
                {
                    result += num0 * num1;
                    num0 = -1;
                    num1 = -1;
                    step = Step::NONE;
                    expect = enabled ? enabledExpect : disabledExpect;
                }
            }
        }
        else if(step == Step::DO)
        {
            if(matchedType == LexerType::OPEN)
            {
                expect = { static_cast<int>(LexerType::CLOSE) };
            }
            else if(matchedType == LexerType::CLOSE)
            {
                enabled = true;
                expect = enabled ? enabledExpect : disabledExpect;
                step = Step::NONE;
            }
        }
        else if(step == Step::DONT)
        {
            if (matchedType == LexerType::OPEN)
            {
                expect = { static_cast<int>(LexerType::CLOSE) };
            }
            else if (matchedType == LexerType::CLOSE)
            {
                enabled = false;
                expect = enabled ? enabledExpect : disabledExpect;
                step = Step::NONE;
            }
        }
    }

    return { result };
}

void Aoc2024D03::run(std::vector<AocDayPartResult>& resultList, int part)
{
    switch (part)
    {
    case -1:
        resultList.push_back(runPart1());
        resultList.push_back(runPart2());
        break;

    case 1:
        resultList.push_back(runPart1());
        break;

    case 2:
        resultList.push_back(runPart2());
        break;

    default:
        break;
    }
}
