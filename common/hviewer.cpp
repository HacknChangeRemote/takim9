#include "hviewer.h"

#include <iostream>

HViewer::HViewer(QWidget *parent) : QWidget(parent)
{
    mLayout = new QStackedLayout();
    setLayout(mLayout);
    mLayout->setStackingMode(QStackedLayout::StackAll);

    setStyleSheet("background:rgb(0,0,0)");
}

QStackedLayout *HViewer::getMainLayout()
{
    return mLayout;
}

void HViewer::updateLayoutGeometry(int pX, int pY, int pW, int pH)
{
    if(mLayout != nullptr)
        mLayout->setGeometry(QRect(pX, pY, pW, pH));
}
