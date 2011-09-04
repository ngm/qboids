/**
 * QBoids
 *
 * Visual art based on Craig Reynolds' boids algorithm
 * (www.red3d.com/cwr/boids/)
 *
 * Algorithms based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
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
#include "simulation.hpp"


int INITIAL_NUM_BOIDS = 4;

MainWindow::MainWindow()
{
    qsrand( 20 );
    setGeometry( 0, 0, 500, 300 );
    setupLandscape();

    createActions();
    createMenus();

    createSimulation();
}


MainWindow::~MainWindow()
{}

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


void MainWindow::createSimulation()
{
    simulation_ = new Simulation(
            INITIAL_NUM_BOIDS,
            landscapeView_,
            landscapeScene_);
}


void MainWindow::addBoid()
{
    simulation_->addBoid();
}


void MainWindow::addBoid2()
{
    simulation_->addBoid2();
}


void MainWindow::removeBoid()
{
    simulation_->removeBoid();
}


void MainWindow::removeBoid2()
{
    simulation_->removeBoid2();
}


void MainWindow::togglePause()
{
    simulation_->togglePause();
}

void MainWindow::toggleScatter()
{
    simulation_->toggleScatter();
}

void MainWindow::toggleAvoid()
{
    simulation_->toggleAvoid();
}

void MainWindow::toggleMatch()
{
    simulation_->toggleMatch();
}

void MainWindow::toggleTails()
{
    simulation_->toggleTails();
}


int MainWindow::ticksSinceStart()
{
    return simulation_->ticksSinceStart();
}


