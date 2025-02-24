#ifndef NOTE_H
#define NOTE_H
#include <QString>
struct Note {
    int id{-1};
    QString title;
    QString url;
    QString username;
    QString email;
    QString password;
};


#endif // NOTE_H
