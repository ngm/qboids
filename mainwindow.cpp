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
#include "boid.hpp"
#include "flock.hpp"
#include "landscape.hpp"
#include "landscapescene.hpp"


int INITIAL_NUM_BOIDS = 4;

MainWindow::MainWindow()
    : showTails_( true ),
      moveWeight_( 1 ),
      matchWeight_( 1 ),
      avoidWeight_( 1 )
{
    qsrand( 20 );
    setGeometry( 0, 0, 500, 300 );
    setupLandscape();

    createActions();
    createMenus();

    initialiseBoids();

    setupTime();
}

void MainWindow::setupLandscape()
{
    landscapeView_ = new Landscape( this );
    landscapeView_->setGeometry( 0, 0, 500, 300 );

    landscapeScene_ = new LandscapeScene( this, this );
    landscapeScene_->setBackgroundBrush( Qt::black );
    landscapeView_->setBackgroundBrush( Qt::black );
    landscapeView_->setScene( landscapeScene_ );

    landscapeScene_->setSceneRect( 0, 0, landscapeView_->viewport()->width(),
                      landscapeView_->viewport()->height() );

    setCentralWidget( landscapeView_ );
}


void MainWindow::setupTime()
{
    timer_ = new QTimer( this );
    connect( timer_, SIGNAL(timeout()), this, SLOT(updateBoids()) );
    timer_->start( 10 );
    ticksSinceStart_ = 0;
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
    flock1 = new Flock(landscapeView_, landscapeScene_);
    flock2 = new Flock(landscapeView_, landscapeScene_);
    flock1->createBoids(INITIAL_NUM_BOIDS);
    flock2->createBoids(INITIAL_NUM_BOIDS);
}


void MainWindow::updateBoids()
{
    ticksSinceStart_++;

    // TODO: obviously refactor into method...
    updateFlock(flock1, flock2, 0);
    updateFlock(flock2, flock1, 200);

    landscapeScene_->update();
}


void MainWindow::updateFlock(Flock *flock, Flock *flockToAvoid, int ticksOffset)
{
    foreach ( Boid *boid, flock->getBoids() )
    {
        QPointF v1 = moveWeight_ * moveTowardsCentre( boid, flock );
        QPointF v2 = avoidWeight_ * avoidObjects( boid, flock );
        QPointF v3 = matchWeight_ * matchVelocity( boid, flock );
        QPointF v4 = avoidWeight_ * avoidOtherFlock( boid, flockToAvoid );

        boid->setVelocity( (boid->velocity() + v1 + v2 + v3 + v4 ) );
        boundBoid(boid);

        boid->limitVelocity();

        boid->setPos( boid->pos() + boid->velocity() );
        boid->setMsSinceStart( ticksSinceStart_ + ticksOffset);
        
        boid->addToTrail( boid->x(), boid->y() );
    }
}


/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
QPointF MainWindow::moveTowardsCentre( Boid *thisBoid, Flock *flock )
{
    QPointF flockCentre( 0, 0 );

    foreach ( Boid *boid, flock->getBoids() )
    {
        if ( boid != thisBoid )
        {
            flockCentre += boid->pos();
        }
    }
    flockCentre /= ( flock->numberOfBoids() - 1 );

    return ( flockCentre - thisBoid->pos() ) / 100;
}

/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
QPointF MainWindow::avoidObjects( Boid *thisBoid, Flock *flock )
{
    QPointF avoidVec( 0, 0 );

    Boid *boid;

    foreach ( boid, flock->getBoids() )
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

QPointF MainWindow::avoidOtherFlock(Boid *thisBoid, Flock *flock)
{
    QPointF avoidVec( 0, 0 );

    Boid *boid;
    float xDiff, yDiff, magnitude;

    foreach ( boid, flock->getBoids() )
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
QPointF MainWindow::matchVelocity(Boid *thisBoid, Flock *flock)
{
    QPointF averageVel(0, 0);

    Boid *boid;
    foreach(boid, flock->getBoids())
    {
        if (boid != thisBoid)
        {
            averageVel += boid->velocity();
        }
    }

    averageVel /= ( flock->numberOfBoids() - 1 );

    return ( averageVel - thisBoid->velocity() ) / 8;
}

/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
void MainWindow::boundBoid(Boid *boid)
{
    int xmin = 0;
    int xmax = landscapeView_->viewport()->width();
    int ymin = 0;
    int ymax = landscapeView_->viewport()->height();

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

    foreach (Boid *boid, flock1->getBoids())
    {
        centre += boid->pos();
    }
    centre /= (flock1->numberOfBoids());

    Boid *newBoid = new Boid(flock1->numberOfBoids(), centre.x(), centre.y());
    flock1->addBoid(newBoid);
    landscapeScene_->addItem(newBoid);
}

void MainWindow::removeBoid()
{
    flock1->removeBoid();
}

void MainWindow::addBoid2()
{
    QPointF centre( 0, 0 );

    foreach ( Boid *boid, flock2->getBoids() )
    {
        centre += boid->pos();
    }
    centre /= ( flock2->numberOfBoids() );

    Boid *newBoid = new Boid(flock2->numberOfBoids(), centre.x(), centre.y() );
    flock2->addBoid(newBoid);
    landscapeScene_->addItem( newBoid );
}

void MainWindow::removeBoid2()
{
    flock2->removeBoid();
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
    if ( showTails_ == false ) 
    {
        showTails_ = true;
        foreach( Boid *boid, flock1->getBoids() )
        {
            boid->setTail( true );
        }
        foreach( Boid *boid, flock2->getBoids() )
        {
            boid->setTail( true );
        }
    } 
    else if ( showTails_ == true ) 
    {
        showTails_ = false;
        foreach( Boid *boid, flock1->getBoids() )
        {
            boid->setTail( false );
        }
        foreach( Boid *boid, flock2->getBoids() )
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


