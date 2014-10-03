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

class Thing
{
public:
  QString ch;
  QString name;
  quint8 pos;
  bool show;
  Thing(QString, QString, quint8, bool show = true);
  ~Thing();
};

class Things
{
public:
  Things(){}
  void fillComboBox(QComboBox *);
  int itr(QString);
  QString find(QString, int);
  void add(Thing * ...);
private:
  QList<Thing *> listThings;
};

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
  Ui::Widget *ui;
  QSqlTableModel *model;
  Things things;
};

#endif // WIDGET_H
