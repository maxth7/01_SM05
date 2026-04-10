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
    ensureServiceIsRunning(serviceName);

    setupStatusBar();
    setupCustomTabs();
}

// void MainWindow::monitoringServices(QString serviceName){
//     if(!ChServices.isPostgresServiceRunning(serviceName)) {
//         QString msg = "Служба: " + serviceName + " не запущена.";
//         QMessageBox::critical(nullptr,
//                               "Ошибка",
//                               "Не удалось запустить приложение. " + msg);
//         Label[0]->setText(msg);
//         QCoreApplication::exit(1);
//     }
// }
#include "constants.h"

// Основная функция
void MainWindow::ensureServiceIsRunning(const QString& serviceName) {
    validateUi();

    if (!areServiceCheckPrerequisitesValid()) {
        exitApplicationWithError();
        return;
    }

    if (!isServiceRunning(serviceName)) {
        showServiceErrorAndExit(serviceName);
    }
}

// 1. Проверка предварительных условий
bool MainWindow::areServiceCheckPrerequisitesValid() const {
    // Проверка Label[0]
    if (!Label[0]) {
        qDebug() << "areServiceCheckPrerequisitesValid: Label[0] is nullptr";
        return false;
    }

    return true;
}

// 2. Проверка статуса службы
bool MainWindow::isServiceRunning(const QString& serviceName)  {
    return ChServices.isPostgresServiceRunning(serviceName);
}

// 3. Отображение ошибки и выход
void MainWindow::showServiceErrorAndExit(const QString& serviceName) {
    using namespace ErrorMessages;

    const QString errorMsg = QString(SERVICE_NOT_RUNNING).arg(serviceName);
    const QString fullMsg = APP_START_FAILED + errorMsg;

    // Показываем сообщение пользователю
    QMessageBox::critical(this, "Ошибка", fullMsg);

    // Обновляем статусную метку
    updateStatusLabelServiceIsRunning(errorMsg);

    // Завершаем приложение
    exitApplicationWithError();
}

// 4. Обновление статусной метки
void MainWindow::updateStatusLabelServiceIsRunning(const QString& message) {
    if (Label[0]) {
        Label[0]->setText(message);
    } else {
        qDebug() << "updateStatusLabel: Label[0] is nullptr";
    }
}

// 5. Корректный выход из приложения
void MainWindow::exitApplicationWithError(int exitCode) {
    qDebug() << "exitApplicationWithError: exiting with code" << exitCode;
    QCoreApplication::exit(exitCode);
}

void MainWindow::restoreStateMainWindow() {
    validateUi();

    // Восстановление геометрии
    restoreWindowGeometry();

    // Восстановление индекса темы
    restoreSelectedThemeIndex();

    // Восстановление пути к графикам
    restoreGraphPath();
}

void MainWindow::restoreWindowGeometry() {
    using namespace UiConstants;

    state.RestoreState(pos, size);

    const QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    const QRect windowRect(pos, size);

    if (!screenGeometry.contains(windowRect)) {
        // Устанавливаем значения по умолчанию
        pos = QPoint(DEFAULT_WINDOW_POSITION_X, DEFAULT_WINDOW_POSITION_Y);
        size = QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

        qDebug() << "restoreWindowGeometry: window out of screen, using defaults";
    }

    this->move(pos);
    this->resize(size);
}

void MainWindow::restoreSelectedThemeIndex() {
    constexpr int MIN_INDEX = 0;  // 0-based index

    // Конвертация из 1-based (сохраненный) в 0-based (используемый)
    selectedIndex = state.CurrentThema - 1;

    if (selectedIndex < MIN_INDEX) {
        qDebug() << "restoreSelectedThemeIndex: invalid index" << selectedIndex
                 << ", resetting to" << MIN_INDEX;
        selectedIndex = MIN_INDEX;
    }

    // Опционально: проверка максимального индекса
    const int maxIndex = getMaxThemeIndex();  // Нужно реализовать
    if (selectedIndex >= maxIndex) {
        qDebug() << "restoreSelectedThemeIndex: index" << selectedIndex
                 << "out of range (max:" << maxIndex - 1 << ")";
        selectedIndex = MIN_INDEX;
    }
}

void MainWindow::restoreGraphPath() {
    if (ui->lineEditGraph) {
        ui->lineEditGraph->setText(state.pathGraph);
    } else {
        qDebug() << "restoreGraphPath: lineEditGraph is nullptr";
    }
}

// Основная функция
void MainWindow::setupStatusBar() {
    validateUi();

    if (!areStatusLabelsValid()) {
        qDebug() << "setupStatusBar: status labels are invalid";
        return;
    }

    if (!initializeDatabaseConnection()) {
        return;  // Ошибка уже обработана
    }

    updateStatusLabels();
}

// 1.Проверка валидности лейблов
bool MainWindow::areStatusLabelsValid() const {
    if (!Label[UiConstants::THEME_LABEL_INDEX]) {
        qDebug() << "areStatusLabelsValid: theme label is nullptr";
        return false;
    }
    if (!Label[UiConstants::STATUS_LABEL_INDEX]) {
        qDebug() << "areStatusLabelsValid: status label is nullptr";
        return false;
    }
    return true;
}

// 2.Инициализация подключения к БД
bool MainWindow::initializeDatabaseConnection() {
    using namespace DbConstants;

    if (!dbFacade) {
        qDebug() << "initializeDatabaseConnection: dbFacade is nullptr";
        return false;
    }

    if (!dbFacade->openDatabaseQPSQL(DB_HOST, DB_NAME, DB_USER, DB_PASSWORD, DB_PORT)) {
        showDatabaseErrorAndExit("Невозможно открыть БД.");
        return false;
    }

    return true;
}

// 3.Отображение ошибки и выход
void MainWindow::showDatabaseErrorAndExit(const QString& errorMsg) {
    const QString fullMsg = "Не удалось запустить приложение. " + errorMsg;
    QMessageBox::critical(this, "Ошибка", fullMsg);

    if (Label[UiConstants::STATUS_LABEL_INDEX]) {
        Label[UiConstants::STATUS_LABEL_INDEX]->setText(errorMsg);
    }

    QCoreApplication::exit(1);
}

// 4.Обновление статусных лейблов
void MainWindow::updateStatusLabels() {
    updateThemeLabel();
    updateTimeLabel();
}

