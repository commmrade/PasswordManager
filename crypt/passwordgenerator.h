#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H
class QString;
class PasswordGenerator
{
    PasswordGenerator();
public:
    static QString generatePassword(unsigned int len = 16);


};

#endif // PASSWORDGENERATOR_H
