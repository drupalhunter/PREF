#ifndef ABSTRACTVIEWMODE_H
#define ABSTRACTVIEWMODE_H

#include <QtCore>
#include <QtGui>
#include "qhexedit/qhexedit.h"

class AbstractViewMode : public QObject
{
    Q_OBJECT

    public:
        explicit AbstractViewMode(QHexEdit* hexedit, QObject *parent = 0);
        virtual qint64 length() const = 0;
        virtual qint64 offset(const QVector2D &v) const = 0;
        virtual void render(QPainter&, qint64 width);

    protected:
        virtual qint64 preferredHeight(const QPainter& painter);

    protected:
        QHexEdit* _hexedit;
        qint64 _width;
};

#endif // ABSTRACTVIEWMODE_H
