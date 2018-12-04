#include "mainwindow.h"
#include "rust_bindings.h"

#include "consolewidget.h"
#include "romswidget.h"
#include "sidebarwidget.h"
#include "downloadwidget.h"

#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QProgressDialog>
#include <QMessageBox>

#include <iostream>

MainWindow::MainWindow(rust::App *app, QWidget *parent) : QMainWindow(parent), app_(app) {
	resize(1280, 960);
	
	// Create a placeholder central widget
	QWidget *central = new QWidget;
	setCentralWidget(central);
	central->hide();


  	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    QDockWidget *consoleDock = new QDockWidget(tr("Console"), this);
    ConsoleWidget *console = new ConsoleWidget(consoleDock);
    consoleDock->setWidget(console);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);

    QDockWidget *romsDock = new QDockWidget(tr("Roms"), this);
    roms_ = new RomsWidget(romsDock);
    romsDock->setWidget(roms_);
    addDockWidget(Qt::TopDockWidgetArea, romsDock);


    connect(roms_, &RomsWidget::downloadRomClicked, [this]() {
        DownloadWidget download;
        download.setDatalinks(app_->datalinks());
        download.setPlatforms(app_->platforms());

        connect(&download, &DownloadWidget::downloadClicked, this, &MainWindow::download);

        download.exec();
    });

    // "Central" (top) widget
    QDockWidget *dashboardDock = new QDockWidget(tr("Dashboard"), this);
    splitDockWidget(romsDock, dashboardDock, Qt::Horizontal);

    // Sidebar dock
    QDockWidget *sidebarDock = new QDockWidget(tr("Sidebar"), this);
    sidebarDock->setWidget(new SidebarWidget(sidebarDock));
    splitDockWidget(dashboardDock, sidebarDock, Qt::Horizontal);

    // Setup corners
    setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);

    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::download(const QString &name, const QString &id, std::size_t datalink, const QString &platform)
{
    try {
        std::cout << "Creating platform link" << std::endl;
        rust::PlatformLink link = app_->createPlatformLink(datalink, platform.toUtf8().data());
        std::cout << "Created link" << std::endl;

        QProgressDialog progressDialog("Downloading ROM...", "Abort", 0, 10000, this);
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.show();

        app_->download(link, id.toStdString(), name.toStdString(), [&progressDialog](float progress) {
            progressDialog.setValue(progress * 10000);
        });
    } catch (const std::runtime_error &err) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Download error");
        msgBox.setText("An error occured while downloading");
        msgBox.setDetailedText(err.what());
        msgBox.exec();
    }
}
