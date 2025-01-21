#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Modbus Parser");
    ui->_comboBox_modbus_type->addItem("");
    ui->_comboBox_modbus_type->addItem("ModbusRTU");
    ui->_comboBox_modbus_type->addItem("ModbusTCP");
    ui->_comboBox_modbus_type->setCurrentIndex(0);

    ui->_comboBox_order->addItem("ABCDEFGH");
    ui->_comboBox_order->addItem("BADCFEHG");
    ui->_comboBox_order->addItem("CDABGHEF");
    ui->_comboBox_order->addItem("DCBAHGFE");
    ui->_comboBox_order->addItem("EFGHABCD");
    ui->_comboBox_order->addItem("FEHGBADC");
    ui->_comboBox_order->addItem("GHEFCDAB");
    ui->_comboBox_order->addItem("HGFEDCBA");
    ui->_comboBox_order->setCurrentIndex(0);

    ui->_comboBox_bit_order->addItem("MSB-LSB");
    ui->_comboBox_bit_order->addItem("LSB-MSB");
    ui->_comboBox_bit_order->setCurrentIndex(0);

    ui->_tableWidget_rsp->setColumnCount(3);
    ui->_tableWidget_rsp->setHorizontalHeaderLabels({"Hex", "Decimal", "Bin"});
    ui->_tableWidget_rsp->horizontalHeader()->setStretchLastSection(true);
    ui->_tableWidget_rsp->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->_tableWidget_req->setColumnCount(3);
    ui->_tableWidget_req->setHorizontalHeaderLabels({"Hex", "Decimal", "Bin"});
    ui->_tableWidget_req->horizontalHeader()->setStretchLastSection(true);
    ui->_tableWidget_req->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QObject::connect(ui->_pushButton_count, &QPushButton::clicked, this, &Widget::slot_button_clicked, Qt::DirectConnection);
    QObject::connect(this, &Widget::sig_display_data, this, &Widget::slot_display_data, Qt::DirectConnection);
    QObject::connect(ui->_pushButton_clear, &QPushButton::clicked, this, [=](){
        ui->_textEdit_req->clear();
        ui->_textEdit_rsp->clear();
    });
    QObject::connect(ui->_comboBox_bit_order, &QComboBox::currentIndexChanged, this, [=](int index){
        if(index == 1){
            QMessageBox::warning(this, "Error", "LSB-MSB is unavailable now");
            ui->_comboBox_bit_order->setCurrentIndex(0);
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slot_button_clicked()
{
    if(ui->_comboBox_modbus_type->currentIndex() == 0){
        QMessageBox::warning(this, "Error", "please check modbus protocol type");
        return;
    }
    try{
        QStringList deploy_list;
        deploy_list << ui->_comboBox_modbus_type->currentText();
        deploy_list << ui->_comboBox_order->currentText();
        deploy_list << ui->_comboBox_bit_order->currentText();
        QString req_frame_str = cleanText(ui->_textEdit_req->toPlainText());
        QString rsp_frame_str = cleanText(ui->_textEdit_rsp->toPlainText());
        // qDebug() << req_frame_str << "---" << rsp_frame_str;
        if(!req_frame_str.isEmpty()){
            if(!parser::isHex(req_frame_str)){
                QMessageBox::warning(this, "Error", "Modbus request frame is illegitimate");
                return;
            }
            req_frame_parse req_parse(deploy_list, req_frame_str);
            emit sig_display_data(req_parse.get_list(), "REQ");
        }
        if(!rsp_frame_str.isEmpty()){
            if(!parser::isHex(rsp_frame_str)){
                QMessageBox::warning(this, "Error", "Modbus response frame is illegitimate");
                return;
            }
            rsp_frame_parse rsp_parse(deploy_list, rsp_frame_str);
            QList<QStringList> temp_data = rsp_parse.get_list();
            emit sig_display_data(rsp_parse.get_list(), "RSP");
        }
    }catch(const QString& error){
        QMessageBox::warning(this, "Error", error);
    }
}

void Widget::slot_display_data(const QList<QStringList> &data, const QString &type)
{
    if(type == "REQ"){
        ui->_tableWidget_req->setRowCount(data.size());
    }else if(type == "RSP"){
        ui->_tableWidget_rsp->setRowCount(data.size());
    }
    for(int i = 0; i < data.size(); ++i){
        const QStringList& item_list = data[i];
        for(int j = 0; j < item_list.size(); ++j){
            QTableWidgetItem* item = new QTableWidgetItem(item_list[j]);
            if(type == "REQ"){
                ui->_tableWidget_req->setItem(i, j, item);
            }else if(type == "RSP"){
                ui->_tableWidget_rsp->setItem(i, j, item);
            }
        }
    }
}
