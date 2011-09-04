
#include "mainwindow.hpp"
#include "landscapescene.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

    
/**
 * LandscapeScene.
 */
LandscapeScene::LandscapeScene( MainWindow *mainWin, QWidget *parent )
    : QGraphicsScene( parent )
{
    mainWin_ = mainWin;
    target_.setX( 0.0 );
    target_.setY( 0.0 );
}

void LandscapeScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    //target_ = event->scenePos();
    //if ( event->button() == Qt::LeftButton )
        //mainWin_->setTargetWeight( 1 );
    //if ( event->button() == Qt::RightButton )
        //mainWin_->setTargetWeight( -1 );
}

void LandscapeScene::mouseMoveEvent( QGraphicsSceneMouseEvent *event )

{
    //if ( event->buttons() == Qt::LeftButton ) {
        //mainWin_->setTargetWeight( 1 );
        //target_ = event->scenePos();
    //}
    //if ( event->buttons() == Qt::RightButton ) {
        //mainWin_->setTargetWeight( -1 );
        //target_ = event->scenePos();
    //}
}

void LandscapeScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    target_.setX( 0.0 );
    target_.setY( 0.0 );
}

void LandscapeScene::keyPressEvent ( QKeyEvent * keyEvent )
{
    switch ( keyEvent->key() )
    {
    case Qt::Key_S:
        mainWin_->toggleScatter();
        break;
    case Qt::Key_A:
        mainWin_->toggleAvoid();
        break;
    case Qt::Key_M:
        mainWin_->toggleMatch();
        break;
    case Qt::Key_Up:
        mainWin_->addBoid();
        break;
    case Qt::Key_Down:
        mainWin_->removeBoid();
        break;
    case Qt::Key_Right:
        mainWin_->addBoid2();
        break;
    case Qt::Key_Left:
        mainWin_->removeBoid2();
        break;
    case Qt::Key_T:
        mainWin_->toggleTails();
        break;
    }

}


QPointF LandscapeScene::target() const
{
    return target_;
}
