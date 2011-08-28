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
      avoidWeight_( 1 ),
      targetWeight_( 1 )
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
    flock2 = new Flock(
                landscapeView_, landscapeScene_,
                moveWeight_, matchWeight_,
                avoidWeight_, targetWeight_,
                this);
    flock1 = new Flock(
                landscapeView_, landscapeScene_,
                moveWeight_, matchWeight_,
                avoidWeight_, targetWeight_,
                this);

    flock1->createBoids(INITIAL_NUM_BOIDS);
    flock2->createBoids(INITIAL_NUM_BOIDS);
}


void MainWindow::updateBoids()
{
    ticksSinceStart_++;

    flock1->updateBoids(flock2, 0);
    flock2->updateBoids(flock1, 200);

    landscapeScene_->update();
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


int MainWindow::ticksSinceStart()
{
    return ticksSinceStart_;
}


