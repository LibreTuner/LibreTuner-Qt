#include "downloadwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

DownloadWidget::DownloadWidget(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QFormLayout *form = new QFormLayout;
    layout->addLayout(form);

    editName_ = new QLineEdit;
    editId_ = new QLineEdit;
    comboPlatform_ = new QComboBox;
    comboDatalink_ = new QComboBox;

    form->addRow("Name:", editName_);
    form->addRow("Id:", editId_);
    form->addRow("Platform:", comboPlatform_);
    form->addRow("Datalink:", comboDatalink_);

    QPushButton *buttonDownload = new QPushButton("Download");
    layout->addWidget(buttonDownload);

    connect(buttonDownload, &QPushButton::clicked, [this]() {
        emit downloadClicked(editName_->text(), editId_->text(), comboDatalink_->currentData().toUInt(), comboPlatform_->currentData().toString());
        close();
    });
}

void DownloadWidget::setDatalinks(const std::vector<std::string> &datalinks)
{
    for (auto i = 0; i < datalinks.size(); ++i) {
        comboDatalink_->addItem(QString::fromStdString(datalinks[i]), QVariant(i));
    }
}

void DownloadWidget::setPlatforms(const std::vector<std::pair<std::string, std::string> > &platforms)
{
    for (auto &platform : platforms) {
        comboPlatform_->addItem(QString::fromStdString(platform.first), QVariant(QString::fromStdString(platform.second)));
    }
}
