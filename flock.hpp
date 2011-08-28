
#include <QVector>

class Landscape;
class LandscapeScene;
class Boid;

class Flock {
    public:
        Flock(Landscape *landscapeView, LandscapeScene *landscapeScene);
        ~Flock();

        void createBoids(int numBoids);

        QVector<Boid*> getBoids();
        int numberOfBoids();
        void addBoid(Boid *boid);
        void removeBoid();


    private:
        QVector<Boid*> boids;
        Landscape *landscapeView_;
        LandscapeScene *landscapeScene_;
};
