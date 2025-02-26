#ifndef MAINWINDOWSTATE_H
#define MAINWINDOWSTATE_H
#include <QPoint>
#include <QSize>
#include <QSettings>

class MainWindowState : public QObject
{
    Q_OBJECT
public:
    QString pathGraph;
    int CurrentThema=1;
    float testExecutionTime=1;
    explicit MainWindowState();
    void SaveState(const QPoint &pos, const QSize &size);
    void RestoreState(QPoint &pos, QSize &size);
};

#endif // MAINWINDOWSTATE_H
