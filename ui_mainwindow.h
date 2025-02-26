/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QWidget *centralwidget;
    QGridLayout *gridLayout_4;
    QFrame *framTasksButton;
    QTabWidget *tabWidget;
    QWidget *tabtestResult;
    QGraphicsView *graphicsViewGrade;
    QWidget *tabSelectThemes;
    QListView *listViewThemes;
    QWidget *tabTest;
    QHBoxLayout *horizontalLayout_2;
    QTextBrowser *textBrowserQwest;
    QFrame *frameRadioButton;
    QTextBrowser *textBrowserAnser;
    QWidget *tabTranManual;
    QGridLayout *gridLayout;
    QFrame *frameMet;
    QGridLayout *gridLayout_6;
    QTextBrowser *textBrowserMet;
    QWidget *tabSetting;
    QGroupBox *groupBoxStretch;
    QVBoxLayout *L2;
    QGridLayout *gridLayout_3;
    QRadioButton *radioButtonStretch;
    QRadioButton *radioButtonNoStretch;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelGraph;
    QLineEdit *lineEditGraph;
    QToolButton *toolButtonGraph;
    QWidget *tabHelp;
    QGridLayout *gridLayout_2;
    QFrame *frameHelp;
    QGridLayout *gridLayout_5;
    QTextBrowser *textBrowserHelp;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonTestBreak;
    QSpacerItem *horizontalSpacer;
    QLabel *labelTaskNotComleted;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonPrevious;
    QPushButton *pushButtonNext;
    QPushButton *pushButtonEndTest;
    QProgressBar *progressBar;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(916, 743);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setStyleSheet(QString::fromUtf8("/*background-image: url(\"E:/qtpet/sm02/Tesla/tesla_m_S02_yellow.png\" );\n"
"background-image:url(\"E:/qtpet/settingSmile/help/water_effect_1024x0_326.webp\");*/ "));
        action = new QAction(MainWindow);
        action->setObjectName("action");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_4 = new QGridLayout(centralwidget);
        gridLayout_4->setObjectName("gridLayout_4");
        framTasksButton = new QFrame(centralwidget);
        framTasksButton->setObjectName("framTasksButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(framTasksButton->sizePolicy().hasHeightForWidth());
        framTasksButton->setSizePolicy(sizePolicy1);
        framTasksButton->setMinimumSize(QSize(0, 80));
        framTasksButton->setBaseSize(QSize(0, 23));
        framTasksButton->setStyleSheet(QString::fromUtf8("border-width: 2px; \n"
"border-style: solid;\n"
"border-color: : #000000;/* #000000\320\246\320\262\320\265\321\202 \320\263\321\200\320\260\320\275\320\270\321\206\321\213*/\n"
""));
        framTasksButton->setFrameShape(QFrame::Shape::StyledPanel);
        framTasksButton->setFrameShadow(QFrame::Shadow::Raised);

        gridLayout_4->addWidget(framTasksButton, 1, 0, 1, 1);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(9);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy2);
        tabWidget->setStyleSheet(QString::fromUtf8("border-width: 2px; \n"
"border-style: solid;\n"
"/*border: none;*/\n"
"border-color: : #000000;/* #000000\320\246\320\262\320\265\321\202 \320\263\321\200\320\260\320\275\320\270\321\206\321\213*/\n"
"\n"
"\n"
"\n"
"\n"
"\n"
""));
        tabWidget->setTabShape(QTabWidget::TabShape::Triangular);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        tabtestResult = new QWidget();
        tabtestResult->setObjectName("tabtestResult");
        graphicsViewGrade = new QGraphicsView(tabtestResult);
        graphicsViewGrade->setObjectName("graphicsViewGrade");
        graphicsViewGrade->setGeometry(QRect(180, 140, 256, 192));
        graphicsViewGrade->setFrameShape(QFrame::Shape::NoFrame);
        graphicsViewGrade->setLineWidth(0);
        tabWidget->addTab(tabtestResult, QString());
        tabSelectThemes = new QWidget();
        tabSelectThemes->setObjectName("tabSelectThemes");
        listViewThemes = new QListView(tabSelectThemes);
        listViewThemes->setObjectName("listViewThemes");
        listViewThemes->setGeometry(QRect(9, 9, 256, 192));
        tabWidget->addTab(tabSelectThemes, QString());
        tabTest = new QWidget();
        tabTest->setObjectName("tabTest");
        horizontalLayout_2 = new QHBoxLayout(tabTest);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        textBrowserQwest = new QTextBrowser(tabTest);
        textBrowserQwest->setObjectName("textBrowserQwest");
        textBrowserQwest->setBaseSize(QSize(0, 0));
        textBrowserQwest->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(textBrowserQwest);

        frameRadioButton = new QFrame(tabTest);
        frameRadioButton->setObjectName("frameRadioButton");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frameRadioButton->sizePolicy().hasHeightForWidth());
        frameRadioButton->setSizePolicy(sizePolicy3);
        frameRadioButton->setMinimumSize(QSize(40, 0));
        frameRadioButton->setStyleSheet(QString::fromUtf8("border-width: 2px;\n"
"border-style: solid;\n"
"border-color: : #000000;/* #000000\320\246\320\262\320\265\321\202 \320\263\321\200\320\260\320\275\320\270\321\206\321\213*/\n"
"background-color: lightblue\n"
""));
        frameRadioButton->setFrameShape(QFrame::Shape::StyledPanel);
        frameRadioButton->setFrameShadow(QFrame::Shadow::Raised);

        horizontalLayout_2->addWidget(frameRadioButton);

        textBrowserAnser = new QTextBrowser(tabTest);
        textBrowserAnser->setObjectName("textBrowserAnser");

        horizontalLayout_2->addWidget(textBrowserAnser);

        tabWidget->addTab(tabTest, QString());
        tabTranManual = new QWidget();
        tabTranManual->setObjectName("tabTranManual");
        gridLayout = new QGridLayout(tabTranManual);
        gridLayout->setObjectName("gridLayout");
        frameMet = new QFrame(tabTranManual);
        frameMet->setObjectName("frameMet");
        frameMet->setStyleSheet(QString::fromUtf8("border: none;\n"
"\n"
"\n"
"\n"
"\n"
""));
        frameMet->setFrameShape(QFrame::Shape::StyledPanel);
        frameMet->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_6 = new QGridLayout(frameMet);
        gridLayout_6->setObjectName("gridLayout_6");
        textBrowserMet = new QTextBrowser(frameMet);
        textBrowserMet->setObjectName("textBrowserMet");

        gridLayout_6->addWidget(textBrowserMet, 0, 0, 1, 1);


        gridLayout->addWidget(frameMet, 0, 0, 1, 1);

        tabWidget->addTab(tabTranManual, QString());
        tabSetting = new QWidget();
        tabSetting->setObjectName("tabSetting");
        groupBoxStretch = new QGroupBox(tabSetting);
        groupBoxStretch->setObjectName("groupBoxStretch");
        groupBoxStretch->setGeometry(QRect(10, 10, 211, 91));
        groupBoxStretch->setFlat(false);
        L2 = new QVBoxLayout(groupBoxStretch);
        L2->setObjectName("L2");
        L2->setContentsMargins(-1, 9, -1, -1);
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        radioButtonStretch = new QRadioButton(groupBoxStretch);
        radioButtonStretch->setObjectName("radioButtonStretch");
        radioButtonStretch->setChecked(true);

        gridLayout_3->addWidget(radioButtonStretch, 0, 0, 1, 1);

        radioButtonNoStretch = new QRadioButton(groupBoxStretch);
        radioButtonNoStretch->setObjectName("radioButtonNoStretch");

        gridLayout_3->addWidget(radioButtonNoStretch, 1, 0, 1, 1);


        L2->addLayout(gridLayout_3);

        layoutWidget = new QWidget(tabSetting);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 130, 851, 25));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        labelGraph = new QLabel(layoutWidget);
        labelGraph->setObjectName("labelGraph");
        labelGraph->setEnabled(true);

        horizontalLayout_3->addWidget(labelGraph);

        lineEditGraph = new QLineEdit(layoutWidget);
        lineEditGraph->setObjectName("lineEditGraph");
        lineEditGraph->setEnabled(true);

        horizontalLayout_3->addWidget(lineEditGraph);

        toolButtonGraph = new QToolButton(layoutWidget);
        toolButtonGraph->setObjectName("toolButtonGraph");
        toolButtonGraph->setEnabled(true);

        horizontalLayout_3->addWidget(toolButtonGraph);

        tabWidget->addTab(tabSetting, QString());
        tabHelp = new QWidget();
        tabHelp->setObjectName("tabHelp");
        gridLayout_2 = new QGridLayout(tabHelp);
        gridLayout_2->setObjectName("gridLayout_2");
        frameHelp = new QFrame(tabHelp);
        frameHelp->setObjectName("frameHelp");
        frameHelp->setStyleSheet(QString::fromUtf8("border: none;\n"
"\n"
"\n"
"\n"
"\n"
""));
        frameHelp->setFrameShape(QFrame::Shape::StyledPanel);
        frameHelp->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_5 = new QGridLayout(frameHelp);
        gridLayout_5->setObjectName("gridLayout_5");
        textBrowserHelp = new QTextBrowser(frameHelp);
        textBrowserHelp->setObjectName("textBrowserHelp");
        textBrowserHelp->setStyleSheet(QString::fromUtf8("  border: none; "));
        textBrowserHelp->setFrameShape(QFrame::Shape::NoFrame);

        gridLayout_5->addWidget(textBrowserHelp, 0, 0, 1, 1);


        gridLayout_2->addWidget(frameHelp, 0, 0, 1, 1);

        tabWidget->addTab(tabHelp, QString());

        gridLayout_4->addWidget(tabWidget, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(32);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(7, 3, 11, -1);
        pushButtonTestBreak = new QPushButton(centralwidget);
        pushButtonTestBreak->setObjectName("pushButtonTestBreak");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        pushButtonTestBreak->setFont(font);
        pushButtonTestBreak->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #00FF7F; /* \320\246\320\262\320\265\321\202 \321\204\320\276\320\275\320\260 ffcc00 - \320\260\320\277\320\265\320\273\321\214\321\201\320\270\320\275 */\n"
"    border: 2px solid #ff0000; /* \320\242\320\276\320\273\321\211\320\270\320\275\320\260 \320\270 \321\206\320\262\320\265\321\202 \321\200\320\260\320\274\320\272\320\270 */\n"
"    border-radius: 10px; /* \320\227\320\260\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 \321\200\320\260\320\274\320\272\320\270 */\n"
"    padding: 5px; /* \320\236\321\202\321\201\321\202\321\203\320\277\321\213 \320\262\320\275\321\203\321\202\321\200\320\270 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: #000000; /* \321\201\320\270\320\275\320\270\320\271 \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 ffffff - \321\215\321\202\320\276 \320\261\320\265\320\273\321\213 */\n"
"}"));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::WindowClose));
        pushButtonTestBreak->setIcon(icon);

        horizontalLayout->addWidget(pushButtonTestBreak);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        labelTaskNotComleted = new QLabel(centralwidget);
        labelTaskNotComleted->setObjectName("labelTaskNotComleted");
        labelTaskNotComleted->setFont(font);

        horizontalLayout->addWidget(labelTaskNotComleted);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButtonPrevious = new QPushButton(centralwidget);
        pushButtonPrevious->setObjectName("pushButtonPrevious");
        pushButtonPrevious->setMinimumSize(QSize(0, 25));
        pushButtonPrevious->setFont(font);
        pushButtonPrevious->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #00FF7F; /* \320\246\320\262\320\265\321\202 \321\204\320\276\320\275\320\260 ffcc00 - \320\260\320\277\320\265\320\273\321\214\321\201\320\270\320\275 */\n"
