#include "mainwindowstate.h"

MainWindowState::MainWindowState(){
}

void MainWindowState::SaveState(const QPoint &pos, const QSize &size){

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("pos", pos);
    settings.setValue("size", size);
    settings.endGroup();

    settings.beginGroup("pathGraph");
    settings.setValue("path", pathGraph);
    settings.endGroup();

    settings.beginGroup("CurrentThema");
    settings.setValue("CurrentThema", CurrentThema);//
    settings.endGroup();

    if(testExecutionTime!=0){
        settings.beginGroup("testExecutionTime");// время теста
        settings.setValue("testExecutionTime", testExecutionTime);//
        settings.endGroup();
    }
}

void MainWindowState::RestoreState(QPoint &pos, QSize &size){

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    QPoint oldPoint = settings.value("pos", QPoint(0,0)).toPoint();
    if (oldPoint.x()!=0 && oldPoint.y()!=0){
        pos.setX(oldPoint.x());
        pos.setY(oldPoint.y());
    }
    QSize oldSize = settings.value("size", QSize(0, 0)).toSize();

    if (oldSize.width()!=0 && oldSize.height()!=0) {
        size.setWidth(oldSize.width());
        size.setHeight(oldSize.height());
    }
    settings.endGroup();

    settings.beginGroup("pathGraph");
    pathGraph=settings.value("path").toString();
    settings.endGroup();

    settings.beginGroup("CurrentThema");
    CurrentThema=settings.value("CurrentThema").toInt();
    settings.endGroup();

    settings.beginGroup("testExecutionTime");

    testExecutionTime=settings.value("testExecutionTime").toFloat();
    settings.endGroup();
}
