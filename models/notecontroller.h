#ifndef NOTECONTROLLER_H
#define NOTECONTROLLER_H

#include <QObject>
#include "notemodel.h"
#include <QSqlError>

class NoteController : public QObject
{
    Q_OBJECT
private:
    SqlNoteModel* model{nullptr};

    NoteController();
public:
    NoteController(const NoteController&) = delete;
    NoteController& operator=(const NoteController&) = delete;

    static NoteController& instance() {
        static NoteController controller;
        return controller;
    }

    Q_INVOKABLE int createNote(const QString& title, const QString& url,
                   const QString& username,
                   const QString& email, const QString& password);
    Q_INVOKABLE void editNote(const int noteId, const QString& title, const QString& url,
                  const QString& username,
                  const QString& email, const QString& password);
    Q_INVOKABLE void deleteNote(const int noteId);
    Q_INVOKABLE void setTitle(const int noteId, const QString& title);
    Q_INVOKABLE QString getTitle(const int noteId) const;
    Q_INVOKABLE void setUrl(const int noteId, const QString& url);
    Q_INVOKABLE QString getUrl(const int noteId) const;
    Q_INVOKABLE void setUsername(const int noteId, const QString& username);
    Q_INVOKABLE QString getUsername(const int noteId) const;
    Q_INVOKABLE void setEmail(const int noteId, const QString& email);
    Q_INVOKABLE QString getEmail(const int noteId) const;
    Q_INVOKABLE void setPassword(const int noteId, const QString& password);
    Q_INVOKABLE QString getPassword(const int noteId) const;
    Q_INVOKABLE QDate getCreatedDatetime(const int noteId) const;

    Q_INVOKABLE int getLastInsertId() const {
        return model->getLastInsertId();
    }

    Q_INVOKABLE void resetStorage();
    Q_INVOKABLE void removeFromCache(int id);

    Q_INVOKABLE QAbstractItemModel* getModel() {
        model->select();
        qDebug() << model->database().databaseName();
        qDebug() << model->database().isOpen();
        qDebug() << model->rowCount();
        qDebug() << model->lastError().text();
        return model;
    }

signals:
};

#endif // NOTECONTROLLER_H
