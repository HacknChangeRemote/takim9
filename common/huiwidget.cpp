#include "huiwidget.h"

#include <iostream>
#include <QQmlEngine>
#include <QApplication>

HUIWidget::HUIWidget() :
    mRenderer(nullptr)
{
    setClearColor(Qt::transparent);
    setAttribute(Qt::WA_AlwaysStackOnTop, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setAttribute(Qt::WA_AcceptTouchEvents, true);

    QSurfaceFormat tFormat = format();
    tFormat.setSamples(0);
    setFormat(tFormat);

    setResizeMode(SizeRootObjectToView);

    connect(this, &HUIWidget::statusChanged, this, &HUIWidget::onStatusChanged);

    installEventFilter(this);

    connect(engine(), &QQmlEngine::quit, [=]{
        QApplication::quit();
    });

}

void HUIWidget::onStatusChanged(QQuickWidget::Status pStatus)
{
    if(pStatus == QQuickWidget::Ready)
    {

    }
}

void HUIWidget::setRenderer(harf::render::HIRenderer *renderer)
{
    mRenderer = renderer;
}

void HUIWidget::mousePressEvent(QMouseEvent *pEvent)
{
    QQuickWidget::mousePressEvent(pEvent);

    if(mRenderer)
    {
        int tButton = 0;
        if(pEvent->button() == Qt::MouseButton::LeftButton)
            tButton = 1;
        if(pEvent->button() == Qt::MouseButton::RightButton)
            tButton = 2;
        if(pEvent->button() == Qt::MouseButton::MiddleButton)
            tButton = 3;

        mRenderer->onMouseEventRaw(0, pEvent->pos().x(), pEvent->pos().y(), tButton);
    }
}

void HUIWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    QQuickWidget::mouseMoveEvent(pEvent);

    if(mRenderer)
    {
        int tButton = 0;
        if(pEvent->button() == Qt::MouseButton::LeftButton)
            tButton = 1;
        if(pEvent->button() == Qt::MouseButton::RightButton)
            tButton = 2;
        if(pEvent->button() == Qt::MouseButton::MiddleButton)
            tButton = 3;

        mRenderer->onMouseEventRaw(2, pEvent->pos().x(), pEvent->pos().y(), tButton);
    }
}

void HUIWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    QQuickWidget::mouseReleaseEvent(pEvent);

    if(mRenderer)
    {
        int tButton = 0;
        if(pEvent->button() == Qt::MouseButton::LeftButton)
            tButton = 1;
        if(pEvent->button() == Qt::MouseButton::RightButton)
            tButton = 2;
        if(pEvent->button() == Qt::MouseButton::MiddleButton)
            tButton = 3;

        mRenderer->onMouseEventRaw(1, pEvent->pos().x(), pEvent->pos().y(), tButton);
    }
}

void HUIWidget::wheelEvent(QWheelEvent *pEvent)
{
    QQuickWidget::wheelEvent(pEvent);

    if(mRenderer)
        mRenderer->onMouseEventRaw(3, pEvent->pos().x(), pEvent->pos().y(), 3);
}

void HUIWidget::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    QQuickWidget::mouseDoubleClickEvent(pEvent);

    if(mRenderer)
    {
        int tButton = 0;
        if(pEvent->button() == Qt::MouseButton::LeftButton)
            tButton = 1;
        if(pEvent->button() == Qt::MouseButton::RightButton)
            tButton = 2;
        if(pEvent->button() == Qt::MouseButton::MiddleButton)
            tButton = 3;

        mRenderer->onMouseEventRaw(4, pEvent->pos().x(), pEvent->pos().y(), tButton);
    }
}

void HUIWidget::tabletEvent(QTabletEvent *pEvent)
{
    QQuickWidget::tabletEvent(pEvent);

    std::cout << "Tablet Event" << std::endl;
}

bool HUIWidget::event(QEvent *pEvent)
{
#ifdef __ANDROID__
    if(pEvent->type() == QEvent::TouchBegin)
    {
        std::cout << "pressed" << std::endl;
        QTouchEvent* tEvent = dynamic_cast<QTouchEvent*>(pEvent);
        mRenderer->onMouseEventRaw(0, tEvent->touchPoints().first().pos().x(), tEvent->touchPoints().first().pos().y(), 1); //same as leftbutton
    }
    else if(pEvent->type() == QEvent::TouchEnd)
    {
        QTouchEvent* tEvent = dynamic_cast<QTouchEvent*>(pEvent);
        mRenderer->onMouseEventRaw(1, tEvent->touchPoints().first().pos().x(), tEvent->touchPoints().first().pos().y(), 1); //same as leftbutton
    }
    else if(pEvent->type() == QEvent::TouchUpdate)
    {
        QTouchEvent* tEvent = dynamic_cast<QTouchEvent*>(pEvent);
        mRenderer->onMouseEventRaw(2, tEvent->touchPoints().first().pos().x(), tEvent->touchPoints().first().pos().y(), 1); //same as leftbutton
    }
    else if(pEvent->type() == QEvent::TouchCancel)
    {
        QTouchEvent* tEvent = dynamic_cast<QTouchEvent*>(pEvent);
        mRenderer->onMouseEventRaw(1, tEvent->touchPoints().first().pos().x(), tEvent->touchPoints().first().pos().y(), 1); //same as leftbutton
    }
#endif

    return QQuickWidget::event(pEvent);
}
