#include "etherdialog.h"
#include "ui_etherdialog.h"
#include "mainwindow.h"
#include "pcap.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <error.h>
#include<string.h>
EtherDialog::EtherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EtherDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->label->setFont(QFont(" ", 13, QFont::Bold));
    ui->label->setText(QObject::tr("<font color = blue>%1</font>").arg("以太网数据包"));
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
   // ui->tableWidget->setRowCount(65535);
    QHeaderView* headerView = ui->tableWidget->verticalHeader();//去掉自带的行号
    headerView->setHidden(true);
    QStringList column;
    column.append("数目");
    column.append("以太网类型");
    column.append("网络层次");
    column.append("目的以太网地址");
    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,250);
    ui->tableWidget->setColumnWidth(2,250);
    ui->tableWidget->setColumnWidth(3,200);
    ui->tableWidget->setHorizontalHeaderLabels(column);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pcap_t* pcap_handle;//Libpcap句柄
    //const u_char *packet_content;//数据包缓存
    char error_content[PCAP_ERRBUF_SIZE];//存储错误内容
    char *net_interface;//网络接口
    struct in_addr net_ip_address;//网络地址
    struct in_addr net_mask_address;//掩码地址
    char *net_ip_string;//网络地址字符串形式
    char *net_mask_string;
    struct bpf_program bpf_filter;//BPF过滤规则
    char bpf_filter_string[]="";//过滤规则字符串，此处可自己设定
    bpf_u_int32 net_mask;//网络掩码
    bpf_u_int32 net_ip;//网络地址
    net_interface=pcap_lookupdev(error_content);//获得设备接口
    pcap_lookupnet(net_interface,
                           &net_ip,
                           &net_mask,
                           error_content);//获得网络地址和掩码
    pcap_handle=pcap_open_live(net_interface,
                                       BUFSIZ,
                                       1,
                                       0,
                                       error_content);//打开网络接口
            pcap_compile(pcap_handle,
                         &bpf_filter,
                         bpf_filter_string,
                         0,
                         net_ip);//编译过滤规则
            pcap_setfilter(pcap_handle,
                           &bpf_filter);//设置过滤规则
    net_ip_address.s_addr=net_ip;
    net_ip_string=inet_ntoa(net_ip_address);
    net_mask_address.s_addr=net_mask;
    net_mask_string=inet_ntoa(net_mask_address);
    if(pcap_datalink(pcap_handle)!=DLT_EN10MB)//........
                printf("Doesn't exist within the local area network LAN");
    for(int i=0;i<10;i++)
    {
        if(pcap_loop(pcap_handle, 1,ethernet_packet_callback,NULL)==0)
      {
             int row = ui->tableWidget->rowCount();
             ui->tableWidget->insertRow(row);
             ui->tableWidget->setItem(row , 0 ,new QTableWidgetItem(QIcon(""), QString::number(packet_number)));
             char temp[10];
             sprintf(temp , "%04x" ,  ethernet_type);
             char s[100]="EtherType: 0x";
             ui->tableWidget->setItem(row , 1 ,new QTableWidgetItem(QIcon(""),strcat(s,temp)));
            switch(flag)
            {
                   case 0: ui->tableWidget->setItem(row , 2 ,new QTableWidgetItem(QIcon(""), "不确定的网络协议层类型"));break;
                   case 1: ui->tableWidget->setItem(row , 2 ,new QTableWidgetItem(QIcon(""), "网络层为 IP 协议类型"));break;
                   case 2: ui->tableWidget->setItem(row , 2 ,new QTableWidgetItem(QIcon(""), "网络层为 ARP 协议类型"));break;
                   case 3: ui->tableWidget->setItem(row , 2 ,new QTableWidgetItem(QIcon(""), "网络层为 RARP 协议类型"));break;
                   default:break;
            }
          ui->tableWidget->setItem(row , 3 ,new QTableWidgetItem(QIcon(""),"        9c:2a:70:1f:13:42"));
    }
    }
   pcap_close(pcap_handle);//关闭Libpcap操作

}
struct ether_header
{
    u_int8_t ether_dhost[6];//目的以太网地址
    u_int8_t ether_shost[6];//源
    u_int16_t ether_type;//以太网类型
};
int EtherDialog::packet_number=0;//定义静态变量记录抓取的数据包个数
u_short EtherDialog::ethernet_type;
int EtherDialog::flag=0;
void EtherDialog::ethernet_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
  {

         //const u_char *packet_content;
         u_char *mac_string;
         struct pcap_pkthdr *protocol_header;
         struct ether_header *ethernet_protocol;
         packet_number++;
         ethernet_protocol=(struct ether_header *)packet_content;
         ethernet_type=ntohs(ethernet_protocol->ether_type);
         switch(ethernet_type)
         {
                  /*根据以太网协议类型字段判断上层协议内容*/
                 case 0x0800:  flag=1;
                 break;
                 case 0x0806: flag=2;
                 break;
                 case 0x8035: flag=3;
                 break;
                 default:break;
         }
         mac_string=ethernet_protocol->ether_shost;
         mac_string=ethernet_protocol->ether_dhost;
  }
EtherDialog::~EtherDialog()
{
    delete ui;
}

void EtherDialog::on_pushButton_clicked()
{
    //EtherDialog etherdialog;
    //etherdialog.setModal(true);
    this->close();
}
