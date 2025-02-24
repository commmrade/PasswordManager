#ifndef NOTECONTROLLER_H
#define NOTECONTROLLER_H

#include <QObject>
#include "notemodel.h"

class NoteController
{
private:
    SqlNoteModel model;

    NoteController();
public:
    NoteController(const NoteController&) = delete;
    NoteController& operator=(const NoteController&) = delete;

    static NoteController& instance() {
        static NoteController controller;
        return controller;
    }

    int createNote(const QString& title, const QString& url,
                   const QString& username,
                   const QString& email, const QString& password);
    void editNote(const int noteId, const QString& title, const QString& url,
                  const QString& username,
                  const QString& email, const QString& password);
    void deleteNote(const int noteId);
    void setTitle(const int noteId, const QString& title);
    QString getTitle(const int noteId) const;
    void setUrl(const int noteId, const QString& url);
    QString getUrl(const int noteId) const;
    void setUsername(const int noteId, const QString& username);
    QString getUsername(const int noteId) const;
    void setEmail(const int noteId, const QString& email);
    QString getEmail(const int noteId) const;
    void setPassword(const int noteId, const QString& password);
    QString getPassword(const int noteId) const;
    QDate getCreatedDatetime(const int noteId) const;


    QAbstractItemModel* getModel() {
        return &model;
    }

signals:
};

#endif // NOTECONTROLLER_H
