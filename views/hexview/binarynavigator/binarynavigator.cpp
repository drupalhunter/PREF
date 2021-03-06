#include "binarynavigator.h"

const qreal BinaryNavigator::BYTES_PER_LINE = 0x10;

BinaryNavigator::BinaryNavigator(QWidget *parent): QGLWidget(parent), _hexeditdata(nullptr), _hexedit(nullptr), _size(-1), _maxwidth(-1), _maxheight(-1), _offset(-1)
{
    this->_displaymode = BinaryNavigator::Default;
}

void BinaryNavigator::displayDefault()
{
    this->_displaymode = BinaryNavigator::Default;
    this->update();
}

void BinaryNavigator::displayEntropy()
{
    this->_displaymode = BinaryNavigator::Entropy;
    this->update();
}

void BinaryNavigator::setData(QHexEdit *hexedit)
{
    this->_hexedit = hexedit;
    this->_hexeditdata = hexedit->data();

    connect(this->_hexedit, SIGNAL(positionChanged(qint64)), this, SLOT(updateSquare(qint64)));
    this->renderMap();
}

void BinaryNavigator::renderMap(int)
{
    this->adjust();
    this->update();
}

void BinaryNavigator::renderEntropy(QPainter &p, qint64 x, qint64 y, QRectF& cursorrect, QColor& cursorcolor)
{
    qreal e = entropy(this->_hexeditdata, this->_offset);
    QColor c = ByteColors::entropyColor(e);
    QRectF r = QRectF(x, y, this->_size, this->_size);
    p.fillRect(r, c);

    if(this->_offset == this->_hexedit->cursorPos())
    {
        cursorrect = r;
        cursorcolor = QColor(0xFF, 0xFF, 0x00);
    }
}

void BinaryNavigator::renderByteClass(QPainter &p, qint64 x, qint64 y, QRectF& cursorrect, QColor& cursorcolor)
{
    QHexEditDataReader reader(this->_hexeditdata);
    uchar b = reader.at(this->_offset);
    QColor c = ByteColors::byteClassColor(b);
    QRectF r = QRectF(x, y, this->_size, this->_size);
    p.fillRect(r, c);

    if(this->_offset == this->_hexedit->cursorPos())
    {
        cursorrect = r;
        cursorcolor = QColor(0xFF, 0x00, 0xFF);
    }
}

qint64 BinaryNavigator::indexFromPoint(const QPoint &pt)
{
    qint64 y = pt.y() / this->_size;
    qint64 x = pt.x() / this->_size;

    return this->_hexedit->visibleStartOffset() + (x + (y * BinaryNavigator::BYTES_PER_LINE));
}

void BinaryNavigator::adjust()
{
    if(!this->_hexedit)
        return;

    this->_size = qFloor(static_cast<qreal>(this->width()) / BinaryNavigator::BYTES_PER_LINE);
    this->_maxwidth = this->width();
    this->_maxheight = this->height();
    this->_offset = this->_hexedit->visibleStartOffset();
}

void BinaryNavigator::updateSquare(qint64)
{
    this->adjust();
    this->update();
}

void BinaryNavigator::wheelEvent(QWheelEvent *event)
{
    if(this->_hexedit && this->_hexeditdata)
        this->_hexedit->scroll(event); /* Forward Scroll */

    QGLWidget::wheelEvent(event);
}

void BinaryNavigator::mousePressEvent(QMouseEvent *event)
{
    if(this->_hexedit && ((event->modifiers() == Qt::NoModifier) && (event->button() == Qt::LeftButton)))
    {
        qint64 idx = this->indexFromPoint(event->pos());

        if(idx != -1)
            this->_hexedit->selectPos(idx);
    }
    else
        event->ignore();

    QGLWidget::mousePressEvent(event);
}

void BinaryNavigator::paintEvent(QPaintEvent *)
{
    if(!this->_hexeditdata || ! this->_hexedit || !this->isVisible())
        return;

    this->adjust();

    QColor cursorcolor;
    QRectF cursorrect;
    QPainter p(this);

    for(qreal x = 0, y = 0; (this->_offset < this->_hexeditdata->length()) && (y <= this->_maxheight); x += this->_size, this->_offset++)
    {
        if(x >= this->_maxwidth)
        {
            y += this->_size;
            x = 0;
        }

        switch(this->_displaymode)
        {
            case BinaryNavigator::ByteClass:
                this->renderByteClass(p, x, y, cursorrect, cursorcolor);
                break;

            case BinaryNavigator::Entropy:
                this->renderEntropy(p, x, y, cursorrect, cursorcolor);
                break;

            default:
                break;
        }
    }

    if(!cursorrect.isEmpty())
    {
        p.setPen(cursorcolor);
        p.drawRect(cursorrect);
    }
}
