#include "notecontroller.h"
#include <QDate>
NoteController::NoteController()
{

}

int NoteController::createNote(const QString &title, const QString &url, const QString &username, const QString &email, const QString &password)
{
    return model.createNote(title, url, username, email, password);
}

void NoteController::editNote(const int noteId, const QString &title, const QString &url, const QString &username, const QString &email, const QString &password)
{
    model.editNote(noteId, title, url, username, email, password);
}

void NoteController::deleteNote(const int noteId)
{
    model.deleteNote(noteId);
}

void NoteController::setTitle(const int noteId, const QString &title)
{
    model.setTitle(noteId, title);
}

QString NoteController::getTitle(const int noteId) const
{
    return model.getTitle(noteId);
}

void NoteController::setUrl(const int noteId, const QString &url)
{
    model.setUrl(noteId, url);
}

QString NoteController::getUrl(const int noteId) const
{
    return model.getUrl(noteId);
}

void NoteController::setUsername(const int noteId, const QString &username)
{
    model.setUsername(noteId, username);
}

QString NoteController::getUsername(const int noteId) const
{
    return model.getUsername(noteId);
}

void NoteController::setEmail(const int noteId, const QString &email)
{
    model.setEmail(noteId, email);
}

QString NoteController::getEmail(const int noteId) const
{
    return model.getEmail(noteId);
}

void NoteController::setPassword(const int noteId, const QString &password)
{
    model.setPassword(noteId, password);
}

QString NoteController::getPassword(const int noteId) const
{
    return model.getPassword(noteId);
}

QDate NoteController::getCreatedDatetime(const int noteId) const
{
    return model.getCreatedDatetime(noteId);
}
