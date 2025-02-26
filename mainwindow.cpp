#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->centralwidget);

    setWindowIcon(QIcon(":/images/Tesla/smile0.webp"));
    setWindowTitle("iSmile v1.6");

    restoreStateMainWindow();
    setupMenuBar();

    QString serviceName="postgresql-x64-17";
    monitoringServices(serviceName);

    setupStatusBar();
    settingTab();
}

void MainWindow::monitoringServices(QString serviceName){
    if(!ChServices.isPostgresServiceRunning(serviceName)) {
        QString msg = "Служба: " + serviceName + " не запущена.";
        QMessageBox::critical(nullptr,
                              "Ошибка",
                              "Не удалось запустить приложение. " + msg);
        Label[0]->setText(msg);
        QCoreApplication::exit(1);
    }
}
void MainWindow::restoreStateMainWindow() {
    state.RestoreState(pos, size);
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    if (!screenGeometry.contains(QRect(pos, size))) {
        pos = QPoint(0, 0);
        size = QSize(800, 600);
    }
    this->move(pos);
    this->resize(size);
    selectedIndex=state.CurrentThema-1;
    if (selectedIndex<1){
        selectedIndex=1;
    }
    ui->lineEditGraph->setText(state.pathGraph);
}

void MainWindow::setupMenuBar() {
    QAction*  choosingTheme   = new QAction(tr("&Темы"), this);
    QAction*  testing         = new QAction(tr("&Тест"), this);
    QAction*  trainingManual  = new QAction(tr("&Методичка"), this);
    QAction*  setting         = new QAction(tr("&Настройки"), this);
    QAction*  help            = new QAction(tr("&Справка"), this);

    connect(choosingTheme, &QAction::triggered, this, &MainWindow::getListThemes);
    connect(testing,       &QAction::triggered, this, &MainWindow::testing_clicked);
    connect(trainingManual,&QAction::triggered, this, &MainWindow::trainingManual_clicked);
    connect(setting,       &QAction::triggered, this, &MainWindow::setting_clicked);
    connect(help,          &QAction::triggered, this, &MainWindow::help_clicked);

    ui->menubar->addAction(choosingTheme);
    ui->menubar->addAction(testing);
    ui->menubar->addAction(trainingManual);
    ui->menubar->addAction(setting);
    ui->menubar->addAction(help);

    ui->menubar->setStyleSheet(
        "QMenuBar {"
        "   background-color: #FFDEAD;"
        "   color: black;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QMenuBar::item {"
        "   spacing: 3px;"
        "   padding: 5px;"
        "}"
        "QMenuBar::item:selected {"
        "   background-color: #2E2E2E;"
        "   border: 2px solid #FF0000;"
        "}"
        );
    connect(ui->pushButtonNext, &QPushButton::clicked, this, &MainWindow::pushButtonNext_clicked);
    connect(ui->pushButtonEndTest, &QPushButton::clicked, this, &MainWindow::pushButtonEndTest_clicked);
    connect(ui->pushButtonPrevious, &QPushButton::clicked, this, &MainWindow::pushButtonPrevious_clicked);
    connect(ui->pushButtonTestBreak, &QPushButton::clicked, this, &MainWindow::testBreak_clicked);
}
void MainWindow::setupStatusBar() {
    //5 не проверяю открыта ли бд------------------------------
    if(!dbFacade.openDatabaseQPSQL()){
        QString msg = "Невозможно открыть БД.";
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось запустить приложение. " + msg);
        Label[1]->setText(msg);
        QCoreApplication::exit(1);
    }
    createLabelsForStatusbar();
    QString condition = " Where n="+QString::number(selectedIndex);
    QString tableName = "tasks";
    QString fieldName = "task";

    QString htmlValue=dbFacade.getValueDatabaseQPSQL(fieldName,
                                                       tableName,
                                                       condition);
    QString q1 ="<!--<body background=";
    int index = htmlValue.indexOf(q1);

    if (index != -1) {
        QString ThemahtmlValue=htmlValue.left(index);
        ThemahtmlValue = htmlValue.left(index).simplified();

        Label[0]->setText(ThemahtmlValue);
    }
    QString timeTest = QString("Время теста: %1 мин").
                       arg(QString::number(state.testExecutionTime,
                                           'f', 2));
    Label[1]->setText(timeTest);
}

