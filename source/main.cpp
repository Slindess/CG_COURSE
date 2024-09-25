#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.setFocus();
    mainWindow.show();
    return app.exec();
}
