#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,dbFacade(new DBFacade())
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
    connect(testing,       &QAction::triggered, this, &MainWindow::onTestingClicked);
    connect(trainingManual,&QAction::triggered, this, &MainWindow::onTrainingManualButtonClicked);
    connect(setting,       &QAction::triggered, this, &MainWindow::onSettingClicked);
    connect(help,          &QAction::triggered, this, &MainWindow::onHelpClicked);

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
    connect(ui->pushButtonNext, &QPushButton::clicked, this, &MainWindow::onPushButtonNextClicked);
    connect(ui->pushButtonEndTest, &QPushButton::clicked, this, &MainWindow::onPushButtonEndTestClicked);
    connect(ui->pushButtonPrevious, &QPushButton::clicked, this, &MainWindow::onPushButtonPreviousClicked);
    connect(ui->pushButtonTestBreak, &QPushButton::clicked, this, &MainWindow::onTestBreakClicked);
}
void MainWindow::setupStatusBar() {
    if (!dbFacade->openDatabaseQPSQL("localhost", "iSmile", "postgres",
                                    "800900", 5432)) {
        QString msg = "Невозможно открыть БД.";
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось запустить приложение. " + msg);
        Label[1]->setText(msg);
        QCoreApplication::exit(1);
    }
    setupStatusBarLabels();

    QString condition = " Where n="+QString::number(selectedIndex);
    QString tableName = "tasks";
    QString fieldName = "task";

    QString htmlValue = dbFacade->getValueFromDB<QString>(fieldName,
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
    showOnlyThisTab(customTabAnime);
    hideAllTestControls();
}

void MainWindow::getListThemes(){
    QString tableName = "tasks";
    QString fieldName = "task";
    QString condition="";
    QStringList stringList;
    stringList=dbFacade->getRecordsDatabaseQPSQL(fieldName,
                                                tableName,
                                                condition);

    onChooseThemeClicked(stringList);
}

void MainWindow::onChooseThemeClicked(const QStringList& stringList) {
    QMenu themeMenu(tr("Выбор темы"), this);

    for (int i = 0; i < stringList.size(); ++i) {
        QAction* const themeAction = themeMenu.addAction(stringList[i]);

        connect(themeAction, &QAction::triggered, this,
                [this, theme = stringList[i], i]() {
                    changeTheme(theme, i);  // индексация с 0
                });
    }

    themeMenu.exec(QCursor::pos());
    restoreTabsAndWidgets();
    showOnlyThisTab(customTabAnime);
}

void MainWindow::changeTheme(const QString& theme, int index) {
    // Проверка валидности указателей
    if (!Label[0] || !Label[1]) {
        qDebug() << "changeTheme: Label[0] or Label[1] is nullptr";
        return;
    }

    // Обновляем индекс первым (более критичное изменение)
    selectedIndex = index;

    // Обновляем UI
    Label[0]->setText(theme);

    // Форматируем строку времени
    const QString timeString = (state.testExecutionTime > 0)
                                   ? "Время теста: " + QString::number(state.testExecutionTime)
                                   : "Время теста: не задано";
    Label[1]->setText(timeString);
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
    validateUi();  // Фатальная ошибка если ui == nullptr

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

 // Основная функция
 void MainWindow::showOnlyThisTab(QWidget* widget) {
     validateUi();

     if (!widget) {
         qDebug() << "showOnlyThisTab: widget is nullptr";
         return;
     }

     removeAllTabsExcept(widget);
     hideTabBar();
     hideGradeView();
 }

 // 1.Удаление всех вкладок кроме указанной
 void MainWindow::removeAllTabsExcept(QWidget* widgetToKeep) {
     if (!ui->tabWidget) {
         qDebug() << "removeAllTabsExcept: tabWidget is nullptr";
         return;
     }

     // даляем в обратном порядке
     for (int i = ui->tabWidget->count() - 1; i >= 0; --i) {
         QWidget* tab = ui->tabWidget->widget(i);
         if (tab != widgetToKeep) {
             ui->tabWidget->removeTab(i);
             tab->deleteLater();  //Безопасное удаление
         }
     }
 }

 // 2.Скрытие панели вкладок
 void MainWindow::hideTabBar() {
     if (!ui->tabWidget || !ui->tabWidget->tabBar()) {
         qDebug() << "hideTabBar: tabWidget or tabBar is nullptr";
         return;
     }

     ui->tabWidget->tabBar()->setStyleSheet("QTabBar::tab { height: 0px; }");
 }

 // 3.Скрытие graphicsView
 void MainWindow::hideGradeView() {
     if (ui->graphicsViewGrade) {
         ui->graphicsViewGrade->hide();
     } else {
         qDebug() << "hideGradeView: graphicsViewGrade is nullptr";
     }
 }



// Основная фасадная функция
void MainWindow::hideAllTestControls() {
    validateUi();  // Проверка ui (из вашего кода)

    hideTestButtons();
    hideTestContainers();
    hideTestProgressIndicator();
    resetTestTimer();
}

// 1.Скрытие всех кнопок теста
void MainWindow::hideTestButtons() {
    const QList<QPushButton*> buttonsToHide = {
        ui->pushButtonEndTest,
        ui->pushButtonPrevious,
        ui->pushButtonNext,
        ui->pushButtonTestBreak
    };

    for (const auto& button : buttonsToHide) {
        if (button) button->hide();
    }
}

// 2.Скрытие контейнеров
void MainWindow::hideTestContainers() {
    if (ui->framTasksButton) {
        ui->framTasksButton->hide();
    } else {
        qDebug() << "hideTestContainers: framTasksButton is nullptr";
    }
}

// 3.Скрытие индикатора прогресса
void MainWindow::hideTestProgressIndicator() {
    if (ui->progressBar) {
        ui->progressBar->hide();
    }

    if (ui->labelTaskNotComleted) {
        ui->labelTaskNotComleted->hide();
    }
}

// 4.Сброс таймера
void MainWindow::resetTestTimer() {
    currentTime = 0;
    // Если есть таймер, его тоже можно остановить здесь
    if (timer) {
        timer->stop();
    }
}

void MainWindow::onSettingClicked() {
    //Основная функция
    validateUi();  //Фатальная ошибка если ui == nullptr
    setupSettingTabUI();
    configureFontSettings();
    setupStretchRadioButtons();
    setupTabBackground();
    ensureCentralWidget();
}
// 1. Настройка базового UI
void MainWindow::setupSettingTabUI() {
    validateUi();  // Фатальная ошибка если ui == nullptr

    restoreTabsAndWidgets();
    showOnlyThisTab(ui->tabSetting);
    hideAllTestControls();
    ui->tabWidget->show();
}

// 2.Настройка шрифтов
void MainWindow::configureFontSettings() {

    validateUi();  // Фатальная ошибка если ui == nullptr
    if (!ui->groupBoxStretch) {
        qDebug() << "configureFontSettings: groupBoxStretch is nullptr";
        return;
    }

    QFont font = ui->groupBoxStretch->font();
    font.setPointSize(UiConstants ::SETTINGS_FONT_SIZE);
    ui->groupBoxStretch->setFont(font);
    ui->groupBoxStretch->setAlignment(Qt::AlignTop);

    // Настройка размера радиокнопок
    if (ui->radioButtonStretch) ui->radioButtonStretch->adjustSize();
    if (ui->radioButtonNoStretch) ui->radioButtonNoStretch->adjustSize();
}

// 3.Настройка соединений
void MainWindow::setupStretchRadioButtons() {

    validateUi();  // Фатальная ошибка если ui == nullptr

    if (!ui->radioButtonStretch || !ui->radioButtonNoStretch) {
        qDebug() << "setupStretchRadioButtons: radio buttons not initialized";
        return;
    }

    // Отключаем старые соединения, чтобы избежать дублирования
    disconnect(ui->radioButtonStretch, &QRadioButton::clicked,
               this, &MainWindow::radioButtonsStretch_clicked);
    disconnect(ui->radioButtonNoStretch, &QRadioButton::clicked,
               this, &MainWindow::radioButtonsStretch_clicked);

    // Устанавливаем новые соединения
    connect(ui->radioButtonStretch, &QRadioButton::clicked,
            this, &MainWindow::radioButtonsStretch_clicked);
    connect(ui->radioButtonNoStretch, &QRadioButton::clicked,
            this, &MainWindow::radioButtonsStretch_clicked);
}

// 4. Настройка фона
void MainWindow::setupTabBackground() {
    validateUi();  // Фатальная ошибка если ui == nullptr
    if (!ui->tabSetting) {
        qDebug() << "setupTabBackground: tabSetting is nullptr";
        return;
    }

    const QPixmap background(UiConstants ::SETTINGS_BACKGROUND_PATH);

    if (background.isNull()) {
        qDebug() << "setupTabBackground Error: failed to load image" << UiConstants ::SETTINGS_BACKGROUND_PATH;
        return;
    }

    QPalette palette = ui->tabSetting->palette();
    palette.setBrush(QPalette::Window, background);
    ui->tabSetting->setPalette(palette);
    ui->tabSetting->setAutoFillBackground(true);
}

// 5. Установка центрального виджета
void MainWindow::ensureCentralWidget() {
    validateUi();  // Фатальная ошибка если ui == nullptr

    if (!ui || !ui->centralwidget) {
        qDebug() << "ensureCentralWidget: ui or centralwidget is nullptr";
        return;
    }

    if (centralWidget() != ui->centralwidget) {
        setCentralWidget(ui->centralwidget);
    }
}
//=======================================================
void MainWindow::radioButtonsStretch_clicked() {

    if (ui->radioButtonNoStretch->isChecked()) {
        this->setFixedSize(800, 800);
    } else if (ui->radioButtonStretch->isChecked()) {
        this->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    }
}


void MainWindow::onHelpClicked() {
    // Основная функция
    // if (!ui) {
    //     qDebug() << "onHelpClicked: ui is nullptr";
    //     return;
    // }
    validateUi();

    ensureCorrectCentralWidget();
    setupHelpTabUI();
    loadHelpContent();
}

// 1.Обеспечение правильного центрального виджета
void MainWindow::ensureCorrectCentralWidget() {
    if (!ui || !ui->centralwidget) {
        qDebug() << "ensureCorrectCentralWidget: ui or centralwidget is nullptr";
        return;
    }

    if (centralWidget() != ui->centralwidget) {
        setCentralWidget(ui->centralwidget);
    }
}

// 2.Настройка UI для вкладки Help
void MainWindow::setupHelpTabUI() {
    // if (!ui) {
    //     qDebug() << "setupHelpTabUI: ui is nullptr";
    //     return;
    // }
    validateUi();

    restoreTabsAndWidgets();

    if (ui->tabHelp) {
        showOnlyThisTab(ui->tabHelp);
    } else {
        qDebug() << "setupHelpTabUI: tabHelp is nullptr";
    }

    hideAllTestControls();
}

// 3. Загрузка содержимого Help
void MainWindow::loadHelpContent() {
    if (!ui || !ui->textBrowserHelp) {
        qDebug() << "loadHelpContent: ui or textBrowserHelp is nullptr";
        return;
    }

    const QString fieldNameHtm = HelpConfig::HTML_FIELD;
    const QString tableNameHtm = HelpConfig::TABLE_NAME;
    const QString fieldNameFileGraph = HelpConfig::GRAPH_FIELD_FILENAME;
    const QString fieldNameImageData = HelpConfig::GRAPH_FIELD_DATA;
    const QString tableNameGraph = HelpConfig::GRAPH_TABLE_NAME;
    const QString condition;  // Пустая строка по умолчанию

    loadBrowser(ui->textBrowserHelp,
                fieldNameHtm,
                tableNameHtm,
                fieldNameFileGraph,
                fieldNameImageData,
                tableNameGraph,
                condition);
}
//====================================================

void MainWindow::onTestingClicked() {
    tGrade = 0;

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

    showTestControls();
    showOnlyThisTab(ui->tabTest);

    ui->tabWidget->show();

    const QString condition = " Where n=" + QString::number(selectedIndex);
    const QString tableName = "tasks";
    const QString fieldName = "task";

    QString htmlValue = dbFacade->getValueFromDB<QString>(fieldName,
                                                          tableName,
                                                          condition);
    htmlValue = htmlValue.simplified();
    readStyleSheet(htmlValue);

    const QString fieldNameHtm = "task";
    const QString tableNameHtm = "tasks";
    const QString fieldNameFileGraph = "filename";
    const QString fieldNameImageData = "graph";
    const QString tableNameGraph = "graphtest";
    const QString conditionGraph = "Where idtasks=" + QString::number(selectedIndex);

    uploadingGraphFiles2(fieldNameFileGraph,
                         fieldNameImageData,
                         tableNameGraph,
                         conditionGraph);

    num_cur_task.getTasks(htmlValue, state.pathGraph);

    const int totalQuestion = num_cur_task.getTotalQwest();
    ui->labelTaskNotComleted->setText("Не выполнено заданий: "
                                      + QString::number(totalQuestion - ready_madeTasks));
    setupTestInterface();
}


// Основная фасадная функция
void MainWindow::showTestControls() {
    validateUi();

    showTestButtons();
    showTestContainer();
    showTestLabel();
}

// 1.Показ всех кнопок теста
void MainWindow::showTestButtons() {
    const QList<QPushButton*> buttons = {
        ui->pushButtonEndTest,
        ui->pushButtonPrevious,
        ui->pushButtonNext,
        ui->pushButtonTestBreak
    };

    for (const auto& button : buttons) {
        if (button) button->show();
    }
}

// 2.Показ контейнера
void MainWindow::showTestContainer() {
    if (ui->framTasksButton) {
        ui->framTasksButton->show();
    } else {
        qDebug() << "showTestContainer: framTasksButton is nullptr";
    }
}

// 3. Показ метки
void MainWindow::showTestLabel() {
    if (ui->labelTaskNotComleted) {
        ui->labelTaskNotComleted->show();
    } else {
        qDebug() << "showTestLabel: labelTaskNotComleted is nullptr";
    }
}

// 4.Универсальная функция показа виджетов
void MainWindow::showWidgets(const QList<QWidget*>& widgets) {
    for (const auto& widget : widgets) {
        if (widget) widget->show();
    }
}
//=================================================

void MainWindow::onTrainingManualButtonClicked(){

    ensureCorrectCentralWidget();
    restoreTabsAndWidgets();

    showOnlyThisTab(ui->tabTranManual);
    hideAllTestControls();

     ui->tabWidget->show();

    if (selectedIndex < 0) {
        qWarning() << "Invalid selected index:" << selectedIndex;
        return;
    }
    const QString condition="Where idtasks="+QString::number(selectedIndex);

    loadBrowser(ui->textBrowserMet,
                ManualConstants::TEXT_FIELD,
                ManualConstants::TABLE_NAME,
                ManualConstants::GRAPH_FIELD,
                ManualConstants::IMAGE_FIELD,
                ManualConstants::GRAPH_TABLE,
                condition);
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
        connect(radioButton, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
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
        connect(pushButton[i], &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);

        layout->addWidget(pushButton[i], i / colCount, i % colCount);

    }
}

//=================================================
// Главная функция - только координация
void MainWindow::setupTestInterface() {
    configureInitialVisibility();
    configureTabAppearance();
    int totalQuestions=prepareTestData();
    if (totalQuestions <= 0) {
        qWarning() << "No questions available!";
        return ;
    }
    applyStylesToBrowsers();

    uploadingBrowser();

    int maxRadioBut=num_cur_task.ctest[0].saveTotalAnswer;
    setupRadioButtons(maxRadioBut);
    const int rowCount = (totalQuestions < 20) ? 1 : 2;
    const int colCount = (totalQuestions % rowCount == 0) ? (totalQuestions / rowCount) : (totalQuestions / rowCount + 1);
    setupPushButtons(totalQuestions,colCount);
}
// Настройка видимости элементов
void MainWindow::configureInitialVisibility() {
    ui->pushButtonEndTest->setVisible(false);
    ui->pushButtonPrevious->setVisible(true);
    ui->pushButtonNext->setVisible(true);
}

// Настройка внешнего вида табов
void MainWindow::configureTabAppearance() {
    ui->tabWidget->setTabShape(QTabWidget::Triangular);
}

int MainWindow::prepareTestData() {

    int totalQuestions=num_cur_task.getTotalQwest();
    if (totalQuestions <= 0) {
      //  qWarning() << "No questions available!";
        return 0;
    }
    return totalQuestions;

    shuffleAndCopyTasks(totalQuestions);
}
// рименение стилей к браузерам
void MainWindow::applyStylesToBrowsers() {

    ui->textBrowserAnser->setStyleSheet(styleSheet);
    ui->textBrowserQwest->setStyleSheet(styleSheet);
}
//==================================================


void MainWindow::setupStatusBarLabels() {
    if (!isStatusBarValid()) return;

    cleanupStatusBarLabels();  // Удаляем старые лейблы

    configureStatusBarStyle();

    QFont labelFont = createLabelFont();

    for (int i = 0; i < MAX_LABEL; ++i) {
        QLabel* label = createStatusLabel(i, labelFont);
        addLabelToStatusBar(label);
        storeLabelPointer(i, label);
    }
}

//Проверка валидности статусбара
bool MainWindow::isStatusBarValid() const {
    if (!ui->statusbar) {
        qWarning() << "Status bar is null!";
        return false;
    }
    return true;
}


//Очистка старых лейблов (предотвращение утечек)
void MainWindow::cleanupStatusBarLabels() {
    // Удаляем существующие лейблы из statusbar
    for (int i = 0; i < MAX_LABEL; ++i) {
        if (Label[i]) {
            ui->statusbar->removeWidget(Label[i]);
            delete Label[i];
            Label[i] = nullptr;  // Важно обнулять!
        }
    }
}

//  Настройка стиля статусбара
void MainWindow::configureStatusBarStyle() {
    ui->statusbar->setStyleSheet(getStatusBarStyleSheet());
}

//  Получение стиля статусбара
QString MainWindow::getStatusBarStyleSheet() const {
    return "border: 2px solid black";
}

//  Создание шрифта для лейблов
QFont MainWindow::createLabelFont() const {
    QFont font;
    font.setPointSize(getLabelFontSize());
    font.setBold(isLabelFontBold());
    return font;
}

//  Получение размера шрифта (легко переопределить)
int MainWindow::getLabelFontSize() const {
    return 12;  // Можно вынести в константу или настройки
}

//  Получение жирности шрифта
bool MainWindow::isLabelFontBold() const {
    return true;
}

//  Создание одного лейбла
QLabel* MainWindow::createStatusLabel(int index, const QFont& font) {
    QLabel* label = new QLabel(getLabelText(index));
    label->setAlignment(getLabelAlignment());
    label->setFont(font);
    label->setStyleSheet(getLabelStyleSheet());
    return label;
}

//  Получение текста лейбла
QString MainWindow::getLabelText(int index) const {
    return "Label" + QString::number(index);
}

//  Получение выравнивания лейбла
Qt::Alignment MainWindow::getLabelAlignment() const {
    return Qt::AlignLeft;
}

//  Получение стиля лейбла
QString MainWindow::getLabelStyleSheet() const {
    return "background-color: #FF8C00; color: #2E2E2E;";
}

//  Добавление лейбла в статусбар
void MainWindow::addLabelToStatusBar(QLabel* label) {
    if (label && ui->statusbar) {
        ui->statusbar->addWidget(label);
    }
}

//  Сохранение указателя на лейбл
void MainWindow::storeLabelPointer(int index, QLabel* label) {
    if (index >= 0 && index < MAX_LABEL) {  // Проверка границ!
        Label[index] = label;
    } else {
        qWarning() << "Label index out of bounds:" << index;
        delete label;  // Предотвращаем утечку
    }
}



//============================================================
void MainWindow::onPushButtonClicked() {
    // 1. Проверяем отправителя
    const QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) {
        qDebug() << "onPushButtonClicked: invalid sender()";
        return;
    }

    //2. Проверяем текст кнопки
    const QString buttonText = clickedButton->text();
    bool conversionOk = false;
    const int buttonNumber = buttonText.toInt(&conversionOk);

    if (!conversionOk || buttonNumber <= 0) {
        qDebug() << "onPushButtonClicked: invalid button text:" << buttonText;
        return;
    }

    //3. Получаем текущее состояние
    const int numPreviousTask = num_cur_task.getNumTask();
    int numCurrentTask = buttonNumber - 1;  // Конвертируем в 0-based индекс

    //4. Проверяем, что это действительно новое задание
    if (numCurrentTask == numPreviousTask) {
        return;  // Та же задача, ничего не делаем
    }

    //5. Проверяем границы (если num_cur_task знает максимальное количество)
    if (numCurrentTask < 0 || numCurrentTask >= num_cur_task.getTotalQwest()) {
        qDebug() << "onPushButtonClicked: task index out of range:" << numCurrentTask;
        return;
    }

    //6. Обновляем состояние
    num_cur_task.setNumTask(numCurrentTask);

    //7. Обновляем UI
    showPushButtonTask(numCurrentTask, numPreviousTask);
    uploadingBrowser();
    setupRadioButtonsForCurrentTask();
}

void MainWindow::onPushButtonPreviousClicked() {
    // Получаем текущую задачу
    const int currentTask = num_cur_task.getNumTask();

    // Проверка на первую задачу
    if (currentTask <= 0) {
        qDebug() << "onPushButtonPreviousClicked: already at first task, currentTask =" << currentTask;

        // Опционально: визуальная обратная связь для пользователя
        QMessageBox::information(this, "Информация", "Это первое задание");
        return;
    }

    // Вычисляем предыдущую задачу
        int previousTask = currentTask - 1;

    // Дополнительная проверка валидности
    const int totalTasks = num_cur_task.getTotalQwest();
    if (previousTask >= totalTasks) {
        qDebug() << "onPushButtonPreviousClicked: invalid task index" << previousTask;
        return;
    }

    // Обновляем состояние
    num_cur_task.setNumTask(previousTask);

    // Обновляем UI
    showPushButtonTask(previousTask, currentTask);
    uploadingBrowser();
    setupRadioButtonsForCurrentTask();
}

void MainWindow::onPushButtonNextClicked() {
    // Получаем текущий индекс
    const int currentIndex = num_cur_task.getNumTask();
    const int totalTasks = num_cur_task.getTotalQwest();

    // Проверка, что не на последнем задании
    if (currentIndex >= totalTasks - 1) {
        qDebug() << "onPushButtonNextClicked: already at last task";

        // Опционально: отключаем кнопку, если она есть
        if (ui->pushButtonNext) {
            ui->pushButtonNext->setEnabled(false);
        }
        return;
    }

    // Вычисляем новый индекс
    //const int newIndex = currentIndex + 1;
    int newIndex = currentIndex + 1;

    // Обновляем состояние
    num_cur_task.setNumTask(newIndex);

    // Обновляем интерфейс
    showPushButtonTask(newIndex, currentIndex);
    uploadingBrowser();
    setupRadioButtonsForCurrentTask();

    // Включаем кнопку "Назад", если она была отключена
    if (ui->pushButtonPrevious && !ui->pushButtonPrevious->isEnabled()) {
        ui->pushButtonPrevious->setEnabled(true);
    }
}

void MainWindow::showPushButtonTask(const int num_current_task, const int num_previous_task) {
    // Получение общего количества задач
    const int totalTasks = num_cur_task.getTotalQwest();

    // Проверка границ
    if (num_previous_task < 0 || num_previous_task >= totalTasks) {
        qDebug() << "showPushButtonTask: invalid previous_task index =" << num_previous_task;
        return;
    }

    if (num_current_task < 0 || num_current_task >= totalTasks) {
        qDebug() << "showPushButtonTask: invalid current_task index =" << num_current_task;
        return;
    }

    // Проверка существования кнопок
    if (!pushButton[num_previous_task]) {
        qDebug() << "showPushButtonTask: pushButton[" << num_previous_task << "] is nullptr";
        return;
    }

    if (!pushButton[num_current_task]) {
        qDebug() << "showPushButtonTask: pushButton[" << num_current_task << "] is nullptr";
        return;
    }

    // Константные стили
    const QString completedStyle = "QPushButton {"
                                   "color: black;"
                                   "border-color: black;"
                                   "background-color: #DDDDDD;"
                                   "font-weight: bold;"
                                   "padding: 2px 2px;"
                                   "text-align: center;"
                                   "text-decoration: none;"
                                   "font-size: 16px;"
                                   "}";

    const QString incompleteStyle = "QPushButton {"
                                    "color: black;"
                                    "border-color: black;"
                                    "background-color: #4CAF50;"
                                    "font-weight: bold;"
                                    "padding: 2px 2px;"
                                    "text-align: center;"
                                    "text-decoration: none;"
                                    "font-size: 16px;"
                                    "}";

    const QString currentTaskStyle = "QPushButton {"
                                     "color: white;"
                                     "border-color: red;"
                                     "background-color: #4CAF50;"
                                     "font-weight: bold;"
                                     "padding: 2px 2px;"
                                     "text-align: center;"
                                     "text-decoration: none;"
                                     "font-size: 16px;"
                                     "}";

    // Обновление предыдущей кнопки
    const bool isPreviousCompleted = num_cur_task.saveTaskPushButton[num_previous_task];
    pushButton[num_previous_task]->setStyleSheet(isPreviousCompleted ? completedStyle : incompleteStyle);
    pushButton[num_previous_task]->show();

    // Обновление текущей кнопки
    const bool isCurrentCompleted = num_cur_task.saveTaskPushButton[num_current_task];
    if (isCurrentCompleted) {
        pushButton[num_current_task]->setStyleSheet(completedStyle);
    } else {
        pushButton[num_current_task]->setStyleSheet(currentTaskStyle);
    }
    pushButton[num_current_task]->show();
}


void MainWindow::setupRadioButtonsForCurrentTask() {
    cleanupRadioButtons();
    createRadioButtonLayout();

    int numAnswers = getCurrentTaskAnswerCount();
    if (numAnswers <= 0) return;

    createRadioButtons(numAnswers);
    restoreRadioButtonState();
}
void MainWindow::cleanupRadioButtons() {
    // Удаляем все кнопки из массива
    for (int i = 0; i < MAX_ANSWER; ++i) {
        if (radioButtons[i]) {
            delete radioButtons[i];
            radioButtons[i] = nullptr;  // Важно обнулять!
        }
    }

    // Удаляем layout
    if (ui->frameRadioButton->layout()) {
        QLayoutItem *item;
        while ((item = ui->frameRadioButton->layout()->takeAt(0))) {
            if (item->widget()) delete item->widget();
            delete item;
        }
        delete ui->frameRadioButton->layout();
    }
}
//  Создание layout
void MainWindow::createRadioButtonLayout() {
    auto *layout = new QVBoxLayout(ui->frameRadioButton);
    ui->frameRadioButton->setLayout(layout);
}

//  Получение данных (инкапсуляция)
int MainWindow::getCurrentTaskAnswerCount()  {
    int currentTask = num_cur_task.getNumTask();
        return num_cur_task.saveAnswerRadioButton[currentTask];
}

void MainWindow::createRadioButtons(int count) {
    if (count > MAX_ANSWER) {
        count = MAX_ANSWER;  // Защита от переполнения
    }

    for (int i = 0; i < count; ++i) {
        radioButtons[i] = createRadioButton(i + 1);
        ui->frameRadioButton->layout()->addWidget(radioButtons[i]);
        connect(radioButtons[i], &QRadioButton::clicked,
                this, &MainWindow::onRadioButtonClicked);
    }
}
//  Создание стилизованной кнопки
QRadioButton* MainWindow::createRadioButton(int number) const {
    auto *radioButton = new QRadioButton(QString::number(number));
    radioButton->setStyleSheet(getRadioButtonStyleSheet());
    return radioButton;
}
//  Выделение стилей (легко менять/тестировать)
QString MainWindow::getRadioButtonStyleSheet() const {
    return R"(
        QRadioButton {
            font-size: 16px;
            color: #4682B4;
            font-weight: bold;
            background-color: #FFFFFF;
        }
        QRadioButton::indicator {
            width: 30px;
            height: 30px;
        }
    )";
}
//  Восстановление состояния
void MainWindow::restoreRadioButtonState() {
    int currentTask = num_cur_task.getNumTask();
    int savedAnswer = num_cur_task.saveAnswerRadioButton[currentTask];

    if (savedAnswer > 0 && savedAnswer <= MAX_ANSWER) {
        radioButtons[savedAnswer - 1]->setChecked(true);
    }
}
//======================================
void MainWindow::uploadingBrowser() {
    // 1. Проверка валидности текущей задачи
    const int numCurrentTask = num_cur_task.getNumTask();
    const int totalTasks = num_cur_task.getTotalQwest();

    if (numCurrentTask < 0 || numCurrentTask >= totalTasks) {
        qDebug() << "uploadingBrowser: invalid task index" << numCurrentTask;
        ui->textBrowserQwest->setHtml("<b>Ошибка: задача не найдена</b>");
        ui->textBrowserAnser->setHtml("<b>Ошибка: задача не найдена</b>");
        return;
    }

    // 2. Проверка существования данных задачи
    // if (numCurrentTask >= num_cur_task.ctest.size()) {
     if (numCurrentTask >= MAX_TASK) {

        qDebug() << "uploadingBrowser: ctest index out of range" << numCurrentTask;
         return;
     }

    const auto& currentTest = num_cur_task.ctest[numCurrentTask];

    // 3. Отображение вопроса
    const QString questionHtml = "<b>Вопрос " + QString::number(numCurrentTask + 1) +
                                 ". </b>" + currentTest.saveQuest;
    ui->textBrowserQwest->setHtml(questionHtml);

    // 4. Формирование ответов
    QString answersHtml;
    const int totalAnswers = currentTest.saveTotalAnswer;

    for (int j = 0; j < totalAnswers; ++j) {
        // Проверка границ массива ответов
        //if (j >= currentTest.saveAnswer.size()) {
        if (j >= MAX_ANSWER) {
            qDebug() << "uploadingBrowser: saveAnswer index out of range" << j;
            break;
        }

        QString answer = currentTest.saveAnswer[j];
        const QString wordAnswer = "<b>Ответ " + QString::number(j + 1) + ". ";

        // Безопасная замена тега
        const QString answerFormatted = formatAnswerText(answer, wordAnswer);
        answersHtml += answerFormatted;
    }

    ui->textBrowserAnser->setHtml(answersHtml);
}

