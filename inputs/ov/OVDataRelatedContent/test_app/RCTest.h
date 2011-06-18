#ifndef RCTEST_H
#define RCTEST_H

#include <QWidget>
#include <QtGui/QTreeWidgetItem>
#include "OVDataRelatedContent.h"

namespace Ui {
    class RCTest;
}

class RCTest : public QWidget {
    Q_OBJECT
public:
    RCTest(QWidget *parent = 0);
    ~RCTest();

protected slots:
    void pushButtonClicked();
    void relatedContentResultsReadySlot(quint16,QList<OVDataRelatedContentResult*>&);
    void contentResultUpdated(OVDataRelatedContentResult::RCSField);
    void itemChanged(QTreeWidgetItem*);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RCTest *ui;
    OVDataRelatedContent *m_rc;
    QMap<quint16, QString>m_equiv;
    QMap<OVDataRelatedContentResult *,QTreeWidgetItem *> m_items;
    int m_slidePos ;
};

#endif // RCTEST_H
