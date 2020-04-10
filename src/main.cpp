//qt
#include <QApplication>
#include <QQuickWidget>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QPainter>

//std
#include <iostream>

//common
//#include "androidhelper.h"
#include "hqmlviewer.h"
#include "huseriobridge.h"

//harfsdk
#include <harfcore/core/device/hdevicefactory.h>
#include <arengine/harenginefactory.h>
#include <stream/video/hicamerasendstream.h>
#include <stream/hstreamfactory.h>
#include <harfcore/utils/hparameters.h>
#include <arengine/cv/tracker/hitracker.h>
#include <arengine/cv/tracker/hmarkertrackerdefinitions.h>
#include <harfcore/stream/hstreamhelper.h>

#include "hviewer.h"
#include "huiwidget.h"
#include "hopenglwidget.h"
#include "huiobject.h"

void init(HViewer *pViewer);
void initTracker();
void renderCompleted();

void loadModelFile(std::string pModelPath);
void sceneItemLoaderStarted(harf::scene::HISceneItem *pItem);
void sceneItemLoaderStopped(harf::scene::HISceneItem *pItem);
void mouseHandlerFunction(HEventStructure pEvent, harf::scene::HISceneItem* pItem);
void aspectRefSizeChanged(int pW, int pH);
void rendererGeometryChanged(int pW, int pH);
void fitToScreen();
void onCameraIntrinsicsUpdated(float pFx, float pFy, float pCx, float pCy, int pWidth, int pHeight);

void bgLayerVideoPropCB(int pWidth, int pHeight, int pFps, harf::stream::HVideoFormat pFormat);
void bgLayerReceivedCB(char* ptr, unsigned long long size);

harf::scene::HISyntheticScene* m3DScene = nullptr;
bool mModelLoadingState = false;
bool mEditorOpened = false;
HViewer *mViewer = nullptr;
harf::scene::HIARLayer* mLayer = nullptr;
harf::scene::HIARLayer* mLayer2 = nullptr;
harf::process::HImageBuffer* mImageBuffer = nullptr;

int mRefLayerWidth = 0;
int mRefLayerHeight = 0;
harf::render::HIRenderer* mRenderer = nullptr;
QImage* mPaintArea = nullptr;
QPainter* mPainter = nullptr;

HUiObject* mUiObj = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, true);
    a.setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents, true);

#if defined(__ANDROID__)
    AndroidHelper::initialize(true);
#endif

    HViewer* tViewer = new HViewer();

    init(tViewer);

    tViewer->show();

    return a.exec();
}

void init(HViewer *pViewer)
{
    if(pViewer == nullptr)
        return;

    mViewer = pViewer;

    std::cout << "Viewer Width: " << mViewer->width() << " Height: " << mViewer->height() << std::endl;

    harf::utils::device::HIDevice* tDevice = HDeviceFactory::getDevice();

    mRenderer = harf::HAREngineFactory::createRenderer(mViewer->width(), mViewer->height());
    mRenderer->setFPSMeasurementEnabled(true);
    HOpenGLWidget *tGLWidget = new HOpenGLWidget();
    tGLWidget->setRenderer(mRenderer);
    tGLWidget->setTargetRenderFps(0);

    HUIWidget* tUIWidget = new HUIWidget();
    tUIWidget->setRenderer(mRenderer);
    tUIWidget->setSource(QUrl("qrc:/main.qml"));

    mUiObj = new HUiObject();
    tUIWidget->rootContext()->setContextProperty("uiObject", mUiObj);

    mViewer->getMainLayout()->addWidget(tUIWidget);//this widget top on
    mViewer->getMainLayout()->addWidget(tGLWidget);

    //layer scene
    harf::scene::HILayerScene* tLayerScene = harf::HAREngineFactory::createLayerScene(mRenderer, 0, 0, mViewer->width(), mViewer->height(), "layerscene", 0);
    //    tLayerScene->enableMouseHandler(std::bind(&mouseHandlerFunction, std::placeholders::_1, std::placeholders::_2));

    mLayer = tLayerScene->createARLayer("bglayer", 0, true);
    tLayerScene->addLayer(mLayer);

    //auto control for aspect ratio changes
    //    tRenderer->enableAspectRatioProtector(tLayer);
    mLayer->setSizeChangedFunction(std::bind(&aspectRefSizeChanged, std::placeholders::_1, std::placeholders::_2));
    mRenderer->setContextGeometryChangedFunction(std::bind(&rendererGeometryChanged, std::placeholders::_1, std::placeholders::_2));
    mRenderer->setGLUpdateFunction(std::bind(renderCompleted));

    mLayer2 = tLayerScene->createARLayer("paintLayer", 1, false);
    tLayerScene->addLayer(mLayer2);

    tLayerScene->prepareLayer();

    mLayer->loadFromFile(tDevice->getDataPath() + "/data/images/hvlLogo.png");
    mLayer2->loadFromFile(tDevice->getDataPath() + "/data/images/hvlLogo.png");

    //synthetic scene
    m3DScene = harf::HAREngineFactory::createSyntheticScene(mRenderer,
                                                            0, 0, mViewer->width(), mViewer->height(),
                                                            575.586975f, 0.00001f, 10000000.0f,
                                                            "scene3d", 2);
    //    m3DScene->enableMouseHandler(std::bind(&mouseHandlerFunction, std::placeholders::_1, std::placeholders::_2));

    m3DScene->setStartItemLoadingFunction(std::bind(sceneItemLoaderStarted, std::placeholders::_1));
    m3DScene->setEndItemLoadingFunction(std::bind(sceneItemLoaderStopped, std::placeholders::_1));

    mRenderer->setMouseEventFunction(std::bind(&mouseHandlerFunction, std::placeholders::_1, std::placeholders::_2));
    mRenderer->enableMouseEventHandler();

    QString tPath = QString::fromStdString(tDevice->getDataPath() + "/data/models/ship.obj");
    tPath = tPath.replace("\\", "/");
    loadModelFile(tPath.toStdString());

    //pose tracker
    initTracker();

    //video capture
    harf::stream::video::HICameraSendStream* mCapturer = harf::stream::HStreamFactory::createCameraSendStream();
    mCapturer->setInput(harf::stream::HInput::INPUT_FROM_CAMERA);
    mCapturer->setOutput(harf::stream::HOutput::OUTPUT_TO_BUFFER);
    mCapturer->setFormat(harf::stream::HVideoFormat::BGRx);
    mCapturer->setRawDataFunction(std::bind(&bgLayerReceivedCB, std::placeholders::_1, std::placeholders::_2));
    mCapturer->setVideoPropertyFunction(std::bind(&bgLayerVideoPropCB, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,std::placeholders::_4));
    mCapturer->setResolution(640, 480);
    mCapturer->initialize(harf::stream::HCodec::CODEC_NONE);

    mCapturer->play();
}

