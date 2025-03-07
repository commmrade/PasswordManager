#include "notemodel.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include <QSqlError>
#include "consts.h"
#include <QStandardPaths>
#include <QDir>
#include <QPixmap>
#include <QIcon>
#include <QtLogging>

SqlNoteModel::SqlNoteModel(QObject *parent)
    : QSqlTableModel{parent, SqlNoteModel::makeDatabase()}
{
    QSqlTableModel::setEditStrategy(QSqlTableModel::OnFieldChange);
    QSqlTableModel::setTable("notes");
    generateRoles();
    if (!QSqlTableModel::select()) {
        throw std::runtime_error("Data could not be set in the model");
    }
}

QVariant SqlNoteModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const
{
    if (role == Qt::DecorationRole) {
        const QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        const auto record = this->record(index.row());

        QPixmap pixmap;
        const QDir appDataDir(appDataLoc);
        const QString iconPath = appDataDir.filePath("images/" + record.value("id").toString() + ".ico");
        if (!pixmap.load(iconPath)) {
            return QVariant{};
        }
        return QIcon{pixmap};
    } else if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    } else {
        const int columnIdx = role - Qt::UserRole - 1;
        const QModelIndex modelIndex = this->index(index.row(), columnIdx);
        return QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    }
}

bool SqlNoteModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
    if (role == Qt::EditRole) {
        return QSqlTableModel::setData(index, value, role);
    } else {
        const int columnIdx = role - Qt::UserRole - 1;
        const QModelIndex modelIndex = this->index(index.row(), columnIdx);
        return QSqlTableModel::setData(modelIndex, value, Qt::EditRole);
    }
}

int SqlNoteModel::createNote(const QString& title, const QString& url,
                             const QString& username, const QString& email, const QString& password)
{
    auto record = this->record();
    record.remove(0);
    record.setValue("title", title);
    record.setValue("url", url);
    record.setValue("username", username);
    record.setValue("email", email);
    record.setValue("password", password);

    if (!insertRecord(-1, record)) {
        qWarning("Warning: Could not create a note");
        return -1;
    }
    select();
    return this->query().lastInsertId().toInt();
}

void SqlNoteModel::editNote(const int noteId, const QString &title, const QString &url,
                            const QString &username, const QString &email,
                            const QString &password)
{
    // TODO: add check if note exists
    const auto startIndex = this->index(0, 0);
    auto modelList = this->match(startIndex, Qt::EditRole, noteId);
    if (modelList.size() <= 0) {
        qWarning("Warning: Note doesn't exist, can't edit");
        return;
    }
    const int row = modelList.first().row();
    auto record = this->record(row);
    record.setValue("title", title);
    record.setValue("url", url);
    record.setValue("username", username);
    record.setValue("email", email);
    record.setValue("password", password);
    if (!setRecord(row, record)) {
        qWarning("Warning: Could not edit the note");
    }
}

void SqlNoteModel::deleteNote(const int noteId)
{
    const auto startModel = index(0, fieldIndex("id"));
    if (const auto row = this->match(startModel, Qt::EditRole, noteId)[0].row(); !removeRow(row)) {
        qWarning("Warning: Could not delete the note");
    }
    select();
}

void SqlNoteModel::setTitle(const int noteId, const QString& title)
{
    setFieldValue(noteId, "title", QVariant{title});
}

QString SqlNoteModel::getTitle(const int noteId) const
{
    const auto value = getFieldValue(noteId, "title");
    assert(!value.isNull() && "Could note get title");
    return value.toString();
}

void SqlNoteModel::setUrl(const int noteId, const QString& url)
{
    setFieldValue(noteId, "url", url);
}

QString SqlNoteModel::getUrl(const int noteId) const
{
    const auto value = getFieldValue(noteId, "url");
    assert(!value.isNull() && "Could note get url");
    return value.toString();
}

void SqlNoteModel::setUsername(const int noteId, const QString &username)
{
    setFieldValue(noteId, "username", username);
}

QString SqlNoteModel::getUsername(const int noteId) const
{
    const auto value = getFieldValue(noteId, "username");
    assert(!value.isNull() && "Could note get username");
    return value.toString();
}

void SqlNoteModel::setEmail(const int noteId, const QString &email)
{
    setFieldValue(noteId, "email", email);
}

QString SqlNoteModel::getEmail(const int noteId) const
{
    const auto value = getFieldValue(noteId, "email");
    assert(!value.isNull() && "Could note get email");
    return value.toString();
}

void SqlNoteModel::setPassword(const int noteId, const QString &password)
{
    // Encrypt TODO
    setFieldValue(noteId, "password", password);
}

QString SqlNoteModel::getPassword(const int noteId) const
{
    const auto value = getFieldValue(noteId, "password");
    assert(!value.isNull() && "Could not get password");
    // Decrypt TODO
    return value.toString();
}

QDate SqlNoteModel::getCreatedDatetime(const int noteId) const
{
    const auto value = getFieldValue(noteId, "created_at");
    assert(!value.isNull() && "Could not get created_at");
    QDate date = QDate::fromString(value.toString(), Qt::DateFormat::ISODate);
    return date;
}

int SqlNoteModel::getLastInsertId() const
{
    return query().lastInsertId().toInt();
}

void SqlNoteModel::resetStorage()
{
    beginResetModel();
    select();
    endResetModel();
}


QSqlDatabase SqlNoteModel::makeDatabase()
{
    const QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setHostName("klewy");
    database.setDatabaseName(appDataLoc + PasswordManager::PM_FILENAME);
    database.setUserName("root");
    database.setPassword("root"); // TODO: may be set password

    if (!database.open()) {
        throw std::runtime_error("Could not setup a connection with the database");
    }

    const QString creationQuery =
        "CREATE TABLE IF NOT EXISTS notes ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    title TEXT NOT NULL,"
        "    url TEXT,"
        "    username TEXT NOT NULL,"
        "    email TEXT,"
        "    password TEXT NOT NULL,"
        "    created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    if (QSqlQuery query(creationQuery); !query.exec()) {
        qDebug() << query.lastError();
        throw std::runtime_error("Could not create a table");
    }

    return database;
}

void SqlNoteModel::setFieldValue(const int noteId, QAnyStringView fieldName, const QVariant &fieldValue)
{
    const auto startIndex = index(0, 0);
    if (auto modelList = this->match(startIndex, Qt::EditRole, noteId); modelList.size() > 0) {
        const int row = modelList.first().row();
        auto record = this->record(row);
        record.setValue(fieldName, fieldValue);
        if (!setRecord(row, record)) {
            qDebug() << lastError() << "could set";
            const QString errorStr = QString{"Could not set %1 with value %2"}.arg(fieldName.toString()).arg(fieldValue.toString());
            qWarning() << "Warning: " + errorStr;
        }
        select();
        return;
    }
    qDebug() << "field was not found" << fieldName;
}

QVariant SqlNoteModel::getFieldValue(const int noteId, QAnyStringView fieldName) const
{
    const auto startIndex = index(0, 0);
    if (auto modelList = this->match(startIndex, Qt::EditRole, noteId); modelList.size() != 0) {
        const int row = modelList[0].row();
        const auto record = this->record(row);
        QVariant value = record.value(fieldName);
        return value;
    }
    return QVariant{};
}

void SqlNoteModel::generateRoles()
{
    roles.clear();
    int const nCols = this->columnCount();
    for (auto i = 0; i < nCols; ++i) {
        roles[Qt::UserRole + i + 1] = QVariant(headerData(i, Qt::Horizontal).toString()).toByteArray();
    }
}

