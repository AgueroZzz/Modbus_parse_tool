#include "parser.h"

parser::parser(const QStringList &deploy, const QPair<QString, QString>& frame_pair, QObject *parent)
    : QObject{parent}
{
    _byte_order = deploy[deploy_byte_order];
    _bit_order = deploy[deploy_bit_order];

    _isLittleEndain = (_byte_order == "小端");

    this->_req_frame = string2bytearray(frame_pair.first);
    this->_rsp_frame = string2bytearray(frame_pair.second);

    if(is_modbus_rtu(this->_req_frame)){
        _protocol_type = "MODBUSRTU";
        if(this->_req_frame.size() < 8){
            throw QString("MODBUSRTU request frame is error");
        }
        if(this->_rsp_frame.size() < 6){
            throw QString("ModbusRTU response frame is error");
        }
    }else{
        _protocol_type = "MODBUSTCP";
        if(this->_req_frame.size() < 12){
            throw QString("ModbusTCP request frame is error");
        }else if(this->_req_frame.size() != ((static_cast<quint8>(this->_req_frame.at(4)) | static_cast<quint8>(this->_req_frame.at(5))) + 6)){
            qDebug() << ((static_cast<quint8>(this->_req_frame.at(4)) | static_cast<quint8>(this->_req_frame.at(5))) + 6);
            throw QString("ModbusTCP request frame is error");
        }
        if(this->_rsp_frame.size() < 12){
            throw QString("ModbusTCP response frame is error");
        }else if(this->_rsp_frame.size() != ((static_cast<quint8>(this->_rsp_frame.at(4)) | static_cast<quint8>(this->_rsp_frame.at(5))) + 6)){
            throw QString("ModbusTCP response frame is error");
        }
    }

    parse_frame();
}

void parser::parse_frame()
{
    QByteArray _req_pdu;
    QByteArray _rsp_pdu;
    if(this->_protocol_type == "MODBUSTCP"){
        _req_pdu = this->_req_frame.mid(7);
        _rsp_pdu = this->_rsp_frame.mid(7);

    }else if(this->_protocol_type == "MODBUSRTU"){
        _req_pdu = this->_req_frame.mid(1);
        _rsp_pdu = this->_rsp_frame.mid(1);
    }
    quint8 _func_code = static_cast<quint8>(_req_pdu.at(0));
    quint16 _start_register_addr = this->bytearray2quint16(_req_pdu.mid(1, 2));
    quint32 _start_modbus_addr = this->registeraddr2modbusaddr(_start_register_addr, _func_code);
    QDataStream stream(_rsp_pdu.mid(2));
    if(_isLittleEndain){
        stream.setByteOrder(QDataStream::LittleEndian);
    }else{
        stream.setByteOrder(QDataStream::BigEndian);
    }
    while(!stream.atEnd()){
        QStringList _value_list;
        quint16 regValue;
        stream >> regValue;
        QByteArray regHex = QByteArray::number(regValue, 16).toUpper().rightJustified(4, '0');
        QString regBin = QString::number(regValue, 2).rightJustified(16, '0');
        _value_list << QString::number(_start_modbus_addr) << QString::fromUtf8(regHex) << QString::number(regValue) << regBin;
        ++_start_modbus_addr;
        _all_list.append(_value_list);
    }
}
