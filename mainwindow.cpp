/**
 * QBoids
 *
 * Visual art based on Craig Reynolds' boids algorithm
 * (www.red3d.com/cwr/boids/)
 */

#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QSettings>
#include <QGraphicsSceneMouseEvent>

#include <math.h>

#include "mainwindow.hpp"

namespace {
	int INITIAL_NUM_BOIDS = 4;
}

MainWindow::MainWindow()
	: bTails_( true ),
	  moveWeight_( 1 ),
	  matchWeight_( 1 ),
	  avoidWeight_( 1 )
{
	qsrand( 20 );
	setGeometry( 0, 0, 500, 300 );

	landscapeView_ = new Landscape( this );
	landscapeView_->setGeometry( 0, 0, 500, 300 );

	landscapeScene_ = new LandscapeScene( this, this );
	landscapeScene_->setBackgroundBrush( Qt::black );
	landscapeView_->setBackgroundBrush( Qt::black );
	landscapeView_->setScene( landscapeScene_ );

	landscapeScene_->setSceneRect( 0, 0, landscapeView_->viewport()->width(),
				      landscapeView_->viewport()->height() );

	setCentralWidget( landscapeView_ );

	createActions();
	createMenus();

	initialiseBoids();

	timer_ = new QTimer( this );
	connect( timer_, SIGNAL(timeout()), this, SLOT(updateBoids()) );
	timer_->start( 10 );

	msSinceStart_ = 0;

}

MainWindow::~MainWindow()
{}

void MainWindow::createActions()
{
	QSettings settings( "NoodleSoft", "qboids" );

	pauseAct_ = new QAction( tr("Toggle &Pause"), this );
	QByteArray pauseKey = settings.value( "keys/pause", "Space" ).toByteArray();
	pauseAct_->setShortcut( tr( pauseKey.data() ) );
	pauseAct_->setStatusTip( tr("Pause/unpause the simulation") );
	connect( pauseAct_, SIGNAL( triggered() ),
		 this, SLOT( togglePause() ) );

	quitAct_ = new QAction(tr("&Quit"), this);
	quitAct_->setShortcut(tr("Ctrl+Q"));
	quitAct_->setStatusTip(tr("Quit the application"));
	connect( quitAct_, SIGNAL(triggered()), this, SLOT(close()) );
}

void MainWindow::createMenus()
{
	fileMenu_ = menuBar()->addMenu(tr("&File"));
	fileMenu_->addAction(quitAct_);

	simMenu_ = menuBar()->addMenu(tr("&Simulation"));
	simMenu_->addAction( pauseAct_ );
}

void MainWindow::initialiseBoids()
{
	for ( int ii = 0; ii < INITIAL_NUM_BOIDS; ii++ ) 
	{
		int x = landscapeView_->width()/2 + qrand()%20-20;
		int y = landscapeView_->height()/2 + qrand()%20-20;

		Boid *boid = new Boid( ii, x, y );
		boids.push_back( boid );
		landscapeScene_->addItem( boid );
	}

	for ( int ii = 0; ii < INITIAL_NUM_BOIDS; ii++ ) 
	{
		int x = landscapeView_->width() + qrand()%20-20;
		int y = landscapeView_->height() + qrand()%20-20;

		Boid *boid = new Boid( ii, x, y );
		boids2.push_back( boid );
		landscapeScene_->addItem( boid );
	}
}

void MainWindow::updateBoids()
{
	msSinceStart_++;

	int xmin = 0;
	int xmax = landscapeView_->viewport()->width();
	int ymin = 0;
	int ymax = landscapeView_->viewport()->height();

 	foreach ( Boid *boid, boids )
	{
		QPointF v1 = moveWeight_ * moveTowardsCentre( boid, boids );
		QPointF v2 = avoidWeight_ * avoidObjects( boid, boids );
		QPointF v3 = matchWeight_ * matchVelocity( boid, boids );

		QPointF v4 = avoidWeight_ * avoidOtherFlock( boid, boids2 );

		boid->setVelocity( (boid->velocity() + v1 + v2 + v3 + v4 ) );
		boundBoid( boid, xmin, xmax, ymin, ymax );

		boid->limitVelocity();

		boid->setPos( boid->pos() + boid->velocity() );
		boid->setMsSinceStart( msSinceStart_ );
		
		boid->addToTrail( boid->x(), boid->y() );
	}

 	foreach ( Boid *boid, boids2 )
	{
		QPointF v1 = moveWeight_ * moveTowardsCentre( boid, boids2 );
		QPointF v2 = avoidWeight_ * avoidObjects( boid, boids2 );
		QPointF v3 = matchWeight_ * matchVelocity( boid, boids2 );

		QPointF v4 = avoidWeight_ * avoidOtherFlock( boid, boids );
		boid->setVelocity( (boid->velocity() + v1 + v2 + v3 + v4 ) );
		boundBoid( boid, xmin, xmax, ymin, ymax );

		boid->limitVelocity();

		boid->setPos( boid->pos() + boid->velocity() );
		boid->setMsSinceStart( msSinceStart_+200 );
		
		boid->addToTrail( boid->x(), boid->y() );
	}

	landscapeScene_->update();
}

