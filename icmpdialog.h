#ifndef ICMPDIALOG_H
#define ICMPDIALOG_H

#include <QDialog>

namespace Ui {
class ICMPDialog;
}

class ICMPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICMPDialog(QWidget *parent = 0);
    void static ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    void static ip_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    void static icmp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    static int packet_number;
    static u_int8_t icmptype;//类型
   // static u_int8_t icmp_code;//代码
    static u_int16_t icmpchecksum;//校验和
    //static u_int16_t icmp_id;//标识
   // static u_int16_t icmp_sequence;//序列号
    static int flag_5;
    ~ICMPDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ICMPDialog *ui;
};

#endif // ICMPDIALOG_H
