#include <QVector>
#include <QPointF>
#include <QGraphicsScene>
#include <QObject>


class QTimer;
class QMenu;
class QAction;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
class MainWindow;
class Boid;
class Flock;
class Landscape;
class LandscapeScene;


class Simulation : QObject {
Q_OBJECT
public: 
    Simulation(
        int numBoids, 
        Landscape *landscapeView,
        LandscapeScene *landscapeScene
    );
    ~Simulation();
    void setupTime();
    void initialiseBoids(int numBoids);
    void toggleScatter();
    void toggleMatch();
    void toggleAvoid();
    void toggleTails();
    void togglePause();
    int ticksSinceStart();
    void addBoid();
    void removeBoid( );
    void addBoid2( );
    void removeBoid2( );

public slots:
    void updateBoids();

private:
    Flock *flock1;
    Flock *flock2;
    QTimer *timer_;
    bool showTails_;

    int moveWeight_;
    int matchWeight_;
    int avoidWeight_;
    int targetWeight_;

    int ticksSinceStart_;

    Landscape *landscapeView_;
    LandscapeScene *landscapeScene_;
};
