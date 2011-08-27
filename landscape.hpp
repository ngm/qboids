
#include <QGraphicsView>
#include <QRectF>

class QPainter;
class QGraphicsScene;

class Landscape : public QGraphicsView {
public:
	Landscape( QWidget *parent = 0 );
	~Landscape();

//	void drawBackground ( QPainter * painter, const QRectF & rect );

private:
	QGraphicsScene *scene;

};
