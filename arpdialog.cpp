#include "arpdialog.h"
#include "ui_arpdialog.h"
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
ARPDialog::ARPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ARPDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->label->setFont(QFont(" ", 13, QFont::Bold));
    ui->label->setText(QObject::tr("<font color = blue>%1</font>").arg("ARP数据包"));
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    //ui->tableWidget->setRowCount(65535);
    QStringList column;
    column.append("硬件类型");
    column.append("协议类型");
    column.append("硬件地址长度");
    column.append("协议地址长度");
    column.append("ARP操作码");
    column.append("请求／应答");
    column.append("源IP地址");
    column.append("目的IP地址");
    ui->tableWidget->setColumnWidth(0,150);
    ui->tableWidget->setColumnWidth(1,00);
    ui->tableWidget->setColumnWidth(2,150);
    ui->tableWidget->setColumnWidth(3,150);
    ui->tableWidget->setColumnWidth(4,80);
    ui->tableWidget->setColumnWidth(5,150);
    ui->tableWidget->setColumnWidth(6,180);
    ui->tableWidget->setColumnWidth(7,180);
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
    for(int i=0;i<4;i++)
    {
        if(pcap_loop(pcap_handle, 1,ethernet_protocol_packet_callback,NULL)==0)
      {
             int row = ui->tableWidget->rowCount();
             ui->tableWidget->insertRow(row);
             ui->tableWidget->setItem(row , 0 ,new QTableWidgetItem(QIcon(""), QString::number(hardwaretype)));
             ui->tableWidget->setItem(row , 1 ,new QTableWidgetItem(QIcon(""),QString::number(protocoltype)));
             ui->tableWidget->setItem(row , 2 ,new QTableWidgetItem(QIcon(""),QString::number(hardwarelength)));
             ui->tableWidget->setItem(row , 3 ,new QTableWidgetItem(QIcon(""),QString::number(protocollength)));
             ui->tableWidget->setItem(row , 4 ,new QTableWidgetItem(QIcon(""),QString::number(operationcode)));
             switch(flag_2)
            {
                   case 0: ui->tableWidget->setItem(row , 5 ,new QTableWidgetItem(QIcon(""), "不确定的协议层类型"));break;
                   case 1: ui->tableWidget->setItem(row , 5,new QTableWidgetItem(QIcon(""), "ARP 请求协议类型"));break;
                   case 2: ui->tableWidget->setItem(row , 5 ,new QTableWidgetItem(QIcon(""), "ARP 回显协议类型"));break;
                   case 3: ui->tableWidget->setItem(row , 5 ,new QTableWidgetItem(QIcon(""), "RARP 请求协议类型"));break;
                   case 4: ui->tableWidget->setItem(row , 5 ,new QTableWidgetItem(QIcon(""), "RARP 回显协议类型"));break;
                   default:break;
            }
             ui->tableWidget->setItem(row , 6 ,new QTableWidgetItem(QIcon(""),strcat("会话的源IP地址:",src)));
             ui->tableWidget->setItem(row , 7,new QTableWidgetItem(QIcon(""),strcat("会话的目的IP地址:",dst)));
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
/*typedef u_int32_t in_addr_t;
struct in_addr_t{
    in_addr_t s_addr;
};*/

struct arp_header
{
    u_int16_t arp_hardware_type;//硬件类型
    u_int16_t arp_protocol_type;//协议类型
    u_int8_t arp_hardware_length;//硬件地址长度
    u_int8_t arp_protocol_length;//协议地址长度
    u_int16_t arp_operation_code;//ARP操作码
    u_int8_t arp_source_ethernet_address[6];//源以太网地址
    u_int8_t arp_source_ip_address[4];//源IP地址
    u_int8_t arp_destination_ethernet_address[6];//目的以太网地址
    u_int8_t arp_destination_ip_address[4];//目的IP地址
};
void ARPDialog::ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
  {

         //const u_char *packet_content;
         u_char *mac_string;
         int packet_number=0;
         struct pcap_pkthdr *protocol_header;
         struct ether_header *ethernet_protocol;
         u_short ethernet_type;
         packet_number++;
         ethernet_protocol=(struct ether_header *)packet_content;
         ethernet_type=ntohs(ethernet_protocol->ether_type);
         switch(ethernet_type)
        {
                        case 0x0806:arp_protocol_packet_callback(argument,
                                                                packet_header,
                                                                packet_content);
                         break;
                 default:break;
          }
  }
u_short  ARPDialog:: protocoltype=0;
u_short  ARPDialog:: hardwaretype=0;
u_short  ARPDialog:: hardwarelength=0;//硬件地址长度
u_short  ARPDialog:: protocollength=0;//协议地址长度
u_short  ARPDialog:: operationcode=0;//操作码
char ARPDialog::src[100]="";
char ARPDialog::dst[100]="";
int ARPDialog::flag_2=0;
void ARPDialog::arp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
{
    struct arp_header *arp_protocol;
    u_char *mac_string;
    struct in_addr source_ip_address;
    struct in_addr destination_ip_address;
    printf("--------------------   ARP Protocol (Network Layer)   --------------------\n");
    arp_protocol=(struct arp_header *)(packet_content+14);
    /*要获得arp协议数据内容，需跳过以太网协议部分*/
    hardwaretype=ntohs(arp_protocol->arp_hardware_type);
    protocoltype=ntohs(arp_protocol->arp_protocol_type);
    operationcode=ntohs(arp_protocol->arp_operation_code);
    hardwarelength=arp_protocol->arp_hardware_length;
    protocollength=arp_protocol-> arp_protocol_length;
    printf("ARP Hardware Type:%d\n",hardwaretype);
    printf("ARP Protocol Type:%d\n",protocoltype);
    printf("ARP Hardware Length:%d\n",hardwarelength);
    printf("ARP Protocol Length:%d\n",protocollength);
    printf("ARP Operation:%d\n",operationcode);
    switch(operationcode)
    {/*根据操作码判断ARP协议类型*/
           case 1: flag_2=1;printf("ARP Request Protocol\n");break;
            //ARP请求协议
           case 2:flag_2=2;printf("ARP Reply Protocol\n");break;
            //ARP应答协议
           case 3:flag_2=3;printf("RARP Request Protocol\n");break;
            //RARP请求协议
           case 4:flag_2=4;printf("RARP Reply Protocol\n");break;
            //RARP应答协议
           default:break;
    }
printf("Ethernet Source Address is:");
mac_string=arp_protocol->arp_source_ethernet_address;
printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*mac_string,*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
//打印源以太网地址
memcpy((void*)&source_ip_address,(void *)&arp_protocol->arp_source_ip_address,sizeof(struct in_addr));
strcpy(src,inet_ntoa(source_ip_address));
printf("Source IP Address:%s\n",inet_ntoa(source_ip_address));
printf("Ethernet Destination Addresss is :");
mac_string=arp_protocol->arp_destination_ethernet_address;
printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*mac_string,*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
//打印目的以太网地址
strcpy(dst,inet_ntoa(destination_ip_address));
//memcpy((void*)&destination_ip_address,(void *)&arp_protocol->arp_destination_ip_address,sizeof(struct in_addr));
printf("Destination IP Address:%s\n",inet_ntoa(destination_ip_address));
}
ARPDialog::~ARPDialog()
{
    delete ui;
}

void ARPDialog::on_pushButton_clicked()
{
    this->close();
}
