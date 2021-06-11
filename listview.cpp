#include "listview.h"

#include <QStandardItemModel>

ListView::ListView(QWidget *parent) : QListView(parent)
{
    setAutoFillBackground(true);
    setSelectionMode(QListView::SingleSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_itemModel = new QStandardItemModel;
    setModel(m_itemModel);

    init();
}

void ListView::init()
{
    const QList<QString> textList = {
        "淡入淡出","图片翻转","百叶窗","水波特效"
    };

    const QList<QString> iconList = {
        ":/images/opacity.svg",":/images/overturn.svg",":/images/overturn.svg", ":/images/波浪能.svg"
    };

    for (int i=0;i<textList.size();i++) {
        addItem(QIcon::fromTheme(iconList[i]),textList[i]);
    }
}

void ListView::addItem(const QIcon &icon, const QString &text)
{
    QStandardItem *account = new QStandardItem(icon,text);
    m_itemModel->appendRow(account);
}

QStandardItemModel *ListView::itemModel() const
{
    return m_itemModel;
}
