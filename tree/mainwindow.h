#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include <QTreeView>
#include "datawritethread.h"
#include "datarecvthread.h"
#include "menu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define DSV_PORT                8511        //端口号
#define ORDERHEAD               0xBCBCAAAA
#define DSV_PACKET_HEADER       0x55555555 // 包头
#define DSV_PACKET_TAIL         0xAAAAAAAA// 包尾

typedef struct
{
    uint32_t order_head;    //指令的头

    //原协议内容
    uint32_t head ;         //包头
    uint32_t source_ID ;    //源组件ID，0
    uint32_t dest_ID ;      //目的组件ID，0
    uint32_t oper_type ;    //操作类型
    uint32_t oper_ID ;      //操作ID
    uint32_t package_num;   //包序列号，0
    uint32_t fun_type ;     //指令类型：1->新建文件 2->新建文件夹
    char16_t fun_para1[1024];
    char16_t fun_para2[1024];
    uint32_t check;         //校验码
    uint32_t end ;          //包尾

} Cmd_Folder_And_File_Info;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showContextMenu(const QPoint &pos);
private slots:
    void slotRefresh();

private:
    Ui::MainWindow *ui;
    menu* contextMenu;
    QAction* refreshAction;
    QTcpSocket *m_socket;
    QTcpServer *m_tcpSever;
    //多线程对tcp操作，用于接收和发送数据
    DataRecvThread *m_dataRecvThread;
    DataWriteThread *m_dataWriteThread;
    QThread  *writeThread;           //用于数据写入
    QThread  *recvThread;            //用于数据接收
};
#endif // MAINWINDOW_H
