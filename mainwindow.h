#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTextCodec>
#include <QTableWidget>
#include <QMainWindow>
#include "etherdialog.h"
#include"ipdialog.h"
#include"icmpdialog.h"
#include"arpdialog.h"
#include"tcpdialog.h"
#include"udpdialog.h"
#include "pcap.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <error.h>
#include <time.h>
#include <sys/time.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void static packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
     //void static tcp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
     //void static ip_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
     //void static udp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
     //void static icmp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
     //void static arp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
    ~MainWindow();
    // u_char *mac_string;
    static u_short ethernet_type;
    static int packet_number;
    static int flag;
    //static char mac[100];
signals:
private slots:
    void on_Button_1_clicked();

    //void on_comboBox_activated(const QString &arg1);

    void on_Button_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
    //EtherDialog *etherdialog;
    //IPDialog *ipdialog;
};

#endif // MAINWINDOW_H
