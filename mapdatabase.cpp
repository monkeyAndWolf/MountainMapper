#include "mapdatabase.h"

#include <QDebug>
#include <QFile>

MapDataBase::MapDataBase(QObject *parent)
    : QAbstractListModel{parent}
{

}

void MapDataBase::openFile(QUrl earl)
{
    QString fileName = earl.toLocalFile();
    QFile file(fileName);
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            if (!file.atEnd())
            {
                QByteArray ba = file.readLine();
                if (ba.endsWith("FEATURE_ID|FEATURE_NAME|FEATURE_CLASS|STATE_ALPHA|STATE_NUMERIC|COUNTY_NAME|COUNTY_NUMERIC|PRIMARY_LAT_DMS|PRIM_LONG_DMS|PRIM_LAT_DEC|PRIM_LONG_DEC|SOURCE_LAT_DMS|SOURCE_LONG_DMS|SOURCE_LAT_DEC|SOURCE_LONG_DEC|ELEV_IN_M|ELEV_IN_FT|MAP_NAME|DATE_CREATED|DATE_EDITED\r\n"))
                {
                    while (!file.atEnd())
                    {
                        processLine(file.readLine());
                    }
                    emit dataTypesChanged();
                }
                else
                {
                    emit errorMsg(fileName + " has incorrect header details");
                }
            }
            else
            {
                emit errorMsg(fileName + " has no data at all");
            }
        }
        else
        {
            emit errorMsg("Can not open " + fileName);
        }
    }
    else
    {
        emit errorMsg(fileName + " does not exist");
    }
}

Feature::Feature(int id, QString name, QString type, double lat, double longg)
{
    this->FEATURE_ID = id;
    this->FEATURE_NAME = name;
    this->FEATURE_CLASS = type;
    this->PRIM_LAT_DEC = lat;
    this->PRIM_LONG_DEC = longg;
}

void MapDataBase::processLine(QString line)
{
    QStringList list = line.split("|");

    if (list.length() == 20)
    {
        Feature *feature = new Feature(list[0].toInt(), list[1].trimmed(), list[2].trimmed(), list[9].toDouble(), list[10].toDouble());

        QList<Feature*> featuresOfAType;
        if (m_featuresByType.keys().contains(feature->FEATURE_CLASS)) {
            featuresOfAType = m_featuresByType[feature->FEATURE_CLASS];
        }
        featuresOfAType.append(feature);
        if (!m_dataTypes.contains(feature->FEATURE_CLASS))
        {
            m_dataTypes.append(feature->FEATURE_CLASS);
        }
        m_featuresByType[feature->FEATURE_CLASS] = featuresOfAType;
    }
}

QStringList MapDataBase::getDataTypes()
{
//    return m_featuresByType.keys();
    return m_dataTypes;
}

void MapDataBase::setType(QString type)
{
    if (m_featuresByType.keys().contains(type))
    {
        if (type != m_current)
        {
            beginRemoveRows(QModelIndex(), 0, m_featuresByType[m_current].length()-1);
            endRemoveRows();
            m_current = type;
            beginInsertRows(QModelIndex(), 0, m_featuresByType[m_current].length()-1);
            endInsertRows();
            emit typeChanged();
        }
    }
}
int MapDataBase::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    int arsehole = m_featuresByType[m_current].length();
    return m_featuresByType[m_current].length();
}
QVariant MapDataBase::data(const QModelIndex &index, int role) const
{
    QVariant var;
    int row = index.row();
    if (!(row < 0 || row > m_featuresByType[m_current].length()))
    {
        switch (role) {
        case (Qt::UserRole+1):
            var = m_featuresByType.keys().at(row);
            break;
        case (Qt::UserRole+2):
            var = m_featuresByType[m_current].at(row)->FEATURE_NAME;
            break;
        case (Qt::UserRole+3):
            var = m_featuresByType[m_current].at(row)->PRIM_LAT_DEC;
            break;
        case (Qt::UserRole+4):
            var = m_featuresByType[m_current].at(row)->PRIM_LONG_DEC;
            break;
        default:
            var = "lol";
        break;
        }
    }
    return var;
}
QHash<int, QByteArray> MapDataBase::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Qt::UserRole+1, "key");
    roles.insert(Qt::UserRole+2, "name");
    roles.insert(Qt::UserRole+3, "lat");
    roles.insert(Qt::UserRole+4, "longi");
    return roles;
}
QList<double> MapDataBase::getACenterPoint()
{
    Feature *feato = m_featuresByType[m_current].at(0);
    QList<double> dub;
    dub.append(feato->PRIM_LAT_DEC);
    dub.append(feato->PRIM_LONG_DEC);
    return dub;
}

