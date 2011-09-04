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

    void initialiseWeights();
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
    static const int DEFAULT_MOVE_WEIGHT = 1;
    static const int ALTERNATE_MOVE_WEIGHT = -5;

    int matchWeight_;
    static const int DEFAULT_MATCH_WEIGHT = 1;
    static const int ALTERNATE_MATCH_WEIGHT = -1;

    int avoidWeight_;
    static const int DEFAULT_AVOID_WEIGHT = 1;
    static const int ALTERNATE_AVOID_WEIGHT = -1;

    int targetWeight_;
    static const int DEFAULT_TARGET_WEIGHT = 1;
    static const int ALTERNATE_TARGET_WEIGHT = 1;

    int ticksSinceStart_;

    Landscape *landscapeView_;
    LandscapeScene *landscapeScene_;
};
