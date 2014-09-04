#include "widget.h"
#include "ui_widget.h"
#include <QtGui>

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
  model = new QSqlTableModel(this, db);
  model->setTable("zyel");
  model->select();
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  ui->runewordsTableView->setModel(model);
  ui->runewordsTableView->resizeColumnToContents(model->fieldIndex("level"));
  ui->runewordsTableView->resizeColumnToContents(model->fieldIndex("runes"));
  //ui->runewordsTableView->resizeColumnToContents(model->fieldIndex("number"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("id"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("name"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("warning"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("character"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("number"));
  ui->runewordsTableView->verticalHeader()->setVisible(false);
  ui->runewordsTableView->horizontalHeader()->setVisible(false);
  //ui->runewordsTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  //QItemSelectionModel selection(model, this);
  //ui->runewordsTableView->setSelectionModel(&selection);
  ui->splitter->setChildrenCollapsible(false);
}

void Widget::mySlot(){
  QString str;
  bool first = true;
  if (ui->levelBox->value()){
      str = "level <= " + QString::number(ui->levelBox->value());
      first = false;
  }
  if(ui->soketBox->currentIndex()){
      if (!first) str += " and ";
      str += "number <= " + QString::number(ui->soketBox->currentIndex() + 1);
      first = false;
    }
   QDEBUG(str);

  model->setSort(model->fieldIndex("level"),ui->sortBox->isChecked() ? Qt::DescendingOrder : Qt::AscendingOrder);
  model->setFilter(str);
}

Widget::~Widget()
{
  delete ui;
}