void MainWindow::settingTab(){
    //--------------------------------------------------
    stretchLabel->setGif(":/images/Tesla/tesla_m_s.gif");

    customTabAnime->setObjectName("tabCustomTabAnime");
    customTabAnime->setStyleSheet("QTabWidget::pane { border: none; }");

    customTab->setObjectName("tabcustomTab"); // было не задано
    QLayout *layout = customTabAnime->layout();

    if (!customTabAnime->layout()) {
        customTabAnime->setLayout(new QVBoxLayout(customTabAnime));
    }
    //--------------------------------------------------------------------------
    customTabAnime->layout()->addWidget(stretchLabel);
    ui->tabWidget->addTab(customTabAnime,"customTabAnime");
    ui->tabWidget->addTab(customTab,"customTab");
    saveTabsAndWidgets();
    hideTab(customTabAnime);
    hideButtonTest();
}

void MainWindow::getListThemes(){
    QString tableName = "tasks";
    QString fieldName = "task";
    QString condition="";
    QStringList stringList;
    stringList=dbFacade.getRecordsDatabaseQPSQL(fieldName,
                                                tableName,
                                                condition);

    choosingTheme_clicked(stringList);
}
void MainWindow::choosingTheme_clicked(QStringList stringList){
    QMenu* themeMenu = new QMenu(tr("Выбор темы"), this);
    int Index=0;
    for (const QString &theme : stringList) {
        Index++;
        QAction* themeAction = new QAction(theme, this);
        themeMenu->addAction(themeAction);

        connect(themeAction, &QAction::triggered, this, [this, theme,Index]() {

            QString submenu = theme;
            changeTheme(submenu,Index);
        });
    }
    themeMenu->exec(QCursor::pos());
    restoreTabsAndWidgets();
    hideTab(customTabAnime);
}

void MainWindow:: changeTheme(const QString &theme,int Index){
    Label[0]->setText(theme);
    selectedIndex = Index;
}

void MainWindow::saveTabsAndWidgets() {
     savedWidgets.clear();
    for (int i = 0; i < ui->tabWidget->count(); ++i) {

        QWidget* widget = ui->tabWidget->widget(i);
        if (widget) {
            savedWidgets.append(widget);
        }
     }
}

 void MainWindow::restoreTabsAndWidgets() {
     ui->tabWidget->tabBar()->show();
     ui->tabWidget->tabBar()->setStyleSheet("");

     while (ui->tabWidget->count() > 0) {
         ui->tabWidget->removeTab(0);
     }
     for (QWidget* widget : savedWidgets) {
         if (widget) {
             QString tabText = widget->objectName();
             ui->tabWidget->addTab(widget, tabText);

         }
     }
 }

void MainWindow::hideTab(QWidget* widget){
    int tabCount = ui->tabWidget->count();
    QList<int> tabsToRemove;

    for (int i = 0; i < tabCount; ++i) {
        if (ui->tabWidget->widget(i) != widget) {
            tabsToRemove.append(i);
        }
    }

    for (int i = tabsToRemove.count() - 1; i >= 0; --i) {
        int tabIndex = tabsToRemove.at(i);
        ui->tabWidget->removeTab(tabIndex);
    }

    ui->tabWidget->tabBar()->setStyleSheet("QTabBar::tab { height: 0px; }");
    ui->graphicsViewGrade->hide();
}

void MainWindow::hideButtonTest() {
    // Список кнопок для скрытия
    QList<QPushButton*> buttonsToHide = {
        ui->pushButtonEndTest,
        ui->pushButtonPrevious,
        ui->pushButtonNext,
        ui->pushButtonTestBreak
    };

    for (QPushButton* button : buttonsToHide) {
        button->hide();
    }

    ui->framTasksButton->hide();
    ui->progressBar->hide();
    currentTime = 0;
    ui->labelTaskNotComleted->hide();
}

void MainWindow::setting_clicked(){
    restoreTabsAndWidgets();//setting_clicked
    hideTab(ui->tabSetting);
    hideButtonTest();

    QFont font = ui->groupBoxStretch->font();
    font.setPointSize(14);
    ui->groupBoxStretch->setFont(font);
    ui->groupBoxStretch->setAlignment(Qt::AlignTop);
    ui->radioButtonStretch->adjustSize();
    ui->radioButtonNoStretch->adjustSize();

    connect(ui->radioButtonStretch,   &QRadioButton::clicked,
            this, &MainWindow::radioButtonsStretch_clicked, Qt::UniqueConnection);
    connect(ui->radioButtonNoStretch, &QRadioButton::clicked,
            this, &MainWindow::radioButtonsStretch_clicked, Qt::UniqueConnection);


    QPixmap background(":/images/BackgroundsFP2000/WB00760L.GIF");
    QPalette palette = ui->tabSetting->palette();
    palette.setBrush(QPalette::Window, background);
    ui->tabSetting->setPalette(palette);
    ui->tabSetting->setAutoFillBackground(true);

    ui->tabWidget->     show();
    //setCentralWidget(ui->centralwidget);//Chatson
    if (centralWidget() != ui->centralwidget) {
        setCentralWidget(ui->centralwidget);
    }
}

