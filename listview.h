#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListView>

class QStandardItemModel;

class ListView : public QListView
{
    Q_OBJECT
public:
    explicit ListView(QWidget *parent = nullptr);

    virtual void init();

    void addItem(const QIcon &icon, const QString &text);

    QStandardItemModel *itemModel() const;

signals:

public slots:

private:
    QStandardItemModel *m_itemModel = Q_NULLPTR;
};

#endif // LISTVIEW_H
