#ifndef HUSERIOBRIDGE_H
#define HUSERIOBRIDGE_H

#include <QObject>

#include <arengine/render/hirenderer.h>

class HUserIOBridge : public QObject
{
    Q_OBJECT
public:
    HUserIOBridge();
    virtual ~HUserIOBridge();

    void setRenderer(harf::render::HIRenderer *renderer);

public slots:
    void onMouseEvent(int pEventType, int pX, int pY, int pButton);

private:
    harf::render::HIRenderer* mRenderer;
};

#endif // HUSERIOBRIDGE_H
