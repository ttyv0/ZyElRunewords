#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include <QWidget>
#include <QtSql>
#include <QModelIndex>

#if defined(QT_DEBUG)
  #define QDEBUG(X) qDebug() << X;
#else
  #define QDEBUG(X) ;
#endif

namespace Ui {
  class Widget;
}


class BackgroundItemDelegate : public QItemDelegate
{
public:
  BackgroundItemDelegate(QObject*);
  void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const;
private:
  QStringList charSkill;
};


class Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();

public slots:
  void mySlot();
  void descriptionSlot(const QModelIndex&);

private:
  void filler();
  Ui::Widget *ui;
  QSqlTableModel *model;
};

#endif // WIDGET_H
