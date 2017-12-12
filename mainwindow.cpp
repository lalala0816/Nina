#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include<string.h>
#include<QtGui>
#include<QLayout>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QDialogButtonBox>
#include<QMessageBox>
#include <QFont>
#include<QUrl>
#include <QNetworkInterface>
#include<QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainTable->setFont(QFont("Helvetica"));
    ui->mainTable->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->mainTable->setShowGrid(true);//设置不显示格子线
    ui->mainTable->setFrameShape(QFrame::NoFrame);
   // Qt::WindowMinMaxButtonsHint;
    QStringList column;
    QHeaderView* headerView = ui->mainTable->verticalHeader();//去掉自带的行号
    headerView->setHidden(true);
    ui->mainTable->setFrameShape(QFrame::NoFrame); //设置无边框
   // ui->mainTable->setFont();
    column.append("包序号");
    column.append("其它信息");
    column.append("上层协议类型");
    ui->mainTable->setColumnWidth(0,100);
    //ui->mainTable->setColumnWidth(1,150);
    ui->mainTable->setColumnWidth(1,350);
    ui->mainTable->setColumnWidth(2,300);
    ui->mainTable->setHorizontalHeaderLabels(column);
    ui->mainTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//只用来显示信息，将表格设置为只读模式
    pcap_t* pcap_handle;//Libpcap句柄
    char error_content[PCAP_ERRBUF_SIZE];//存储错误内容
    char *net_interface;//网络接口
    struct in_addr net_ip_address;//网络地址
    struct in_addr net_mask_address;//掩码地址
    char *net_ip_string;//网络地址字符串形式
    char *net_mask_string;
    struct bpf_program bpf_filter;//BPF过滤规则
    char bpf_filter_string[]=" ";//过滤规则字符串，此处可自己设定
    ui->Edit->setText(bpf_filter_string);
    bpf_u_int32 net_mask;//网络掩码
    bpf_u_int32 net_ip;//网络地址
    //struct pcap_stat *t;
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
    printf("Network interface is:%s\n",net_interface);
    ui->Edit_1->setText(net_interface);
    net_ip_address.s_addr=net_ip;
    net_ip_string=inet_ntoa(net_ip_address);
    ui->Edit_2->setText(net_ip_string);
    printf("Network IP Address is:%s\n",net_ip_string);
    net_mask_address.s_addr=net_mask;
    net_mask_string=inet_ntoa(net_mask_address);
    ui->Edit_3->setText(net_mask_string);
    printf("Network Mask Address is:%s\n",net_mask_string);
    if(pcap_datalink(pcap_handle)!=DLT_EN10MB)//........
                printf("Doesn't exist within the local area network LAN");
    for(int i=0;i<10;i++)
    {
        if(pcap_loop(pcap_handle, 1,packet_callback,NULL)==0)
      {
             int row = ui->mainTable->rowCount();
             ui->mainTable->insertRow(row);
             ui->mainTable->setItem(row , 0 ,new QTableWidgetItem(QIcon(""), QString::number(packet_number)));
             //ui->mainTable->setItem(row , 1 ,new QTableWidgetItem(QIcon(""), inet_ntoa(net_ip_address)));
             char temp[10];
             sprintf(temp , "%04x" ,  ethernet_type);
             char s[100]="以太网协议类型: 0x";
             ui->mainTable->setItem(row , 1 ,new QTableWidgetItem(QIcon(""),strcat(strcat(strcat(s,temp),"   ,  网络地址  :"),inet_ntoa(net_ip_address))));
            switch(flag)
            {
                   case 0: ui->mainTable->setItem(row , 2 ,new QTableWidgetItem(QIcon(""), "This Protocol is Unknown"));break;
                   case 1: ui->mainTable->setItem(row , 2 ,new QTableWidgetItem(QIcon(""), "The network layer is IP protocol"));break;
                   case 2: ui->mainTable->setItem(row , 2 ,new QTableWidgetItem(QIcon(""), "The network layer is ARP protocol"));break;
                   case 3: ui->mainTable->setItem(row , 2 ,new QTableWidgetItem(QIcon(""), "The network layer is RARP protocol"));break;
                   default:break;
            }
    }
    }
    ui->Edit_4->setText(QString::number(packet_number));
   pcap_close(pcap_handle);//关闭Libpcap操作
    }

