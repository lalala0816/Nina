#include "tcpdialog.h"
#include "ui_tcpdialog.h"
#include "pcap.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <error.h>
#include<string.h>
TCPDialog::TCPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCPDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->label->setFont(QFont(" ", 13, QFont::Bold));
    ui->label->setText(QObject::tr("<font color = blue>%1</font>").arg("TCP数据包"));
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    //ui->tableWidget->setRowCount(65535);
    QStringList column;
    column.append("源端口号");
    column.append("目的端口号");
    column.append("校验和");
    column.append("序列号");
    column.append("确认码");
    column.append("包头长度");
    column.append("标记");
    column.append("保留字");
    column.append("窗口大小");
    column.append("紧急指针");
    column.append("上层协议类型");
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setColumnWidth(3,80);
    ui->tableWidget->setColumnWidth(4,100);
    ui->tableWidget->setColumnWidth(5,80);
    ui->tableWidget->setColumnWidth(6,80);
    ui->tableWidget->setColumnWidth(7,80);
    ui->tableWidget->setColumnWidth(8,100);
    ui->tableWidget->setColumnWidth(9,100);
    ui->tableWidget->setColumnWidth(10,200);
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
    char bpf_filter_string[]="tcp";//过滤规则字符串，此处可自己设定
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
             ui->tableWidget->setItem(row , 0 ,new QTableWidgetItem(QIcon(""), QString::number(source_port)));
             ui->tableWidget->setItem(row , 1 ,new QTableWidgetItem(QIcon(""),QString::number(destination_port)));
             ui->tableWidget->setItem(row , 2,new QTableWidgetItem(QIcon(""), QString::number(checksum)));
             ui->tableWidget->setItem(row , 3,new QTableWidgetItem(QIcon(""), QString::number(sequence)));
             ui->tableWidget->setItem(row , 4,new QTableWidgetItem(QIcon(""), QString::number(acknowledgement)));
             ui->tableWidget->setItem(row , 5,new QTableWidgetItem(QIcon(""), QString::number(header_length)));
             switch(flags)
             {
                 case 0:   ui->tableWidget->setItem(row , 6,new QTableWidgetItem(QIcon(""), "Unknown"));break;
                 case 1:  ui->tableWidget->setItem(row , 6,new QTableWidgetItem(QIcon(""), "PSH"));break;
                 case 2:  ui->tableWidget->setItem(row , 6,new QTableWidgetItem(QIcon(""), "ACK"));break;
                 case 3:  ui->tableWidget->setItem(row , 6,new QTableWidgetItem(QIcon(""), "SYN"));break;
                 case 4:  ui->tableWidget->setItem(row , 6,new QTableWidgetItem(QIcon(""), "URG"));break;
                 case 5:  ui->tableWidget->setItem(row , 6,new QTableWidgetItem(QIcon(""), "FIN"));break;
                 case 6:  ui->tableWidget->setItem(row , 6,new QTableWidgetItem(QIcon(""), "RST"));break;
             }
             ui->tableWidget->setItem(row , 7,new QTableWidgetItem(QIcon(""), QString::number(reserved)));
             ui->tableWidget->setItem(row , 8,new QTableWidgetItem(QIcon(""), QString::number(windows)));
             ui->tableWidget->setItem(row , 9,new QTableWidgetItem(QIcon(""), QString::number(urgent_pointer)));
            // ui->tableWidget->setItem(row , 9,new QTableWidgetItem(QIcon(""), QString::number(sequence)));
            switch(flag_4)
            {
                   case 0: ui->tableWidget->setItem(row , 10 ,new QTableWidgetItem(QIcon(""), "不确定的应用层协议类型"));break;
                   case 1: ui->tableWidget->setItem(row , 10 ,new QTableWidgetItem(QIcon(""), "应用层为HTTP 协议类型"));break;
                   case 2: ui->tableWidget->setItem(row , 10 ,new QTableWidgetItem(QIcon(""), "应用层为FTP 协议类型"));break;
                   case 3: ui->tableWidget->setItem(row , 10,new QTableWidgetItem(QIcon(""), "应用层为TELNET协议类型"));break;
                   case 4: ui->tableWidget->setItem(row , 10,new QTableWidgetItem(QIcon(""), "应用层为SMTP协议类型"));break;
                   case 5: ui->tableWidget->setItem(row , 10,new QTableWidgetItem(QIcon(""), "应用层为POP3协议类型"));break;
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
struct tcp_header
{
    u_int16_t tcp_source_port;
    u_int16_t tcp_destination_port;
    u_int32_t tcp_acknowledgement;//序列号
    u_int32_t tcp_ack;//确认码
#ifdef WORDS_BIGENDIAN
    u_int8_t tcp_offset:4,//偏移
                   tcp_reserved:4;//保留
#else
    u_int8_t tcp_reserved:4,
                  tcp_offset:4;
#endif
    u_int8_t tcp_flags;//标记
    u_int16_t tcp_windows;//窗口大小
    u_int16_t tcp_checksum;//校验和
    u_int16_t tcp_urgent_pointer;//紧急指针
};
void TCPDialog::ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
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
                        case 0x0800: ip_protocol_packet_callback(argument,
                                                                packet_header,
                                                                packet_content );
                        break;
                 default:break;
                 }
         mac_string=ethernet_protocol->ether_shost;
         mac_string=ethernet_protocol->ether_dhost;
  }
