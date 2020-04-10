#ifndef HOPENGLWIDGET_H
#define HOPENGLWIDGET_H

#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTimer>

#include <arengine/harenginefactory.h>

class HOpenGLWidget : public QOpenGLWidget
{
public:
    HOpenGLWidget();

    void setRenderer(harf::render::HIRenderer *renderer);

    enum class HRenderMode{
        RM_MAX,
        RM_5ms,
        RM_10ms,
        RM_15ms,
        RM_20ms,
        RM_25ms,
        RM_30ms,
        RM_33ms,
        RM_40ms,
        RM_50ms,
        RM_100ms,
        RM_200ms,
        RM_500ms,
        RM_1000ms,
    };

    void setRenderMode(HRenderMode pMode);

    void setTargetRenderFps(int targetRenderFps);

protected:
    void paintGL() override;
    void resizeGL(int w, int h) override;

    //    void mousePressEvent(QMouseEvent *pEvent) override;
    //    void mouseReleaseEvent(QMouseEvent *pEvent) override;
    //    void mouseMoveEvent(QMouseEvent *pEvent) override;
    //    void wheelEvent(QWheelEvent *pEvent) override;
    //    void mouseDoubleClickEvent(QMouseEvent* pEvent) override;

    harf::render::HIRenderer* mRenderer;

    int mTargetRenderFps;
    int mCurrentRenderTimeMs;

    QTimer mRenderTimer;
    QElapsedTimer mElapsed;
    int mTimeBuff;
};

#endif // HOPENGLWIDGET_H
