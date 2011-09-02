
#include <QVector>

class Landscape;
class LandscapeScene;
class Boid;
class MainWindow;

class Flock {
    public:
        Flock(
            Landscape *landscapeView, LandscapeScene *landscapeScene,
            int moveWeight, int matchWeight,
            int avoidWeight, int targetWeight,
            MainWindow *mainWin
        );
        ~Flock();

        void createBoids(int numBoids);
        Boid* spawnBoid();

        QVector<Boid*> getBoids();
        int numberOfBoids();
        void addBoid(Boid *boid);
        void removeBoid();
        void updateBoids(Flock *flockToAvoid, int ticksOffset);
        QPointF centreOfFlock();

        void setMoveWeight(int newWeight);
        void setMatchWeight(int newWeight);
        void setAvoidWeight(int newWeight);
        void setTargetWeight(int newWeight);


    private:
        void boundBoid(Boid *boid);
        QPointF moveTowardsCentre(Boid *boid);
        QPointF avoidOtherBoids(Boid *boid);
        QPointF avoidOtherFlock(Boid *boid, Flock *flock);
        QPointF matchVelocity( Boid *boid);

        float distanceBetween(Boid *boid1, Boid *boid2);

        QVector<Boid*> boids;
        Landscape *landscapeView_;
        LandscapeScene *landscapeScene_;
        MainWindow *mainWin_;

        int moveWeight_;
        int matchWeight_;
        int avoidWeight_;
        int targetWeight_;
};
