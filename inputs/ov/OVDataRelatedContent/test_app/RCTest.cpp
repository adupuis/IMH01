#include "RCTest.h"
#include "ui_RCTest.h"
#include <QDebug>

RCTest::RCTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RCTest)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(pushButtonClicked()));
    m_rc = new OVDataRelatedContent( QStringList() << "./" << "../plugins/build/" );
    connect(m_rc,SIGNAL(relatedContentResultsReady(quint16,QList<OVDataRelatedContentResult*>&)),this,SLOT(relatedContentResultsReadySlot(quint16,QList<OVDataRelatedContentResult*>&)));
    connect( ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemChanged(QTreeWidgetItem*)) );
    m_rc->loadPlugins();
    m_equiv.clear();
    m_items.clear();
    m_slidePos=0;
}

RCTest::~RCTest()
{
    delete ui;
}

void RCTest::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void RCTest::pushButtonClicked(){
//        qDebug("Button clicked");
        QStringList query = ui->queryLineEdit->text().split(" ");
        m_equiv.insert(m_rc->getRelatedContent(query), ui->queryLineEdit->text());
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0,ui->queryLineEdit->text());
        ui->treeWidget->addTopLevelItem(item);
}

void RCTest::relatedContentResultsReadySlot(quint16 checksum,QList<OVDataRelatedContentResult*>& results){
        QTreeWidgetItem *item = ui->treeWidget->findItems( m_equiv.value(checksum), Qt::MatchExactly, 0 ).at(0);
        foreach(OVDataRelatedContentResult *r, results){
                QTreeWidgetItem *it = new QTreeWidgetItem(item);
                it->setText(0, r->title());
                it->setToolTip(0,r->description());
                m_items.insert(r,it);
//                qDebug() << "CONNECTING";
                connect( r, SIGNAL(contentUpdated(OVDataRelatedContentResult::RCSField)), this, SLOT(contentResultUpdated(OVDataRelatedContentResult::RCSField)) );
                if(r->thumbnails().size() > 0){
//                        qDebug() << "BOOOOOOO";
                        it->setIcon(0, QIcon( QPixmap::fromImage(r->thumbnails().at(0)) ));
                }
        }
}

void RCTest::contentResultUpdated(OVDataRelatedContentResult::RCSField field){
//        qDebug() << "YAAAAAAAH";
        if( field == OVDataRelatedContentResult::Thumbnails ){
//                qDebug()<< "Field is Thumbnails";
                OVDataRelatedContentResult *r = qobject_cast<OVDataRelatedContentResult *>(sender());
                QList<QImage> l = r->thumbnails();
//                qDebug() << "Image list size: " << l.size();
//                if(l.at(0).isNull())
//                        qDebug() << "FUCK!";
                QTreeWidgetItem *it = m_items.value(r);
                ui->pixLabel->setPixmap(QPixmap::fromImage(l.at(0)));
                if(it){
                        it->setIcon(0, QIcon( QPixmap::fromImage(l.at(0)) ));
                }
        }
//        else
//                qDebug() << "BORK";
}

void RCTest::itemChanged(QTreeWidgetItem * it){
//        qDebug() << "ITEM CHANGED";
        OVDataRelatedContentResult *r = m_items.key(it);
        if( r ){
                m_slidePos++;
                if( m_slidePos >= r->thumbnails().size() )
                        m_slidePos=0;
                ui->pixLabel->setPixmap( QPixmap::fromImage( r->thumbnails().at( m_slidePos ) ) );
        }
        ui->queryLineEdit->setText(r->title());
}
