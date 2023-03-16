#include "tree-model-lib/treemodel.h"

#include "mergeproxymodel/mergeproxymodel.h"

int main(int argc, char *argv[])
{
    TreeModel model;
    model.populateModel("first");

    TreeModel model2;
    model2.populateModel("second");

    //    QModelIndex m = model.getFirst();

    //    QString str = m.data(Qt::DisplayRole).toString();

    //    auto sd = model.index(0, 0, m);

    //    QString sdsd = sd.data(Qt::DisplayRole).toString();

    //    auto sdd = model.index(0, 1, m);

    //    QString sdsdd = sdd.data(Qt::DisplayRole).toString();

    //    auto in3 = sdd.parent();

    //    QString inn3 = in3.data(Qt::DisplayRole).toString();

    //    MergeProxyModel prModel(&model);

    //    prModel.attachModel(1, 0, &model2);

    //    QModelIndex firstItem = prModel.getFirst();

    //    QModelIndex connectionPoint = prModel.index(0, 1, firstItem);

    //    QModelIndex attachedIndex = prModel.index(0, 1, connectionPoint);

    //    QString data = attachedIndex.data(Qt::DisplayRole).toString();

    return 0;
}
