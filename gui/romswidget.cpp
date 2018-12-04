#include "romswidget.h"

#include "rustapp.h"
#include "rust_bindings.h"

#include <QVBoxLayout>
#include <QPushButton>


RomsWidget::RomsWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    treeView_ = new QTreeView;
    layout->addWidget(treeView_);

    QPushButton *downloadButton = new QPushButton(tr("Download ROM"));
    layout->addWidget(downloadButton);

    connect(downloadButton, &QPushButton::clicked, [this]() {
        emit downloadRomClicked();
    });
}



void RomsWidget::setModel(QAbstractItemModel *model)
{
    treeView_->setModel(model);
}



RomsModel::RomsModel(rust::App *app) : app_(app)
{

}

void RomsModel::setApp(rust::App *app)
{
    app_ = app;
}



QModelIndex RomsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!app_) {
        return QModelIndex();
    }

    if (parent.isValid() && parent.internalId() != 0) {
        return QModelIndex();
    }

    if (row < 0 || column < 0) {
        return QModelIndex();
    }

    if (column > 3) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        if (row < app_->romsLen()) {
            return createIndex(row, column);
        }
    }

    int rom_id = parent.row();

    if (row < app_->tunesLen(rom_id)) {
        return createIndex(row, column, reinterpret_cast<void*>(rom_id + 1));
    }
    return QModelIndex();
}



QModelIndex RomsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || child.internalId() == 0) {
        return QModelIndex();
    }

    return createIndex(child.internalId() - 1, 0);
}



int RomsModel::rowCount(const QModelIndex &parent) const
{
    if (!app_) {
        return 0;
    }

    if (!parent.isValid()) {
        return app_->romsLen();
    }

    if (parent.internalId() == 0) {
        return app_->tunesLen(parent.row());
    }

    return 0;
}



int RomsModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}



QVariant RomsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !app_) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (index.column() != 0) {
        return QVariant();
    }

    if (index.internalId() == 0) {
        // Rom
        return QVariant(QString::fromStdString(app_->romName(index.row())));
    } else {
        // Tune
        // Get the tune ID
        // This is reeeeeeally ugly and LibreTuner's entire tune/rom model should be changed.
        unsigned tuneId = app_->getTuneId(index.internalId() - 1, index.row());
        if (tuneId == -1) {
            return QVariant();
        }

        return QVariant(QString::fromStdString(app_->tuneName(tuneId)));
    }
}
