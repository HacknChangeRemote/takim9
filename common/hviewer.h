#ifndef HVIEWER_H
#define HVIEWER_H

#include <QStackedLayout>
#include <QWidget>

class HViewer : public QWidget
{
    Q_OBJECT
public:
    explicit HViewer(QWidget *parent = nullptr);

    QStackedLayout* getMainLayout();

public slots:
    void updateLayoutGeometry(int pX, int pY, int pW, int pH);

private:
    QStackedLayout* mLayout;
};

#endif // HVIEWER_H
