#include "hopenglwidget.h"

#include <QElapsedTimer>
#include <QMouseEvent>
#include <QTimer>

HOpenGLWidget::HOpenGLWidget() :
    mRenderer(nullptr)
  , mTargetRenderFps(20)
  , mCurrentRenderTimeMs(50)
  , mTimeBuff(0)
{
    setAcceptDrops(true);
    setMouseTracking(true);
    
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    
    //    installEventFilter(this);
    
    mRenderTimer.setSingleShot(true);
    connect(&mRenderTimer, &QTimer::timeout, this, [=]{
        update();
    });
    
    connect(this, &QOpenGLWidget::frameSwapped, [=]{

        if(mTargetRenderFps == 0)
            update();
        else if(!mRenderTimer.isActive())
        {
            if(mRenderer)
            {
                if(mTimeBuff < 0)
                    mTimeBuff = 0;

                if(qAbs(mTimeBuff) > 100)
                    mTimeBuff = 100;

//                int tBuff = mCurrentRenderTimeMs - mTimeBuff;

//                std::cout << "Dead time:" << tBuff << std::endl;

//                mRenderTimer.start(mCurrentRenderTimeMs - tBuff);
                mRenderTimer.start(mCurrentRenderTimeMs);
            }
        }
    });
}

void HOpenGLWidget::paintGL()
{
//    mElapsed.restart();

#if defined(H_OS_ANDROID)
    glClear(GL_COLOR_BUFFER_BIT);
#endif
    
    if(mRenderer != nullptr)
        mRenderer->doRender();

//    mTimeBuff = mElapsed.elapsed();
}

void HOpenGLWidget::resizeGL(int w, int h)
{
    if(mRenderer != nullptr)
        mRenderer->doResize(w, h);
}

void HOpenGLWidget::setTargetRenderFps(int targetRenderFps)
{
    mTargetRenderFps = targetRenderFps;
    if(mTargetRenderFps != 0)
        mCurrentRenderTimeMs = 1000.0f / ((float) mTargetRenderFps);
    else
        mCurrentRenderTimeMs = 1;
}

void HOpenGLWidget::setRenderer(harf::render::HIRenderer *renderer)
{
    mRenderer = renderer;
}