// 5.Обновление лейбла темы
void MainWindow::updateThemeLabel() {
    using namespace DbConstants;

    const QString condition = " Where n=" + QString::number(selectedIndex);
    const QString tableName = "tasks";
    const QString fieldName = "task";

    const QString htmlValue = dbFacade->getValueFromDB<QString>(fieldName, tableName, condition);
    const QString themeText = extractThemeFromHtml(htmlValue);

    if (!themeText.isEmpty() && Label[UiConstants::THEME_LABEL_INDEX]) {
        Label[UiConstants::THEME_LABEL_INDEX]->setText(themeText);
    }
}

// 6.Извлечение темы из HTML
QString MainWindow::extractThemeFromHtml(const QString& html) {
    using namespace HtmlConstants;

    const int index = html.indexOf(BODY_BACKGROUND_PATTERN);
    if (index == -1) {
        return QString();
    }

    return html.left(index).simplified();
}

// 7.Обновление лейбла времени
void MainWindow::updateTimeLabel() {
    const QString timeTest = QString("Время теста: %1 мин")
                                 .arg(QString::number(state.testExecutionTime, 'f', 2));

    if (Label[UiConstants::STATUS_LABEL_INDEX]) {
        Label[UiConstants::STATUS_LABEL_INDEX]->setText(timeTest);
    }
}

void MainWindow::setupMenuBar() {
    validateUi();

    if (!ui->menubar) {
        qDebug() << "setupMenuBar: menubar is nullptr";
        return;
    }

    // Создание действий
    QAction* const choosingTheme = new QAction(tr("&Темы"), this);
    QAction* const testing = new QAction(tr("&Тест"), this);
    QAction* const trainingManual = new QAction(tr("&Методичка"), this);
    QAction* const setting = new QAction(tr("&Настройки"), this);
    QAction* const help = new QAction(tr("&Справка"), this);

    // Подключение сигналов
    connect(choosingTheme, &QAction::triggered, this, &MainWindow::onChoosingThemeClicked);
    connect(testing, &QAction::triggered, this, &MainWindow::onTestingClicked);
    connect(trainingManual, &QAction::triggered, this, &MainWindow::onTrainingManualButtonClicked);
    connect(setting, &QAction::triggered, this, &MainWindow::onSettingClicked);
    connect(help, &QAction::triggered, this, &MainWindow::onHelpClicked);

    // Добавление в меню
    ui->menubar->addAction(choosingTheme);
    ui->menubar->addAction(testing);
    ui->menubar->addAction(trainingManual);
    ui->menubar->addAction(setting);
    ui->menubar->addAction(help);

    // Настройка стиля
    setupMenuBarStyle();
}

void MainWindow::setupMenuBarStyle() {
    if (!ui->menubar) return;

    // Настройка стиля
    ui->menubar->setStyleSheet(UiConstants::MENUBAR_STYLE);
}

void MainWindow::setupTestButtons() {
    validateUi();

    // Вспомогательная лямбда для безопасного подключения
    auto safeConnect = [this](QPushButton* button, void (MainWindow::*slot)()) {
        if (button) {
            connect(button, &QPushButton::clicked, this, slot);
        } else {
            qDebug() << "setupTestButtons: button is nullptr";
        }
    };

    safeConnect(ui->pushButtonNext, &MainWindow::onPushButtonNextClicked);
    safeConnect(ui->pushButtonEndTest, &MainWindow::onPushButtonEndTestClicked);
    safeConnect(ui->pushButtonPrevious, &MainWindow::onPushButtonPreviousClicked);
    safeConnect(ui->pushButtonTestBreak, &MainWindow::onTestBreakClicked);
}


// void MainWindow::settingTab(){
//     //--------------------------------------------------
//     stretchLabel->setGif(":/images/Tesla/tesla_m_s.gif");

//     customTabAnime->setObjectName("tabCustomTabAnime");
//     customTabAnime->setStyleSheet("QTabWidget::pane { border: none; }");

//     customTab->setObjectName("tabcustomTab"); // было не задано
//     QLayout *layout = customTabAnime->layout();

//     if (!customTabAnime->layout()) {
//         customTabAnime->setLayout(new QVBoxLayout(customTabAnime));
//     }
//     //--------------------------------------------------------------------------
//     customTabAnime->layout()->addWidget(stretchLabel);
//     ui->tabWidget->addTab(customTabAnime,"customTabAnime");
//     ui->tabWidget->addTab(customTab,"customTab");
//     saveTabWidgets();
//     showOnlyThisTab(customTabAnime);
//     hideAllTestControls();
// }

// Основная функция
void MainWindow::setupCustomTabs() {
    validateUi();

    if (!areCustomTabsValid()) {
        qDebug() << "setupCustomTabs: required widgets are nullptr";
        return;
    }

    setupAnimationLabel();
    configureCustomTabWidgets();
    ensureLayoutExists();
    addTabsToTabWidget();
    applyTabStyles();

    saveTabWidgets();
    showOnlyThisTab(customTabAnime);
    hideAllTestControls();
}

// 1. Проверка валидности виджетов
bool MainWindow::areCustomTabsValid() const {
    if (!stretchLabel) {
        qDebug() << "areCustomTabsValid: stretchLabel is nullptr";
        return false;
    }
    if (!customTabAnime) {
        qDebug() << "areCustomTabsValid: customTabAnime is nullptr";
        return false;
    }
    if (!customTab) {
        qDebug() << "areCustomTabsValid: customTab is nullptr";
        return false;
    }
    if (!ui->tabWidget) {
        qDebug() << "areCustomTabsValid: tabWidget is nullptr";
        return false;
    }
    return true;
}

// 2. Настройка анимации
void MainWindow::setupAnimationLabel() {
    using namespace UiConstants;

    stretchLabel->setGif(ANIMATION_GIF_PATH);
    qDebug() << "setupAnimationLabel: animation loaded from" << ANIMATION_GIF_PATH;
}

// 3. Конфигурация виджетов вкладок
void MainWindow::configureCustomTabWidgets() {
    using namespace UiConstants;

    customTabAnime->setObjectName(TAB_ANIME_OBJECT_NAME);
    customTab->setObjectName(TAB_CUSTOM_OBJECT_NAME);
}

// 4. Обеспечение существования layout
void MainWindow::ensureLayoutExists() {
    if (!customTabAnime->layout()) {
        auto* layout = new QVBoxLayout(customTabAnime);
        customTabAnime->setLayout(layout);
        qDebug() << "ensureLayoutExists: created new layout for customTabAnime";
    } else {
        qDebug() << "ensureLayoutExists: layout already exists";
    }
}

