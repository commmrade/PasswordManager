#include "notecontroller.h"
#include <QDate>
#include "cipher.h"

NoteController::NoteController() : QObject(nullptr)
{

}

int NoteController::createNote(const QString &title, const QString &url, const QString &username, const QString &email, const QString &password)
{
    auto passwordSalt = Cipher::instance().aesEncrypt(password);
    return model.createNote(title, url, username, email, passwordSalt.first, passwordSalt.second);
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
    auto passwordSalt = Cipher::instance().aesEncrypt(password);
    model.setPassword(noteId, passwordSalt.first, passwordSalt.second);
}

QString NoteController::getPassword(const int noteId) const
{
    auto passwordSalt = model.getPassword(noteId);
    return Cipher::instance().aesDecrypt(passwordSalt.first, passwordSalt.second);
}

QDate NoteController::getCreatedDatetime(const int noteId) const
{
    return model.getCreatedDatetime(noteId);
}

void NoteController::resetStorage()
{
    model.resetStorage();
}

void NoteController::removeFromCache(int id)
{
    model.removeFromCache(id);
}