"    border: 2px solid #ff0000; /* \320\242\320\276\320\273\321\211\320\270\320\275\320\260 \320\270 \321\206\320\262\320\265\321\202 \321\200\320\260\320\274\320\272\320\270 */\n"
"    border-radius: 10px; /* \320\227\320\260\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 \321\200\320\260\320\274\320\272\320\270 */\n"
"    padding: 5px; /* \320\236\321\202\321\201\321\202\321\203\320\277\321\213 \320\262\320\275\321\203\321\202\321\200\320\270 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: #000000; /* \321\201\320\270\320\275\320\270\320\271 \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 ffffff - \321\215\321\202\320\276 \320\261\320\265\320\273\321\213 */\n"
"}"));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious));
        pushButtonPrevious->setIcon(icon1);

        horizontalLayout->addWidget(pushButtonPrevious);

        pushButtonNext = new QPushButton(centralwidget);
        pushButtonNext->setObjectName("pushButtonNext");
        pushButtonNext->setMinimumSize(QSize(0, 25));
        pushButtonNext->setFont(font);
        pushButtonNext->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #00FF7F; /* \320\246\320\262\320\265\321\202 \321\204\320\276\320\275\320\260 ffcc00 - \320\260\320\277\320\265\320\273\321\214\321\201\320\270\320\275 */\n"
