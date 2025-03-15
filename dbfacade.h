#ifndef DBFACADE_H
#define DBFACADE_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QMessageBox>

class DBFacade : public QObject{
    Q_OBJECT
public:
    DBFacade();

    ~DBFacade();

    //bool openDatabaseQPSQL();
    bool openDatabaseQPSQL(const QString &host, const QString &dbName,
                           const QString &user, const QString &password,
                           int port);

    QString getValueDatabaseQPSQL(const QString &fieldName,
                                  const QString &tableName,
                                  const QString &condition);
    QStringList getRecordsDatabaseQPSQL(
                                const QString &fieldName,
                                const QString &tableName,
                                const QString &condition);
    QByteArray getGradeDatabaseQPSQL(const QString &fieldName,
                                     const QString &tableName,
                                     const QString &condition);
    QStringList getFileNameQPSQL(const QString &fieldName,
                                const QString &tableName,
                                const QString &condition);
    QList<QByteArray>  getFileImageDataQPSQL(const QString &fieldName,
                                      const QString &tableName,
                                      const QString &condition);

// protected:
//     QSqlDatabase m_db;
//     QSqlQuery *m_query;
private:
    QSqlDatabase m_db;
    std::unique_ptr<QSqlQuery> m_query; // Используем умный указатель
};
#endif // DBFACADE_H
