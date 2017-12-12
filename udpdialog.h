#ifndef UDPDIALOG_H
#define UDPDIALOG_H

#include <QDialog>

namespace Ui {
class UDPDialog;
}

class UDPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UDPDialog(QWidget *parent = 0);
    void static ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    void static ip_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    void static udp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    static u_short source_port;
   static  u_short destination_port;
    static u_short  length;//长度
    static u_short checksum;
    static int flag_3;
    ~UDPDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::UDPDialog *ui;
};

#endif // UDPDIALOG_H
