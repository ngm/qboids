
#include "boid.hpp"
#include "flock.hpp"
#include "landscape.hpp"
#include "landscapescene.hpp"
#include "mainwindow.hpp"

#include <math.h>


Flock::Flock(
    Landscape *landscapeView, LandscapeScene *landscapeScene,
    int moveWeight, int matchWeight,
    int avoidWeight, int targetWeight,
    MainWindow *mainWin
)
{
    landscapeView_ = landscapeView;
    landscapeScene_ = landscapeScene;

    moveWeight_ = moveWeight;
    matchWeight_ = matchWeight;
    avoidWeight_ = avoidWeight;
    targetWeight = targetWeight;

    mainWin_ = mainWin;
}   


Flock::~Flock()
{}


void Flock::createBoids(int numBoids)
{
    for ( int boidCount = 0; boidCount < numBoids; boidCount++ ) 
    {
        int x = landscapeView_->getRandomX();
        int y = landscapeView_->getRandomY();

        Boid *boid = new Boid( boidCount, x, y );
        boids.push_back( boid );
        landscapeScene_->addItem( boid );
    }
}


void Flock::addBoid(Boid *newBoid)
{
    boids.push_back(newBoid);
}


void Flock::removeBoid()
{
    int flockSize = numberOfBoids();
    if ( flockSize > 2 ) {
        Boid *boid = boids.at( 0 );
        landscapeScene_->removeItem( boid );
        boids.remove( 0 );
        boids.resize( flockSize - 1 );
        delete boid;
    }
}


int Flock::numberOfBoids()
{
    return boids.size();
}


QVector<Boid*> Flock::getBoids()
{
    return boids;
}


void Flock::updateBoids(Flock *flockToAvoid, int ticksOffset)
{
    foreach ( Boid *boid, boids )
    {
        QPointF v1 = moveWeight_ * moveTowardsCentre(boid);
        QPointF v2 = avoidWeight_ * avoidOtherBoids(boid);
        QPointF v3 = matchWeight_ * matchVelocity(boid);
        QPointF v4 = avoidWeight_ * avoidOtherFlock(boid, flockToAvoid);

        boid->setVelocity( (boid->velocity() + v1 + v2 + v3 + v4 ) );
        boundBoid(boid);

        boid->limitVelocity();

        boid->setPos( boid->pos() + boid->velocity() );
        boid->setMsSinceStart( mainWin_->ticksSinceStart() + ticksOffset);
        
        boid->addToTrail( boid->x(), boid->y() );
    }
}


QPointF Flock::moveTowardsCentre(Boid *thisBoid)
{
    return (centreOfFlock() - thisBoid->pos()) / 100;
}


QPointF Flock::centreOfFlock()
{
    QPointF flockCentre(0, 0);

    foreach (Boid *boid, boids)
    {
        flockCentre += boid->pos();
    }
    
    return flockCentre /= (numberOfBoids());
}


/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
QPointF Flock::avoidOtherBoids(Boid *thisBoid)
{
    QPointF avoidVec( 0, 0 );

    Boid *boid;

    foreach (boid, boids)
    {
        if (boid != thisBoid)
        {
            if (distanceBetween(boid, thisBoid) < 20 ) 
            {
                avoidVec -= (boid->pos() - thisBoid->pos());
            }
        }
    }
     
    return avoidVec;
}


float Flock::distanceBetween( Boid* boid1, Boid* boid2 )
{
    float xDiff, yDiff, magnitude;

    xDiff = boid1->pos().x() - boid2->pos().x();
    yDiff = boid1->pos().y() - boid2->pos().y();
    magnitude = sqrt( pow(xDiff,2) + pow(yDiff,2) );

    return magnitude;
}


/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
QPointF Flock::matchVelocity(Boid *thisBoid)
{
    QPointF averageVel(0, 0);

    Boid *boid;
    foreach(boid, boids)
    {
        if (boid != thisBoid)
        {
            averageVel += boid->velocity();
        }
    }

    averageVel /= (numberOfBoids() - 1);

    return (averageVel - thisBoid->velocity()) / 8;
}



QPointF Flock::avoidOtherFlock(Boid *thisBoid, Flock *flockToAvoid)
{
    QPointF avoidVec( 0, 0 );

    Boid *boid;
    float xDiff, yDiff, magnitude;

    foreach (boid, flockToAvoid->getBoids())
    {
        if (boid != thisBoid)
        {
            xDiff = boid->pos().x() - thisBoid->pos().x();
            yDiff = boid->pos().y() - thisBoid->pos().y();
            magnitude = sqrt( pow(xDiff,2) + pow(yDiff,2) );

            if (magnitude < 100) {
                avoidVec -= (boid->pos() - thisBoid->pos());
            }
        }
    }
     
    return avoidVec*10;
}


/**
 * Algorithm based on the pseudocode at:
 * http://www.vergenet.net/~conrad/boids/pseudocode.html
 */
void Flock::boundBoid(Boid *boid)
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


void Flock::setMoveWeight(int newWeight)
{
    moveWeight_ = newWeight;
}


void Flock::setMatchWeight(int newWeight)
{
    matchWeight_ = newWeight;
}


void Flock::setAvoidWeight(int newWeight)
{
    avoidWeight_ = newWeight;
}


void Flock::setTargetWeight(int newWeight)
{
    targetWeight_ = newWeight;
}
