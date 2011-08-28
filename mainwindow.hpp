#include <QMainWindow>

#include <QVector>
#include <QPointF>
#include <QGraphicsScene>

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


class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    void setMoveWeight( int newWeight );
    void setMatchWeight( int newWeight );
    void setAvoidWeight( int newWeight );
    void setTargetWeight( int newWeight );
    void toggleScatter();
    void toggleAvoid();
    void toggleMatch();
    void addBoid();
    void removeBoid( );
    void addBoid2( );
    void removeBoid2( );
    void toggleTails();
    int ticksSinceStart();

    Flock *flock1;
    Flock *flock2;

public slots:
    void updateBoids();
    void togglePause();
    
private:
    void createActions();
    void createMenus();

    Landscape *landscapeView_;
    LandscapeScene *landscapeScene_;

    void initialiseBoids();
    void initialiseFlock(Flock *flock);
    void setupLandscape();
    void setupTime();
    
    QTimer *timer_;

    QMenu *fileMenu_;
    QMenu *simMenu_;

    QAction *quitAct_;
    QAction *pauseAct_;

    bool showTails_;

    int moveWeight_;
    int matchWeight_;
    int avoidWeight_;
    int targetWeight_;

    int ticksSinceStart_;
};
