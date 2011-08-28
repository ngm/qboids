
#include <QGraphicsScene>

class MainWindow;

class LandscapeScene: public QGraphicsScene {
public:
    LandscapeScene( MainWindow *mainWin, QWidget *parent = 0 );

    QPointF target() const;

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    void keyPressEvent ( QKeyEvent * keyEvent );

private:
    QPointF target_;
    MainWindow *mainWin_;
};
