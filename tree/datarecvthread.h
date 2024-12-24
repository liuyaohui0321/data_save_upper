#ifndef DATARECVTHREAD_H
#define DATARECVTHREAD_H

#include <QObject>

class DataRecvThread : public QObject
{
    Q_OBJECT
public:
    explicit DataRecvThread(QObject *parent = nullptr);

signals:

};

#endif // DATARECVTHREAD_H
