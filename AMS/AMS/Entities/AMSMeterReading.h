#ifndef AMSMETERREADING_H
#define AMSMETERREADING_H

#include <QObject>

class AMSMeterReading : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString flatNo
                   READ flatNo
                       CONSTANT)

    Q_PROPERTY(int readingMonth
                   READ readingMonth
                       CONSTANT)

    Q_PROPERTY(int readingYear
                   READ readingYear
                       CONSTANT)

    Q_PROPERTY(int meterReading
                   READ meterReading
                       WRITE setMeterReading
                           NOTIFY meterReadingChanged)

public:

    explicit AMSMeterReading(
        const QString& flatNo,
        int readingMonth,
        int readingYear,
        int meterReading,
        QObject *parent = nullptr);

    QString flatNo() const;

    int readingMonth() const;

    int readingYear() const;

    int meterReading() const;

    void setMeterReading(int meterReading);

signals:

    void meterReadingChanged();

private:

    QString m_flatNo;

    int m_readingMonth;

    int m_readingYear;

    int m_meterReading;
};

#endif // AMSMETERREADING_H