#ifndef TERRAINVIEW_H
#define TERRAINVIEW_H

#include <QWidget>
#include <QPointer>
#include <memory>
#include <QVector>
#include <QSharedPointer>
#include <QScopedPointer>

class Terrain;
class TerrainViewOptionsWindow;

class TerrainViewDrawingContext
{
protected:
    TerrainViewDrawingContext() { }
    virtual ~TerrainViewDrawingContext() { }
public:
    virtual void projectBitmap(QImage&, const QPointF &origin, const QSizeF &size, bool additive) = 0;
};

struct TerrainViewOptions
{
    bool showEdges;
    bool colorizeAltitude;
    bool ambientOcclusion;
    float ambientOcclusionStrength;
    bool axises;

    TerrainViewOptions() :
        showEdges(true),
        colorizeAltitude(false),
        ambientOcclusion(true),
        ambientOcclusionStrength(0.5f),
        axises(true)
    { }
};

class TerrainView : public QWidget
{
    Q_OBJECT
    struct Coordinate
    {
        short x, y;
        Coordinate() = default;
        Coordinate(short x, short y) :
            x(x), y(y) { }
    };

public:
    explicit TerrainView(QWidget *parent = 0);
    ~TerrainView();

    QPoint rayCast(const QPoint&);
    void setTerrain(QSharedPointer<Terrain>);
    QSharedPointer<Terrain> terrain();

    void setViewOptions(const TerrainViewOptions&);
    const TerrainViewOptions &viewOptions();

    void addCursorOverride(QCursor *);
    void removeCursorOverride(QCursor *);

protected:
    void paintEvent(QPaintEvent *) override;

    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

signals:
    void clientKeyPressed(QKeyEvent *);
    void clientKeyReleased(QKeyEvent *);
    void clientMousePressed(QMouseEvent *);
    void clientMouseReleased(QMouseEvent *);
    void clientMouseMoved(QMouseEvent *);
    void clientPaint(QPaintEvent *);
    void clientEnter(QEvent *);
    void clientLeave(QEvent *);
    void terrainPaint(TerrainViewDrawingContext *);
    void viewOptionsChanged(TerrainViewOptions);

public slots:
    void showOptionsWindow();
    void terrainUpdate(const QRect &);

private:
    struct SceneDefinition;
    class TerrainViewPrivate;
    TerrainViewPrivate *d_ptr;
    QScopedPointer<TerrainViewOptionsWindow> optionsWindow;

    QCursor *cursorOverride = nullptr;

    Q_DECLARE_PRIVATE(TerrainView)

private slots:
    void optionsWindowClosed();

};

#endif // TERRAINVIEW_H