"    border: 2px solid #ff0000; /* \320\242\320\276\320\273\321\211\320\270\320\275\320\260 \320\270 \321\206\320\262\320\265\321\202 \321\200\320\260\320\274\320\272\320\270 */\n"
"    border-radius: 10px; /* \320\227\320\260\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 \321\200\320\260\320\274\320\272\320\270 */\n"
"    padding: 5px; /* \320\236\321\202\321\201\321\202\321\203\320\277\321\213 \320\262\320\275\321\203\321\202\321\200\320\270 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: #000000; /* \321\201\320\270\320\275\320\270\320\271 \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 ffffff - \321\215\321\202\320\276 \320\261\320\265\320\273\321\213 */\n"
"}"));
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::GoNext));
        pushButtonNext->setIcon(icon2);

        horizontalLayout->addWidget(pushButtonNext);

        pushButtonEndTest = new QPushButton(centralwidget);
        pushButtonEndTest->setObjectName("pushButtonEndTest");
        pushButtonEndTest->setMinimumSize(QSize(0, 25));
        pushButtonEndTest->setFont(font);
        pushButtonEndTest->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #00FF7F; /* \320\246\320\262\320\265\321\202 \321\204\320\276\320\275\320\260 ffcc00 - \320\260\320\277\320\265\320\273\321\214\321\201\320\270\320\275 */\n"
