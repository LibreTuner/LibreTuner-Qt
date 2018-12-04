#ifndef LIBRETUNER_H
#define LIBRETUNER_H

#include <QApplication>
#include "mainwindow.h"

#include "rustapp.h"
#include "romswidget.h"

class LibreTuner : public QApplication {
	Q_OBJECT
public:
	LibreTuner(int &argc, char *argv[]);

	~LibreTuner() override;

private:
	MainWindow mainWindow_;
    rust::App app_;
    RomsModel romsModel_;
};



#endif