// Вспомогательная функция для форматирования ответа
QString MainWindow::formatAnswerText(const QString& originalAnswer, const QString& answerPrefix) {
    const QString answerInfoStart = "<b>";
    const int index = originalAnswer.indexOf(answerInfoStart);

    if (index == -1) {
        qDebug() << "formatAnswerText: Tag <b> not found in answer:" << originalAnswer;
        return "<b>Ответ: [Ошибка форматирования]</b>";
    }

    // Создаем копию и заменяем префикс
    QString formattedAnswer = originalAnswer;
    formattedAnswer.remove(0, index);
    formattedAnswer.replace("<b>", answerPrefix);

    return formattedAnswer;
}


void MainWindow::onRadioButtonClicked() {
    // Проверка отправителя
    const QRadioButton* clickedRadioButton = qobject_cast<QRadioButton*>(sender());
    if (!clickedRadioButton) {
        qDebug() << "radioButtons_clicked: invalid sender";
        return;
    }

    // Проверка текста кнопки
    const QString buttonText = clickedRadioButton->text();
    bool ok = false;
    const int answerValue = buttonText.toInt(&ok);
    if (!ok) {
        qDebug() << "radioButtons_clicked: invalid text format:" << buttonText;
        return;
    }

    // Проверка текущей задачи
    const int currentTask = num_cur_task.getNumTask();
    const int totalTasks = num_cur_task.getTotalQwest();
    if (currentTask < 0 || currentTask >= totalTasks) {
        qDebug() << "radioButtons_clicked: invalid task index:" << currentTask;
        return;
    }

    // Проверка указателя на кнопку
    if (!pushButton[currentTask]) {
        qDebug() << "radioButtons_clicked: pushButton is nullptr for task:" << currentTask;
        return;
    }

    // Если задача уже выполнена - игнорируем повторное нажатие
    if (num_cur_task.saveTaskPushButton[currentTask]) {
        qDebug() << "radioButtons_clicked: task already completed:" << currentTask;
        return;
    }

    // Отмечаем задачу как выполненную
    const QString completedStyle = "color: black;"
                                   "border-color: red;"
                                   "background-color: #DDDDDD;"
                                   "font-weight: bold;"
                                   "padding: 2px 2px;"
                                   "text-align: center;"
                                   "text-decoration: none;"
                                   "font-size: 16px;";
    pushButton[currentTask]->setStyleSheet(completedStyle);

    // Сохраняем ответ
    num_cur_task.saveTaskPushButton[currentTask] = true;
    num_cur_task.saveAnswerRadioButton[currentTask] = answerValue;
    ready_madeTasks++;

    // Обновляем счетчик
    ui->labelTaskNotComleted->setText("Не выполнено заданий: " +
                                      QString::number(totalTasks - ready_madeTasks));

    // Показываем кнопку завершения, если все задачи выполнены
    if (ready_madeTasks == totalTasks) {
        ui->pushButtonEndTest->setVisible(true);
    }

    // Обновляем оценку
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

 void MainWindow::shuffleAndCopyTasks(int totalQuestions){
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

 void MainWindow::onPushButtonEndTestClicked() {
     // Проверка таймера
     if (timer) {
         disconnect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
     }

     // Проверка необходимых объектов
     if (!calckgrade || !dbFacade || !ui) {
         QMessageBox::critical(this, "Ошибка", "Внутренняя ошибка приложения");
         return;
     }

     // Получение и проверка данных
     const int totalQuestion = num_cur_task.getTotalQwest();
     if (totalQuestion <= 0) {
         QMessageBox::critical(this, "Ошибка", "Некорректное количество вопросов");
         return;
     }

     const double totalQuestionDouble = static_cast<double>(totalQuestion);
     const double totalGradeDouble = static_cast<double>(tGrade);

     // Проверка диапазона оценки
     if (totalGradeDouble < 0.0 || totalGradeDouble > totalQuestionDouble) {
         QMessageBox::critical(this, "Ошибка",
                               QString("Некорректная оценка: %1 из %2").arg(tGrade).arg(totalQuestion));
         return;
     }

     // Расчет оценки
     const QString sGrade = calckgrade->calculategrade(totalGradeDouble, totalQuestionDouble);

     // Проверка результата
     if (sGrade.isEmpty() || sGrade.length() != 1 || !sGrade[0].isDigit()) {
         QMessageBox::critical(this, "Ошибка",
                               "Ошибка при расчете оценки: " + (sGrade.isEmpty() ? "пустой результат" : sGrade));
         return;
     }

     // Получение изображения
     const QString condition = " Where figure=" + sGrade;
     const QString tableName = "grade";
     const QString fieldName = "graph";

     const QByteArray imageData = dbFacade->getValueFromDB<QByteArray>(fieldName, tableName, condition);

     if (!imageData.isEmpty()) {
         loadImageData(imageData);
     } else {
         qDebug() << "onPushButtonEndTestClicked: No image for grade" << sGrade;
         QMessageBox::warning(this, "Предупреждение",
                              "Изображение для оценки " + sGrade + " не найдено");
     }

     for (int i = 0; i < totalQuestion; ++i) {
         num_cur_task.saveTaskPushButton[i] = false;
         num_cur_task.saveAnswerRadioButton[i] = 0;
     }
     ready_madeTasks = 0;
     tGrade = 0;
     delFileDir(state.pathGraph);
     ui->menubar->show();

     qDebug() << "Test completed. Grade:" << sGrade << "Score:" << tGrade << "/" << totalQuestion;
 }

 void MainWindow::loadImageData(QByteArray imageData){

     restoreTabsAndWidgets();
     showOnlyThisTab(customTab);
     hideAllTestControls();

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

 void MainWindow::onTestBreakClicked() {
     // Проверка основного UI
     if (!ui) {
         qDebug() << "onTestBreakClicked: ui is nullptr";
         return;
     }

     qDebug() << "onTestBreakClicked: Breaking test execution";

     // Скрытие всех элементов управления тестом
     auto hideIfExists = [](QWidget* widget) {
         if (widget) widget->hide();
     };

     hideIfExists(ui->framTasksButton);
     hideIfExists(ui->pushButtonEndTest);
     hideIfExists(ui->pushButtonPrevious);
     hideIfExists(ui->pushButtonNext);
     hideIfExists(ui->pushButtonTestBreak);

     // Показ меню
     if (ui->menubar) {
         ui->menubar->show();
     }

     // Сброс состояния заданий (безопасно)
     const int totalTasks = num_cur_task.getTotalQwest();
     const int taskCount = std::min(MAX_TASK, totalTasks);

     if (taskCount > 0) {
         for (int i = 0; i < taskCount; ++i) {
             num_cur_task.saveTaskPushButton[i] = false;
             num_cur_task.saveAnswerRadioButton[i] = 0;
         }
         qDebug() << "onTestBreakClicked: Reset" << taskCount << "tasks";
     } else {
         qDebug() << "onTestBreakClicked: No tasks to reset";
     }

     // Сброс глобальных переменных состояния
     ready_madeTasks = 0;
     tGrade = 0;

     // Очистка временных файлов
     if (!state.pathGraph.isEmpty()) {
         delFileDir(state.pathGraph);
     }

     // Восстановление UI
     restoreTabsAndWidgets();
     showOnlyThisTab(customTabAnime);
     hideAllTestControls();

     // Обновление текста метки времени
     if (Label[1]) {
         const QString timeText = "Время теста: " +
                                  (state.testExecutionTime > 0 ?
                                       QString::number(state.testExecutionTime) :
                                       "не задано");
         Label[1]->setText(timeText);
     }

     // Отключение таймера
     if (timer) {
         disconnect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
         qDebug() << "onTestBreakClicked: Timer disconnected";
     } else {
         qDebug() << "onTestBreakClicked: Timer is nullptr";
     }
 }

void MainWindow::closeEvent(QCloseEvent* event) {
    const QMessageBox::StandardButton resBtn = QMessageBox::question(
        this,
        "Подтверждение выхода",
        "Вы действительно хотите выйти?",
        QMessageBox::No | QMessageBox::Yes,
        QMessageBox::No
        );

    if (resBtn != QMessageBox::Yes) {
        event->ignore();
        return; // Важно: не сохранять состояние при отмене
    }

    // Только при подтверждении выхода
    delFileDir(state.pathGraph);

    if (!ui->lineEditGraph->text().isEmpty()) {
        state.pathGraph = ui->lineEditGraph->text();
    } else {
        qDebug() << "Не указан путь к графике справки" << ui->lineEditGraph->text();
    }

    const QPoint pos = this->frameGeometry().topLeft();
    const QSize size = this->geometry().size();
    state.SaveState(pos, size);

    event->accept();
}

MainWindow::~MainWindow(){
    delete ui;
    delete dbFacade; // Освобождаем память

    // for (int i = 0; i < MAX_LABEL; ++i) {
    //     delete Label[i]; // Освобождаем память для каждого QLabel, если они были созданы с помощью new
    // }
    cleanupStatusBarLabels();
    delete calckgrade;
    qDeleteAll(savedWidgets); // Освобождает память для всех виджетов в списке
    savedWidgets.clear(); // Очищает список
}


void MainWindow:: loadBrowser(QTextBrowser* textBrowser,
                              QString fieldNameHtm,
                              QString tableNameHtm,
                              QString fieldNameFileGraph,
                              QString fieldNameImageData,
                              QString tableNameGraph,
                              QString condition ){

    QStringList stringListFileName = dbFacade->getListFromDB<QString>(
                                            fieldNameFileGraph,
                                            tableNameGraph,
                                            condition);

    QList<QByteArray> stringListImageData = dbFacade->getListFromDB<QByteArray>(
                                                fieldNameImageData,
                                                tableNameGraph,
                                                condition);
    QString dirName=state.pathGraph;//+"/";

    saveFiles(stringListFileName,
              stringListImageData,
              dirName) ;
    condition="";

    QString htmlValue = dbFacade->getValueFromDB<QString>(fieldNameHtm,
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

    QStringList stringListFileName = dbFacade->getListFromDB<QString>(
                                                fieldNameFileGraph,
                                                tableNameGraph,
                                                condition);
    QList<QByteArray> stringListImageData = dbFacade->getListFromDB<QByteArray>(
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
        onPushButtonEndTestClicked();
    }
}

