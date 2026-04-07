#ifndef CONSTANTS_H
#define CONSTANTS_H
// constants.h
namespace UiConstants {
constexpr int SETTINGS_FONT_SIZE = 14;
constexpr const char* SETTINGS_BACKGROUND_PATH = ":/images/BackgroundsFP2000/WB00760L.GIF";
}

namespace HelpConfig {
// Используем constexpr char[] для строк (оптимально)
inline constexpr char HTML_FIELD[] = "htmlcode";
inline constexpr char TABLE_NAME[] = "help";
inline constexpr char GRAPH_FIELD_FILENAME[] = "filename";
inline constexpr char GRAPH_FIELD_DATA[] = "graph";
inline constexpr char GRAPH_TABLE_NAME[] = "graphhelp";
}
#endif // CONSTANTS_H