// 5. Добавление вкладок в tabWidget
void MainWindow::addTabsToTabWidget() {
    using namespace UiConstants;

    // Добавляем layout->addWidget(stretchLabel) отдельно
    if (customTabAnime->layout()) {
        customTabAnime->layout()->addWidget(stretchLabel);
    }

    ui->tabWidget->addTab(customTabAnime, TAB_ANIME_DISPLAY_NAME);
    ui->tabWidget->addTab(customTab, TAB_CUSTOM_DISPLAY_NAME);

    qDebug() << "addTabsToTabWidget: added tabs"
             << TAB_ANIME_DISPLAY_NAME << "and" << TAB_CUSTOM_DISPLAY_NAME;
}

// 6. Применение стилей
void MainWindow::applyTabStyles() {
    using namespace UiConstants;

    customTabAnime->setStyleSheet(TAB_ANIME_STYLE);
}

// Основная функция
void MainWindow::onChoosingThemeClicked() {
    validateUi();

    if (!isDatabaseValid()) {
        logError("Database facade is not initialized");
        return;
    }

    const QStringList themes = fetchThemesFromDatabase();

    if (!validateThemesList(themes)) {
        logError("Failed to load themes from database");
        return;
    }

    displayThemesMenu(themes);
}

// 1. Проверка валидности БД
bool MainWindow::isDatabaseValid() const {
    if (!dbFacade) {
        qDebug() << "isDatabaseValid: dbFacade is nullptr";
        return false;
    }
    return true;
}


QStringList MainWindow:: fetchThemesFromDatabase() {
    using namespace DbConstants;
    const QString tableName = THEMES_TABLE_NAME ;
    const QString fieldName = THEMES_FIELD_NAME;
    const QString condition;

    QStringList themes = dbFacade->getRecordsDatabaseQPSQL(fieldName, tableName, condition);
    m_cachedThemeCount = themes.size();  // Обновляем кэш
    return themes;
}

// 3.Валидация списка тем
bool MainWindow::validateThemesList(const QStringList& themes) const {
    if (themes.isEmpty()) {
        qDebug() << "validateThemesList: themes list is empty";
        return false;
    }

    if (themes.contains("")) {
        qDebug() << "validateThemesList: themes list contains empty string";
        return false;
    }

    return true;
}

// 4. Отображение меню тем
void MainWindow::displayThemesMenu(const QStringList& themes) {
    if (themes.isEmpty()) {
        qDebug() << "displayThemesMenu: cannot display empty themes list";
        return;
    }

    onChooseThemeClicked(themes);
    qDebug() << "displayThemesMenu: displayed" << themes.size() << "themes";
}