MainWindow::~MainWindow()
{
    delete ui;
}
struct ether_header
{
    u_int8_t ether_dhost[6];//目的以太网地址
    u_int8_t ether_shost[6];//源
    u_int16_t ether_type;//以太网类型
};

int MainWindow::packet_number=0;//定义静态变量记录抓取的数据包个数
u_short MainWindow::ethernet_type;
int MainWindow::flag=0;
void MainWindow::packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
  {

         //const u_char *packet_content;
         u_char *mac_string;
         struct pcap_pkthdr *protocol_header;
         struct ether_header *ethernet_protocol;
         packet_number++;
         printf("****************************************************************************\n");
         printf("The %dth packet is captured.\n",packet_number);
         printf("--------------------   Ethernet Protocol (Link Layer)   --------------------\n");
         ethernet_protocol=(struct ether_header *)packet_content;
         /*获取以太网协议的内容*/
         //printf("Capture the len of packet is:\n");
        // printf("%d\n", protocol_header->len);
         printf("Ethernet type is:");
         ethernet_type=ntohs(ethernet_protocol->ether_type);
         printf("%04x\n",ethernet_type);
         switch(ethernet_type)
         {
                  /*根据以太网协议类型字段判断上层协议内容*/
                 case 0x0800:  flag=1;printf("The network layer is IP protocol\n");
                 break;
                 case 0x0806: flag=2; printf("The network layer is ARP protocol\n");
                 break;
                 case 0x8035: flag=3;printf("The network layer is RARP protocol\n");
                 break;
                 default:break;
         }
         printf("Mac Source Address is:");
         mac_string=ethernet_protocol->ether_shost;
         //char mac1[10]=" ";
         /*for(int j=0;j<6;j++)
         {
              sprintf(mac1,"%02x",ethernet_protocol->ether_shost[j]);
              strcat(mac,mac1);
              //printf("%02x:",ethernet_protocol->ether_shost[j]);
         }*/
         printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*mac_string,*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
         printf("Mac Destination Address is:");
         mac_string=ethernet_protocol->ether_dhost;
         printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*mac_string,*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
         //if (packet_number==5) system("pause");//可设置停止的位置
        // if (packet_number%5==0) getchar();

    }

void MainWindow::on_Button_1_clicked()
{
    QMessageBox::information(this,"Setting","OK");
}


void MainWindow::on_Button_2_clicked()
{
    QString link="http://wenku.baidu.com/view/102a56bb27284b73f24250cc";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_pushButton_clicked()
{
    EtherDialog etherdialog;
    etherdialog.setModal(true);
    etherdialog.exec();
   //etherdialog=new EtherDialog(this);
    //etherdialog->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    IPDialog ipdialog;
    ipdialog.setModal(true);
    ipdialog.exec();
}
void MainWindow::on_pushButton_3_clicked()
{
    ICMPDialog icmpdialog;
    icmpdialog.setModal(true);
    icmpdialog.exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    ARPDialog arpdialog;
    arpdialog.setModal(true);
    arpdialog.exec();
}

void MainWindow::on_pushButton_5_clicked()
{
    TCPDialog tcpdialog;
    tcpdialog.setModal(true);
    tcpdialog.exec();
}

void MainWindow::on_pushButton_6_clicked()
{
    UDPDialog udpdialog;
    udpdialog.setModal(true);
    udpdialog.exec();
}

void MainWindow::on_pushButton_7_clicked()
{
    QString filename=QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/home/zhaoli/桌面/421_1",
                "All files(*.*);;Text File(*.log)");
}


void MainWindow::on_pushButton_8_clicked()
{
    this->close();
}
