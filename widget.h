#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql>

#if defined(QT_DEBUG)
  #define QDEBUG(X) qDebug() << X;
#else
  #define QDEBUG(X) ;
#endif

namespace Ui {
  class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();

public slots:
  void mySlot();

private:
  Ui::Widget *ui;
  QSqlTableModel *model;
};

#endif // WIDGET_H
