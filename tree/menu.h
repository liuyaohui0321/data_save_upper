#ifndef MENU_H
#define MENU_H

#include <QMenu>
#include <QWidgetAction>

enum class ButtonType {
    Refresh = 0,   // 刷新
    Close,       // 关闭
    Open,   // 打开
    AddFolder,        // 新增文件夹
    AddFile,      // 新增文件
    Delete,    // 删除
    Move,        // 移动
    Copy,          // 复制
    Rename,       // 重命名
    Write,// 写入
    PlayBack,//回放
    Export,//导出
    Acquisition//采集
};

class menu : public QMenu
{
    Q_OBJECT
public:
    explicit menu(QWidget *parent = nullptr);

signals:

    void sigRefresh(); //刷新
    void signalNewFile(); //新建文件
private:
/**
 * @brief 创建action
 * @param  text：菜单文字
 * @param  iconurl：菜单图片
 * @param  shortCut：快捷键
 */
    QAction *createAction(const QString &text,
                          const QString &iconurl,
                          const QString &shortCut = "");

/**
 * @brief 增加图标
 * @param  text：菜单文字
 * @param  iconurl：菜单图片
 * @param  menu：所属菜单
 * @param  shortCut：快捷键
 */
    void addLargeAction(const QString &text,
                        const QString &iconurl,
                        const char *slotAction,
                        const ButtonType &type,
                        QMenu *menu = nullptr,
                        const QString &shortCut = "");

};


#endif // MENU_H
