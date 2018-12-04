#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <rustapp.h>

class RomsWidget;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
    explicit MainWindow(rust::App *app, QWidget *parent = nullptr);

    RomsWidget *roms() { return roms_; }

public slots:
    void download(const QString &name, const QString &id, std::size_t datalink, const QString &platform);

private:
    RomsWidget *roms_;
    rust::App *app_;
};

#endif
