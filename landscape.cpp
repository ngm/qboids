
#include "landscape.hpp"

#include <QDebug>

Landscape::Landscape( QWidget *parent )
    : QGraphicsView( parent )
{
    qDebug() << width() << height();
    setAlignment( Qt::AlignLeft | Qt::AlignTop );
//  setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
}

Landscape::~Landscape()
{}

int Landscape::getRandomX()
{
    return width()/2 + qrand()%20-20;
}

int Landscape::getRandomY()
{
    return height()/2 + qrand()%20-20;
}

// void Landspace::drawBackground( QPainter * painter, const QRectF & rect )
// {

// }

