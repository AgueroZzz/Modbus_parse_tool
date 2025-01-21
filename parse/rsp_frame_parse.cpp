#include "rsp_frame_parse.h"

rsp_frame_parse::rsp_frame_parse(const QStringList &deploy, const QString &rsp_frame, QObject *parent)
    : parser{deploy, parent}
{
    QByteArray temp_frame = string2bytearray(rsp_frame);
    if(_protocol_type == "MODBUSTCP"){
        if(rsp_frame.size() < 12){
            throw QString("ModbusTCP response frame is error");
        }else if(rsp_frame.size() != ((static_cast<quint8>(temp_frame.at(4)) | static_cast<quint8>(temp_frame.at(5))) + 6)){
            throw QString("ModbusTCP response frame is error");
        }
    }else if(_protocol_type == "MODBUSRTU"){
        quint16 rsp_frame_crc = static_cast<quint8>(temp_frame.at(temp_frame.size() - 1)) | static_cast<quint8>(temp_frame.at(temp_frame.size() - 2));
        if(rsp_frame.size() < 6){
            throw QString("MODBUSRTU response frame is error");
        }else if(crc_calu(temp_frame.mid(0, sizeof(temp_frame.size() - 3))) != rsp_frame_crc){
            // qDebug() << "rsp frame cal crc is " << temp_frame.mid(0, sizeof(temp_frame.size() - 3)) << "---" << "rsp frame crc is " << rsp_frame_crc;
            throw QString("MODBUSRTU response frame is error");
        }
    }

    this->_rsp_frame = temp_frame;
    get_addr2data();
}

void rsp_frame_parse::get_addr2data()
{
    for (int i = 0; i < _rsp_frame.size(); ++i) {
        QStringList temp_list;
        temp_list << QString::number(_rsp_frame[i] & 0xFF, 16).rightJustified(2, '0');
        temp_list << QString::number(static_cast<unsigned char>(_rsp_frame[i]));
        temp_list << QString::number(_rsp_frame[i] & 0xFF, 2).rightJustified(8, '0');
        no2hex2dec.append(temp_list);
    }
}
