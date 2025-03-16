#include "dbfacade.h"

DBFacade::DBFacade() {

    m_db = QSqlDatabase::addDatabase("QPSQL");

    m_query = std::make_unique<QSqlQuery>();
}

DBFacade::~DBFacade() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DBFacade::openDatabaseQPSQL(const QString &host, const QString &dbName,
                                 const QString &user, const QString &password, int port) {
    m_db.setHostName(host);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);
    m_db.setPort(port);

    if (!m_db.open()) {
        qDebug() << "Ошибка при открытии базы данных:" << m_db.lastError().text();
        return false;
    }else {
        return true;
    }
}

QStringList DBFacade:: getRecordsDatabaseQPSQL(const QString &fieldName,
                                               const QString &tableName,
                                               const QString &condition){
    QStringList stringList;
    if (!m_db.isValid()) {
        QMessageBox::information(nullptr, "Состояние соединения",
                                 "Нет соединения с БД");
        stringList<<"Нет соединения с БД";
        return stringList;
    }

    QString query = QString("SELECT %1 FROM %2 %3 ORDER BY n ASC").
                    arg(fieldName,
                        tableName,
                        condition);
    if(!m_query->exec(query)){
        QString errorMessage = m_query->lastError().text();
        qDebug() << "Failed to execute query:"+errorMessage;
    }
    while (m_query->next()) {

        QString htmlValue = m_query->value(0).toString();

        htmlValue = htmlValue.simplified();

        QString q1 ="<!--<body background=";
        int index = htmlValue.indexOf(q1);

        if (index != -1) {
            QString ThemahtmlValue=htmlValue.left(index);
            stringList<<ThemahtmlValue;
        }
    }
    return stringList;
}