// 5. Логирование ошибок
void MainWindow::logError(const QString& error) const {
    qDebug() << "getListThemes Error:" << error;
    // Можно добавить запись в файл лога
    // Можно показать сообщение пользователю
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

// void MainWindow::saveTabsAndWidgets() {
//      savedWidgets.clear();
//     for (int i = 0; i < ui->tabWidget->count(); ++i) {

//         QWidget* widget = ui->tabWidget->widget(i);
//         if (widget) {
//             savedWidgets.append(widget);
//         }
//      }
// }
void MainWindow::saveTabWidgets() {//Использование STL алгоритмов (C++17)
    validateUi();

    if (!ui->tabWidget) {
        qDebug() << "saveTabsAndWidgets: tabWidget is nullptr";
        return;
    }

    const int tabCount = ui->tabWidget->count();
    QVector<QWidget*> widgets;
    widgets.reserve(tabCount);  // Предварительное выделение памяти

    for (int i = 0; i < tabCount; ++i) {
        if (QWidget* const widget = ui->tabWidget->widget(i)) {
            widgets.append(widget);
        }
    }

    savedWidgets = std::move(widgets);  // Эффективное перемещение
}

 // void MainWindow::restoreTabsAndWidgets() {
 //    validateUi();  // Фатальная ошибка если ui == nullptr

 //     ui->tabWidget->tabBar()->show();
 //     ui->tabWidget->tabBar()->setStyleSheet("");

 //     while (ui->tabWidget->count() > 0) {
 //         ui->tabWidget->removeTab(0);
 //     }
 //     for (QWidget* widget : savedWidgets) {
 //         if (widget) {
 //             QString tabText = widget->objectName();
 //             ui->tabWidget->addTab(widget, tabText);

 //         }
 //     }
 // }
void MainWindow::restoreTabsAndWidgets() {
    validateUi();

    if (!ui->tabWidget) {
        qDebug() << "restoreTabsAndWidgets: tabWidget is nullptr";
        return;
    }

    if (ui->tabWidget->tabBar()) {
        ui->tabWidget->tabBar()->show();
        ui->tabWidget->tabBar()->setStyleSheet("");
    }

    while (ui->tabWidget->count() > 0) {
        QWidget* widget = ui->tabWidget->widget(0);
        ui->tabWidget->removeTab(0);
        if (widget) widget->deleteLater();
    }

     for (QWidget* const widget : savedWidgets) {
        if (widget) {
            const QString tabText = widget->objectName();
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
               this, &MainWindow::onRadioButtonStretchClicked);
    disconnect(ui->radioButtonNoStretch, &QRadioButton::clicked,
               this, &MainWindow::onRadioButtonStretchClicked);

    // Устанавливаем новые соединения
    connect(ui->radioButtonStretch, &QRadioButton::clicked,
            this, &MainWindow::onRadioButtonStretchClicked);
    connect(ui->radioButtonNoStretch, &QRadioButton::clicked,
            this, &MainWindow::onRadioButtonStretchClicked);
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
// void MainWindow::radioButtonsStretch_clicked() {

//     if (ui->radioButtonNoStretch->isChecked()) {
//         this->setFixedSize(800, 800);
//     } else if (ui->radioButtonStretch->isChecked()) {
//         this->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

//     }
// }

#include "constants.h"

// Основная функция
void MainWindow::onRadioButtonStretchClicked() {
    validateUi();

    if (!areRadioButtonsValid()) {
        qDebug() << "onStretchModeChanged: radio buttons not initialized";
        return;
    }

    if (isNoStretchModeEnabled()) {
        setFixedWindowSize(UiConstants::FIXED_WINDOW_WIDTH, UiConstants::FIXED_WINDOW_HEIGHT);
    } else if (isStretchModeEnabled()) {
        setStretchWindowSize();
    }
}

// 1. Проверка валидности радио-кнопок
bool MainWindow::areRadioButtonsValid() const {
    return ui->radioButtonNoStretch && ui->radioButtonStretch;
}

// 2. Проверка режима "No Stretch"
bool MainWindow::isNoStretchModeEnabled() const {
    return ui->radioButtonNoStretch && ui->radioButtonNoStretch->isChecked();
}

// 3. Проверка режима "Stretch"
bool MainWindow::isStretchModeEnabled() const {
    return ui->radioButtonStretch && ui->radioButtonStretch->isChecked();
}

// 4. Установка фиксированного размера
void MainWindow::setFixedWindowSize(int width, int height) {
    setFixedSize(width, height);
    qDebug() << "Window size set to fixed:" << width << "x" << height;
}

// 5. Установка растянутого размера
void MainWindow::setStretchWindowSize() {
    // Альтернативный подход вместо QWIDGETSIZE_MAX
    const QRect screenGeometry = QApplication::primaryScreen()->geometry();
    setFixedSize(screenGeometry.width(), screenGeometry.height());

    // Или использовать максимальный размер
    // setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    qDebug() << "Window size set to stretch mode";
}

// // Старая функция для обратной совместимости
// void MainWindow::radioButtonsStretch_clicked() {
//     onStretchModeChanged();
// }
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

    loadContentIntoBrowser(ui->textBrowserHelp,
                           HelpConfig::HTML_FIELD,
                           HelpConfig::TABLE_NAME,
                           HelpConfig::GRAPH_FIELD_FILENAME,
                           HelpConfig::GRAPH_FIELD_DATA,
                           HelpConfig::GRAPH_TABLE_NAME,
                           "");
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
    setStyleSheetBackground(htmlValue);

    const QString fieldNameHtm = "task";
    const QString tableNameHtm = "tasks";
    const QString fieldNameFileGraph = "filename";
    const QString fieldNameImageData = "graph";
    const QString tableNameGraph = "graphtest";
    const QString conditionGraph = "Where idtasks=" + QString::number(selectedIndex);

    saveGraphFilesToDisk(fieldNameFileGraph,
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

    loadContentIntoBrowser(ui->textBrowserMet,
                ManualConstants::TEXT_FIELD,
                ManualConstants::TABLE_NAME,
                ManualConstants::GRAPH_FIELD,
                ManualConstants::IMAGE_FIELD,
                ManualConstants::GRAPH_TABLE,
                condition);
}


// void MainWindow::setupRadioButtons(int maxRadioBut) {
//     if (ui->frameRadioButton->layout() != nullptr) {
//         QLayoutItem* item;
//         while ((item = ui->frameRadioButton->layout()->takeAt(0)) != nullptr) {
//             if (item->widget()) {
//                 item->widget()->deleteLater();
//             }
//             delete item;
//         }
//         delete ui->frameRadioButton->layout();
//     }
//     auto* layoutRadioButton = new QVBoxLayout(ui->frameRadioButton);
//     ui->frameRadioButton->setLayout(layoutRadioButton);

//     for (int i = 0; i < maxRadioBut; ++i) {
//         auto* radioButton = new QRadioButton(QString::number(i + 1));
//         radioButton->setStyleSheet("QRadioButton {"
//                                    "   font-size: 16px;"
//                                    "   color: #4682B4;"
//                                    "   font-weight: bold;"
//                                    "   background-color: #FFFFFF;"
//                                    "}"
//                                    "QRadioButton::indicator {"
//                                    "   width: 30px;"
//                                    "   height: 30px;"
//                                    "}");
//         layoutRadioButton->addWidget(radioButton);
//         connect(radioButton, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
//     }
// }


// Основная функция
void MainWindow::setupDynamicRadioButtons(int maxRadioBut) {
    validateUi();

    if (!isValidButtonCount(maxRadioBut)) {
        qDebug() << "setupRadioButtons: invalid button count" << maxRadioBut;
        return;
    }

    if (!ui->frameRadioButton) {
        qDebug() << "setupRadioButtons: frameRadioButton is nullptr";
        return;
    }

    clearRadioButtonPanel();
    createDynamicRadioButtonLayout();
    createDynamicRadioButtons(maxRadioBut);
}

// 1.Валидация количества кнопок
bool MainWindow::isValidButtonCount(int count) const {
    return count >= UiConstants::MIN_RADIO_BUTTONS && count <= MAX_ANSWER;
}

// 2.Очистка панели радио-кнопок
void MainWindow::clearRadioButtonPanel() {
    if (!ui->frameRadioButton) return;

    QLayout* oldLayout = ui->frameRadioButton->layout();
    if (!oldLayout) return;

    // Безопасное удаление всех виджетов и layout
    QLayoutItem* item;
    while ((item = oldLayout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    delete oldLayout;
    ui->frameRadioButton->setLayout(nullptr);
}

// 3.Создание layout для радио-кнопок
void MainWindow::createDynamicRadioButtonLayout() {
    if (!ui->frameRadioButton) return;

    auto* layout = new QVBoxLayout(ui->frameRadioButton);
    ui->frameRadioButton->setLayout(layout);
}

// 4.Создание всех радио-кнопок
void MainWindow::createDynamicRadioButtons(int count) {
    if (!ui->frameRadioButton || !ui->frameRadioButton->layout()) {
        qDebug() << "createRadioButtons: layout is not initialized";
        return;
    }

    for (int i = 0; i < count; ++i) {
        QRadioButton* button = createSingleRadioButton(i);
        ui->frameRadioButton->layout()->addWidget(button);
        connectRadioButton(button);
    }
}

// 5.Создание одной радио-кнопки
QRadioButton* MainWindow::createSingleRadioButton(int index) {
    const QString buttonText = QString::number(index + 1);
    auto* radioButton = new QRadioButton(buttonText);
    applyRadioButtonStyle(radioButton);
    return radioButton;
}

// 6.Применение стиля к кнопке
void MainWindow::applyRadioButtonStyle(QRadioButton* button) {
    using namespace UiConstants;
    button->setStyleSheet(RADIO_BUTTON_STYLE);
}

// 7.Подключение сигнала
void MainWindow::connectRadioButton(QRadioButton* button) {
    connect(button, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
}

// void MainWindow::setupPushButtons(int maxPushButtons,int colCount) {
//     if (ui->framTasksButton->layout() != nullptr) {
//         QLayoutItem* item;
//         while ((item = ui->framTasksButton->layout()->takeAt(0)) != nullptr) {
//             if (item->widget()) {
//                 item->widget()->deleteLater();
//             }
//             delete item;
//         }
//         delete ui->framTasksButton->layout();
//     }

//     QGridLayout *layout = new QGridLayout(ui->framTasksButton);

//     for (int i = 0; i < maxPushButtons; ++i) {
//         pushButton[i] = new QPushButton(QString::number(i + 1)); // Установка текста на кнопке

//         if(i==0){
//             pushButton[i]->setStyleSheet("color: white;"
//                                          "border-color: red;"
//                                          "background-color: #4CAF50; "
//                                          "font-weight: bold;"
//                                          " padding: 2px 2px;"
//                                          " text-align: center;"
//                                          " text-decoration: none;"
//                                          " font-size: 16px;");
//         }
//         else{
//             pushButton[i]->setStyleSheet("color: black;"
//                                          "border-color: black;"
//                                          "background-color:#4CAF50; "
//                                          "font-weight: bold;"
//                                          " padding: 2px 2px;"
//                                          " text-align: center;"
//                                          " text-decoration: none;"
//                                          " font-size: 16px;");
//         }
//         pushButton[i]->setFixedSize(30, 30);
//         pushButton[i]->setProperty("index", i);
//         connect(pushButton[i], &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);

//         layout->addWidget(pushButton[i], i / colCount, i % colCount);

//     }
// }

// Основная функция
void MainWindow::setupPushButtons(int maxPushButtons, int colCount) {
    validateUi();

    using namespace UiConstants;

    if (!arePushButtonParamsValid(maxPushButtons, colCount)) {
        qDebug() << "setupPushButtons: invalid parameters - buttons:"
                 << maxPushButtons << "columns:" << colCount;
        return;
    }

    if (!ui->framTasksButton) {
        qDebug() << "setupPushButtons: framTasksButton is nullptr";
        return;
    }

    clearPushButtonPanel();
    createPushButtonLayout();
    createAllPushButtons(maxPushButtons, colCount);
}

// 1. Валидация параметров
bool MainWindow::arePushButtonParamsValid(int maxPushButtons, int colCount) const {
    using namespace UiConstants;
    return maxPushButtons > 0 && maxPushButtons <= MAX_TASK && colCount > 0;
}

// 2. Очистка панели кнопок
void MainWindow::clearPushButtonPanel() {
    if (!ui->framTasksButton) return;

    QLayout* oldLayout = ui->framTasksButton->layout();
    if (!oldLayout) return;

    // Безопасное удаление всех виджетов и layout
    QLayoutItem* item;
    while ((item = oldLayout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    delete oldLayout;
    ui->framTasksButton->setLayout(nullptr);
}

// 3. Создание layout для кнопок
void MainWindow::createPushButtonLayout() {
    if (!ui->framTasksButton) return;

    auto* layout = new QGridLayout(ui->framTasksButton);
    ui->framTasksButton->setLayout(layout);
}

// 4. Создание всех кнопок
void MainWindow::createAllPushButtons(int maxPushButtons, int colCount) {
    for (int i = 0; i < maxPushButtons; ++i) {
        QPushButton* button = createSinglePushButton(i);
        configurePushButton(button, i);
        addPushButtonToLayout(button, i, colCount);
    }
}

// 5. Создание одной кнопки
QPushButton* MainWindow::createSinglePushButton(int index) {
    const QString buttonText = QString::number(index + 1);
    return new QPushButton(buttonText);
}

// 6. Применение стиля к кнопке
void MainWindow::applyPushButtonStyle(QPushButton* button, int index) {
    using namespace UiConstants;

    const QString& style = (index == 0) ? FIRST_BUTTON_STYLE : DEFAULT_BUTTON_STYLE;
    button->setStyleSheet(style);
}

// 7. Настройка кнопки
void MainWindow::configurePushButton(QPushButton* button, int index) {
    using namespace UiConstants;

    applyPushButtonStyle(button, index);
    button->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    button->setProperty("index", index);
    connect(button, &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);
}

// 8. Добавление кнопки в layout
void MainWindow::addPushButtonToLayout(QPushButton* button, int index, int colCount) {
    if (!ui->framTasksButton || !ui->framTasksButton->layout()) {
        qDebug() << "addPushButtonToLayout: layout is not initialized";
        return;
    }

    const int row = index / colCount;
    const int col = index % colCount;

    auto* gridLayout = qobject_cast<QGridLayout*>(ui->framTasksButton->layout());
    if (gridLayout) {
        gridLayout->addWidget(button, row, col);
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
    setupDynamicRadioButtons(maxRadioBut);
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

    for (int i = 0; i < UiConstants::MAX_LABEL; ++i) {
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
    for (int i = 0; i < UiConstants::MAX_LABEL; ++i) {
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
    if (index >= 0 && index < UiConstants::MAX_LABEL) {  // Проверка границ!
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
     clearDirectory(state.pathGraph);
     ui->menubar->show();

     qDebug() << "Test completed. Grade:" << sGrade << "Score:" << tGrade << "/" << totalQuestion;
 }


 // Основная функция
 void MainWindow::loadImageData(const QByteArray& imageData) {
     validateUi();

     if (!prepareImageDisplay()) {
         qDebug() << "loadImageData: failed to prepare display";
         return;
     }

     QImage image;
     if (!loadImageFromData(imageData, image)) {
         qDebug() << "loadImageData: failed to load image from data";
         return;
     }

     displayImageInGraphicsView(image);
 }

 // 1.Подготовка к отображению
 bool MainWindow::prepareImageDisplay() {
     if (!ui->graphicsViewGrade) {
         qDebug() << "prepareImageDisplay: graphicsViewGrade is nullptr";
         return false;
     }

     if (!customTab) {
         qDebug() << "prepareImageDisplay: customTab is nullptr";
         return false;
     }

     restoreTabsAndWidgets();
     showOnlyThisTab(customTab);
     hideAllTestControls();

     setupGraphicsView();
     setupCustomTabLayout();

     return true;
 }

 // 2.Настройка GraphicsView
 void MainWindow::setupGraphicsView() {
     using namespace UiConstants;

     ui->graphicsViewGrade->show();
     ui->graphicsViewGrade->setFrameShape(QFrame::NoFrame);
     ui->graphicsViewGrade->setStyleSheet(GRAPHICS_VIEW_STYLE);

     // Очистка старой сцены
     cleanupOldScene(ui->graphicsViewGrade);
 }

 // 3.Очистка старой сцены
 void MainWindow::cleanupOldScene(QGraphicsView* graphicsView) {
     if (graphicsView->scene()) {
         delete graphicsView->scene();
         graphicsView->setScene(nullptr);
         qDebug() << "cleanupOldScene: old scene deleted";
     }
 }

 // 4.Настройка layout для customTab
 void MainWindow::setupCustomTabLayout() {
     QLayout* existingLayout = customTab->layout();

     if (!existingLayout) {
         auto* layout = new QHBoxLayout(customTab);
         layout->addWidget(ui->graphicsViewGrade, 0, Qt::AlignCenter);
         customTab->setLayout(layout);
         qDebug() << "setupCustomTabLayout: created new layout";
     } else {
         addGraphicsViewToLayout(ui->graphicsViewGrade);
     }
 }

 // 5.Добавление GraphicsView в существующий layout
 void MainWindow::addGraphicsViewToLayout(QGraphicsView* graphicsView) {
     QLayout* existingLayout = customTab->layout();
     auto* hLayout = qobject_cast<QHBoxLayout*>(existingLayout);

     if (hLayout) {
         hLayout->addWidget(graphicsView, 0, Qt::AlignCenter);
         qDebug() << "addGraphicsViewToLayout: added to existing layout";
     } else {
         qDebug() << "addGraphicsViewToLayout: layout is not QHBoxLayout";
     }
 }

 // 6.Загрузка изображения из данных
 bool MainWindow::loadImageFromData(const QByteArray& imageData, QImage& image) const {
     return image.loadFromData(imageData);
 }

 // 7.Отображение изображения
 void MainWindow::displayImageInGraphicsView(const QImage& image) {
     if (!ui->graphicsViewGrade) return;

     // Создаем новую сцену с parent = graphicsView (автоматическое удаление)
     auto* scene = new QGraphicsScene(ui->graphicsViewGrade);
     ui->graphicsViewGrade->setScene(scene);

     // Добавляем изображение
     scene->addPixmap(QPixmap::fromImage(image));

     // Устанавливаем размер
     ui->graphicsViewGrade->setFixedSize(image.width(), image.height());
     ui->graphicsViewGrade->update();
     ui->graphicsViewGrade->show();

     qDebug() << "displayImageInGraphicsView: displayed image"
              << image.width() << "x" << image.height();
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
         clearDirectory(state.pathGraph);
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
    clearDirectory(state.pathGraph);

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


// void MainWindow:: loadBrowser(QTextBrowser* textBrowser,
//                               QString fieldNameHtm,
//                               QString tableNameHtm,
//                               QString fieldNameFileGraph,
//                               QString fieldNameImageData,
//                               QString tableNameGraph,
//                               QString condition ){

//     QStringList stringListFileName = dbFacade->getListFromDB<QString>(
//                                             fieldNameFileGraph,
//                                             tableNameGraph,
//                                             condition);

//     QList<QByteArray> stringListImageData = dbFacade->getListFromDB<QByteArray>(
//                                                 fieldNameImageData,
//                                                 tableNameGraph,
//                                                 condition);
//     QString dirName=state.pathGraph;//+"/";

//     saveFiles(stringListFileName,
//               stringListImageData,
//               dirName) ;
//     condition="";

//     QString htmlValue = dbFacade->getValueFromDB<QString>(fieldNameHtm,
//                                                           tableNameHtm,
//                                                           condition);

//     QString fullHtml=num_cur_task.insertPath(htmlValue, state.pathGraph);

//     readStyleSheet(fullHtml);
//     textBrowser->clear();
//     textBrowser->setStyleSheet(styleSheet);
//     textBrowser->setHtml(fullHtml);

// }

void MainWindow::loadContentIntoBrowser(QTextBrowser* textBrowser,
                             const QString& fieldNameHtm,
                             const QString& tableNameHtm,
                             const QString& fieldNameFileGraph,
                             const QString& fieldNameImageData,
                             const QString& tableNameGraph,
                             const QString& condition) {
    validateUi();

    // Проверки
    if (!textBrowser) {
        qDebug() << "loadBrowser: textBrowser is nullptr";
        return;
    }

    if (!dbFacade) {
        qDebug() << "loadBrowser: dbFacade is nullptr";
        return;
    }

    // Загрузка медиафайлов
    const QStringList stringListFileName = dbFacade->getListFromDB<QString>(
        fieldNameFileGraph, tableNameGraph, condition);

    const QList<QByteArray> stringListImageData = dbFacade->getListFromDB<QByteArray>(
        fieldNameImageData, tableNameGraph, condition);

    const QString dirName = state.pathGraph;

    if (!stringListFileName.isEmpty() && !stringListImageData.isEmpty()) {
        saveFiles(stringListFileName, stringListImageData, dirName);
    }

    // Загрузка и обработка HTML
    const QString htmlValue = dbFacade->getValueFromDB<QString>(
        fieldNameHtm, tableNameHtm, condition);

    const QString fullHtml = num_cur_task.insertPath(htmlValue, state.pathGraph);

    // Отображение
    setStyleSheetBackground(fullHtml);
    textBrowser->clear();
    textBrowser->setStyleSheet(styleSheet);
    textBrowser->setHtml(fullHtml);
}

// void MainWindow:: saveFiles(const QStringList& stringListFileName,
//                             const QList<QByteArray>& stringListImageData,
//                             const QString& dirName) {
//     if (stringListFileName.size() != stringListImageData.size()) {
//         qDebug() << "Error: The number of file names does not match the number of image data entries.";
//         return;
//     }
//     QDir dir(dirName);
//     if (!dir.exists()) {
//         if (!dir.mkpath(".")) {
//             qDebug() << "Error: Failed to create directory:" << dirName;
//             return;
//         }
//     }
//     for (int i = 0; i < stringListFileName.size(); ++i) {
//         QString fileName = stringListFileName[i];
//         QByteArray imageData = stringListImageData[i];

//         QString fullFilePath = dir.filePath(fileName);
//         QFile file(fullFilePath);
//         if (!file.open(QIODevice::WriteOnly)) {
//             qDebug() << "Error: Failed to open file for writing:" << fullFilePath;
//             continue;
//         }

//         qint64 bytesWritten = file.write(imageData);
//         if (bytesWritten == -1) {
//             qDebug() << "Error: Failed to write data to file:" << fullFilePath;
//         } else {
//             //qDebug() << "File saved successfully:" << fullFilePath;
//         }

//         file.close();
//     }
// }

// Основная функция (Facade)
void MainWindow::saveFiles(const QStringList& fileNames,
                           const QList<QByteArray>& imageDataList,
                           const QString& directoryPath) const {
    validateUi();

    // Проверка соответствия размеров
    if (fileNames.size() != imageDataList.size()) {
        qDebug() << "saveFiles: size mismatch - fileNames:" << fileNames.size()
        << "imageData:" << imageDataList.size();
        return;
    }

    // Создание директории
    if (!ensureDirectoryExists(directoryPath)) {
        return;
    }

    // Сохранение файлов
    int successCount = 0;
    for (int i = 0; i < fileNames.size(); ++i) {
        if (saveSingleFile(directoryPath, fileNames[i], imageDataList[i])) {
            successCount++;
        }
    }

    qDebug() << "saveFiles: saved" << successCount << "of" << fileNames.size() << "files";
}

// 1. Обеспечение существования директории
bool MainWindow::ensureDirectoryExists(const QString& dirPath) const {
    QDir dir(dirPath);

    if (dir.exists()) {
        return true;
    }

    if (dir.mkpath(dirPath)) {
        qDebug() << "ensureDirectoryExists: created directory" << dirPath;
        return true;
    } else {
        qDebug() << "ensureDirectoryExists: failed to create directory" << dirPath;
        return false;
    }
}

// 2.Сохранение одного файла
bool MainWindow::saveSingleFile(const QString& directoryPath,
                                const QString& fileName,
                                const QByteArray& imageData) const {
    // Формируем полный путь
    const QString fullFilePath = QDir(directoryPath).filePath(fileName);

    // Открываем файл
    QFile file(fullFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "saveSingleFile: failed to open" << fullFilePath;
        return false;
    }

    // Записываем данные
    const qint64 bytesWritten = file.write(imageData);
    if (bytesWritten == -1) {
        qDebug() << "saveSingleFile: failed to write to" << fullFilePath;
        file.close();
        return false;
    }

    // Закрываем файл (close() возвращает void, не проверяем)
    file.close();

    qDebug() << "saveSingleFile: saved" << fileName << "(" << bytesWritten << "bytes)";
    return true;
}

// 3.Валидация данных файла
bool MainWindow::isValidFileData(const QString& fileName, const QByteArray& data) const {
    if (fileName.isEmpty()) {
        qDebug() << "isValidFileData: empty file name";
        return false;
    }

    if (data.isEmpty()) {
        qDebug() << "isValidFileData: empty data for file" << fileName;
        return false;
    }

    return true;
}

// 4.Логирование ошибок
void MainWindow::logFileError(const QString& error, const QString& filePath) const {
    qDebug() << "saveFiles Error:" << error << "-" << filePath;
}


void MainWindow::clearDirectory(const QString path) {
    // Проверка пустого пути
    if (path.isEmpty()) {
        qDebug() << "delFileDir: path is empty";
        return;
    }

    QDir dir(path);

    // Проверка существования директории
    if (!dir.exists()) {
        qDebug() << "delFileDir: directory not found:" << path;
        return;
    }

    // Получение списка файлов
    const QStringList files = dir.entryList(QDir::Files);

    // Удаление файлов
    for (const QString& file : files) {  // ✅ Используем range-based for
        if (!dir.remove(file)) {
            qDebug() << "delFileDir: failed to remove file:" << file;
        }
    }

    qDebug() << "delFileDir: removed" << files.size() << "files from" << path;
}

void MainWindow::setStyleSheetBackground(const QString& html) {  // const reference
    validateUi();  // Проверка


    QString backgroundBlock = extractFileNameFromHtml(html,
                                        HtmlConstants ::BACKGROUND_BLOCK_START,
                                        HtmlConstants ::BACKGROUND_BLOCK_END);
    if (backgroundBlock.isEmpty()) return;


    const QString filename = extractFileNameFromHtml(backgroundBlock,
                                        HtmlConstants ::BACKGROUND_ATTR_START,
                                        HtmlConstants ::BACKGROUND_ATTR_END);
    if (filename.isEmpty()) return;

    // Проверка пути
    const QString pathDirGraph = state.pathGraph;
    if (pathDirGraph.isEmpty()) {
        qDebug() << "readStyleSheet: pathGraph is empty";
        return;
    }

    // Формирование стиля
    styleSheet = "background-image: url(\"" +
                 pathDirGraph + "/" +
                 filename + "\")";
}

QString MainWindow::extractFileNameFromHtml(const QString& html,
                                           const QString& startSequence,
                                           const QString& endSequence) const {
    const QString regexPattern = startSequence + "(.*?)" + endSequence;
    const QRegularExpression regex(regexPattern);

    if (!regex.isValid()) {
        qDebug() << "readFileNameBackground: invalid regex pattern:" << regexPattern;
        return QString();
    }

    const QRegularExpressionMatch match = regex.match(html);

    if (match.hasMatch()) {
        return match.captured(1);
    }

    const QString errorMessage = "Pattern not found: " + startSequence + "..." + endSequence;
    qDebug() << errorMessage;
    return QString();
}


// void MainWindow::uploadingGraphFiles(QString fieldNameFileGraph,
//                                       QString fieldNameImageData,
//                                       QString tableNameGraph,
//                                       QString condition ){

//     QStringList stringListFileName = dbFacade->getListFromDB<QString>(
//                                                 fieldNameFileGraph,
//                                                 tableNameGraph,
//                                                 condition);
//     QList<QByteArray> stringListImageData = dbFacade->getListFromDB<QByteArray>(
//                                                 fieldNameImageData,
//                                                 tableNameGraph,
//                                                 condition);
//     QString dirName=state.pathGraph;//+"/";

//     saveFiles(stringListFileName,
//               stringListImageData,
//               dirName) ;
// }
void MainWindow::saveGraphFilesToDisk(const QString& fieldNameFileGraph,
                                     const QString& fieldNameImageData,
                                     const QString& tableNameGraph,
                                     const QString& condition) {
    validateUi();

    // Проверка dbFacade
    if (!dbFacade) {
        qDebug() << "uploadingGraphFiles: dbFacade is nullptr";
        return;
    }

    // Проверка пути
    if (state.pathGraph.isEmpty()) {
        qDebug() << "uploadingGraphFiles: state.pathGraph is empty";
        return;
    }

    const QStringList fileNames = dbFacade->getListFromDB<QString>(
        fieldNameFileGraph,
        tableNameGraph,
        condition);

    const QList<QByteArray> imageDataList = dbFacade->getListFromDB<QByteArray>(
        fieldNameImageData,
        tableNameGraph,
        condition);

    // Проверка, что размеры списков совпадают
    if (fileNames.size() != imageDataList.size()) {
        qDebug() << "uploadingGraphFiles: size mismatch - fileNames:"
                 << fileNames.size() << "imageData:" << imageDataList.size();
        return;
    }

    const QString dirName = state.pathGraph;
    saveFiles(fileNames, imageDataList, dirName);
}

// void MainWindow::setServiceStatus(const QString &status) {
//     Label[1]->setText(status);
// }
void MainWindow::setServiceStatus(const QString& status) {
    validateUi();  // Проверка ui

    //Проверка индекса
       if (UiConstants::SERVICE_LABEL_INDEX >= UiConstants::MAX_LABEL) {
        qDebug() << "setServiceStatus: label index out of range";
        return;
    }

    //Проверка на nullptr
    if (!Label[UiConstants::SERVICE_LABEL_INDEX]) {
        qDebug() << "setServiceStatus: Label[1] is nullptr";
        return;
    }

    Label[UiConstants::SERVICE_LABEL_INDEX]->setText(status);
}


// Основная функция
void MainWindow::showProgressBar(float testExecutionTime) {
    validateUi();

    using namespace UiConstants;

    // 1.Валидация
    if (!isValidTime(testExecutionTime)) {
        qDebug() << "showProgressBar: invalid time" << testExecutionTime;
        return;
    }

    // 2.Проверка progressBar
    if (!ui->progressBar) {
        qDebug() << "showProgressBar: progressBar is nullptr";
        return;
    }

    // 3.Расчет времени
    totalTime = static_cast<int>(testExecutionTime * SECONDS_PER_MINUTE);

    // 4.Настройка прогресс-бара
    setupProgressBar(totalTime);
    setupProgressBarStyle();
    setupProgressBarFont();

    // 5.Настройка таймера
    setupTimer();
}

// 1.Валидация времени
bool  MainWindow::isValidTime(float testExecutionTime) const {
    return testExecutionTime > 0.0f && testExecutionTime < 1000.0f;  // Разумный предел
}

// 2.Настройка диапазона прогресс-бара
void MainWindow::setupProgressBar(int totalSeconds) {
    using namespace UiConstants;

    ui->progressBar->setRange(PROGRESS_BAR_MIN_VALUE, totalSeconds);
    ui->progressBar->setValue(PROGRESS_BAR_MIN_VALUE);
    ui->progressBar->setTextVisible(true);
    ui->progressBar->show();
}

// 3.Настройка стиля прогресс-бара
void MainWindow::setupProgressBarStyle() {
    using namespace UiConstants;

    ui->progressBar->setStyleSheet(PROGRESS_BAR_STYLE);
}

// 4.Настройка шрифта прогресс-бара
void MainWindow::setupProgressBarFont() {
    using namespace UiConstants;

    QFont font = ui->progressBar->font();
    font.setBold(true);
    font.setPointSize(PROGRESS_BAR_FONT_SIZE);
    ui->progressBar->setFont(font);
}

// 5. Настройка таймера
void MainWindow::setupTimer() {
    // Очистка старого таймера
    cleanupOldTimer();

    timer = new QTimer(this);

    // Отключаем старые соединения, чтобы избежать дублирования
    disconnect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
    timer->start(UiConstants::TIMER_INTERVAL_MS);
}

// 6. Очистка старого таймера
void MainWindow::cleanupOldTimer() {
    if (timer) {
        if (timer->isActive()) {
            timer->stop();
        }
        timer->deleteLater();  // Безопасное удаление
        timer = nullptr;
    }
}

// void MainWindow::updateProgress() {
//     if (currentTime < totalTime ) {
//         currentTime++;
//         ui->progressBar->setValue(currentTime);
//         int remainingTime = (totalTime ) - currentTime;
//         QTime time(0, 0);
//         time = time.addSecs(remainingTime);
//         Label[1]->setText("Текущее время теста: "+time.toString("mm:ss"));
//     } else {
//         timer->stop();
//         Label[1]->setText("Тест завершен!");
//         currentTime=0;
//         disconnect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
//         onPushButtonEndTestClicked();
//     }
// }


// Основная функция
void MainWindow::updateProgress() {
    validateUi();

    if (!isProgressComplete()) {
        // Обновление прогресса
        updateProgressValue();

        const int remainingSeconds = totalTime - currentTime;
        updateProgressDisplay(remainingSeconds);
    } else {
        // Завершение теста
        finishTest();
    }
}

// 1.Проверка завершения
bool MainWindow::isProgressComplete() const {
    return currentTime >= totalTime;
}

// 2.Обновление значения прогресса
void MainWindow::updateProgressValue() {
    if (currentTime < totalTime) {
        currentTime++;
        updateProgressBar(currentTime);
    }
}

// 3.Обновление отображения прогресса
void MainWindow::updateProgressDisplay(int remainingSeconds) {
    using namespace UiConstants;

    QTime time(0, 0);
    time = time.addSecs(remainingSeconds);
    const QString timeString = time.toString(TIME_FORMAT);
    const QString statusText = PROGRESS_TEXT_PREFIX + timeString;

    updateStatusLabel(statusText);
}

// 4.Завершение теста
void MainWindow::finishTest() {
    stopAndDisconnectTimer();
    resetProgressState();
    updateStatusLabel(UiConstants::TEST_COMPLETED_TEXT);
    onPushButtonEndTestClicked();
}

// 5.Сброс состояния прогресса
void MainWindow::resetProgressState() {
    currentTime = 0;
    if (ui->progressBar) {
        ui->progressBar->setValue(0);
    }
}

// 6.Обновление статусной метки
void MainWindow::updateStatusLabel(const QString& text) {
    if (Label[1]) {
        Label[1]->setText(text);
    } else {
        qDebug() << "updateStatusLabel: Label[1] is nullptr";
    }
}

// 7.Обновление прогресс-бара
void MainWindow::updateProgressBar(int value) {
    if (ui->progressBar) {
        ui->progressBar->setValue(value);
    } else {
        qDebug() << "updateProgressBar: progressBar is nullptr";
    }
}

// 8.Остановка и отключение таймера
void MainWindow::stopAndDisconnectTimer() {
    if (timer) {
        if (timer->isActive()) {
            timer->stop();
        }
        disconnect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
    } else {
        qDebug() << "stopAndDisconnectTimer: timer is nullptr";
    }
}
int MainWindow::getMaxThemeIndex() const {
    if (m_cachedThemeCount == -1) {
        // Принудительно загружаем, если еще не загружено
        const_cast<MainWindow*>(this)->fetchThemesFromDatabase();
    }
    return m_cachedThemeCount;
}
