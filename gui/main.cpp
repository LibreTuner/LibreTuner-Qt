#include "libretuner.h"

#include <QMessageBox>

#include <iostream>

int main(int argc, char **argv) {
    try {
        LibreTuner app(argc, argv);
        app.exec();
    } catch (const std::runtime_error &err) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Initialization error");
        msgBox.setText("An error occured while initializing the internal application");
        msgBox.setDetailedText(err.what());
        msgBox.exec();
    }
}