void initTracker()
{
    //tracker
    harf::utils::HParameters tTrackerParams;

    tTrackerParams.set<float>("fx", 575.586975f);
    tTrackerParams.set<float>("fy", 575.586975f);
    //for 640x480 capture resolution!
    tTrackerParams.set<float>("cx", 319.5f);
    tTrackerParams.set<float>("cy", 239.5f);
    tTrackerParams.set<int>("dictionary", MarkerTrackerDictionaryTypes::DICT_ARUCO_ORIGINAL);
    tTrackerParams.set<int>("searchId", 1);
    tTrackerParams.set<float>("markersidesize", 0.182f);

    harf::cvis::tracker::HITracker* tARTracker = harf::HAREngineFactory::createArucoTrackerProcess(tTrackerParams);

    mImageBuffer = new harf::process::HImageBuffer();
    mImageBuffer->setBufferLimit(2);
    mImageBuffer->setBufferManagementMethod(harf::process::BufferManagementMethod::BMM_RetainNew);

    tARTracker->setProcessStructure(mImageBuffer, m3DScene->getInputBuffer(), harf::process::HPipelineProcessorBase::PT_NOTHREAD);
    tARTracker->setCameraIntrinsicsFunction(std::bind(&onCameraIntrinsicsUpdated, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    tARTracker->calculatePoseForHarf();
}

void aspectRefSizeChanged(int pW, int pH)
{
    mRefLayerWidth = pW;
    mRefLayerHeight = pH;
    std::cout << "Reference layer size changed. New Width: " << pW << " New Height: " << pH << std::endl;
    fitToScreen();
}

void rendererGeometryChanged(int pW, int pH)
{
    fitToScreen();
    std::cout << "Renderer geometry changed: W: " << mViewer->width() << " Y: " << mViewer->height() << std::endl;
}

void fitToScreen()
{
    if(mRefLayerWidth == 0 || mRefLayerHeight == 0)
        return;

    if(mViewer == nullptr)
        return;

    double tCurrWidth = mViewer->width();
    double tCurrHeight = mViewer->height();
    double tCurrentAspectRatio = tCurrWidth/tCurrHeight;

    std::cout << "Current W: " << tCurrWidth << " H: " << tCurrHeight << " R: " << tCurrentAspectRatio << std::endl;

    double tNewHeight = 0.0;
    double tNewWidth = 0.0;

    double tW1 = (double) mRefLayerWidth;
    double tW2 = (double) mRefLayerHeight;

    double mAspectRatio = tW1 / tW2;

    if(tCurrentAspectRatio > mAspectRatio)
    {
        tNewHeight = tCurrHeight;
        tNewWidth = mAspectRatio * tNewHeight;
    }
    else
    {
        tNewWidth = tCurrWidth;
        tNewHeight = (1.0/mAspectRatio) * tNewWidth;
    }

    std::cout << "New W: " << tNewWidth << " H: " << tNewHeight << std::endl;

    if(QThread::currentThread() == QCoreApplication::instance()->thread())
        mViewer->updateLayoutGeometry((mViewer->width() - tNewWidth)/2, (mViewer->height() - tNewHeight)/2, tNewWidth, tNewHeight);
    else
        QMetaObject::invokeMethod(mViewer, "updateLayoutGeometry",
                                  Q_ARG(int, (mViewer->width() - tNewWidth)/2),
                                  Q_ARG(int, (mViewer->height() - tNewHeight)/2),
                                  Q_ARG(int, tNewWidth),
                                  Q_ARG(int, tNewHeight)); //GUI Thread
}

void sceneItemLoaderStarted(harf::scene::HISceneItem *pItem)
{
    (void) pItem;
    mModelLoadingState = true;
    std::cout << "Loading Started" << std::endl;
}

void sceneItemLoaderStopped(harf::scene::HISceneItem *pItem)
{
    mModelLoadingState = false;
    std::cout << "Object Loaded" << std::endl;

    if(pItem == nullptr)
        return;

    harf::scene::HISceneItem* tLoadedModel = pItem;

    //    tLoadedModel->setPosition(0.0, 0.0, -0.2);
    //    tLoadedModel->setPosition(0.0, 0.0, 0.0);
    tLoadedModel->createEditor(harf::scene::HISceneItem::DRAGGER_TYPE_TrackballDragger);
    //    tLoadedModel->releaseEditor();
    std::cout << "after postioning" << std::endl;
}

void loadModelFile(std::string pModelPath)
{
    m3DScene->loadItemAsync(pModelPath, 0.065f);
}

bool mPaintEnabled = false;

void mouseHandlerFunction(HEventStructure pEvent, harf::scene::HISceneItem* pItem)
{
    (void) pItem; //unused

    if(mViewer == nullptr)
        return;

    int tX = 0;
    int tY = 0;
    int tW = mViewer->width()*0.1;
    int tH = mViewer->height()*0.1;

    switch (pEvent.eventType)
    {
    case HEventType::DOUBLECLICK:
    {
        QColor tColor = QColor::fromRgba(qRgba(0,0,0,0));
        mPaintArea->fill(tColor);
        break;
    }
    case HEventType::MOVE:
    {
        if(!mPaintEnabled)
            return;

        mPainter->drawPoint(pEvent.x, -pEvent.y);

        break;
    }
    case HEventType::DRAG: //like move event for touch display devices
    {
        if(!mPaintEnabled)
            return;

        mPainter->drawPoint(pEvent.x, -pEvent.y);

        break;
    }
    case HEventType::PUSH:
    {
        mPaintEnabled = true;

        break;
    }
    case HEventType::RELEASE:
    {
        mPaintEnabled = false;
        break;
    }
    }
}

void onCameraIntrinsicsUpdated(float pFx, float pFy, float pCx, float pCy, int pWidth, int pHeight)
{
    if(m3DScene)
        m3DScene->updateCameraIntrinsics(pFx, pFy, pCx, pCy, pWidth, pHeight);

    std::cout << "scene camera params updated." << std::endl;
}

void bgLayerVideoPropCB(int pWidth, int pHeight, int pFps, harf::stream::HVideoFormat pFormat)
{
    if(mLayer != nullptr)
        mLayer->bufferControl(pWidth, pHeight, pFps, pFormat);

    if(mImageBuffer != nullptr)
        mImageBuffer->bufferControl(pWidth, pHeight, pFps, pFormat);

    if(mPaintArea != nullptr)
    {
        delete mPaintArea;
        mPaintArea = nullptr;
    }
    if(mPainter != nullptr)
    {
        delete mPainter;
        mPainter = nullptr;
    }

    if(mLayer2 != nullptr)
    {
        mPaintArea = new QImage(pWidth, pHeight, QImage::Format_RGBA8888);
        QColor tColor = QColor::fromRgba(qRgba(0,0,0,0));
        mPaintArea->fill(tColor);
        mPainter = new QPainter(mPaintArea);
        mPainter->setPen(QPen(Qt::red, 3));
        mPainter->setBrush(QBrush(Qt::green));

        mLayer2->bufferControl(pWidth, pHeight, pFps, pFormat);
    }
}

void bgLayerReceivedCB(char *ptr, unsigned long long size)
{
    if(mLayer != nullptr)
        mLayer->acceptBuffer(ptr, size);

    if(mImageBuffer != nullptr)
        mImageBuffer->acceptBuffer(ptr, size);

    if(mLayer2 != nullptr)
        mLayer2->acceptBuffer((char*) mPaintArea->bits(), size);
}

void renderCompleted()
{
    if(mRenderer == nullptr)
        return;

    static int tCount = 0;
    static float tList[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};

    tList[tCount] = mRenderer->renderFps();

    if(tCount == 9)
    {
        tCount = 0;

        float tTemp = 0.0f;
        for(int t = 0; t<10; t++)
            tTemp += tList[t];

        mUiObj->setFps(QString::number(tTemp * 0.1f));
    }

    tCount++;
}