void MainWindow::radioButtonsStretch_clicked() {

    if (ui->radioButtonNoStretch->isChecked()) {
        this->setFixedSize(800, 800);
    } else if (ui->radioButtonStretch->isChecked()) {
        this->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    }
}

void MainWindow::help_clicked(){
    if (ui->centralwidget && centralWidget() != ui->centralwidget) {
        setCentralWidget(ui->centralwidget);
    }

    restoreTabsAndWidgets();
    if (ui->tabHelp) {
        hideTab(ui->tabHelp);
    }
    hideButtonTest();

    QString fieldNameHtm="htmlcode";
    QString tableNameHtm="help";
    QString fieldNameFileGraph="filename";
    QString fieldNameImageData="graph";
    QString tableNameGraph="graphhelp";
    QString condition="";
    loadBrowser(ui->textBrowserHelp,
                fieldNameHtm,
                tableNameHtm,
                fieldNameFileGraph,
                fieldNameImageData,
                tableNameGraph,
                condition  );
}

void MainWindow::testing_clicked(){

    tGrade=0;

    if (centralWidget() != ui->centralwidget) {
        setCentralWidget(ui->centralwidget);
    }
    restoreTabsAndWidgets();

    ui->menubar->hide();

    if (state.testExecutionTime > 0) {
        showProgressBar(state.testExecutionTime);
    } else {
        qDebug() << "Некорректное время выполнения теста: " << state.testExecutionTime;
    }

    showButtonTest();
    hideTab(ui->tabTest);

    ui->tabWidget-> show();

    QString condition=" Where n="+QString::number(selectedIndex);
    QString tableName = "tasks";
    QString fieldName = "task";


    QString htmlValue=dbFacade.getValueDatabaseQPSQL(fieldName,
                                                     tableName,
                                                     condition);
    htmlValue = htmlValue.simplified();
    readStyleSheet(htmlValue);

    QString fieldNameHtm="task";
    QString tableNameHtm="tasks";
    QString fieldNameFileGraph="filename";
    QString fieldNameImageData="graph";
    QString tableNameGraph="graphtest";
    condition="Where idtasks="+QString::number(selectedIndex);

    uploadingGraphFiles2(fieldNameFileGraph,
                         fieldNameImageData,
                         tableNameGraph,
                         condition );


    num_cur_task.getTasks(htmlValue,state.pathGraph);

    int totalQuestion=num_cur_task.getTotalQwest();
    ui->labelTaskNotComleted->setText("Не выполнено заданий: "
                            +QString::number(totalQuestion-ready_madeTasks));
    createInterface();
}

void MainWindow::showButtonTest() {
    QList<QWidget*> widgets = {
        ui->framTasksButton,
        ui->pushButtonEndTest,
        ui->pushButtonPrevious,
        ui->pushButtonNext,
        ui->pushButtonTestBreak,
        ui->labelTaskNotComleted
    };

    for (QWidget* widget : widgets) {
        if (widget && !widget->isVisible()) {
            widget->show();
        }
    }
}

