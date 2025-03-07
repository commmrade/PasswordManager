#ifndef NOTEMODEL_H
#define NOTEMODEL_H

#include <QSqlTableModel>
#include <QHash>


class SqlNoteModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit SqlNoteModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override { return roles; }

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

    int getLastInsertId() const;

    void resetStorage();

    static QSqlDatabase makeDatabase();

private:
    void setFieldValue(const int noteId, QAnyStringView fieldName, const QVariant& fieldValue);
    QVariant getFieldValue(const int noteId, QAnyStringView fieldName) const;

    void generateRoles();
    QHash<int, QByteArray> roles;


signals:
};

#endif // NOTEMODEL_H
