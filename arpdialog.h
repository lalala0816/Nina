#ifndef ARPDIALOG_H
#define ARPDIALOG_H
#include <QDialog>

namespace Ui {
class ARPDialog;
}

class ARPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ARPDialog(QWidget *parent = 0);
    void static ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    void static arp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    static u_short protocoltype;
    static u_short hardwaretype;
    static u_short hardwarelength;//硬件地址长度
    static u_short protocollength;//协议地址长度
    static u_short operationcode;//操作码
    static int flag_2;
    static char src[100];
    static char dst[100];
    ~ARPDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ARPDialog *ui;
};

#endif // ARPDIALOG_H
