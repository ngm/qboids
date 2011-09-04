
#include "simulation.hpp"

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


Simulation::Simulation(
    int numBoids,
    Landscape *landscapeView,
    LandscapeScene *landscapeScene
)
    : showTails_(true),
      moveWeight_(1),
      matchWeight_(1),
      avoidWeight_(1),
      targetWeight_(1)
{
    landscapeView_ = landscapeView;
    landscapeScene_ = landscapeScene;
    initialiseBoids(numBoids);
    setupTime();
}

Simulation::~Simulation()
{}

void Simulation::setupTime()
{
    timer_ = new QTimer();
    connect( timer_, SIGNAL(timeout()), this, SLOT(updateBoids()) );
    timer_->start( 10 );
    ticksSinceStart_ = 0;
}


void Simulation::initialiseBoids(int numBoids)
{
    flock1 = new Flock(
                landscapeView_, landscapeScene_,
                moveWeight_, matchWeight_,
                avoidWeight_, targetWeight_,
                this);
    flock2 = new Flock(
                landscapeView_, landscapeScene_,
                moveWeight_, matchWeight_,
                avoidWeight_, targetWeight_,
                this);

    flock1->createBoids(numBoids);
    flock2->createBoids(numBoids);
}


void Simulation::updateBoids()
{
    ticksSinceStart_++;

    flock1->updateBoids(flock2, 0);
    flock2->updateBoids(flock1, 200);

    landscapeScene_->update();
}


void Simulation::toggleScatter()
{
    if (moveWeight_ == 1)
    {
        moveWeight_ = -5;
    }
    else 
    {
        moveWeight_ = 1;
    }

    flock1->setMoveWeight(moveWeight_);
    flock2->setMoveWeight(moveWeight_);
}


void Simulation::toggleMatch()
{
    if (matchWeight_ == 1)
    {
        matchWeight_ = -1;
    }
    else 
    {
        matchWeight_ = 1;
    }

    flock1->setMatchWeight(matchWeight_);
    flock2->setMatchWeight(matchWeight_);
}

void Simulation::toggleAvoid()
{
    if (avoidWeight_ == 1)
    {
        avoidWeight_ = -1;
    }
    else 
    {
        avoidWeight_ = 1;
    }

    flock1->setAvoidWeight(avoidWeight_);
    flock2->setAvoidWeight(avoidWeight_);
}

void Simulation::toggleTails()
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

int Simulation::ticksSinceStart()
{
    return ticksSinceStart_;
}


void Simulation::addBoid()
{
    Boid *newBoid = flock1->spawnBoid();
    landscapeScene_->addItem(newBoid);
}


void Simulation::addBoid2()
{
    Boid *newBoid = flock2->spawnBoid();
    landscapeScene_->addItem(newBoid);
}


void Simulation::removeBoid()
{
    flock1->removeBoid();
}


void Simulation::removeBoid2()
{
    flock2->removeBoid();
}


void Simulation::togglePause()
{
    if ( timer_->isActive() )
        timer_->stop();
    else
        timer_->start();
}

