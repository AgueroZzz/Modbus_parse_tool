#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Modbus Parser");

    ui->_comboBox_order->addItem("大端");
    ui->_comboBox_order->addItem("小端");
    ui->_comboBox_order->setCurrentIndex(0);

    ui->_comboBox_bit_order->addItem("MSB-LSB");
    ui->_comboBox_bit_order->addItem("LSB-MSB");
    ui->_comboBox_bit_order->setCurrentIndex(0);

    ui->_tableWidget_rsp->setColumnCount(4);
    ui->_tableWidget_rsp->setHorizontalHeaderLabels({"Modbus addr", "Hex", "Decimal", "Bin"});
    ui->_tableWidget_rsp->horizontalHeader()->setStretchLastSection(true);
    ui->_tableWidget_rsp->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    try{
        QPair<QString, QString> frame_pair;
        QStringList deploy_list;
        deploy_list << ui->_comboBox_order->currentText();
        deploy_list << ui->_comboBox_bit_order->currentText();
        QString req_frame_str = cleanText(ui->_textEdit_req->toPlainText());
        QString rsp_frame_str = cleanText(ui->_textEdit_rsp->toPlainText());
        if(!req_frame_str.isEmpty()){
            if(!parser::isHex(req_frame_str)){
                QMessageBox::warning(this, "Error", "Modbus request frame is illegitimate");
                return;
            }
        }
        if(!rsp_frame_str.isEmpty()){
            if(!parser::isHex(rsp_frame_str)){
                QMessageBox::warning(this, "Error", "Modbus response frame is illegitimate");
                return;
            }
        }
        frame_pair.first = req_frame_str;
        frame_pair.second = rsp_frame_str;

        parser _parser(deploy_list, frame_pair);
        emit sig_display_data(_parser.get_value());
    }catch(const QString& error){
        QMessageBox::warning(this, "Error", error);
    }
}

void Widget::slot_display_data(const QList<QStringList> &data)
{
    ui->_tableWidget_rsp->setRowCount(data.size());
    for (int row = 0; row < data.size(); ++row) {
        for (int col = 0; col < 4; ++col) {
            ui->_tableWidget_rsp->setItem(row, col, new QTableWidgetItem(data[row][col]));
        }
    }
}
