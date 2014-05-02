#include "tablemodel.h"

namespace PrefSDK
{
    TableModel::TableModel(int rows, int columns, QObject *parent): FieldDataModel(parent)
    {
        this->_monospacefont.setFamily("Monospace");
        this->_monospacefont.setPointSize(qApp->font().pointSize());
        this->_monospacefont.setStyleHint(QFont::TypeWriter);

        for(int i = 0; i < columns; i++)
            this->_headeritems.append(QString());

        for(int i = 0; i < rows; i++)
        {
            ElementList elements;

            for(int j = 0; j < columns; j++)
                elements.append(nullptr);

            this->_items.append(elements);
        }
    }

    void TableModel::setHeaderItem(int column, const QString &s)
    {
        this->_headeritems[column] = s;
    }

    void TableModel::setItem(int row, int column, FormatElement *element)
    {
        this->_items[row][column] = element;
    }

    int TableModel::columnCount(const QModelIndex &) const
    {
        return this->_headeritems.length();
    }

    QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole && section < this->_headeritems.length())
            return this->_headeritems.at(section);

        return QVariant();
    }

    QVariant TableModel::data(const QModelIndex &index, int role) const
    {
        if(!index.isValid())
            return QVariant();

        if(role == Qt::DisplayRole || role == Qt::FontRole || role == Qt::ForegroundRole)
        {
            const TableModel::ElementList& elements = this->_items.at(index.row());
            FormatElement* formatelement = elements.at(index.column());

            if(!formatelement)
                return QVariant();

            if(role == Qt::DisplayRole)
                return elements.at(index.column())->displayValue();
            else if(role == Qt::FontRole)
            {
                if(formatelement->elementType() == ElementType::Field || formatelement->elementType() == ElementType::FieldArray || formatelement->elementType() == ElementType::BitField)
                {
                    FieldElement* fieldelement = qobject_cast<FieldElement*>(formatelement);

                    if(fieldelement->isInteger())
                        return this->_monospacefont;
                }
            }
            else if(role == Qt::ForegroundRole)
            {
                if(formatelement->elementType() == ElementType::Field)
                {
                    FieldElement* fieldelement = qobject_cast<FieldElement*>(formatelement);

                    if(fieldelement->isOverflowed())
                        return QColor(Qt::red);

                    if(fieldelement->isInteger())
                        return QColor(Qt::darkBlue);

                    if(fieldelement->dataType() == DataType::Character)
                        return QColor(Qt::darkGreen);

                    if(fieldelement->dataType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(formatelement->elementType() == ElementType::FieldArray)
                {
                    FieldArray* fa = qobject_cast<FieldArray*>(formatelement);

                    if(fa->itemType() == DataType::Character)
                        return QColor(Qt::darkGreen);
                    else if(fa->itemType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(formatelement->elementType() == ElementType::BitField)
                    return QColor(Qt::darkRed);
            }
        }

        return QVariant();
    }

    bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        return false;
    }

    QModelIndex TableModel::index(int row, int column, const QModelIndex &parent) const
    {
        if(!this->hasIndex(row, column))
            return QModelIndex();

        return this->createIndex(row, column);
    }

    QModelIndex TableModel::parent(const QModelIndex&) const
    {
        return QModelIndex(); /* It's a list, no parents! */
    }

    int TableModel::rowCount(const QModelIndex&) const
    {
        return this->_items.length();
    }

    Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
    {
        if(!index.isValid())
            return Qt::NoItemFlags;

        return Qt::ItemIsEnabled | Qt::ItemIsSelectable; // | Qt::ItemIsEditable;
    }
}