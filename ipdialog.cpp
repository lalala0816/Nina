#include "ipdialog.h"
#include "ui_ipdialog.h"
#include "etherdialog.h"
#include "pcap.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <error.h>
#include<string.h>
IPDialog::IPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IPDialog)
{
    ui->setupUi(this);
    ui->label->setFont(QFont(" ", 15, QFont::Bold));
    ui->label->setText("IP数据包");
    ui->label->setText(QObject::tr("<font color = blue>%1</font>").arg("IP数据包"));
   this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);//
    //ui->tableWidget->setRowCount(65535);
    QStringList column;
    column.append("版本号");
    column.append("包头长度");
    column.append("包总长度");
    column.append("服务质量");
    column.append("标识");
    column.append("偏移");
    column.append("生存时间");
    column.append("协议类型");
    column.append("IP校验和");
    column.append("源IP地址");
    column.append("目的IP地址");
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,80);
    ui->tableWidget->setColumnWidth(3,80);
    ui->tableWidget->setColumnWidth(4,80);
    ui->tableWidget->setColumnWidth(5,80);
    ui->tableWidget->setColumnWidth(6,80);
    ui->tableWidget->setColumnWidth(7,100);
    ui->tableWidget->setColumnWidth(8,80);
    ui->tableWidget->setColumnWidth(9,100);
    ui->tableWidget->setColumnWidth(10,100);
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
    char bpf_filter_string[]="ip";//过滤规则字符串，此处可自己设定
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
        if(pcap_loop(pcap_handle, 1,ethernet_protocol_packet_callback,NULL)==0)
      {
             int row = ui->tableWidget->rowCount();
             ui->tableWidget->insertRow(row);
             ui->tableWidget->setItem(row , 0 ,new QTableWidgetItem(QIcon(""), "IPv4"));
             //ui->tableWidget->setItem(row , 0 ,new QTableWidgetItem(QIcon(""), QString::number(ip_version)));
             ui->tableWidget->setItem(row , 1 ,new QTableWidgetItem(QIcon(""),QString::number( header_length)));
            // ui->tableWidget->setItem(row , 3 ,new QTableWidgetItem(QIcon(""), "");
             ui->tableWidget->setItem(row , 2 ,new QTableWidgetItem(QIcon(""),QString::number(total_length)));
             ui->tableWidget->setItem(row , 3 ,new QTableWidgetItem(QIcon(""),QString::number(tos)));
             ui->tableWidget->setItem(row , 4 ,new QTableWidgetItem(QIcon(""),QString::number(id)));
             ui->tableWidget->setItem(row , 5 ,new QTableWidgetItem(QIcon(""),QString::number( (offset & 0x1ffff)*8)));
             ui->tableWidget->setItem(row , 6 ,new QTableWidgetItem(QIcon(""),QString::number(ttl)));
             switch(flag_1)
            {
                   case 0: ui->tableWidget->setItem(row , 7 ,new QTableWidgetItem(QIcon(""), "不确定的协议类型"));break;
                   case 1: ui->tableWidget->setItem(row , 7,new QTableWidgetItem(QIcon(""), "上层协议为TCP 协议类型"));break;
                   case 2: ui->tableWidget->setItem(row , 7 ,new QTableWidgetItem(QIcon(""), "上层协议为UDP协议类型"));break;
                   case 3: ui->tableWidget->setItem(row , 7 ,new QTableWidgetItem(QIcon(""), "上层协议为ICMP 协议类型"));break;
                   default:break;
            }
             ui->tableWidget->setItem(row , 8 ,new QTableWidgetItem(QIcon(""),QString::number( checksum)));
             ui->tableWidget->setItem(row , 9 ,new QTableWidgetItem(QIcon(""),src));
             ui->tableWidget->setItem(row , 10,new QTableWidgetItem(QIcon(""),dst));
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
struct ip_header
{
#ifdef WORDS_BIGENDIAN
   u_int8_t ip_version:4,//版本
                 ip_header_length:4;//首部长
#else
    u_int8_t ip_header_length:4,
                   ip_version:4;

#endif
    u_int8_t ip_tos;//服务质量
    u_int16_t ip_length;//总长度
    u_int16_t ip_id;//标识
    u_int16_t ip_off;//偏移
    u_int8_t ip_ttl;//生存时间
    u_int8_t ip_protocol;//协议类型
    u_int16_t ip_checksum;//校验和
    struct in_addr ip_source_address;//源IP
    struct in_addr ip_destination_address;//目的IP
  };
void IPDialog::ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
  {

         //const u_char *packet_content;
         int packet_number=0;
         u_char *mac_string;
         struct pcap_pkthdr *protocol_header;
         struct ether_header *ethernet_protocol;
         u_short ethernet_type;
         packet_number++;
         ethernet_protocol=(struct ether_header *)packet_content;
         ethernet_type=ntohs(ethernet_protocol->ether_type);
         switch(ethernet_type)
                 {
                        case 0x0800: ip_protocol_packet_callback(argument,
                                                                packet_header,
                                                                packet_content );
                        break;
                 default:break;
                 }
         mac_string=ethernet_protocol->ether_shost;
         mac_string=ethernet_protocol->ether_dhost;
  }

//struct IPDialog:: ip_header *ip_protocol;
u_int IPDialog:: ip_version=4;
u_int IPDialog:: header_length=0;
u_int IPDialog:: total_length=0;
u_int IPDialog::  id=0;
u_int IPDialog:: offset=0;//偏移
u_int IPDialog::  tos=0;//服务质量
u_int16_t IPDialog:: checksum=0;
u_int IPDialog:: ttl=0;
char  IPDialog::src[100]="";
char  IPDialog::dst[100]="";
int IPDialog::flag_1=0;
void IPDialog:: ip_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
{
     struct ip_header *ip_protocol;
    ip_protocol=(struct ip_header *)(packet_content+14);
    checksum=ntohs(ip_protocol->ip_checksum);
    header_length=ip_protocol->ip_header_length*4;
    tos=ip_protocol->ip_tos;
    offset=ntohs(ip_protocol->ip_off);
    ttl=ip_protocol->ip_ttl;
    total_length=ntohs(ip_protocol->ip_length);
    id=ntohs(ip_protocol->ip_id);
     printf("--------------------   IP protocol (Transport Layer)   ---------------------\n\n");
     printf("IP Version:%d\n",ip_protocol->ip_version);
     printf("Header length:%d\n",header_length);
     printf("TOS:%d\n",tos);
     printf("Total length:%d\n",ntohs(ip_protocol->ip_length));
     printf("Identification:%d\n",ntohs(ip_protocol->ip_id));
     printf("Offset:%d\n",(offset & 0x1ffff)*8);
     printf("TTL:%d\n",ip_protocol->ip_ttl);
     printf("Protocol:%d\n",ip_protocol->ip_protocol);
     switch(ip_protocol->ip_protocol)
     {
          case 6: flag_1=1;printf("The Transport Layer Protocol is TCP\n");break;
          case 17: flag_1=2;printf("The Transport Layer Protocol is UDP\n");break;
          case 1:flag_1=3;printf("The Transport Layer Protocol is ICMP\n");break;
         default:break;
     }
    printf("Header checksum:%d\n",checksum);
    strcpy(src,inet_ntoa(ip_protocol->ip_source_address));
    printf("Source address:%s\n",inet_ntoa(ip_protocol->ip_source_address));
    strcpy(dst,inet_ntoa(ip_protocol->ip_destination_address));
    printf("Destination address:%s\n",inet_ntoa(ip_protocol->ip_destination_address));
}
IPDialog::~IPDialog()
{
    delete ui;
}

void IPDialog::on_pushButton_clicked()
{
    this->close();
}