void MainWindow::trainingManual_clicked(){
    if (centralWidget() != ui->centralwidget) {
        setCentralWidget(ui->centralwidget);
    }

    restoreTabsAndWidgets();

    hideTab(ui->tabTranManual);
    hideButtonTest();

     ui->tabWidget->show();


    QString fieldNameHtm="met";
    QString tableNameHtm="tasks";
    QString fieldNameFileGraph="filename";
    QString fieldNameImageData="graph";
    QString tableNameGraph="graphmet";
    QString condition="Where idtasks="+QString::number(selectedIndex);

    loadBrowser(ui->textBrowserMet,
                fieldNameHtm,
                tableNameHtm,
                fieldNameFileGraph,
                fieldNameImageData,
                tableNameGraph,
                condition  );
}
void MainWindow::setupRadioButtons(int maxRadioBut) {
    if (ui->frameRadioButton->layout() != nullptr) {
        QLayoutItem* item;
        while ((item = ui->frameRadioButton->layout()->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
        delete ui->frameRadioButton->layout();
    }
    auto* layoutRadioButton = new QVBoxLayout(ui->frameRadioButton);
    ui->frameRadioButton->setLayout(layoutRadioButton);

    for (int i = 0; i < maxRadioBut; ++i) {
        auto* radioButton = new QRadioButton(QString::number(i + 1));
        radioButton->setStyleSheet("QRadioButton {"
                                   "   font-size: 16px;"
                                   "   color: #4682B4;"
                                   "   font-weight: bold;"
                                   "   background-color: #FFFFFF;"
                                   "}"
                                   "QRadioButton::indicator {"
                                   "   width: 30px;"
                                   "   height: 30px;"
                                   "}");
        layoutRadioButton->addWidget(radioButton);
        connect(radioButton, &QRadioButton::clicked, this, &MainWindow::radioButtons_clicked);
    }
}
void MainWindow::setupPushButtons(int maxPushButtons,int colCount) {
    if (ui->framTasksButton->layout() != nullptr) {
        QLayoutItem* item;
        while ((item = ui->framTasksButton->layout()->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
        delete ui->framTasksButton->layout();
    }

    QGridLayout *layout = new QGridLayout(ui->framTasksButton);

    for (int i = 0; i < maxPushButtons; ++i) {
        pushButton[i] = new QPushButton(QString::number(i + 1)); // Установка текста на кнопке

        if(i==0){
            pushButton[i]->setStyleSheet("color: white;"
                                         "border-color: red;"
                                         "background-color: #4CAF50; "
                                         "font-weight: bold;"
                                         " padding: 2px 2px;"
                                         " text-align: center;"
                                         " text-decoration: none;"
                                         " font-size: 16px;");
        }
        else{
            pushButton[i]->setStyleSheet("color: black;"
                                         "border-color: black;"
                                         "background-color:#4CAF50; "
                                         "font-weight: bold;"
                                         " padding: 2px 2px;"
                                         " text-align: center;"
                                         " text-decoration: none;"
                                         " font-size: 16px;");
        }
        pushButton[i]->setFixedSize(30, 30);
        pushButton[i]->setProperty("index", i);
        connect(pushButton[i], &QPushButton::clicked, this, &MainWindow::pushButton_clicked);

        layout->addWidget(pushButton[i], i / colCount, i % colCount);

    }
}
void MainWindow::createInterface(){
    ui->pushButtonEndTest->setVisible(false);
    ui->pushButtonPrevious->setVisible(true);
    ui->pushButtonNext->setVisible(true);
    ui->tabWidget->setTabShape(QTabWidget::Triangular);

    int totalQuestions=num_cur_task.getTotalQwest();

    copytask(totalQuestions);

    ui->textBrowserAnser->setStyleSheet(styleSheet);
    ui->textBrowserQwest->setStyleSheet(styleSheet);

    uploadingBrowser();

    int maxRadioBut=num_cur_task.ctest[0].saveTotalAnswer;
    setupRadioButtons(maxRadioBut);
    const int rowCount = (totalQuestions < 20) ? 1 : 2;
    const int colCount = (totalQuestions % rowCount == 0) ? (totalQuestions / rowCount) : (totalQuestions / rowCount + 1);
    setupPushButtons(totalQuestions,colCount);
}

void MainWindow::createLabelsForStatusbar(){
       ui->statusbar->setStyleSheet("border: 2px solid black");
       QFont font;
       font.setPointSize(12);
       font.setBold(true);

       for(int i=0;i<maximumNumberlabel;++i){
           Label[i] = new QLabel("Label"+QString::number(i));
           Label[i]->setAlignment(Qt::AlignLeft);
           Label[i]->setFont(font);

           Label[i]->setStyleSheet("background-color: #FF8C00; color: #2E2E2E;");
           ui->statusbar->addWidget(Label[i]);
       }
}

void MainWindow::pushButton_clicked(){
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());

    int numPreviousTask =num_cur_task.getNumTask();

    if (clickedButton) {
        QString buttonText = clickedButton->text();
        int numCurrentTask=buttonText.toInt()-1;
         num_cur_task.setNumTask(numCurrentTask);

        showPushButtonTask(numCurrentTask,numPreviousTask );
        uploadingBrowser();
        showRadioButtonTask();
    }
}

void MainWindow::pushButtonPrevious_clicked(){
    int i_task=num_cur_task.getNumTask();
    int num_previous_task =i_task;
    i_task--;
    num_cur_task.setNumTask(i_task);
    i_task=num_cur_task.getNumTask();
    showPushButtonTask(i_task,num_previous_task);
    uploadingBrowser();
    showRadioButtonTask();
}

void MainWindow::pushButtonNext_clicked(){
    int i_task=num_cur_task.getNumTask();
    int num_previous_task =i_task;
    i_task++;
    num_cur_task.setNumTask(i_task);
    i_task=num_cur_task.getNumTask();

     showPushButtonTask(i_task,num_previous_task );
     uploadingBrowser();
     showRadioButtonTask();
}

void MainWindow:: showPushButtonTask(int num_current_task,int num_previous_task){
    QString StyleSheet;
    if(num_cur_task.saveTaskPushButton[num_previous_task]){
        StyleSheet="QPushButton {"
                "color:  black;"
                "border-color: black;"
                "background-color: #DDDDDD;"
                "font-weight: bold;"
                " padding: 2px 2px;"
                " text-align: center;"
                " text-decoration: none;"
                " font-size: 16px;"
                "}";
    }
    else{
    StyleSheet="QPushButton {"
                "color:  black;"
                "border-color:  black;"
                "background-color: #4CAF50; "
                "font-weight: bold;"
                " padding: 2px 2px;"
                " text-align: center;"
                " text-decoration: none;"
                " font-size: 16px;"
                "}";
  }
    pushButton[num_previous_task]->setStyleSheet(StyleSheet);
    pushButton[num_previous_task]->show();

    if( num_cur_task.saveTaskPushButton[num_current_task]){

       StyleSheet = "QPushButton {"
                   "color: white;"
                   "border-color: red;"
                   "background-color: #DDDDDD;"
                   "font-weight: bold;"
                   "padding: 2px 2px;"
                   "text-align: center;"
                   "text-decoration: none;"
                   "font-size: 16px;"
                   "}";
    }
    else{
       StyleSheet ="QPushButton {"
                   "color: white;"
                   "border-color: red;"
                   "background-color: #4CAF50;"
                   "font-weight: bold;"
                   "padding: 2px 2px;"
                   "text-align: center;"
                   "text-decoration: none;"
                   "font-size: 16px;"
                "}";
    }
   pushButton[num_current_task]->setStyleSheet(StyleSheet);
   pushButton[num_current_task]->show();
}

void MainWindow:: deleteRadioButtonTask(){

    QVBoxLayout* layoutRadioButton = qobject_cast<QVBoxLayout*>(ui->frameRadioButton->layout());
    QLayoutItem* item;
    if(layoutRadioButton) {
        while ((item = layoutRadioButton->takeAt(0)) != nullptr) {
              delete item->widget();
              delete item;
        }
    }

    QLayout* previousLayout = ui->frameRadioButton->layout();

    if (previousLayout) {
       delete previousLayout;
    }
}

void MainWindow:: showRadioButtonTask(){
    deleteRadioButtonTask();
    QVBoxLayout *layoutRadioButton = new QVBoxLayout(ui->frameRadioButton);
    ui->frameRadioButton->setLayout(layoutRadioButton);
    int numCurrentTask=num_cur_task.getNumTask();
    int numAnswerRadioButton=num_cur_task.ctest[numCurrentTask].saveTotalAnswer;
   for(int i=0;i<numAnswerRadioButton;++i){
       radioButtons[i] = new QRadioButton(QString("%1").arg(i+1));

       radioButtons[i]->setStyleSheet("QRadioButton {"
                          "   font-size: 16px;"
                          "   color: #4682B4;"
                          "   font-weight: bold;"
                          "background-color:#FFFFFF"
                          "}"
                          "QRadioButton::indicator {"
                          "   width: 30px;"
                          "   height: 30px;"
                          "}");
       ui->frameRadioButton->layout()->addWidget(radioButtons[i]);

       connect(radioButtons[i], &QRadioButton::clicked, this, &MainWindow::radioButtons_clicked);

       if(i==num_cur_task.saveAnswerRadioButton[numCurrentTask]-1){
            radioButtons[i]->setChecked(true);
        }
   }
}

void MainWindow::uploadingBrowser(){
    int numCurrentTask=num_cur_task.getNumTask();
    QString html ="<b>Вопрос "+QString::number(numCurrentTask+1)
                  +". </b>"+num_cur_task.ctest[numCurrentTask].saveQuest;
    ui->textBrowserQwest->setHtml(html);

    html="";
    for (int j = 0; j <num_cur_task.ctest[numCurrentTask].saveTotalAnswer; ++j) {

        QString wordAnswer="<b>Ответ "+QString::number(j+1)+". ";
        QString newAnswer= num_cur_task.ctest[numCurrentTask].saveAnswer[j];
        newAnswer.replace("<b>",wordAnswer);
        QString   htmlRight=newAnswer;
        QString  htmlAnswerClear="";
        QString answerInfoStart ="<b>";
        int index = htmlRight.indexOf(answerInfoStart);

        if (index != -1) {
            htmlAnswerClear=htmlRight.remove(0, index);
        }else{
            QMessageBox::critical(this,
                "Critical", "<b>Не найден тег /<b/> в ответе в процедуре uploadingBrowser</b>");
            QCoreApplication::quit();
        }
        html=html+htmlAnswerClear;
    }
    ui->textBrowserAnser->setHtml(html);
}

void MainWindow::radioButtons_clicked(){
    QRadioButton *clickedRadioButton = qobject_cast<QRadioButton*>(sender());
    if (clickedRadioButton) {
        QString buttonText = clickedRadioButton->text();
        QString paleColor = "color: black;"
                            "border-color: red;"
                            "background-color: #DDDDDD;"
                            "font-weight: bold;"
                            "padding: 2px 2px;"
                            "text-align: center;"
                            "text-decoration: none;"
                            "font-size: 16px;";
       int num_current_task=num_cur_task.getNumTask();
       pushButton[num_current_task]->setStyleSheet(paleColor);
       num_cur_task.setNumTask(num_current_task);
       if(!num_cur_task.saveTaskPushButton[num_current_task]){
          ready_madeTasks++;
          int totalQuestion=num_cur_task.getTotalQwest();
          ui->labelTaskNotComleted->setText("Не выполнено заданий: "+QString::number(totalQuestion-ready_madeTasks));

          if(ready_madeTasks==totalQuestion){
            ui->pushButtonEndTest->setVisible(true);
          }
       }
       num_cur_task.saveTaskPushButton[num_current_task]=true;
       num_cur_task.saveAnswerRadioButton[num_current_task]=buttonText.toInt();
    }

    testGrade();
}
void MainWindow::testGrade(){
   int numCurrentTask=num_cur_task.getNumTask();
   int selectedRadioButton=num_cur_task.saveAnswerRadioButton[numCurrentTask]-1;
    QString rightAnswer=num_cur_task.ctest[numCurrentTask].saveAnswer[selectedRadioButton].left(1);
    if(rightAnswer=="+"){
      tGrade++;
    }
}

 void MainWindow::copytask(int totalQuestions){
    randomNumberQwest(totalQuestions);
    currTest randomTest[MAX_TASK];
    for (int i = 0; i <totalQuestions; ++i) {
        int n=shufflingQuest[i];
        randomTest[i].saveQuest      = num_cur_task.ctest[n].saveQuest;
        randomTest[i].saveTotalAnswer= num_cur_task.ctest[n].saveTotalAnswer;
        randomNumberAnswer(randomTest[i].saveTotalAnswer);
        for (int j = 0; j <randomTest[i].saveTotalAnswer; ++j) {
            int m=shufflingAnswer[j];
            randomTest[i].saveAnswer[j]=num_cur_task.ctest[n].saveAnswer[m];
        }
    }
     for (int i = 0; i <totalQuestions; ++i) {
        num_cur_task.ctest[i].saveQuest=randomTest[i].saveQuest;
        num_cur_task.ctest[i].saveTotalAnswer=randomTest[i].saveTotalAnswer;

        for (int j = 0; j <num_cur_task.ctest[i].saveTotalAnswer; ++j) {
            num_cur_task.ctest[i].saveAnswer[j]=randomTest[i].saveAnswer[j];
        }
    }
 }

 void MainWindow:: randomNumberQwest(int totalQuest){
    QVector<int> numbers;
    for (int i = 0; i < totalQuest; i++) {
        numbers.append(i);
    }
    for (int i = 0; i < totalQuest; i++) {
        int randomIndex = QRandomGenerator::global()->bounded(numbers.size());
        shufflingQuest[i] = numbers[randomIndex];
        numbers.remove(randomIndex);
    }
}

 void  MainWindow::randomNumberAnswer(int totalAnswer){
    QVector<int> numbers;
    for (int i = 0; i < totalAnswer; i++) {
        numbers.append(i);
    }
    for (int i = 0; i < totalAnswer; i++) {
        int randomIndex = QRandomGenerator::global()->bounded(numbers.size());
        shufflingAnswer[i] = numbers[randomIndex];
        numbers.remove(randomIndex);
    }
 }

 void MainWindow::pushButtonEndTest_clicked(){

    disconnect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
    int totalQuestion=num_cur_task.getTotalQwest();
    double tQuestion=static_cast<double>(totalQuestion);
    double totalGrade=static_cast<double>(tGrade);

    QString sGrade=calckgrade->calculategrade(totalGrade,tQuestion);

    if(sGrade.length()>1){// errors /0 or totalGrade>tQuestion

        QMessageBox::critical(nullptr, "Ошибка", "Ошибка при  расчете оценки. "+sGrade);
        return;
    }

    QString condition=" Where figure="+sGrade;
    QString tableName = "grade";
    QString fieldName ="*";
    QByteArray imageData=dbFacade.getGradeDatabaseQPSQL(fieldName,tableName,condition);
    if (!imageData.isEmpty()) {
        loadImageData(imageData);
    } else {
        qDebug() << "Image data is empty.";
    }
//================================================================
    ui->menubar->show();
    for (int i = 0; i < MAX_TASK; i++)  {
        num_cur_task.saveTaskPushButton[i]=false;
        num_cur_task.saveAnswerRadioButton[i]=0;
    }
    ready_madeTasks=0;
    delFileDir(state.pathGraph);
 }
 void MainWindow::loadImageData(QByteArray imageData){

     restoreTabsAndWidgets();
     hideTab(customTab);
     hideButtonTest();

     ui->graphicsViewGrade->show();
     QGraphicsView* graphicsView = ui->graphicsViewGrade;
     graphicsView->setFrameShape(QFrame::NoFrame);

     QLayout *layout = customTab->layout();

     if (!layout) {
         QHBoxLayout* layout = new QHBoxLayout(customTab);
         layout->addWidget(graphicsView, 0, Qt::AlignCenter);
         customTab->setLayout(layout);
     }else{
         QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(layout);
         if (hLayout) {
             hLayout->addWidget(graphicsView, 0, Qt::AlignCenter );
             customTab->setLayout(layout);
         }
     }
     QImage image;
     if (image.loadFromData(imageData)) {
         QGraphicsScene* scene = new QGraphicsScene;
         graphicsView->setScene(scene);
         scene->addPixmap(QPixmap::fromImage(image));
         ui->graphicsViewGrade->setFixedSize(image.width(), image.height());
        // graphicsView->setFrameShape(QFrame::NoFrame);
         graphicsView->setStyleSheet("QGraphicsView { border: none; }");// показывает скроллинг
         graphicsView->update();
         graphicsView->show();

     } else {
         qDebug() << "Failed to load image from data.";
     }
 }
void MainWindow:: testBreak_clicked(){
    ui->framTasksButton->    hide();
    ui->pushButtonEndTest->  hide();
    ui->pushButtonPrevious-> hide();
    ui->pushButtonNext->     hide();
    ui->pushButtonTestBreak->hide();

    ui->menubar->show();
    for (int i = 0; i < MAX_TASK; i++)  {
        num_cur_task.saveTaskPushButton[i]=false;
        num_cur_task.saveAnswerRadioButton[i]=0;
    }
    delFileDir(state.pathGraph);

    restoreTabsAndWidgets();
    hideTab(customTabAnime);
    hideButtonTest();

    Label[1]->setText("Время теста: "+QString::number(state.testExecutionTime));
    disconnect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    delFileDir(state.pathGraph);
    if(ui->lineEditGraph->text()!=""){
        state.pathGraph=ui->lineEditGraph->text();//lineEdiPathHelp

    }else{
        qDebug()<<"Не указан путь к графике справки"<<ui->lineEditGraph->text();
    }

    QPoint pos = this->frameGeometry().topLeft();
    QSize size = this->geometry().size();
    state.SaveState(pos, size);

    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Подтверждение выхода",
                                        "Вы действительно хотите выйти?",
                                        QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::No);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
  //  dbFacade.closeDatabase();

}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow:: loadBrowser(QTextBrowser* textBrowser,
                              QString fieldNameHtm,
                              QString tableNameHtm,
                              QString fieldNameFileGraph,
                              QString fieldNameImageData,
                              QString tableNameGraph,
                              QString condition ){
    QStringList stringListFileName =dbFacade.getFileNameQPSQL(
                                            fieldNameFileGraph,
                                            tableNameGraph,
                                            condition);
    QList<QByteArray> stringListImageData =dbFacade.getFileImageDataQPSQL(
                                            fieldNameImageData,
                                            tableNameGraph,
                                            condition);
    QString dirName=state.pathGraph;//+"/";

    saveFiles(stringListFileName,
              stringListImageData,
              dirName) ;
    condition="";
    QString htmlValue=dbFacade.getValueDatabaseQPSQL(fieldNameHtm,
                                                     tableNameHtm,
                                                     condition);

    QString fullHtml=num_cur_task.insertPath(htmlValue, state.pathGraph);

    readStyleSheet(fullHtml);
    textBrowser->clear();
    textBrowser->setStyleSheet(styleSheet);
    textBrowser->setHtml(fullHtml);

}
void MainWindow:: saveFiles(const QStringList& stringListFileName,
                            const QList<QByteArray>& stringListImageData,
                            const QString& dirName) {
    if (stringListFileName.size() != stringListImageData.size()) {
        qDebug() << "Error: The number of file names does not match the number of image data entries.";
        return;
    }
    QDir dir(dirName);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Error: Failed to create directory:" << dirName;
            return;
        }
    }
    for (int i = 0; i < stringListFileName.size(); ++i) {
        QString fileName = stringListFileName[i];
        QByteArray imageData = stringListImageData[i];

        QString fullFilePath = dir.filePath(fileName);
        QFile file(fullFilePath);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Error: Failed to open file for writing:" << fullFilePath;
            continue;
        }

        qint64 bytesWritten = file.write(imageData);
        if (bytesWritten == -1) {
            qDebug() << "Error: Failed to write data to file:" << fullFilePath;
        } else {
            //qDebug() << "File saved successfully:" << fullFilePath;
        }

        file.close();
    }
}
void MainWindow::delFileDir(QString path){
    QDir dir(path);
    if (dir.exists()) {
        QStringList files = dir.entryList(QDir::Files);

        foreach (const QString &file, files) {
            if (dir.remove(file)) {
                //qDebug() << "Удален файл:" << file;
            } else {
                qDebug() << "Не удалось удалить файл:" << file;
            }
        }
    } else {
        qDebug() << "Папка не найдена:" << path;
    }
}

