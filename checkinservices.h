#ifndef CHECKINSERVICES_H
#define CHECKINSERVICES_H

#include <QObject>
#include <QWidget>
#include <QProcess>

class CheckinServices : public QObject{

    Q_OBJECT
public:
    CheckinServices();
    bool isPostgresServiceRunning(const QString &serviceName);

};

#endif // CHECKINSERVICES_H
