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

    bool openDatabaseQPSQL();

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

protected:
    QSqlDatabase m_db;
    QSqlQuery *m_query;

};
#endif // DBFACADE_H
