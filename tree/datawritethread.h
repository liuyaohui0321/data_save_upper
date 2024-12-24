#ifndef DATAWRITETHREAD_H
#define DATAWRITETHREAD_H

#include <QObject>

class DataWriteThread : public QObject
{
    Q_OBJECT
public:
    explicit DataWriteThread(QObject *parent = nullptr);

signals:

};

#endif // DATAWRITETHREAD_H
