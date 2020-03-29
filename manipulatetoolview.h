#ifndef MANIPULATETOOLVIEW_H
#define MANIPULATETOOLVIEW_H

#include "toolcontroller.h"
#include <QSharedPointer>
#include <QPoint>
#include <QImage>
#include <QCursor>

class QKeyEvent;
class QMouseEvent;
class TerrainViewDrawingContext;

enum class ManipulateMode : int;

class TerrainEdit;

class ManipulateToolView : public ToolView
{
    Q_OBJECT
public:
    ManipulateToolView(ManipulateMode mode, Session *session, TerrainView *view);
    ~ManipulateToolView();

private:
    Session *session;
    TerrainView *view;
    ManipulateMode mode;

    bool holdAction = false;
    bool hover;
    bool otherActive;
    QPoint pos;

    bool active;
    int dragStartY;
    float dragStartHeight;
    QSharedPointer<TerrainEdit> edit;
    int lastY = 0;

    QImage highlightImage;

    QCursor cursor;

private slots:
    void clientKeyPressed(QKeyEvent *);
    void clientKeyReleased(QKeyEvent *);
    void clientMousePressed(QMouseEvent *);
    void clientMouseReleased(QMouseEvent *);
    void clientMouseMoved(QMouseEvent *);
    void terrainPaint(TerrainViewDrawingContext *);
};

#endif // MANIPULATETOOLVIEW_H
