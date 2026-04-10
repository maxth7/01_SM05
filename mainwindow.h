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
    //static const int MAX_LABEL = 2;
    QLabel* Label[UiConstants::MAX_LABEL];
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

    void saveTabWidgets();
    void restoreTabsAndWidgets();

    void loadContentIntoBrowser(QTextBrowser* textBrowser,
                                 const QString& fieldNameHtm,
                                 const QString& tableNameHtm,
                                 const QString& fieldNameFileGraph,
                                 const QString& fieldNameImageData,
                                 const QString& tableNameGraph,
                                 const QString& condition);

    QString extractFileNameFromHtml(const QString& html,
                                    const QString& startSequence,
                                    const QString& endSequence) const ;
    void clearDirectory(const QString path);
    void setStyleSheetBackground(const QString& html);
    void saveGraphFilesToDisk(const QString& fieldNameFileGraph,
                             const QString& fieldNameImageData,
                             const QString& tableNameGraph,
                             const QString& condition);

    void setServiceStatus(const QString &status);

    void showProgressBar(float testExecutionTime);
        bool isValidTime(float testExecutionTime) const;
        void setupProgressBar(int totalSeconds);
        void setupProgressBarStyle();
        void setupProgressBarFont();
        void setupTimer();
        void cleanupOldTimer();

    void updateProgress();
        bool isProgressComplete() const;
        void updateProgressValue();
        void updateProgressDisplay(int remainingSeconds);
        void finishTest();
        void resetProgressState();
        void updateStatusLabel(const QString& text);
        void updateProgressBar(int value);
        void stopAndDisconnectTimer();


    void onRadioButtonStretchClicked();
        bool areRadioButtonsValid() const ;
        bool isStretchModeEnabled() const;
        bool isNoStretchModeEnabled() const;
        void setFixedWindowSize(int width, int height);
        void setStretchWindowSize();

    void setupDynamicRadioButtons(int maxRadioBut);
        bool isValidButtonCount(int count) const;
        void clearRadioButtonPanel();
        void createDynamicRadioButtonLayout();
        void createDynamicRadioButtons(int count);
        QRadioButton* createSingleRadioButton(int index);
        void applyRadioButtonStyle(QRadioButton* button);
        void connectRadioButton(QRadioButton* button);

    void setupPushButtons (int maxPushButtons,int colCount);
        bool arePushButtonParamsValid(int maxPushButtons, int colCount) const;
        void clearPushButtonPanel();
        void createPushButtonLayout();
        void createAllPushButtons(int maxPushButtons, int colCount);
        QPushButton* createSinglePushButton(int index);
        void applyPushButtonStyle(QPushButton* button, int index);
        void configurePushButton(QPushButton* button, int index);
        void addPushButtonToLayout(QPushButton* button, int index, int colCount);

    void onChoosingThemeClicked();
        bool isDatabaseValid() const;
        QStringList fetchThemesFromDatabase();
        bool validateThemesList(const QStringList& themes) const;
        void displayThemesMenu(const QStringList& themes);
        void logError(const QString& error) const;

    void setupMenuBar();
        void setupMenuBarStyle();
        void setupTestButtons();

    void setupStatusBar() ;
        bool initializeDatabaseConnection();
        void showDatabaseErrorAndExit(const QString& errorMsg);
        void updateStatusLabels();
        QString extractThemeFromHtml(const QString& html);
        void updateThemeLabel();
        void updateTimeLabel();
        bool areStatusLabelsValid() const;

    void restoreStateMainWindow();
        void restoreWindowGeometry();
        void restoreSelectedThemeIndex();
        void restoreGraphPath();

    void ensureServiceIsRunning(const QString& serviceName);
        bool isServiceRunning(const QString& serviceName) ;
        void showServiceErrorAndExit(const QString& serviceName);
        void updateStatusLabelServiceIsRunning(const QString& message);
        void exitApplicationWithError(int exitCode = 1);
        bool areServiceCheckPrerequisitesValid() const;

 //   void settingTab();
    void setupCustomTabs();
        bool areCustomTabsValid() const;
        void setupAnimationLabel();
        void configureCustomTabWidgets();
        void ensureLayoutExists();
        void addTabsToTabWidget();
        void applyTabStyles();

    void loadImageData(const QByteArray& imageData);
        bool prepareImageDisplay();
        void setupGraphicsView();
        void setupCustomTabLayout();
        void addGraphicsViewToLayout(QGraphicsView* graphicsView);
        void displayImageInGraphicsView(const QImage& image);
        void cleanupOldScene(QGraphicsView* graphicsView);
        bool loadImageFromData(const QByteArray& imageData, QImage& image) const;


    void saveFiles(const QStringList& stringListFileName,
                   const QList<QByteArray>& stringListImageData,
                   const QString& dirName)const;
        bool ensureDirectoryExists(const QString& dirPath) const;
        bool saveSingleFile(const QString& directoryPath,
                            const QString& fileName,
                            const QByteArray& data) const;
        bool isValidFileData(const QString& fileName, const QByteArray& data) const;
        void logFileError(const QString& error, const QString& filePath) const;


private:
    Ui::MainWindow *ui;
    mutable int m_cachedThemeCount = -1;// Используется для кэширования числа тем
    int getMaxThemeIndex() const;
};
#endif // MAINWINDOW_H
