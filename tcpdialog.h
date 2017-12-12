#ifndef TCPDIALOG_H
#define TCPDIALOG_H

#include <QDialog>

namespace Ui {
class TCPDialog;
}

class TCPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TCPDialog(QWidget *parent = 0);
    void static ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    void static ip_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    void static tcp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    static u_short  source_port;
    static u_short  destination_port;
    static u_int16_t checksum;
    static u_int sequence;
    static u_int acknowledgement;
    static  u_int reserved;
    static int  header_length;//首部长度
    static u_int flags;//标记
    static u_short windows;//窗口
    static u_short urgent_pointer;//紧急指针
    static int flag_4;
    ~TCPDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TCPDialog *ui;
};

#endif // TCPDIALOG_H
