#include "req_frame_parse.h"

req_frame_parse::req_frame_parse(const QStringList &deploy, const QString &req_frame, QObject *parent)
    : parser{deploy, parent}
{
    QByteArray temp_frame = string2bytearray(req_frame);
    if(_protocol_type == "MODBUSTCP"){
        if(req_frame.size() < 12){
            throw QString("ModbusTCP request frame is error");
        }else if(req_frame.size() != ((static_cast<quint8>(temp_frame.at(4)) | static_cast<quint8>(temp_frame.at(5))) + 6)){
            throw QString("ModbusTCP request frame is error");
        }
    }else if(_protocol_type == "MODBUSRTU"){
        quint16 req_frame_crc = static_cast<quint8>(temp_frame.at(temp_frame.size() - 1)) | static_cast<quint8>(temp_frame.at(temp_frame.size() - 2));
        if(req_frame.size() < 8){
            throw QString("MODBUSRTU request frame is error");
        }else if(crc_calu(temp_frame.mid(0, sizeof(temp_frame.size() - 3))) != req_frame_crc){
            // qDebug() << "req frame cal crc is " << req_frame.mid(0, sizeof(req_frame.size() - 3)) << "---" << "req frame crc is " << req_frame_crc;
            throw QString("MODBUSRTU request frame is error");
        }
    }

    _req_frame = temp_frame;
    get_addr2data();
}

void req_frame_parse::get_addr2data()
{
    for (int i = 0; i < _req_frame.size(); ++i) {
        QStringList temp_list;
        temp_list << QString::number(_req_frame[i] & 0xFF, 16).rightJustified(2, '0');
        temp_list << QString::number(static_cast<unsigned char>(_req_frame[i]));
        temp_list << QString::number(_req_frame[i] & 0xFF, 2).rightJustified(8, '0');
        no2hex2dec.append(temp_list);
    }
}