"    border: 2px solid #ff0000; /* \320\242\320\276\320\273\321\211\320\270\320\275\320\260 \320\270 \321\206\320\262\320\265\321\202 \321\200\320\260\320\274\320\272\320\270 */\n"
"    border-radius: 10px; /* \320\227\320\260\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 \321\200\320\260\320\274\320\272\320\270 */\n"
"    padding: 5px; /* \320\236\321\202\321\201\321\202\321\203\320\277\321\213 \320\262\320\275\321\203\321\202\321\200\320\270 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: #000000; /* \321\201\320\270\320\275\320\270\320\271 \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 ffffff - \321\215\321\202\320\276 \320\261\320\265\320\273\321\213 */\n"
"}"));

        horizontalLayout->addWidget(pushButtonEndTest);


        gridLayout_4->addLayout(horizontalLayout, 2, 0, 1, 1);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        gridLayout_4->addWidget(progressBar, 3, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 916, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "iSmile", nullptr));
        action->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\320\276\321\200 \321\202\320\265\320\274\321\213", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabtestResult), QCoreApplication::translate("MainWindow", "testResult", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSelectThemes), QCoreApplication::translate("MainWindow", "SelectThemes", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabTest), QCoreApplication::translate("MainWindow", "Testing", nullptr));
        textBrowserMet->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\234\320\265\321\202\320\276\320\264\320\270\321\207\320\272\320\260</p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabTranManual), QCoreApplication::translate("MainWindow", "Training Manual", nullptr));
        groupBoxStretch->setTitle(QCoreApplication::translate("MainWindow", "Stretch", nullptr));
        radioButtonStretch->setText(QCoreApplication::translate("MainWindow", "stretchable application", nullptr));
        radioButtonNoStretch->setText(QCoreApplication::translate("MainWindow", "non-stretchable application", nullptr));
        labelGraph->setText(QCoreApplication::translate("MainWindow", "path to graph", nullptr));
        toolButtonGraph->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSetting), QCoreApplication::translate("MainWindow", "Setting", nullptr));
        textBrowserHelp->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\321\201\320\277\321\200\320\260\320\262\320\272\320\260</p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabHelp), QCoreApplication::translate("MainWindow", "Help", nullptr));
        pushButtonTestBreak->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\265\321\200\320\262\320\260\321\202\321\214", nullptr));
        labelTaskNotComleted->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        pushButtonPrevious->setText(QString());
        pushButtonNext->setText(QString());
        pushButtonEndTest->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\272\320\276\320\275\321\207\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
