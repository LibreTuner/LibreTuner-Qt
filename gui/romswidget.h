#ifndef ROMSWIDGET_H
#define ROMSWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QAbstractItemModel>

#include "rustapp.h"

class RomsModel : public QAbstractItemModel {
	Q_OBJECT
public:
    RomsModel(rust::App *app = nullptr);

    void setApp(rust::App *app);

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

private:
    rust::App *app_;
};



class RomsWidget : public QWidget {
	Q_OBJECT
public:
	explicit RomsWidget(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model);

signals:
    void downloadRomClicked();

private:
    QTreeView *treeView_;
};

#endif
