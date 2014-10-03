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
    pitem->setIcon(QIcon(":/images/" + rune + ".png"));
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

  things.add(new Thing("ar", "armor", 0, 0),
              new Thing("be", "belt", 1),
              new Thing("bt", "boot", 1),
              new Thing("gl", "glove", 1),
              new Thing("to", "torso", 1),
              new Thing("he", "helmet", 1),
                new Thing("ci", "circlet", 2),
                new Thing("pe", "druid pelt", 2),
                new Thing("ph", "primal helmet", 2),
              new Thing("sh", "shield", 1),
                new Thing("au", "auiric shield", 2),
                new Thing("hd", "necro head", 2),
            new Thing("we", "weapon", 0, 0),
              new Thing("me", "melee weapon", 1, 0),
                new Thing("ax", "axe", 2),
                  new Thing("ta", "thrown axe", 3),
                new Thing("bl", "blunt weapon", 2, 0),
                  new Thing("cb", "club", 3),
                  new Thing("ha", "hammer", 3),
                  new Thing("ma", "mace", 3),
                  new Thing("ro", "magic rod", 3, 0),
                    new Thing("sc", "scepter", 4),
                    new Thing("st", "staff", 4),
                    new Thing("wa", "wand", 4),
                new Thing("cw", "claw", 2),
                new Thing("kn", "knife", 2),
                  new Thing("tk", "thrown knife", 3),
                new Thing("or", "orb", 2),
                new Thing("po", "polearm", 2),
                new Thing("sp", "spear", 2),
                  new Thing("as", "ama spear", 3),
                new Thing("ja", "javelin", 2),
                  new Thing("aj", "ama javelin", 3),
                new Thing("sw", "sword", 2),
              new Thing("th", "thrown weapon", 1, 0),
                new Thing("ja", "javelin", 2),
                  new Thing("aj", "ama javelin", 3),
                new Thing("ta", "thrown axe", 2),
                new Thing("kn", "knife", 2),
              new Thing("mi", "missile weapon", 1, 0),
                new Thing("bo", "bow", 2),
                  new Thing("ab", "amazon bow", 3),
                new Thing("xb", "crossbow", 2),
            new Thing("cm", "charm", 0, 0),
              new Thing("cg", "charm grand", 1),
              new Thing("ch", "charm hex", 1),
              new Thing("cl", "charm large", 1),
              new Thing("cn", "charm narrow", 1),
              new Thing("cq", "charm quad", 1),
              new Thing("cs", "charm small", 1),
              new Thing("ct", "charm tall", 1),
            NULL);
  things.fillComboBox(ui->thingsBox);
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
  if(ui->thingsBox->currentIndex()){
    if (!first) str += " and";
    str += " (";
    for (int i = things.itr(ui->thingsBox->currentText()); i != 0; i--){
      str += "items LIKE '%" + things.find(ui->thingsBox->currentText(), i) + "%'";
      if (i > 1) str += " or ";
      first = false;
    }
    str += ") ";
  }

  QDEBUG(str);
  model->setSort(model->fieldIndex("level"), ui->sortBox->isChecked() ? Qt::DescendingOrder : Qt::AscendingOrder);
  model->setFilter(str);
}

void Widget::descriptionSlot(const QModelIndex& index){
  if(index.isValid()){
    QDEBUG(index.data());
    QDEBUG(index.row());
    ui->textBrowser->setText("Soon!");
  }
}

Thing::Thing(QString ch, QString name, quint8 pos, bool show) : ch(ch), name(name), pos(pos), show(show){}

void Things::add(Thing *i ...){
  va_list params;
  va_start(params, i);
  for (Thing *p = i; p != NULL; p = va_arg(params, Thing *)){
    listThings.push_back(p);
  }
  va_end(params);
}

void Things::fillComboBox(QComboBox *box){
  box->addItem("");
  foreach(Thing *i, listThings){
    if (i->show) box->addItem(i->name);
  }
}

int Things::itr(QString str){
  QList<Thing *>::const_iterator it = listThings.constBegin();
  for (; it != listThings.constEnd(); ++it)
    if ((*it)->name == str) return (*it)->pos;
  return 0;
}

QString Things::find(QString str, int i){
  QList<Thing *>::iterator it= listThings.begin();
  bool down = true;
  while(it != listThings.end()){
    if (down){
      ++it;
      if ((*it)->name == str) down = false;
    } else {
      if (i == (*it)->pos) break;
      --it;
    }
  }
  return (*it)->ch;
}

BackgroundItemDelegate::BackgroundItemDelegate(QObject* pobj) : QItemDelegate(pobj){
  charSkill << "" << "M" << "A" << "N" << "B" << "P" << "S" << "D" << "@"; //short character name in db
}

void BackgroundItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const{
  int charBoxIndex = parent()->parent()->parent()->findChild<QComboBox *>("charBox")->currentIndex();
  if(charBoxIndex){
    QString character = qobject_cast<const QSqlTableModel *>(index.model())->record(index.row()).value("character").toString();
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
