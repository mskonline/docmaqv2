#ifndef CERTIFICATESCENE_H
#define CERTIFICATESCENE_H

#include <QtGui>

class CredentialScene : public QGraphicsScene
{
public:
    CredentialScene(QWidget *parent=0);
protected:
    void contextMenuEvent ( QGraphicsSceneContextMenuEvent * contextMenuEvent );
};

#endif // CERTIFICATESCENE_H
