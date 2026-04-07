#ifndef DBFACADE_H
#define DBFACADE_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QMessageBox>
#include <QByteArray>
#include <QString>

class DBFacade : public QObject{
    Q_OBJECT
public:
    DBFacade();

    ~DBFacade();

    bool openDatabaseQPSQL(const QString &host, const QString &dbName,
                           const QString &user, const QString &password,
                           int port);

    QStringList getRecordsDatabaseQPSQL(
                                const QString &fieldName,
                                const QString &tableName,
                                const QString &condition);
    template<typename T>
    T getValueFromDB(const QString &fieldName,
                     const QString &tableName,
                     const QString &condition);

    template<typename T>
    QList<T> getListFromDB(const QString &fieldName,
                           const QString &tableName,
                           const QString &condition);
private:
    QSqlDatabase m_db;
    std::unique_ptr<QSqlQuery> m_query;
};
template<typename T>
T DBFacade::getValueFromDB(const QString &fieldName, const QString &tableName, const QString &condition) {
    if (!m_db.isValid()) {
        QMessageBox::information(nullptr, "Состояние соединения", "Нет соединения с БД");
        return T(); // Возвращаем значение по умолчанию для типа T
    }

    QString query = QString("SELECT %1 FROM %2 %3 ORDER BY n ASC").arg(fieldName, tableName, condition);

    if (m_query->exec(query)) {
        if (m_query->next()) {
            if constexpr (std::is_same<T, QByteArray>::value) {
                return m_query->value(0).toByteArray(); // Возвращаем QByteArray
            } else if constexpr (std::is_same<T, QString>::value) {
                return m_query->value(0).toString(); // Возвращаем QString
            }
        }
    } else {
        QString errorMessage = m_query->lastError().text();
        qDebug() << "Failed to execute query:" << errorMessage;
    }

    m_query->finish();
    m_query->clear();
    return T(); // Возвращаем значение по умолчанию для типа T
}

template<typename T>
QList<T> DBFacade::getListFromDB(const QString &fieldName, const QString &tableName, const QString &condition) {
    QList<T> resultList;

    if (!m_db.isValid()) {
        QMessageBox::information(nullptr, "Состояние соединения", "Нет соединения с БД");
        if constexpr (std::is_same<T, QString>::value) {
            resultList.append("Нет соединения с БД");
        } else if constexpr (std::is_same<T, QByteArray>::value) {
            resultList.append(QByteArray()); // Добавляем пустой QByteArray
        }
        return resultList;
    }

    QString query = QString("SELECT %1 FROM %2 %3 ORDER BY n ASC").arg(fieldName, tableName, condition);

    if (!m_query->exec(query)) {
        QString errorMessage = m_query->lastError().text();
        qDebug() << "Failed to execute query:" << errorMessage;
        return resultList; // Возвращаем пустой список
    }

    while (m_query->next()) {
        if constexpr (std::is_same<T, QString>::value) {
            resultList.append(m_query->value(0).toString());
        } else if constexpr (std::is_same<T, QByteArray>::value) {
            resultList.append(m_query->value(0).toByteArray());
        }
    }

    m_query->finish();
    m_query->clear();
    return resultList;
}
#endif // DBFACADE_H
