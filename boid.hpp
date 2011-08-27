
#include <QGraphicsItem>
#include <QPointF>

#include <QVector>

class Boid : public QGraphicsItem {
public:
	Boid( int id, int xpos, int ypos );
	~Boid();

	QPointF velocity() const;
	void setVelocity( QPointF velocity );
	void setVelocityX( int velX );
	void setVelocityY( int velY );

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void addToTrail( int x, int y );
	void limitVelocity();

	void setMsSinceStart( int msSinceStart );

	void setTail( bool tailOn );

private:
	QPointF velocity_;
	QVector<QPointF > trail_;
	int count_;
	int id_;
	int msSinceStart_;
	bool tailOn_;
};
