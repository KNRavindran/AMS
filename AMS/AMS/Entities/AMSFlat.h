#ifndef AMSFLAT_H
#define AMSFLAT_H

#include <QObject>

class AMSFlat : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString flatNo READ flatNo CONSTANT)

    Q_PROPERTY(FlatType flatType READ flatType WRITE setFlatType NOTIFY flatTypeChanged)

    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)

public:
    enum FlatType
    {
        Type1BHK,
        Type2BHK,
        Type3BHK
    };
    Q_ENUM(FlatType)

    enum Status
    {
        Vacant,
        Occupied
    };
    Q_ENUM(Status)
    // Flat Type conversion
    //ll
    static QString flatTypeToString(FlatType flatType);

    //ll
    static FlatType stringToFlatType(
        const QString &flatType);
    static int statusToInt(Status status);

    static Status intToStatus(int value);

    // Status conversion
    //static QString statusToString(Status status);

    //static Status stringToStatus(
    //    const QString& status);
    //--
    // Status conversion

    //--

    explicit AMSFlat(
        const QString& flatNo,
        FlatType flatType,
        Status status,
        QObject *parent = nullptr);

    QString flatNo() const;
    FlatType flatType() const;
    Status status() const;

    //void setFlatNo(const QString& flatNo);
    void setFlatType(FlatType flatType);
    void setStatus(Status status);

signals:

    void flatTypeChanged();

    void statusChanged();

private:

    QString m_flatNo;

    FlatType m_flatType;

    Status m_status;
};

#endif