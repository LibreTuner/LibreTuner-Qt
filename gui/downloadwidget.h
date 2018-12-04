#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <vector>

#include "rustapp.h"

class DownloadWidget : public QDialog
{
    Q_OBJECT
public:
    explicit DownloadWidget(QWidget *parent = nullptr);

    void setDatalinks(const std::vector<std::string> &datalinks);
    void setPlatforms(const std::vector<std::pair<std::string, std::string>> &platforms);

signals:
    void downloadClicked(const QString &name, const QString &id, std::size_t datalink, const QString &platform);

public slots:

private:
    QLineEdit *editName_;
    QLineEdit *editId_;
    QComboBox *comboPlatform_;
    QComboBox *comboDatalink_;
};

#endif // DOWNLOADWIDGET_H
