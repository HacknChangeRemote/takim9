#include "huseriobridge.h"

using namespace harf::io;

HUserIOBridge::HUserIOBridge() :
    mRenderer(nullptr)
{

}

HUserIOBridge::~HUserIOBridge()
{

}

void HUserIOBridge::onMouseEvent(int pEventType, int pX, int pY, int pButton)
{
    if(mRenderer == nullptr)
        return;

    mRenderer->onMouseEventRaw(pEventType, pX, pY, pButton);
}

void HUserIOBridge::setRenderer(harf::render::HIRenderer *renderer)
{
    mRenderer = renderer;
}
