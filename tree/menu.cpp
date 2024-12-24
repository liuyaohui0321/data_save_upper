#include "menu.h"
#pragma execution_character_set("utf-8")
menu::menu(QWidget *parent) : QMenu(parent)
{
    addLargeAction(tr("刷新"), ":/images/refurbish_primary_blue.png", SIGNAL(sigRefresh()),ButtonType::Refresh);
    this->addSeparator();
    addLargeAction(tr("新建文件"), ":/images/app_primary_blue.png", SIGNAL(signalNewFile()),ButtonType::AddFile);
    this->addSeparator();
}

QAction *menu::createAction(const QString &text,
                                   const QString &iconurl,
                                   const QString &shortCut )

{   
    QAction *act = new QAction(this);
    act->setText(text);

    act->setIcon(QIcon(iconurl));

//    act->setObjectName(text);
    if (!shortCut.isEmpty())
    {
        act->setShortcut(QKeySequence(shortCut));
    }
    return act;
}

void menu::addLargeAction(const QString &text,
                                 const QString &iconurl,
                                 const char *slotAction,
                                 const ButtonType &type,
                                 QMenu *menu,
                                 const QString &shortCut )

{
    QAction *action = createAction(text, iconurl, shortCut);

    action->setData(static_cast<int>(type));

    if (nullptr == menu)
    {
        this->addAction(action);
    }
    else
    {
        menu->addAction(action);
    }
    connect(action, SIGNAL(triggered()), this, slotAction);
}
