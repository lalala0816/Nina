#ifndef ETHERDIALOG_H
#define ETHERDIALOG_H

#include <QDialog>

namespace Ui {
class EtherDialog;
}

class EtherDialog : public QDialog
{
    Q_OBJECT

public:
   explicit EtherDialog(QWidget *parent = 0);
   static int packet_number;
   static u_short ethernet_type;
   static int flag;
   void static ethernet_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
   ~EtherDialog();
private slots:
    void on_pushButton_clicked();

private:
    Ui::EtherDialog *ui;
};

#endif // ETHERDIALOG_H
