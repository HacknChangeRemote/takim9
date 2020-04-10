#ifndef HUIWIDGET_H
#define HUIWIDGET_H

#include <QQuickWidget>

#include <arengine/render/hirenderer.h>

class HUIWidget : public QQuickWidget
{
public:
    HUIWidget();

    void setRenderer(harf::render::HIRenderer *renderer);

protected:
    void mousePressEvent(QMouseEvent *pEvent) override;
    void mouseReleaseEvent(QMouseEvent *pEvent) override;
    void mouseMoveEvent(QMouseEvent *pEvent) override;
    void wheelEvent(QWheelEvent *pEvent) override;
    void mouseDoubleClickEvent(QMouseEvent* pEvent) override;
    void tabletEvent(QTabletEvent *pEvent) override;
    bool event(QEvent *pEvent) override;

private slots:
    void onStatusChanged(QQuickWidget::Status pStatus);

private:
    harf::render::HIRenderer* mRenderer;
};

#endif // HUIWIDGET_H
