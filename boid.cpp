
#include "boid.hpp"

#include <math.h>

#include <QBrush>
#include <QPolygonF>
#include <QPainter>
#include <QPolygonF>

#include <QDebug>

#define PI 3.14159265

#define NUM_TRAIL_POINTS 250

Boid::Boid(int id, QPointF position)
{
	id_ = id;
	count_ = 0;
	trail_.fill(position, NUM_TRAIL_POINTS);
	setPos(position.x(), position.y());
	velocity_ = QPointF( 0.1, 0.1 );
	tailOn_ = true;
}

Boid::~Boid()
{}

QPointF Boid::velocity() const
{
	return velocity_;
}

void Boid::setVelocity( QPointF velocity )
{
	velocity_ = velocity;
}

QRectF Boid::boundingRect() const
{
	return QRectF( -1, -1, 2, 2 );
}

void Boid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		 QWidget *widget)
{
	Q_UNUSED( option ); Q_UNUSED( widget );
	painter->setRenderHints( QPainter::Antialiasing );

	QPen pen;
	
	int baseHue = msSinceStart_ % 359;
	int trailLength = trail_.size()-1;

	for ( int pointIndex = 0; pointIndex < trailLength; pointIndex++ )
	{
		if ( trail_.at(pointIndex+1).isNull() ) {
			continue;
		}

		int value;
		int saturation;

		// The 'head' of the trail.
 		if ( pointIndex < 50 ) 
		{
			pen.setWidthF( sin( pointIndex ) + 2 );
			saturation = 255 - ( pow( sin( 50*static_cast<float>(NUM_TRAIL_POINTS-pointIndex) ), 2 ) * 200);
			value = (static_cast<float>(NUM_TRAIL_POINTS-pointIndex)/NUM_TRAIL_POINTS)*255;
			pen.setBrush( QColor::fromHsv( std::min( baseHue+7*id_, 359 ), saturation, value ) );
			painter->setPen( pen );
			painter->drawLine( mapFromScene( trail_[pointIndex] ), mapFromScene( trail_[pointIndex+1] ) );

		} else if ( pointIndex >= 50 && tailOn_ ) // The rest of the trail.
		{
			pen.setWidth( 1 );
			value = (static_cast<float>(NUM_TRAIL_POINTS-pointIndex)/NUM_TRAIL_POINTS)*150;
			saturation = value;
			pen.setBrush( QColor::fromHsv( std::min( baseHue+7*id_, 359 ), saturation, value ) );
			painter->setPen( pen );
			painter->drawLine( mapFromScene( trail_[pointIndex] ), mapFromScene( trail_[pointIndex+1] ) );

		}

//  		if ( pointIndex > ( NUM_TRAIL_POINTS-150 ) ) {
// 			pen.setWidthF( ( float( (pointIndex-(NUM_TRAIL_POINTS-150)) )/NUM_TRAIL_POINTS ) *160 );
//  		}



	}

	pen.setWidth( 1 );
	pen.setBrush( Qt::yellow );
	pen.setColor( Qt::yellow );
	painter->setPen( pen );
	painter->drawEllipse( -2, -2, 4, 4 );
}

void Boid::addToTrail( int x, int y )
{
	QPointF point( x, y );
	trail_.pop_back();
	trail_.prepend( point );
}

void Boid::limitVelocity()
{
	int vlim = 10;

	float magnitude = sqrt( pow(velocity_.x(),2) + pow(velocity_.y(),2) );

	if ( magnitude > vlim )
	{
		setVelocity( ( velocity_ / magnitude )  * vlim );
	}
}

void Boid::setVelocityX( int velX )
{
	velocity_.setX( velX );
}

void Boid::setVelocityY( int velY )
{
	velocity_.setY( velY );
}

void Boid::setMsSinceStart( int msSinceStart )
{
	msSinceStart_ = msSinceStart;
}

void Boid::setTail( bool tailOn )
{
	tailOn_ = tailOn;
 	if ( tailOn == true )
 		trail_.resize( NUM_TRAIL_POINTS );
 	else 
 		trail_.resize( 50 );
}