void TCPDialog::ip_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
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
          case 6:printf("The Transport Layer Protocol is TCP\n");break;
          case 17:printf("The Transport Layer Protocol is UDP\n");break;
          case 1:printf("The Transport Layer Protocol is ICMP\n");break;
         default:break;
     }
    printf("Header checksum:%d\n",checksum);
    printf("Source address:%s\n",inet_ntoa(ip_protocol->ip_source_address));
    printf("Destination address:%s\n",inet_ntoa(ip_protocol->ip_destination_address));
    switch(ip_protocol->ip_protocol)
    {
        case 6://上层协议为TCP协议
           tcp_protocol_packet_callback(argument,
                                        packet_header,
                                        packet_content);
        break;
       default:break;
       }
}
u_short TCPDialog:: source_port=0;
u_short TCPDialog:: destination_port=0;
u_int16_t TCPDialog::checksum=0;
u_int TCPDialog::sequence=0;
u_int TCPDialog::acknowledgement=0;
u_int TCPDialog::reserved=0;
int TCPDialog:: header_length=0;//首部长度
u_int TCPDialog::flags=0;//标记
u_short TCPDialog::windows=0;//窗口
u_short TCPDialog::urgent_pointer=0;//紧急指针
int TCPDialog::flag_4=0;
void TCPDialog::tcp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
{
    struct tcp_header *tcp_protocol;//定义TCP协议变量

    tcp_protocol=(struct tcp_header *)(packet_content+14+20);
    /*获得TCP协议内容，跳过以太网协议和IP协议部分*/
    source_port=ntohs(tcp_protocol->tcp_source_port);//获得源端口
    destination_port=ntohs(tcp_protocol->tcp_destination_port);//获得目的端口
    header_length =tcp_protocol->tcp_offset*4;//获得首部长度
    sequence=ntohl(tcp_protocol->tcp_acknowledgement);//获得序列号
    acknowledgement=ntohl(tcp_protocol->tcp_ack);//获得确认号
    windows=ntohs(tcp_protocol->tcp_windows);//获得窗口大小
    urgent_pointer=ntohs(tcp_protocol->tcp_urgent_pointer);//获得紧急指针
    flags=tcp_protocol->tcp_flags;//获得标记
    checksum=ntohs(tcp_protocol->tcp_checksum);//获得校验和
    printf("--------------------   TCP protocol (Transport Layer)   -------------------- \n");
    printf("Source Port:%d\n",source_port);
    printf("Destination Port:%d\n",destination_port);
    /*根据目的端口号获得应用层协议类型*/
    switch(destination_port)
    {
    case 80:  flag_4=1;printf("HTTP protocol\n");break;
     /*上层协议为HTTP协议，可在此调用分析HTTP协议的函数？？还需再写*/
    case 21: flag_4=2; printf("FTP protocol\n");break;
    case 23: flag_4=3;printf("TELNET protocol\n");break;
    case 25: flag_4=4;printf("SMTP protocol\n");break;
    case 110: flag_4=5;printf("POP3 protocol\n");break;
    default:break;
    }
printf("Sequence Number:%u\n",sequence);
printf("Acknowledgement Number:%u\n",acknowledgement);
printf("Header Length:%d\n",header_length);
reserved=tcp_protocol->tcp_reserved;
printf("Reserved:%d\n",tcp_protocol->tcp_reserved);
printf("Flags:");//判断标记种类
 if(flags & 0x08)
 {
     flags=1;
     printf("PSH");
 }
 if(flags & 0x10)
 {
     flags=2;
     printf("ACK");
 }
if(flags & 0x02)
{
    flags=3;
    printf("SYN");
}
if(flags & 0x20)
 {
    flags=4;
    printf("URG");
}
if(flags &  0x01)
{
    flags=5;
    printf("FIN");
}
if(flags & 0x04)
{
    flags=6;
    printf("RST");
 }
printf("\n");
printf("Window Size:%d\n",windows);
printf("checksum:%d\n",checksum);
printf("Urgent pointer:%d\n",urgent_pointer);
}
TCPDialog::~TCPDialog()
{
    delete ui;
}

void TCPDialog::on_pushButton_clicked()
{
    this->close();
}
