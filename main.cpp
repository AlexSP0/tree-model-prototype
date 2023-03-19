#include "tree-model-lib/treemodel.h"

#include "mergeproxymodel/mergeproxymodel.h"

#include <QApplication>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWindow>

int main(int argc, char *argv[])
{
    TreeModel model;
    model.populateModel("first");

    TreeModel model2;
    model2.populateModel("second");

    QModelIndex item1       = model.getFirst();
    QModelIndex model2Item1 = model2.getFirst();

    QString str = item1.data(Qt::DisplayRole).toString();

    auto item3 = model.index(0, 0, item1);

    QString item3Str = item3.data(Qt::DisplayRole).toString();

    //Attach model to model

    MergeProxyModel proxyModel(&model);

    proxyModel.attachModel(item3, model2Item1);

    //Get first child index and its data

    auto proxyFirstItem = proxyModel.getFirst();

    QString proxyModelFirstStr = proxyFirstItem.data(Qt::DisplayRole).toString();

    //Get "from" index and its data

    auto proxyItem3 = proxyModel.index(0, 0, proxyFirstItem);

    QString proxyModelItem3Str = proxyItem3.data(Qt::DisplayRole).toString();

    //Get attached index and its data

    auto connectedChilds = proxyModel.index(0, 0, proxyItem3);

    QString connectedChildsStr = connectedChilds.data(Qt::DisplayRole).toString();

    //Gets attached parent index and data

    auto connectedParent = proxyModel.parent(connectedChilds);

    QString connectedParentStr = connectedParent.data(Qt::DisplayRole).toString();

    QApplication app(argc, argv);

    QMainWindow mainWindow;
    QVBoxLayout layout;

    QTreeView firstModelView;
    QTreeView secondModelView;

    QTreeView combinedModelView;

    firstModelView.setModel(&model);
    secondModelView.setModel(&model2);

    combinedModelView.setModel(&proxyModel);

    QStandardItemModel modelStd;

    QStandardItem *item = new QStandardItem("Test");
    modelStd.appendRow(item);

    //combinedModelView.setModel(&modelStd);

    layout.addWidget(&firstModelView, 1);
    layout.addWidget(&secondModelView, 1);
    layout.addWidget(&combinedModelView, 1);

    QWidget centralWidget;
    centralWidget.setLayout(&layout);

    mainWindow.setCentralWidget(&centralWidget);

    mainWindow.show();

    return app.exec();
}
