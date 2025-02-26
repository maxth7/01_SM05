#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSharedMemory sharedMemory("MyUniqueAppKey");
    if (!sharedMemory.create(1)) {
        QMessageBox::warning(nullptr, "Warning", "Приложение уже запущено.");
        return 0;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
