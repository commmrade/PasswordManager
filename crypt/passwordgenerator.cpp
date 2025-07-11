#include "passwordgenerator.h"
#include <random>
#include <QString>
#include <QDebug>

QString PasswordGenerator::generatePassword(unsigned int len)
{
    return generatePswd(len);
}

QString PasswordGenerator::generatePswd(unsigned int len)
{
    std::random_device rd;
    std::mt19937_64 engine{rd()};
    QString result;
    result.reserve(len);

    std::uniform_int_distribution<> action_dist{1, 100};
    std::uniform_int_distribution<> digits_dist{0, sizeof(digits) - 2};
    std::uniform_int_distribution<> sym_dist{0, sizeof(symbols) - 2};
    std::uniform_int_distribution<> let_dist{0, sizeof(letters) - 2};

    for (auto i = 0; i < len; ++i) {
        int action = action_dist(engine);
        if (action < 80) {
            int idx = let_dist(engine);
            int shouldBeCapital = let_dist(engine);
            if (shouldBeCapital > sizeof(letters) / 2) {
                result += QChar::fromLatin1(toupper(letters[idx]));
            } else {
                result += letters[idx];
            }
        } else if (action < 90) {
            int symbol_idx = sym_dist(engine);
            result += symbols[symbol_idx];
        } else {
            int digit_idx = digits_dist(engine);
            result += digits[digit_idx];
        }
    }
    return result;
}
