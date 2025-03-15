#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QApplication>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QVector>
#include <QTabWidget>
#include <QScrollArea>
#include <QStatusBar>
#include <QLabel>
#include <QGridLayout>
#include <QFileDialog>
#include <QDebug>
#include <QChar>
#include <QMenu>
#include <QAction>
#include <QListView>
#include <QStringListModel>
#include <QStyle>
#include <QListWidgetItem>
#include <QModelIndex>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTextBrowser>
#include <QFile>
#include <QProcess>
#include <QMoveEvent>
#include <QProgressBar>
#include <QTimer>
#include <QTime>
#include <QIcon>
#include <QLayoutItem>
#include <QSharedMemory>
#include <QRandomGenerator>
#include "dbfacade.h"
#include "createtasks.h"
#include "custtb.h"
#include "mainwindowstate.h"
#include "stretchlabel.h"

#include "calculategrade.h"
#include "checkinservices.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DBFacade ;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    DBFacade *dbFacade;
    CheckinServices ChServices;
    CreateTasks num_cur_task;
    QWidget* previousCentralWidget=new QWidget;
    QPushButton *pushButton[MAX_TASK];
    QRadioButton *radioButtons[MAX_ANSWER];
    int* shufflingQuest=new  int[MAX_TASK]{0};
    int* shufflingAnswer=new int[MAX_ANSWER]{0};
    int tGrade{0};
    int ready_madeTasks{0};
    int selectedIndex ;
    MainWindowState state;
    QPoint pos;
    QSize size;
    int currentIndexThema=1;
    QList<QWidget*> savedWidgets;
    QPixmap background;
    QTimer *timer;
    int totalTime;
    int currentTime = 0;
    CustTB *customTab  = new CustTB(this);
    QWidget *customTabAnime = new QWidget(this);
    QString styleSheet="";
private:
    static const int maximumNumberlabel = 2;
    QLabel* Label[maximumNumberlabel];
    QMovie *movie = new QMovie(this);
    StretchLabel *stretchLabel = new StretchLabel(this);
    CalculateGrade *calckgrade= new CalculateGrade();


private slots:
    void closeEvent(QCloseEvent* event);
    void testing_clicked();
    void choosingTheme_clicked(QStringList stringList);
    void changeTheme(const QString &theme, int Index);
    void pushButton_clicked();
    void radioButtons_clicked();
    void pushButtonPrevious_clicked();
    void pushButtonNext_clicked();
    void pushButtonEndTest_clicked();
    void testBreak_clicked();
    void showPushButtonTask(int num_current_task,int num_previous_task);
    void uploadingBrowser();
    void showRadioButtonTask();
    void deleteRadioButtonTask();
    void testGrade();
    void copytask(int totalQuest);
    void randomNumberQwest (int totalQuest);
    void randomNumberAnswer(int totalAnswer);
    void createInterface();
    void createLabelsForStatusbar();
    void trainingManual_clicked();
    void setting_clicked();
    void help_clicked();
    void hideButtonTest();
    void hideTab(QWidget* widget);
    void showButtonTest();
    void saveTabsAndWidgets();
    void restoreTabsAndWidgets();
    void loadBrowser(QTextBrowser*textBrowser,
                    QString fieldNameHtm,
                    QString tableNameHtm,
                    QString fieldNameFileGraph,
                    QString fieldNameImageData,
                    QString tableNameGraph,
                    QString condition );
    QString readFileNameBackgraund(QString html,
                                   QString startSequence,
                                   QString endSequence);
    void delFileDir(QString path);
    void readStyleSheet(QString html);

    void uploadingGraphFiles2(QString fieldNameFileGraph,
                             QString fieldNameImageData,
                             QString tableNameGraph,
                             QString condition );
    void setServiceStatus(const QString &status);
    void showProgressBar(float  testExecutionTime);
    void updateProgress() ;
    void radioButtonsStretch_clicked();

    void setupRadioButtons(int maxRadioBut);
    void setupPushButtons (int maxPushButtons,int colCount);
    void getListThemes();
    void setupMenuBar();
    void setupStatusBar() ;
    void restoreStateMainWindow();
    void monitoringServices(QString serviceName);
    void settingTab();
    void loadImageData(QByteArray imageData);

    void saveFiles(const QStringList& stringListFileName,
                   const QList<QByteArray>& stringListImageData,
                   const QString& dirName);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
