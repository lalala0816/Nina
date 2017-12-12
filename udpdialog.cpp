#include "udpdialog.h"
#include "ui_udpdialog.h"
#include "pcap.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<string.h>
UDPDialog::UDPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UDPDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->label->setFont(QFont(" ", 13, QFont::Bold));
    ui->label->setText(QObject::tr("<font color = blue>%1</font>").arg("UDP数据包"));
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    //ui->tableWidget->setRowCount(65535);
    QStringList column;
    column.append("源端口号");
    column.append("目的端口号");
    column.append("包长度");
    column.append("校验和");
    column.append("服务类型");
    ui->tableWidget->setColumnWidth(0,120);
    ui->tableWidget->setColumnWidth(1,120);
    ui->tableWidget->setColumnWidth(2,120);
    ui->tableWidget->setColumnWidth(3,120);
    ui->tableWidget->setColumnWidth(4,180);
    ui->tableWidget->setHorizontalHeaderLabels(column);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//只用来显示信息，将表格设置为只读模式
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
        if(pcap_loop(pcap_handle, 1,ethernet_protocol_packet_callback,NULL)==0)
      {
             int row = ui->tableWidget->rowCount();
             ui->tableWidget->insertRow(row);
             ui->tableWidget->setItem(row , 0,new QTableWidgetItem(QIcon(""), QString::number(source_port)));
             ui->tableWidget->setItem(row , 1,new QTableWidgetItem(QIcon(""),QString::number(destination_port)));
             ui->tableWidget->setItem(row , 2,new QTableWidgetItem(QIcon(""), QString::number(length)));
             ui->tableWidget->setItem(row , 3,new QTableWidgetItem(QIcon(""), QString::number(checksum)));
            switch(flag_3)
            {
                   case 0: ui->tableWidget->setItem(row , 4 ,new QTableWidgetItem(QIcon(""), "不确定的服务"));break;
                   case 1: ui->tableWidget->setItem(row , 4 ,new QTableWidgetItem(QIcon(""), "NETBIOS Datagram 服务"));break;
                   case 2: ui->tableWidget->setItem(row , 4 ,new QTableWidgetItem(QIcon(""), "NETBIOS Name 服务"));break;
                   case 3: ui->tableWidget->setItem(row , 4 ,new QTableWidgetItem(QIcon(""), "NETBIOS Session 服务"));break;
                   case 4: ui->tableWidget->setItem(row , 4 ,new QTableWidgetItem(QIcon(""), "域名服务"));break;
                   default:break;
            }

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
struct udp_header
{
    u_int16_t udp_source_port;//源端口
    u_int16_t udp_destination_port;//目的端口
    u_int16_t udp_length;//长度
    u_int16_t udp_checksum;//校验和
};
void UDPDialog::ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
  {

         //const u_char *packet_content;
         int packet_number=0;//定义静态变量记录抓取的数据包个数
         u_char *mac_string;
         struct pcap_pkthdr *protocol_header;
         struct ether_header *ethernet_protocol;
         u_short ethernet_type;
         packet_number++;
         ethernet_protocol=(struct ether_header *)packet_content;
         ethernet_type=ntohs(ethernet_protocol->ether_type);
         switch(ethernet_type)
         {
                  /*根据以太网协议类型字段判断上层协议内容*/
                 case 0x0800:ip_protocol_packet_callback(argument,
                                                         packet_header,
                                                         packet_content );
                 break;
                 default:break;
         }
  }
void UDPDialog::ip_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
{
    struct ip_header *ip_protocol;
    u_int header_length;
    u_int offset;//偏移
    u_char tos;//服务质量
    u_int16_t checksum;
    ip_protocol=(struct ip_header *)(packet_content+14);
    checksum=ntohs(ip_protocol->ip_checksum);
    header_length=ip_protocol->ip_header_length*4;
    tos=ip_protocol->ip_tos;
    offset=ntohs(ip_protocol->ip_off);
     printf("--------------------   IP protocol (Transport Layer)   ---------------------\n");
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
          case 6: printf("The Transport Layer Protocol is TCP\n");break;
          case 17: printf("The Transport Layer Protocol is UDP\n");break;
          case 1: printf("The Transport Layer Protocol is ICMP\n");break;
         default:break;
     }
    printf("Header checksum:%d\n",checksum);
    printf("Source address:%s\n",inet_ntoa(ip_protocol->ip_source_address));
    printf("Destination address:%s\n",inet_ntoa(ip_protocol->ip_destination_address));
    switch(ip_protocol->ip_protocol)
    {
       case 17://上层协议为UDP协议
           udp_protocol_packet_callback(argument,
                                        packet_header,
                                        packet_content);
        break;
       default:break;
       }
}
u_short UDPDialog::source_port;
u_short UDPDialog::destination_port;
u_short UDPDialog:: length;//长度
u_short UDPDialog::checksum;
int UDPDialog::flag_3=0;
void UDPDialog::udp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
{
    struct udp_header *udp_protocol;
    udp_protocol=(struct udp_header *)(packet_content+14+20);
    source_port=ntohs(udp_protocol->udp_source_port);
    destination_port=ntohs(udp_protocol->udp_destination_port);
     length=ntohs(udp_protocol->udp_length);
     printf("--------------------   UDP protocol (Transport Layer)   --------------------\n");
     printf("Source Port:%d\n",source_port);
     printf("Destination Port:%d\n",destination_port);
     switch(destination_port)
     {
     case 138: flag_3=1;printf("NETBIOS Datagram Service\n");break;
     /*上层协议为NetBios数据报服务，此处可写出分析次协议的函数（尝试再写）*/
     case 137: flag_3=2;printf("NETBIOS Name Service\n");break;
     /*上层协议为NetBios名字服务，此处可写出分析次协议的函数（尝试再写）*/
     case 139:flag_3=3;printf("NETBIOS Session Service\n");break;
     /*上层协议为NetBios会话服务，此处可写出分析次协议的函数（尝试再写）*/
     case 53:flag_3=4;printf("name-domain service\n");break;
     /*上层协议为域名服务，此处可写出分析次协议的函数（尝试再写）*/
     default:break;
     }
     printf("Length:%d\n",length);
     checksum=ntohs(udp_protocol->udp_checksum);
     printf("Checksum:%d\n",ntohs(udp_protocol->udp_checksum));//获得校验和
}

UDPDialog::~UDPDialog()
{
    delete ui;
}

void UDPDialog::on_pushButton_clicked()
{
    this->close();
}
