#ifndef NOTE_H
#define NOTE_H
#include <QString>
struct Note {
    int id{-1}; // -1 is used to mark closed Note
    QString title;
    QString url;
    QString username;
    QString email;
    QString password;
};


#endif // NOTE_H
