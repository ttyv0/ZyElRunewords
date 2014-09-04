#include "widget.h"
#include "ui_widget.h"
#include <QtGui>
#include <QtSql>

Widget::Widget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Widget)
{
  ui->setupUi(this);
  QDir *resDir = new QDir(":/images");
  QStringList imgList = resDir->entryList();
  delete resDir;
  QListWidgetItem* pitem = 0;
  //ui->runesWidget->setIconSize(QIcon(imgList.first()).size());
  foreach(QString str, imgList) {
      pitem = new QListWidgetItem(str.remove(".gif"), ui->runesWidget);
      pitem->setIcon(QIcon(":/images/" + str));
      pitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
  }
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("../db/zyelrune.sqlite");
  if (!db.open()) {
    qDebug() << "Cannot open database:" << db.lastError();
  }
  QSqlTableModel *model = new QSqlTableModel(this, db);
  model->setTable("zyel");
  model->select();
  model->removeColumn(model->fieldIndex("id"));
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  ui->runewordsTableView->setModel(model);
  ui->runewordsTableView->resizeColumnToContents(model->fieldIndex("level"));
  ui->runewordsTableView->resizeColumnToContents(model->fieldIndex("character"));
  ui->runewordsTableView->resizeColumnToContents(model->fieldIndex("number"));

  ui->runewordsTableView->hideColumn(model->fieldIndex("name"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("warning"));

  ui->runesWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  //QItemSelectionModel selection(model, this);
  //ui->runewordsTableView->setSelectionModel(&selection);
}

Widget::~Widget()
{
  delete ui;
}
