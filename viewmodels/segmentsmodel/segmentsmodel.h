#ifndef SEGMENTSMODEL_H
#define SEGMENTSMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

class SegmentsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit SegmentsModel(DisassemblerListing* listing, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        DisassemblerListing* _listing;
        QList<Segment*> _segments;
        QFont _monospacefont;
};

#endif // SEGMENTSMODEL_H
