
#include "boid.hpp"
#include "flock.hpp"
#include "landscape.hpp"
#include "landscapescene.hpp"


Flock::Flock(Landscape *landscapeView, 
             LandscapeScene *landscapeScene)
{
    landscapeView_ = landscapeView;
    landscapeScene_ = landscapeScene;
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
