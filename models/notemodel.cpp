#include "notemodel.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>

SqlNoteModel::SqlNoteModel(QObject *parent)
    : QSqlTableModel{parent, SqlNoteModel::makeDatabase()}
{
    setEditStrategy(QSqlTableModel::OnFieldChange);
    setTable("notes");
    generateRoles();
    if (!select()) {
        throw std::runtime_error("Data could not be set in the model");
    }

}

QVariant SqlNoteModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const
{
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        return QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    }

}

bool SqlNoteModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
    if (role == Qt::EditRole) {
        return QSqlTableModel::setData(index, value, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        return QSqlTableModel::setData(modelIndex, value, Qt::EditRole);
    }
}

int SqlNoteModel::createNote(const QString& title, const QString& url,
                             const QString& username, const QString& email, const QString& password)
{
    if (titleExists(title)) {
        throw std::runtime_error("A note with such title already exists");
        return -1;
    }

    auto record = this->record();
    record.remove(0);
    record.setValue("title", title);
    record.setValue("url", url);
    record.setValue("username", username);
    record.setValue("email", email);
    record.setValue("password", password);

    if (!insertRecord(-1, record)) {
        throw std::runtime_error("Could not create a new note");
    }
    return this->query().lastInsertId().toInt();
}

void SqlNoteModel::editNote(const int noteId, const QString &title, const QString &url,
                            const QString &username, const QString &email,
                            const QString &password)
{
    // TODO: add check if note exists
    auto startIndex = this->index(0, 0);
    auto modelList = this->match(startIndex, Qt::EditRole, noteId);
    if (modelList.size() <= 0) {
        throw std::runtime_error("Note doesnt exist");
    }
    int row = modelList.first().row();
    auto record = this->record(row);
    record.setValue("title", title);
    record.setValue("url", url);
    record.setValue("username", username);
    record.setValue("email", email);
    record.setValue("password", password);
    if (!setRecord(row, record)) {
        throw std::runtime_error("could not set record in edit note");
    }
}

void SqlNoteModel::deleteNote(const int noteId)
{
    auto startModel = index(0, fieldIndex("id"));
    auto row = this->match(startModel, Qt::EditRole, noteId)[0].row();
    if (!removeRow(row)) {
        throw std::runtime_error("Could not delete a note");
    }
}

void SqlNoteModel::setTitle(const int noteId, const QString& title)
{
    if (titleExists(title)) {
        throw std::runtime_error("Cant set title: Such title exists");
    }
    setFieldValue(noteId, "title", QVariant{title});
}

QString SqlNoteModel::getTitle(const int noteId) const
{
    auto value = getFieldValue(noteId, "title");
    assert(!value.isNull() && "Could note get title");
    return value.toString();
}

void SqlNoteModel::setUrl(const int noteId, const QString& url)
{
    setFieldValue(noteId, "url", url);
}

QString SqlNoteModel::getUrl(const int noteId) const
{
    auto value = getFieldValue(noteId, "url");
    assert(!value.isNull() && "Could note get url");
    return value.toString();
}

void SqlNoteModel::setUsername(const int noteId, const QString &username)
{
    setFieldValue(noteId, "username", username);
}

QString SqlNoteModel::getUsername(const int noteId) const
{
    auto value = getFieldValue(noteId, "username");
    assert(!value.isNull() && "Could note get username");
    return value.toString();
}

void SqlNoteModel::setEmail(const int noteId, const QString &email)
{
    setFieldValue(noteId, "email", email);
}

QString SqlNoteModel::getEmail(const int noteId) const
{
    auto value = getFieldValue(noteId, "email");
    assert(!value.isNull() && "Could note get email");
    return value.toString();
}

void SqlNoteModel::setPassword(const int noteId, const QString &password)
{
    setFieldValue(noteId, "password", password);
}

QString SqlNoteModel::getPassword(const int noteId) const
{
    auto value = getFieldValue(noteId, "password");
    assert(!value.isNull() && "Could not get password");
    return value.toString();
}

QDate SqlNoteModel::getCreatedDatetime(const int noteId) const
{
    auto value = getFieldValue(noteId, "created_at");
    assert(!value.isNull() && "Could not get created_at");
    QDate date = QDate::fromString(value.toString(), Qt::DateFormat::ISODate);
    return date;
}


QSqlDatabase SqlNoteModel::makeDatabase()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setHostName("klewy");
    database.setDatabaseName("pmanager.sqlite");
    database.setUserName("root");
    database.setPassword("root"); // TODO: may be set password

    if (!database.open()) {
        throw std::runtime_error("Could not setup a connection with the database");
    }

    QString creationQuery =
        "CREATE TABLE IF NOT EXISTS notes ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    title TEXT NOT NULL,"
        "    url TEXT,"
        "    username TEXT NOT NULL,"
        "    email TEXT,"
        "    password TEXT NOT NULL,"
        "    created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    QSqlQuery query(creationQuery);
    if (!query.exec()) {
        throw std::runtime_error("Could not create a table damn fuck cock");
    }

    return database;
}

void SqlNoteModel::setFieldValue(const int noteId, QAnyStringView fieldName, const QVariant &fieldValue)
{
    auto startIndex = index(0, 0);
    if (auto modelList = this->match(startIndex, Qt::EditRole, noteId); modelList.size() > 0) {
        int row = modelList.first().row();
        auto record = this->record(row);
        record.setValue(fieldName, fieldValue);
        if (!setRecord(row, record)) {
            QString errorStr = QString{"Could not set %1 with value %2"}.arg(fieldName.toString()).arg(fieldValue.toString());
            throw std::runtime_error(errorStr.toStdString());
        }
    }
    qDebug() << "field was not found";
}

QVariant SqlNoteModel::getFieldValue(const int noteId, QAnyStringView fieldName) const
{
    auto startIndex = index(0, 0);
    if (auto modelList = this->match(startIndex, Qt::EditRole, noteId); modelList.size() != 0) {
        int row = modelList[0].row();
        auto record = this->record(row);
        QVariant value = record.value(fieldName);
        return value;
    }
    return QVariant{};
}

void SqlNoteModel::generateRoles()
{
    roles.clear();
    int nCols = this->columnCount();
    for (auto i = 0; i < nCols; ++i) {
        roles[Qt::UserRole + i + 1] = QVariant(headerData(i, Qt::Horizontal).toString()).toByteArray();
    }
}

bool SqlNoteModel::titleExists(QStringView title) const
{
    auto start = index(0, fieldIndex("title"));
    return this->match(start, Qt::EditRole, title.toString()).size() > 0;
}
