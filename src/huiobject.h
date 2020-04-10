#ifndef HUIOBJECT_H
#define HUIOBJECT_H

#include <QObject>

class HUiObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fps READ fps WRITE setFps NOTIFY fpsChanged)
    QString m_fps;

public:
    explicit HUiObject(QObject *parent = nullptr);

QString fps() const
{
    return m_fps;
}

public slots:
void setFps(QString fps)
{
    if (m_fps == fps)
        return;

    m_fps = fps;
    emit fpsChanged(m_fps);
}

signals:

void fpsChanged(QString fps);
};

#endif // HUIOBJECT_H
