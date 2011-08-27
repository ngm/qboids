
#include "landscape.hpp"

#include <QDebug>

Landscape::Landscape( QWidget *parent )
	: QGraphicsView( parent )
{
	qDebug() << width() << height();
	setAlignment( Qt::AlignLeft | Qt::AlignTop );
//	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
}

Landscape::~Landscape()
{}


// void Landspace::drawBackground( QPainter * painter, const QRectF & rect )
// {

// }

