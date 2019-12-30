#ifndef HANDLEAUTOUPDATER_H
#define HANDLEAUTOUPDATER_H

#include <QObject>
#include <QProcess>

class HandleAutoUpdater : public QObject
{
    Q_OBJECT
public:
    explicit HandleAutoUpdater(QObject *parent = nullptr);
    //
    void startHandle();

signals:

public slots:

private:
    bool checkHaveUpdateModle();
    //
    QString checkIsUpdateVersion();
    void startCheckUpdateVersion();
    void startZipUpdateVersion();
private:
    QProcess *m_pProcess;
};

#endif // HANDLEAUTOUPDATER_H
