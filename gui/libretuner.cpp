#include "LibreTuner.h"
#include "error.h"

#include <QMessageBox>


LibreTuner::LibreTuner(int &argc, char *argv[]) : QApplication(argc, argv), mainWindow_(&app_) {
    romsModel_.setApp(&app_);

	mainWindow_.show();
    mainWindow_.roms()->setModel(&romsModel_);
}

LibreTuner::~LibreTuner() {

}
