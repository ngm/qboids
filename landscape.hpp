
#include <QGraphicsView>
#include <QRectF>
#include <QPointF>

class QPainter;
class QGraphicsScene;

class Landscape : public QGraphicsView {
public:
	Landscape( QWidget *parent = 0 );
	~Landscape();

    int getRandomX();
    int getRandomY();
    QPointF getRandomPoint();

//	void drawBackground ( QPainter * painter, const QRectF & rect );

private:
	QGraphicsScene *scene;
};