void MainWindow::readStyleSheet(QString html){
    QString startSequence = "<!--<";
    QString endSequence = "-->";
    QString allBackgraund=readFileNameBackgraund( html,startSequence,endSequence);

    if(allBackgraund.isEmpty())
      return;

    startSequence = "background=\"";
    endSequence = "\" ";
    QString filenameBackgraund=readFileNameBackgraund( allBackgraund,startSequence,endSequence);
    if(filenameBackgraund.isEmpty())
      return;
    QString pathDirGraph = state.pathGraph;

    styleSheet ="background-image:url(\""
                 +pathDirGraph+"/"
                 +filenameBackgraund +"\")";
}
    QString MainWindow::readFileNameBackgraund(QString html, QString startSequence,QString endSequence){
    QString found="не найдено регулярное выражение для последовательности!"+startSequence+"(.*?)"+ endSequence;
    QRegularExpression regex(startSequence+"(.*?)"+endSequence);
    QRegularExpressionMatch match = regex.match(html);
    if (match.hasMatch()) {
       found = match.captured(1);
    }else {
       qDebug() <<found;
    }
    return found;
}

void MainWindow::uploadingGraphFiles2(QString fieldNameFileGraph,
                                      QString fieldNameImageData,
                                      QString tableNameGraph,
                                      QString condition ){
    QStringList stringListFileName =dbFacade.getFileNameQPSQL(
        fieldNameFileGraph,
        tableNameGraph,
        condition);
    QList<QByteArray> stringListImageData =dbFacade.getFileImageDataQPSQL(
        fieldNameImageData,
        tableNameGraph,
        condition);
    QString dirName=state.pathGraph;//+"/";

    saveFiles(stringListFileName,
              stringListImageData,
              dirName) ;
}

