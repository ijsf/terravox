#include "manipulatetoolview.h"
#include "manipulatetoolcontroller.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include "terrainview.h"
#include "session.h"
#include "terrain.h"
#include "terrainedit.h"

#if defined(__APPLE__)
#  define memset std::memset
#endif

ManipulateToolView::ManipulateToolView(ManipulateMode mode, Session *session, TerrainView *view) :
    session(session),
    view(view),
    mode(mode),
    hover(false),
    otherActive(false),
    active(false),
    cursor(Qt::SizeVerCursor)
{
    connect(view, SIGNAL(clientKeyPressed(QKeyEvent*)), SLOT(clientKeyPressed(QKeyEvent*)));
    connect(view, SIGNAL(clientKeyReleased(QKeyEvent*)), SLOT(clientKeyReleased(QKeyEvent*)));
    connect(view, SIGNAL(clientMousePressed(QMouseEvent*)), SLOT(clientMousePressed(QMouseEvent*)));
    connect(view, SIGNAL(clientMouseReleased(QMouseEvent*)), SLOT(clientMouseReleased(QMouseEvent*)));
    connect(view, SIGNAL(clientMouseMoved(QMouseEvent*)), SLOT(clientMouseMoved(QMouseEvent*)));
    connect(view, SIGNAL(terrainPaint(TerrainViewDrawingContext*)), SLOT(terrainPaint(TerrainViewDrawingContext*)));

    highlightImage = QImage(8, 8, QImage::Format_RGB32);
    memset(highlightImage.bits(), 0, 8 * 8 * 4);
    for (int i = 0; i < 8; ++i) {
        uint col = 0xff00;
        highlightImage.setPixel(i, 0, col);
        highlightImage.setPixel(i, highlightImage.height() - 1, col);
        highlightImage.setPixel(0, i, col);
        highlightImage.setPixel(highlightImage.width() - 1, i, col);
    }
}

ManipulateToolView::~ManipulateToolView()
{

}

void ManipulateToolView::clientKeyPressed(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Q && hover && !e->isAutoRepeat() && !holdAction) {
        holdAction = true;
        edit = session->beginEdit();
        if (edit) {
            active = true;
            dragStartY = lastY;
            dragStartHeight = session->terrain()->landform(pos.x(), pos.y());
            if (mode == ManipulateMode::Landform)
                view->addCursorOverride(&cursor);
        }
    }
}

void ManipulateToolView::clientKeyReleased(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Q && !e->isAutoRepeat()) {
        holdAction = false;
        if (active) {
            active = false;
            session->endEdit();
            if (mode == ManipulateMode::Landform)
                view->removeCursorOverride(&cursor);
        }
    }
}

void ManipulateToolView::clientMousePressed(QMouseEvent *e)
{
    if (e->isAccepted()) {
        otherActive = true;
        return;
    }
    if (e->button() == Qt::LeftButton && hover) {
        edit = session->beginEdit();
        if (edit) {
            active = true;
            dragStartY = e->y();
            dragStartHeight = session->terrain()->landform(pos.x(), pos.y());
            if (mode == ManipulateMode::Landform)
                view->addCursorOverride(&cursor);
            clientMouseMoved(e);
        }
        e->accept();
    }
}

void ManipulateToolView::clientMouseReleased(QMouseEvent *e)
{
    if (active && e->button() == Qt::LeftButton) {
        active = false;
        session->endEdit();
        e->accept();
        if (mode == ManipulateMode::Landform)
            view->removeCursorOverride(&cursor);
    }
    if (e->buttons() == 0) {
        otherActive = false;
    }
    clientMouseMoved(e);
}

void ManipulateToolView::clientMouseMoved(QMouseEvent *e)
{
    lastY = e->y();
    if (active && mode == ManipulateMode::Landform) {
        float height = dragStartHeight + (e->y() - dragStartY) * .07f;
        height = Terrain::quantizeOne(height);

        edit->beginEdit(QRect(pos.x(), pos.y(), 1, 1), session->terrain().data());
        session->terrain()->landform(pos.x(), pos.y()) = height;
        edit->endEdit(session->terrain().data());
    } else {
        hover = false;
        if (e->x() >= 0 && e->y() >= 0 && e->x() < view->width() && e->y() < view->height()) {
            pos = view->rayCast(e->pos());
            auto t = session->terrain();
            if (pos.x() >= 0 && pos.y() >= 0 && pos.x() < t->size().width() && pos.y() < t->size().height())
                hover = true;
        }
        if (active && mode == ManipulateMode::Color) {
            // set color
            if (hover) {
                edit->beginEdit(QRect(pos.x(), pos.y(), 1, 1), session->terrain().data());
                session->terrain()->color(pos.x(), pos.y()) = session->primaryColor();
                edit->endEdit(session->terrain().data());
            }
        }
    }
    view->update();
}

void ManipulateToolView::terrainPaint(TerrainViewDrawingContext *ctx)
{
    if (hover) {
        ctx->projectBitmap(highlightImage, QPointF(pos.x() + .05f, pos.y() + .05f), QSizeF(.9f, .9f), true);
    }
}

