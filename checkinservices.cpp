#include "checkinservices.h"


CheckinServices::CheckinServices()
{

}
bool CheckinServices:: isPostgresServiceRunning(const QString &serviceName) {
    QProcess process;
    QString command = "sc";
    QStringList arguments;
    arguments << "query" << serviceName;
    process.start(command, arguments);
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();


    if (!output.isEmpty()) {
        // qDebug() << "Output:\n" << output;
    }
    if (!errorOutput.isEmpty()) {
        qDebug() << "Error:\n" << errorOutput;
    }
    return output.contains("RUNNING");
}
