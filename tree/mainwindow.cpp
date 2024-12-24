#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QTextCodec* codec = QTextCodec::codecForName("GBK");//添加编码格式
    // 创建右键菜单
//    contextMenu = new QMenu(this);
    contextMenu= new menu(this);
//    // 创建刷新菜单项
//    refreshAction = contextMenu->addAction("刷新");
//  connect(refreshAction, &QAction::triggered, this, &MainWindow::onRefreshClicked);


    // 设置窗口接收右键点击事件
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::showContextMenu);
    m_socket = new QTcpSocket();
    m_tcpSever = new QTcpServer(this);
    m_dataWriteThread = new DataWriteThread();
    m_dataRecvThread = new DataRecvThread();
    writeThread = new QThread(this);
    recvThread = new QThread(this);
    m_dataRecvThread->moveToThread(recvThread);
    //右键菜单操作
    connect(contextMenu, &menu::sigRefresh, this, &MainWindow::slotRefresh);

    if (m_tcpSever->listen(QHostAddress::Any, DSV_PORT)) {
        qDebug() << "Server started!" << DSV_PORT;
        QString log = QString("%1: 服务器等待连接中...").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        ui->readtext->append(log);
    }
    connect(m_tcpSever,&QTcpServer::newConnection,
    [=]()
    {
        m_socket=m_tcpSever->nextPendingConnection();//用于通信的套接字
        //获取客户端ip和端口号
        QString ip =m_socket->peerAddress().toString();
        quint16 port=m_socket->peerPort();
        QString temp=QString("[%1:%2]:成功连接").arg(ip).arg(port);
        temp.replace("::ffff:", "");
        ui->readtext->setText(temp);

        //通信
        connect(m_socket,&QTcpSocket::readyRead,
        [=]()
        {
            QByteArray Array=m_socket->readAll();
            ui->readtext->append(Array);//不覆盖原来内容
        }
        );

        if (m_dataRecvThread != nullptr) {
            delete m_dataRecvThread;
        }
        if (m_dataWriteThread != nullptr) {
            delete m_dataWriteThread;
        }
    }
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    contextMenu->exec(mapToGlobal(pos));
}

void MainWindow::slotRefresh()
{
    qDebug() << "------执行刷新目录树操作--------";
    QString log = QString("%1: 展示存储版下所有文件和文件夹.").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    ui->readtext->append(log);
    //下发的内容
    Cmd_Folder_And_File_Info disk_cmd_info;
    disk_cmd_info.order_head = ORDERHEAD;
    disk_cmd_info.head = DSV_PACKET_HEADER;
    disk_cmd_info.source_ID = 0;
    disk_cmd_info.dest_ID = 0;
    disk_cmd_info.oper_type = 0xA2;
    disk_cmd_info.oper_ID = 0x01;
    disk_cmd_info.package_num = 0;

    disk_cmd_info.fun_type = 0x13;   //13.获取目录中的文件和子目录列表（dir）
    disk_cmd_info.fun_para1[1024] = {0};
    QString temp_para1 = "0:";
    std::u16string utf16Str_para1 = temp_para1.toStdU16String();
    size_t length1 = utf16Str_para1.size();
    for (size_t i = 0; i < 1024; ++i) {
        if (i < length1)
            disk_cmd_info.fun_para1[i] = utf16Str_para1[i];
        else
            disk_cmd_info.fun_para1[i] = 0;
    }
    disk_cmd_info.fun_para2[1024] = {0};
    disk_cmd_info.check = 0;
    disk_cmd_info.end = DSV_PACKET_TAIL;

    qint64 bytesWritten = m_socket->write((char *) (&disk_cmd_info), sizeof(Cmd_Folder_And_File_Info));
    if (bytesWritten < 0) {
        qDebug() << "Write error:" << m_socket->errorString();
        QString log = QString("%1: 查询存储组件状态失败(指令下发失败).").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        ui->readtext->append(log);
    } else {
        QString log = QString("%1: 查询存储组件状态成功.").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        ui->readtext->append(log);
    }
}