/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
QPointF MainWindow::moveTowardsCentre( Boid *thisBoid, QVector<Boid*> flock )
{
	QPointF flockCentre( 0, 0 );

	foreach ( Boid *boid, flock )
	{
		if ( boid != thisBoid )
		{
			flockCentre += boid->pos();
		}
	}
	flockCentre /= ( flock.size() - 1 );

	return ( flockCentre - thisBoid->pos() ) / 100;
}

/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
QPointF MainWindow::avoidObjects( Boid *thisBoid, QVector<Boid*> flock )
{
	QPointF avoidVec( 0, 0 );

	Boid *boid;

	foreach ( boid, flock )
	{
		if ( boid != thisBoid )
		{
			if ( distanceBetween( boid, thisBoid ) < 20 ) {
				avoidVec -= ( boid->pos() - thisBoid->pos() );
			}
		}
	}
     
	return avoidVec;
}

QPointF MainWindow::avoidOtherFlock( Boid *thisBoid, QVector<Boid*> flock )
{
	QPointF avoidVec( 0, 0 );

	Boid *boid;
	float xDiff, yDiff, magnitude;

	foreach ( boid, flock )
	{
		if ( boid != thisBoid )
		{
			xDiff = boid->pos().x() - thisBoid->pos().x();
			yDiff = boid->pos().y() - thisBoid->pos().y();
			magnitude = sqrt( pow(xDiff,2) + pow(yDiff,2) );

			if ( magnitude < 100 ) {
				avoidVec -= ( boid->pos() - thisBoid->pos() );
			}
		}
	}
     
	return avoidVec*10;
}

/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
QPointF MainWindow::matchVelocity( Boid *thisBoid, QVector<Boid*> flock )
{
	QPointF averageVel( 0, 0 );

	Boid *boid;
	foreach( boid, flock )
	{
		if ( boid != thisBoid )
		{
			averageVel += boid->velocity();
		}
	}

	averageVel /= ( flock.size() - 1 );

	return ( averageVel - thisBoid->velocity() ) / 8;
}

/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
void MainWindow::boundBoid( Boid *boid, int xmin, int xmax,
			    int ymin, int ymax )
{

	if ( !landscapeScene_->target().isNull() )
		boid->setVelocity( boid->velocity() + targetWeight_*( landscapeScene_->target() - boid->pos() )/100 );

	if ( landscapeView_->mapFromScene( boid->scenePos() ).x() > xmax ) {
		boid->setVelocityX( boid->velocity().x() * -10 );
	}
	if ( landscapeView_->mapFromScene( boid->scenePos() ).x() < xmin ) {
		boid->setVelocityX( boid->velocity().x() * -10 );
	}
	if ( landscapeView_->mapFromScene( boid->scenePos() ).y() > ymax ) {
		boid->setVelocityY( boid->velocity().y() * -10 );
	}
	if ( landscapeView_->mapFromScene( boid->scenePos() ).y() < ymin ) {
		boid->setVelocityY( boid->velocity().y() * -10 );
	}
}

void MainWindow::setMoveWeight( int newWeight )
{
	moveWeight_ = newWeight;
}

void MainWindow::setAvoidWeight( int newWeight )
{
	avoidWeight_ = newWeight;
}

void MainWindow::setMatchWeight( int newWeight )
{
	matchWeight_ = newWeight;
}

void MainWindow::setTargetWeight( int newWeight )
{
	targetWeight_ = newWeight;
}

void MainWindow::addBoid()
{
	QPointF centre( 0, 0 );

	foreach ( Boid *boid, boids )
	{
		centre += boid->pos();
	}
	centre /= ( boids.size() );

	Boid *newBoid = new Boid( boids.size(), centre.x(), centre.y() );
	boids.push_back( newBoid );
	landscapeScene_->addItem( newBoid );
}

