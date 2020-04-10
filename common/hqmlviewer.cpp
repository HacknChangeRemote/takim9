#include "hqmlviewer.h"

#include <iostream>

#define log(x) std::cout << x << std::endl

HQMLViewer::HQMLViewer(QObject *parent) :
    mRenderer(nullptr)
  , m_t(0)
  , m_renderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &HQMLViewer::handleWindowChanged);
}

HQMLViewer::~HQMLViewer()
{

}

void HQMLViewer::handleWindowChanged(QQuickWindow *pWin)
{
    std::cout << "WINDOW CHANGED" << std::endl;

    if (pWin) {
        connect(pWin, &QQuickWindow::beforeSynchronizing, this, &HQMLViewer::sync, Qt::DirectConnection);
        connect(pWin, &QQuickWindow::sceneGraphInvalidated, this, &HQMLViewer::cleanup, Qt::DirectConnection);

        pWin->setClearBeforeRendering(false);
    }
}

void HQMLViewer::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = nullptr;
    }
}

void HQMLViewer::sync()
{
    if (!m_renderer) {
        m_renderer = new HQMLRenderer();
        m_renderer->setRenderer(mRenderer);
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &HQMLRenderer::paint, Qt::DirectConnection);
    }

    m_renderer->setWindow(window());
}

void HQMLViewer::setRenderer(harf::render::HIRenderer *renderer)
{
    if(renderer == nullptr)
        return;

    mRenderer = renderer;

    if(m_renderer)
        m_renderer->setRenderer(mRenderer);
}

void HQMLViewer::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if(mRenderer == nullptr)
        return;

    mRenderer->doResizeGeometry(newGeometry.x(), newGeometry.y(), newGeometry.width(), newGeometry.height());
}

void HQMLRenderer::paint()
{
    if(m_window == nullptr)
        return;

    if(mIsFirstTime)
    {
        createOsgContext();
        restoreOsgState();

        mIsFirstTime = false;
    }
    else // restore the osg gl context
        restoreOsgState();

    if(mRenderer != nullptr)
        mRenderer->doRender();

    // we're done with the osg state, restore the Qt one
    saveOsgState();
}

void HQMLRenderer::createOsgContext()
{
    // create a new shared OpenGL context to be used exclusively by OSG
    _osgContext = new QOpenGLContext();
    _osgContext->setFormat(m_window->requestedFormat());
    _osgContext->setShareContext(QOpenGLContext::currentContext());
    _osgContext->create();
}

void HQMLRenderer::saveOsgState()
{
    _osgContext->doneCurrent();
    _qtContext->makeCurrent(m_window);
}

void HQMLRenderer::restoreOsgState()
{
    _qtContext = QOpenGLContext::currentContext();
    _qtContext->doneCurrent();

    _osgContext->makeCurrent(m_window);
}
