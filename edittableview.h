#ifndef EDITTABLEVIEW_H
#define EDITTABLEVIEW_H

#include <QObject>

class EditTableView : public QObject
{
    Q_OBJECT
public:
    explicit EditTableView(QObject *parent = 0);
    ~EditTableView();

signals:

public slots:
};

#endif // EDITTABLEVIEW_H