void MainWindow::removeBoid(  )
{
	int flockSize = boids.size();
	if ( flockSize > 2 ) {
		Boid *boid = boids.at( 0 );
		landscapeScene_->removeItem( boid );
		boids.remove( 0 );
		boids.resize( flockSize - 1 );
		delete boid;
	}
}

void MainWindow::addBoid2()
{
	QPointF centre( 0, 0 );

	foreach ( Boid *boid, boids2 )
	{
		centre += boid->pos();
	}
	centre /= ( boids2.size() );

	Boid *newBoid = new Boid( boids2.size(), centre.x(), centre.y() );
	boids2.push_back( newBoid );
	landscapeScene_->addItem( newBoid );
}

void MainWindow::removeBoid2(  )
{
	int flockSize = boids2.size();
	if ( flockSize > 2 ) {
		Boid *boid = boids2.at( 0 );
		landscapeScene_->removeItem( boid );
		boids2.remove( 0 );
		boids2.resize( flockSize - 1 );
		delete boid;
	}
}


void MainWindow::togglePause()
{
	if ( timer_->isActive() )
		timer_->stop();
	else
		timer_->start();
}

void MainWindow::toggleScatter()
{
	if ( moveWeight_ == 1 )
		moveWeight_ = -5;
	else 
		moveWeight_ = 1;
}

void MainWindow::toggleAvoid()
{
	if ( avoidWeight_ == 1 )
		avoidWeight_ = -1;
	else 
		avoidWeight_ = 1;
}

void MainWindow::toggleMatch()
{
	if ( matchWeight_ == 1 )
		matchWeight_ = -1;
	else 
		matchWeight_ = 1;
}

void MainWindow::toggleTails()
{
	if ( bTails_ == false ) {
		bTails_ = true;
		foreach( Boid *boid, boids )
		{
			boid->setTail( true );
		}
		foreach( Boid *boid, boids2 )
		{
			boid->setTail( true );
		}
	} else if ( bTails_ == true ) {
		bTails_ = false;
		foreach( Boid *boid, boids )
		{
			boid->setTail( false );
		}
		foreach( Boid *boid, boids2 )
		{
			boid->setTail( false );
		}
	}
}

float MainWindow::distanceBetween( Boid* boid1, Boid* boid2 )
{
	float xDiff, yDiff, magnitude;

	xDiff = boid1->pos().x() - boid2->pos().x();
	yDiff = boid1->pos().y() - boid2->pos().y();
	magnitude = sqrt( pow(xDiff,2) + pow(yDiff,2) );

	return magnitude;
}



/**
 * LandscapeScene.
 */
LandscapeScene::LandscapeScene( MainWindow *mainWin, QWidget *parent )
	: QGraphicsScene( parent )
{
	mainWin_ = mainWin;
	target_.setX( 0.0 );
	target_.setY( 0.0 );
}

void LandscapeScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	target_ = event->scenePos();
	if ( event->button() == Qt::LeftButton )
		mainWin_->setTargetWeight( 1 );
	if ( event->button() == Qt::RightButton )
		mainWin_->setTargetWeight( -1 );

}

void LandscapeScene::mouseMoveEvent( QGraphicsSceneMouseEvent *event )

{
	if ( event->buttons() == Qt::LeftButton ) {
		mainWin_->setTargetWeight( 1 );
		target_ = event->scenePos();
	}
	if ( event->buttons() == Qt::RightButton ) {
		mainWin_->setTargetWeight( -1 );
		target_ = event->scenePos();
	}
}

void LandscapeScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	target_.setX( 0.0 );
	target_.setY( 0.0 );
}

void LandscapeScene::keyPressEvent ( QKeyEvent * keyEvent )
{
	switch ( keyEvent->key() )
	{
	case Qt::Key_S:
		mainWin_->toggleScatter();
		break;
	case Qt::Key_A:
		mainWin_->toggleAvoid();
		break;
	case Qt::Key_M:
		mainWin_->toggleMatch();
		break;
	case Qt::Key_Up:
		mainWin_->addBoid();
		break;
	case Qt::Key_Down:
		mainWin_->removeBoid();
		break;
	case Qt::Key_Right:
		mainWin_->addBoid2();
		break;
	case Qt::Key_Left:
		mainWin_->removeBoid2();
		break;
	case Qt::Key_T:
		mainWin_->toggleTails();
		break;
	}

}


QPointF LandscapeScene::target() const
{
	return target_;
}

