#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <QDialog>
#include "etherdialog.h"
namespace Ui {
class IPDialog;
}

class IPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IPDialog(QWidget *parent = 0);
    void static ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    void static ip_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
   //static struct ip_header *ip_protocol;
    static u_int ip_version;
    static u_int header_length;
    static u_int total_length;
    static u_int id;
    static  u_int offset;//偏移
    static  u_int tos;//服务质量
    static u_int ttl;
    static u_int16_t checksum;
    static char src[100];
    static char dst[100];
    static int flag_1;
    ~IPDialog();
private slots:
    void on_pushButton_clicked();

private:
    Ui::IPDialog *ui;
};

#endif // IPDIALOG_H