void MainWindow::setServiceStatus(const QString &status) {
    Label[1]->setText(status);
}

void MainWindow::showProgressBar(float  testExecutionTime){
    totalTime = static_cast<int>(testExecutionTime * 60);
    ui->progressBar->setRange(0, totalTime);
    ui->progressBar->setValue(0);
    ui->progressBar->setTextVisible(true);
    ui->progressBar->setStyleSheet("QProgressBar {"
                                   "background-color: #f0f0f0;"
                                   "border: 2px solid #8f8f8f;"
                                   "border-radius: 5px;"
                                   "text-align: center;"
                                   "}"
                                   "QProgressBar::chunk {"
                                   "background-color: #228B22;"
                                   "border-radius: 5px;"
                                   "}");
    QFont font = ui->progressBar->font();
    font.setBold(true);
    font.setPointSize(10);
    ui->progressBar->setFont(font);
    ui->progressBar->show();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
    timer->start(1000);
}

void MainWindow::updateProgress() {
    if (currentTime < totalTime ) {
        currentTime++;
        ui->progressBar->setValue(currentTime);
        int remainingTime = (totalTime ) - currentTime;
        QTime time(0, 0);
        time = time.addSecs(remainingTime);
        Label[1]->setText("Текущее время теста: "+time.toString("mm:ss"));
    } else {
        timer->stop();
        Label[1]->setText("Тест завершен!");
        currentTime=0;
        disconnect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
        pushButtonEndTest_clicked();
    }
}

