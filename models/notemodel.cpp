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
#include <QBuffer>
SqlNoteModel::SqlNoteModel(QObject *parent)
    : QSqlTableModel{parent, SqlNoteModel::makeDatabase()}
{
    QSqlTableModel::setEditStrategy(QSqlTableModel::OnFieldChange);
    QSqlTableModel::setTable("notes");
    generateRoles();
    if (!QSqlTableModel::select()) {
        throw std::runtime_error("Data could not be set in the model");
    }
    select();
}

QVariant SqlNoteModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const
{
    if (role == Qt::DecorationRole) {
        const auto record = this->record(index.row());
        {
            QByteArray* pixmapBytes = icons.object(record.value("id").toInt());
            if (pixmapBytes) {
                QPixmap pixmap;
                if (!pixmap.loadFromData(*pixmapBytes, "png")) {

                    return {};
                }
                return QIcon{pixmap};
            }
        }

        const QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QPixmap pixmap;
        const QDir appDataDir(appDataLoc);
        const QString iconPath = appDataDir.filePath("images/" + record.value("id").toString() + ".ico");
        if (!pixmap.load(iconPath)) {
            return QVariant{};
        }

        auto* byteArray = new QByteArray;
        QBuffer buffer(byteArray);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "png");
        buffer.close();

        icons.insert(record.value("id").toInt(), byteArray);
        return QVariant{QIcon{pixmap}};
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
                             const QString& username, const QString& email, const QString& password, const QString& salt)
{
    auto record = this->record();
    record.remove(0);
    record.setValue("title", title);
    record.setValue("url", url);
    record.setValue("username", username);
    record.setValue("email", email);
    record.setValue("password", password);
    record.setValue("salt", salt);

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

void SqlNoteModel::setPassword(const int noteId, const QString &password, const QString& salt)
{
    setFieldValue(noteId, "salt", salt);
    setFieldValue(noteId, "password", password);
}

std::pair<QString, QString> SqlNoteModel::getPassword(const int noteId) const
{
    const auto value = getFieldValue(noteId, "password");
    const auto saltValue = getFieldValue(noteId, "salt");
    assert((!value.isNull() || !saltValue.isNull()) && "Could not get password");
    return {value.toString(), saltValue.toString()};
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

void SqlNoteModel::removeFromCache(int id)
{
    icons.remove(id);
}


QSqlDatabase SqlNoteModel::makeDatabase()
{
    qDebug() << "make db";
    const QString appDataLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir(appDataLoc);
    if (!dir.exists()) {
        if (!dir.mkpath(appDataLoc)) {
            qDebug() << "Failed to create directory:" << appDataLoc;
            throw std::runtime_error("Could not create database directory");
        }
    }

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = appDataLoc + PasswordManager::PM_FILENAME;
    qDebug() << dbPath;
    database.setDatabaseName(dbPath);

    if (!database.open()) {
        qDebug() << "Database error:" << database.lastError();
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
        "    salt TEXT NOT NULL,"
        "    created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    QSqlQuery query(database); // Explicitly pass the database to the query
    if (!query.exec(creationQuery)) {
        qDebug() << "Query error:" << query.lastError();
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

