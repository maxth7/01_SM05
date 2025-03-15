#include "dbfacade.h"

// DBFacade::DBFacade() {
//     m_db =QSqlDatabase::addDatabase("QPSQL");
//     m_query = new QSqlQuery();
// }
// DBFacade::~DBFacade() {
//     if (m_db.isOpen()) {
//         m_db.close();
//     }
//     delete m_query;
// }
DBFacade::DBFacade() {

    m_db = QSqlDatabase::addDatabase("QPSQL");

    m_query = std::make_unique<QSqlQuery>();
}

DBFacade::~DBFacade() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}


// bool  DBFacade:: openDatabaseQPSQL(){

//     m_db.setHostName("localhost");
//     m_db.setDatabaseName("iSmile");

//     m_db.setUserName("postgres");
//     m_db.setPassword("800900");
//     m_db.setPort(5432);

//     if (!m_db.open()) {
//        qDebug() << "Ошибка при открытии базы данных:" << m_db.lastError().text();
//        return false;
//     }else {
//         return true;
//     }
// }

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


QString DBFacade:: getValueDatabaseQPSQL(const QString &fieldName,
                                          const QString &tableName,
                                          const QString &condition){
    if (!m_db.isValid()) {
        QMessageBox::information(nullptr, "Состояние соединения",
                                 "Нет соединения с БД");
        return "Нет соединения с БД";
    }

    QString currentTopic;

    QString query = QString("SELECT %1 FROM %2 %3 ORDER BY n ASC" ).arg(fieldName, tableName,condition);
    //qDebug()<<"getValueDatabaseQPSQL query="<<query;

    if(m_query->exec(query)) {//EXEC
        if (m_query->next()) { // NEXT
            currentTopic = m_query->value(0).toString();
        }
    } else {

        QString errorMessage = m_query->lastError().text();
        qDebug() << "Failed to execute query"+errorMessage;
    }
    m_query->finish();
    m_query->clear();
    return currentTopic;
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
QByteArray DBFacade:: getGradeDatabaseQPSQL(const QString &fieldName,
                                            const QString &tableName,
                                            const QString &condition){
   QByteArray imageData;
    if (!m_db.isValid()) {
        QMessageBox::information(nullptr, "Состояние соединения",
                                 "Нет соединения с БД");
        imageData=0;
        return imageData;
    }

    QString query = QString("SELECT %1 FROM %2 %3" ).arg(
                                            fieldName,
                                            tableName,
                                            condition);
    if (m_query->exec(query)){
            if (m_query->next()){
               imageData = m_query->value(2).toByteArray();
            }
    } else {
        QString errorMessage = m_query->lastError().text();
        qDebug() << "Failed to execute query:" << errorMessage;
    }
    m_query->finish();
    m_query->clear();

    return imageData;
}
QStringList DBFacade:: getFileNameQPSQL(const QString &fieldName,
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

    if(!m_query->exec(query)){ // EXEC
        QString errorMessage = m_query->lastError().text();
        qDebug() << "getFileNameQPSQL Failed to execute query:"
                +errorMessage;
    }
    QString htmlValue;

    while(m_query->next()) {

        htmlValue = m_query->value(0).toString();
        stringList<<htmlValue;

    }
    // qDebug() << "QStringList contents:";
    // std::for_each(stringList.begin(), stringList.end(), [](const QString &item) {
    //     qDebug() << item;
    // });

    return stringList;
}
QList<QByteArray> DBFacade:: getFileImageDataQPSQL(
                                        const QString &fieldName,
                                        const QString &tableName,
                                        const QString &condition){
     QList<QByteArray> stringList;
    if (!m_db.isValid()) {
        QMessageBox::information(nullptr, "Состояние соединения",
                                 "Нет соединения с БД");
         stringList.append(0);
        return stringList;
    }
    QByteArray imageData;

    QString query = QString("SELECT %1 FROM %2 %3 ORDER BY n ASC").
                    arg(fieldName,
                        tableName,
                        condition);
     if (!m_query->exec(query)){
        QString errorMessage = m_query->lastError().text();
        qDebug() << "Failed to execute query: " << errorMessage;
    }
     while (m_query->next()) {
        imageData = m_query->value(0).toByteArray();
        stringList.append(imageData);
    }

    m_query->finish();
    m_query->clear();
    // qDebug() << "QStringList contents:";
    // std::for_each(stringList.begin(), stringList.end(), [](const QString &item) {
    //     qDebug() << item;
    // });
    return stringList;
}
