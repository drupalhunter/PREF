#include "categorymodel.h"

CategoryModel::CategoryModel(QObject *parent): QAbstractItemModel(parent)
{
    QImage img;
    img.load(":/misc_icons/res/category.png");

    this->_icocategory = img.scaled(16, 16, Qt::KeepAspectRatio);
}

int CategoryModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant CategoryModel::headerData(int, Qt::Orientation, int) const
{
    return QVariant();
}

QVariant CategoryModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole && index.column() == 0)
    {
        CategoryManager::CategoryPtr c = CategoryManager::category(index.row());
        return c->name();
    }
    else if((role == Qt::DecorationRole) && (index.column() == 0))
        return this->_icocategory;

    return QVariant();
}

QModelIndex CategoryModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex CategoryModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int CategoryModel::rowCount(const QModelIndex &) const
{
    return CategoryManager::categoryCount();
}

Qt::ItemFlags CategoryModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
