#ifndef HDATA_H
#define HDATA_H

#include <QObject>
#include <QString>

class HData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer NOTIFY manufacturerChanged)
    Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString dataPath READ dataPath WRITE setDataPath NOTIFY dataPathChanged)
    Q_PROPERTY(QString deviceType READ deviceType WRITE setDeviceType NOTIFY deviceTypeChanged)

    QString m_manufacturer;
    QString m_model;
    QString m_dataPath;
    QString m_deviceType;

public:
    HData();
    HData(const HData &pObj);

    QString manufacturer() const
    {
        return m_manufacturer;
    }
    QString model() const
    {
        return m_model;
    }

    QString dataPath() const
    {
        return m_dataPath;
    }

    QString deviceType() const
    {
        return m_deviceType;
    }

public slots:
    void setManufacturer(QString manufacturer)
    {
        if (m_manufacturer == manufacturer)
            return;

        m_manufacturer = manufacturer;
        emit manufacturerChanged(m_manufacturer);
    }
    void setModel(QString model)
    {
        if (m_model == model)
            return;

        m_model = model;
        emit modelChanged(m_model);
    }

    void setDataPath(QString dataPath)
    {
        if (m_dataPath == dataPath)
            return;

        m_dataPath = dataPath;
        emit dataPathChanged(m_dataPath);
    }

    void setDeviceType(QString deviceType)
    {
        if (m_deviceType == deviceType)
            return;

        m_deviceType = deviceType;
        emit deviceTypeChanged(m_deviceType);
    }

signals:
    void manufacturerChanged(QString manufacturer);
    void modelChanged(QString model);
    void dataPathChanged(QString dataPath);
    void deviceTypeChanged(QString deviceType);
};

#endif // HDATA_H
