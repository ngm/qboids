#include <QMainWindow>

#include "landscape.hpp"
#include "boid.hpp"

#include <QVector>
#include <QPointF>
#include <QGraphicsScene>

class QTimer;
class QMenu;
class QAction;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
class MainWindow;

class LandscapeScene: public QGraphicsScene {
public:
	LandscapeScene( MainWindow *mainWin, QWidget *parent = 0 );

	QPointF target() const;

protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void keyPressEvent ( QKeyEvent * keyEvent );

private:
	QPointF target_;
	MainWindow *mainWin_;
};

class MainWindow : public QMainWindow {
Q_OBJECT
public:
	MainWindow();
	~MainWindow();

	void setMoveWeight( int newWeight );
	void setMatchWeight( int newWeight );
	void setAvoidWeight( int newWeight );
	void setTargetWeight( int newWeight );
	void toggleScatter();
	void toggleAvoid();
	void toggleMatch();
	void addBoid();
	void removeBoid( );
	void addBoid2( );
	void removeBoid2( );
	void toggleTails();

	QVector<Boid*> flock1;
	QVector<Boid*> flock2;

public slots:
	void updateBoids();
	void togglePause();
	
private:
	void createActions();
	void createMenus();

	Landscape *landscapeView_;
	LandscapeScene *landscapeScene_;

	void initialiseBoids();
	void initialiseFlock(QVector<Boid*> &flock);
    void setupLandscape();
    void setupTime();
    void updateFlock1(int xmin, int xmax, int ymin, int ymax);
    void updateFlock2(int xmin, int xmax, int ymin, int ymax);
	
	void boundBoid( Boid *boid, int xmin, int xmax, int ymin, int ymax );
	QPointF moveTowardsCentre( Boid *boid, QVector<Boid*> flock );
	QPointF avoidObjects( Boid *boid, QVector<Boid*> );
	QPointF avoidOtherFlock( Boid *boid, QVector<Boid*> );
	QPointF matchVelocity( Boid *boid, QVector<Boid*> );

	float distanceBetween( Boid *boid1, Boid *boid2 );

	QTimer *timer_;

	QMenu *fileMenu_;
	QMenu *simMenu_;

	QAction *quitAct_;
	QAction *pauseAct_;

	bool showTails_;

	int moveWeight_;
	int matchWeight_;
	int avoidWeight_;
	int targetWeight_;

	int ticksSinceStart_;

	
};
