#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Widget)
{
  ui->setupUi(this);
//initialize db
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("../db/zyelrune.sqlite");
  if (!db.open()) {
    qDebug() << "Cannot open database:" << db.lastError();
  }
//generate list runes from db for runesWidget
  model = new QSqlTableModel(this, db);
  model->setTable("runes");
  model->select();
  QListWidgetItem* pitem = 0;
  for (int nRow = 0; nRow < model->rowCount(); ++nRow) {
    QSqlRecord rec = model->record(nRow);
    QString rune = rec.value("rune").toString();
    pitem = new QListWidgetItem(rune, ui->runesWidget);
    pitem->setIcon(QIcon(":/images/" + rune + ".gif"));
    pitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
  }

  model->setTable("zyel");
  model->select();
  ui->runewordsTableView->setModel(model);
  ui->runewordsTableView->resizeColumnToContents(model->fieldIndex("items"));
  ui->runewordsTableView->resizeColumnToContents(model->fieldIndex("level"));
  ui->runewordsTableView->resizeColumnToContents(model->fieldIndex("runes"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("id"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("name"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("warning"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("character"));
  ui->runewordsTableView->hideColumn(model->fieldIndex("number"));
  //ui->runewordsTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

  filler();

  ui->runewordsTableView->setItemDelegate(new BackgroundItemDelegate(ui->runewordsTableView));
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
      str += "number == " + QString::number(ui->soketBox->currentIndex() + 1);
      first = false;
  }
//  if(ui->itemBox->currentIndex()){ //TODO: do-doo do-doo!!!11 ;/
//    if (!first) str += " and ";
//    str += "(items LIKE '%ch%' or items LIKE '%cm%')";
//    first = false;
//  }

  QDEBUG(str);
  model->setSort(model->fieldIndex("level"), ui->sortBox->isChecked() ? Qt::DescendingOrder : Qt::AscendingOrder);
  model->setFilter(str);
}

void Widget::descriptionSlot(const QModelIndex& index){
  if(index.isValid()){
    QDEBUG(index.data());
    QDEBUG(index.row());
  }
}

void Widget::filler(){
  QStringList charList;
  charList << tr("Any Hero")// TODO: need rename
           << tr("Amazon")
           << tr("Assasin")
           << tr("Necromancer")
           << tr("Barbarian")
           << tr("Paladin")
           << tr("Sorceress")
           << tr("Druid")
           << tr("All Heroes");
  ui->charBox->addItems(charList);
}

BackgroundItemDelegate::BackgroundItemDelegate(QObject* pobj = 0) : QItemDelegate(pobj){
  charSkill << "" << "M" << "A" << "N" << "B" << "P" << "S" << "D" << "@"; //short character name in db
}

void BackgroundItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const{
  int charBoxIndex = parent()->parent()->parent()->findChild<QComboBox *>("charBox")->currentIndex();
  if(charBoxIndex){
    QString character = qobject_cast<QSqlTableModel *>(qobject_cast<QTableView *>(parent())->model())->record(index.row()).value("character").toString();
    if (character == charSkill.at(charBoxIndex)){
      painter->setBrush(QBrush(Qt::green));
      painter->drawRect(option.rect);
    }
  }
  QItemDelegate::paint(painter, option, index);
}

Widget::~Widget()
{
  delete ui;
}
