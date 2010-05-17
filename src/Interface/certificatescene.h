#ifndef CERTIFICATESCENE_H
#define CERTIFICATESCENE_H

#include <QtGui>

class CertificateScene : public QGraphicsScene
{
public:
    CertificateScene(QWidget *parent=0);
protected:
    void contextMenuEvent ( QGraphicsSceneContextMenuEvent * contextMenuEvent );
};

#endif // CERTIFICATESCENE_H
