#ifndef MAPDATABASE_H
#define MAPDATABASE_H

#include <QAbstractListModel>
#include <QList>
#include <QMap>
#include <QUrl>
#include <QGeoCoordinate>


class Feature
{
public:
    Feature(int id, QString name, QString type, double lat, double longg);
    int FEATURE_ID;
    QString FEATURE_NAME = "";
    QString FEATURE_CLASS = "";
    double PRIM_LAT_DEC;
    double PRIM_LONG_DEC;
};

class MapDataBase : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MapDataBase(QObject *parent = nullptr);

    Q_INVOKABLE QStringList getDataTypes();
    Q_INVOKABLE QList<double> getACenterPoint();

    int rowCount(const QModelIndex &index = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void openFile(QUrl earl);
    void setType(QString type);

signals:
    void dataTypesChanged();
    void errorMsg(QString error);
    void typeChanged();

private:
    QStringList                     m_dataTypes;
    QMap<QString, QList<Feature*>>  m_featuresByType;
    QString                         m_current;

    void processLine(QString line);

};

#endif // MAPDATABASE_H
