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
#include <QString>

#include "dbfacade.h"
#include "createtasks.h"
#include "custtb.h"
#include "mainwindowstate.h"
#include "stretchlabel.h"

#include "calculategrade.h"
#include "checkinservices.h"
#include "constants.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace ManualConstants {
// Используем constexpr для эффективности
inline constexpr char TEXT_FIELD[] = "met";
inline constexpr char TABLE_NAME[] = "tasks";
inline constexpr char GRAPH_FIELD[] = "filename";
inline constexpr char IMAGE_FIELD[] = "graph";
inline constexpr char GRAPH_TABLE[] = "graphmet";
}
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
    //static const int maximumNumberlabel = 2;
    static const int MAX_LABEL = 2;
    QLabel* Label[MAX_LABEL];
    QMovie *movie = new QMovie(this);
    StretchLabel *stretchLabel = new StretchLabel(this);
    CalculateGrade *calckgrade= new CalculateGrade();

    void validateUi() const {
        if (!ui) {
            qFatal("MainWindow Errors: ui is nullptr! This should never happen.");
        }
    }
private slots:
    void closeEvent(QCloseEvent* event);
    void onTestingClicked();
    void onChooseThemeClicked(const QStringList& stringList);
    void changeTheme(const QString &theme, int Index);
    void onPushButtonClicked();
    void onRadioButtonClicked();
    void onPushButtonPreviousClicked();
    void onPushButtonNextClicked();
    void onPushButtonEndTestClicked();
    void onTestBreakClicked();
    void showPushButtonTask(int num_current_task,int num_previous_task);

    void uploadingBrowser();
    QString formatAnswerText(const QString& originalAnswer,
                             const QString& answerPrefix);//13
    //void showRadioButtonTask();
    void setupRadioButtonsForCurrentTask();
        void cleanupRadioButtons();
        void createRadioButtonLayout();
        int getCurrentTaskAnswerCount();
        void createRadioButtons(int count);
        QRadioButton* createRadioButton(int number) const;
        void restoreRadioButtonState();
        QString getRadioButtonStyleSheet() const ;

    void testGrade();
    void shuffleAndCopyTasks(int totalQuest);
    void randomNumberQwest (int totalQuest);
    void randomNumberAnswer(int totalAnswer);

    void setupTestInterface();
        void configureInitialVisibility();
        void configureTabAppearance();
        int prepareTestData();
        void applyStylesToBrowsers();

    void setupStatusBarLabels();
        bool isStatusBarValid() const;
        void cleanupStatusBarLabels() ;
        void configureStatusBarStyle();
        QString getStatusBarStyleSheet() const;
        QFont createLabelFont() const;
        int getLabelFontSize() const;
        bool isLabelFontBold() const;
        QLabel* createStatusLabel(int index, const QFont& font) ;
        QString getLabelText(int index) const;
        Qt::Alignment getLabelAlignment() const;
        QString getLabelStyleSheet() const;
        void addLabelToStatusBar(QLabel* label);
        void storeLabelPointer(int index, QLabel* label);

    void onTrainingManualButtonClicked();
        //void ensureCorrectCentralWidget() ;there are in onHelpClicked

    void onSettingClicked();
        void setupSettingTabUI();
        void configureFontSettings();
        void setupStretchRadioButtons();
        void setupTabBackground();
        void ensureCentralWidget();


    void onHelpClicked();
        void setupHelpTabUI();// privat?
        void ensureCorrectCentralWidget();
        void loadHelpContent();

    void hideAllTestControls();
        void hideTestButtons();
        void hideTestContainers();
        void resetTestTimer();
        void hideTestProgressIndicator();

    void showOnlyThisTab(QWidget* widget);
        void removeAllTabsExcept(QWidget* widgetToKeep);
        void hideTabBar();
        void hideGradeView();

    void showTestControls();
        void showTestButtons();
        void showTestContainer();
        void showTestLabel();
        void showWidgets(const QList<QWidget*>& widgets);

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
