#ifndef CONSTANTS_H
#define CONSTANTS_H
// constants.h
namespace UiConstants {
constexpr int SETTINGS_FONT_SIZE = 14;
constexpr const char* SETTINGS_BACKGROUND_PATH = ":/images/BackgroundsFP2000/WB00760L.GIF";
// label
constexpr int MAX_LABEL = 2;
constexpr int THEME_LABEL_INDEX = 0;
constexpr int SERVICE_LABEL_INDEX = 1;
constexpr int STATUS_LABEL_INDEX = 1;
// Progress bar constants
constexpr int SECONDS_PER_MINUTE = 60;
constexpr int TIMER_INTERVAL_MS = 1000;
constexpr int PROGRESS_BAR_FONT_SIZE = 10;
constexpr int PROGRESS_BAR_MIN_VALUE = 0;

inline constexpr const char* TIME_FORMAT = "mm:ss";
inline constexpr const char* PROGRESS_TEXT_PREFIX = "Текущее время теста: ";
inline constexpr const char* TEST_COMPLETED_TEXT = "Тест завершен!";

// Progress bar style
inline constexpr const char* PROGRESS_BAR_STYLE =
    "QProgressBar {"
    "background-color: #f0f0f0;"
    "border: 2px solid #8f8f8f;"
    "border-radius: 5px;"
    "text-align: center;"
    "}"
    "QProgressBar::chunk {"
    "background-color: #228B22;"
    "border-radius: 5px;"
    "}";
// Window size constants
constexpr int FIXED_WINDOW_WIDTH = 800;
constexpr int FIXED_WINDOW_HEIGHT = 800;
// Radio button
// limits from MIN_RADIO_BUTTONS  to MAX_ANSWER
constexpr int MIN_RADIO_BUTTONS = 1;
inline constexpr const char* RADIO_BUTTON_STYLE =
    "QRadioButton {"
    "   font-size: 16px;"
    "   color: #4682B4;"
    "   font-weight: bold;"
    "   background-color: #FFFFFF;"
    "}"
    "QRadioButton::indicator {"
    "   width: 30px;"
    "   height: 30px;"
    "}";
// Menu bar style
inline constexpr const char* MENUBAR_STYLE =
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
    "}";
// Push button constants
constexpr int BUTTON_SIZE = 30;
constexpr int BUTTON_FONT_SIZE = 16;
constexpr int BUTTON_PADDING = 2;

// Button styles
inline constexpr const char* FIRST_BUTTON_STYLE =
    "color: white;"
    "border-color: red;"
    "background-color: #4CAF50;"
    "font-weight: bold;"
    "padding: 2px 2px;"
    "text-align: center;"
    "text-decoration: none;"
    "font-size: 16px;";

inline constexpr const char* DEFAULT_BUTTON_STYLE =
    "color: black;"
    "border-color: black;"
    "background-color: #4CAF50;"
    "font-weight: bold;"
    "padding: 2px 2px;"
    "text-align: center;"
    "text-decoration: none;"
    "font-size: 16px;";

// Window size constants
constexpr int DEFAULT_WINDOW_WIDTH = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 600;
constexpr int DEFAULT_WINDOW_POSITION_X = 0;
constexpr int DEFAULT_WINDOW_POSITION_Y = 0;

// Theme index constants
constexpr int MIN_THEME_INDEX = 0;  // 0-based index
constexpr int DEFAULT_THEME_INDEX = 0;

// Tab names
inline constexpr const char* TAB_ANIME_OBJECT_NAME = "tabCustomTabAnime";
inline constexpr const char* TAB_CUSTOM_OBJECT_NAME = "tabcustomTab";
inline constexpr const char* TAB_ANIME_DISPLAY_NAME = "customTabAnime";
inline constexpr const char* TAB_CUSTOM_DISPLAY_NAME = "customTab";

// Tab styles
inline constexpr const char* TAB_ANIME_STYLE = "QTabWidget::pane { border: none; }";

// Animation path
inline constexpr const char* ANIMATION_GIF_PATH = ":/images/Tesla/tesla_m_s.gif";

// Graphics view style
inline constexpr const char* GRAPHICS_VIEW_STYLE = "QGraphicsView { border: none; }";
}

namespace HelpConfig {
// Используем constexpr char[] для строк (оптимально)
inline constexpr char HTML_FIELD[] = "htmlcode";
inline constexpr char TABLE_NAME[] = "help";
inline constexpr char GRAPH_FIELD_FILENAME[] = "filename";
inline constexpr char GRAPH_FIELD_DATA[] = "graph";
inline constexpr char GRAPH_TABLE_NAME[] = "graphhelp";
}
namespace ManualConstants {
// Используем constexpr для эффективности
inline constexpr char TEXT_FIELD[] = "met";
inline constexpr char TABLE_NAME[] = "tasks";
inline constexpr char GRAPH_FIELD[] = "filename";
inline constexpr char IMAGE_FIELD[] = "graph";
inline constexpr char GRAPH_TABLE[] = "graphmet";
}
namespace DbConfig {
inline constexpr char HELP_HTML_FIELD[] = "htmlcode";
inline constexpr char HELP_TABLE[] = "help";
inline constexpr char GRAPH_FILENAME_FIELD[] = "filename";
inline constexpr char GRAPH_DATA_FIELD[] = "graph";
inline constexpr char GRAPH_TABLE[] = "graphhelp";
}
namespace DbConstants {
inline constexpr const char* THEMES_TABLE_NAME = "tasks";
inline constexpr const char* THEMES_FIELD_NAME = "task";

inline constexpr const char* DB_HOST = "localhost";
inline constexpr const char* DB_NAME = "iSmile";
inline constexpr const char* DB_USER = "postgres";
inline constexpr const char* DB_PASSWORD = "800900";
constexpr int DB_PORT = 5432;
}
namespace HtmlConstants {
// Для извлечения блока с background
inline constexpr char BACKGROUND_BLOCK_START[] = "<!--<";
inline constexpr char BACKGROUND_BLOCK_END[] = "-->";

// Для извлечения атрибута background
inline constexpr char BACKGROUND_ATTR_START[] = "background=\"";
inline constexpr char BACKGROUND_ATTR_END[] = "\" ";

inline constexpr const char* BODY_BACKGROUND_PATTERN = "<!--<body background=";
}
namespace ErrorMessages {
inline constexpr const char* APP_START_FAILED = "Не удалось запустить приложение. ";
inline constexpr const char* SERVICE_NOT_RUNNING = "Служба: %1 не запущена.";
}
namespace SettingsConstants {
// File name
inline constexpr const char* SETTINGS_FILE = "settings.ini";

// Groups
inline constexpr const char* GROUP_MAIN_WINDOW = "MainWindow";
inline constexpr const char* GROUP_PATH_GRAPH = "pathGraph";
inline constexpr const char* GROUP_CURRENT_THEMA = "CurrentThema";
inline constexpr const char* GROUP_TEST_TIME = "testExecutionTime";

// Keys
inline constexpr const char* KEY_POS = "pos";
inline constexpr const char* KEY_SIZE = "size";
inline constexpr const char* KEY_PATH = "path";
inline constexpr const char* KEY_CURRENT_THEMA = "CurrentThema";
inline constexpr const char* KEY_TEST_TIME = "testExecutionTime";

// Default values
inline const QPoint DEFAULT_POS = QPoint(0, 0);
inline const QSize DEFAULT_SIZE = QSize(0, 0);
inline constexpr float DEFAULT_TEST_TIME = 0.0f;
}
#endif // CONSTANTS_H
