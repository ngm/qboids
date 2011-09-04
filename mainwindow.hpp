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
class Landscape;
class LandscapeScene;
class Simulation;


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

public slots:
    void togglePause();
    
private:
    void createActions();
    void createMenus();

    Simulation *simulation_;
    Landscape *landscapeView_;
    LandscapeScene *landscapeScene_;

    void setupLandscape();
    void createSimulation();
    
    QMenu *fileMenu_;
    QMenu *simMenu_;

    QAction *quitAct_;
    QAction *pauseAct_;

    int ticksSinceStart_;
};
