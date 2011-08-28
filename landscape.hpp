
#include <QGraphicsView>
#include <QRectF>

class QPainter;
class QGraphicsScene;

class Landscape : public QGraphicsView {
public:
	Landscape( QWidget *parent = 0 );
	~Landscape();

    int getRandomX();
    int getRandomY();

//	void drawBackground ( QPainter * painter, const QRectF & rect );

private:
	QGraphicsScene *scene;

};
