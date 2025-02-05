#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QByteArray>
#include <QPair>
#include <QRegularExpression>
#include <QDebug>
#include <QMap>
#include <QtEndian>

class parser : public QObject
{
    Q_OBJECT
public:
    explicit parser(const QStringList& deploy, const QPair<QString, QString>& frame_pair, QObject *parent = nullptr);

    enum idx_deploy{
        deploy_byte_order = 0,
        deploy_bit_order
    };

    static bool isHex(const QString& str){
        static QRegularExpression  hexPattern("^[0-9a-fA-F]+$");
        return hexPattern.match(str).hasMatch();
    }

    inline quint16 crc_calu(const QByteArray& frame){
        quint16 crc = 0xFFFF;
        for (int i = 0; i < frame.size(); ++i) {
            crc ^= (quint8)frame[i];
            for (int j = 0; j < 8; ++j) {
                if (crc & 0x0001) {
                    crc = (crc >> 1) ^ 0xA001;
                } else {
                    crc >>= 1;
                }
            }
        }
        if(!_isLittleEndain){
            return ((crc >> 8) & 0xFF) | ((crc << 8) & 0xFF00);
        }
        return crc;
    }

    inline QByteArray string2bytearray(const QString& str){
        QByteArray hex_byte;
        for(int i = 0; i < str.length(); i += 2){
            QByteArray byte = QByteArray::fromHex(str.mid(i, 2).toUtf8());
            hex_byte.append(byte);
        }
        return hex_byte;
    }

    inline bool is_modbus_rtu(const QByteArray& frame){
        QByteArray crc_byte = frame.right(2);
        quint16 crc_value;
        if(_isLittleEndain){
            crc_value = qFromLittleEndian<quint16>(reinterpret_cast<const uchar*>(crc_byte.data()));
        }else{
            crc_value = qFromBigEndian<quint16>(reinterpret_cast<const uchar*>(crc_byte.data()));
        }
        quint16 crc_calu_value = crc_calu(frame.mid(0, frame.size() - 2));
        if(crc_calu_value == crc_value){
            return true;
        }else{
            return false;
        }
    }

    inline quint16 bytearray2quint16(const QByteArray& array){
        if(_isLittleEndain){
            return qFromLittleEndian<quint16>(reinterpret_cast<const char*>(array.data()));
        }else{
            return qFromBigEndian<quint16>(reinterpret_cast<const char*>(array.data()));
        }
    }

    inline quint32 registeraddr2modbusaddr(const quint16& register_addr, const quint8& func_code){
        if(func_code == 0){
            return register_addr;
        }else if(func_code == 1){
            return register_addr + 100001;
        }else if(func_code == 3){
            return register_addr + 400001;
        }else if(func_code == 4){
            return register_addr + 300001;
        }
        return 0;
    }

    inline QList<QStringList> get_value(){
        return this->_all_list;
    }

    void parse_frame();



private:
    QString _protocol_type;
    QString _byte_order;
    QString _bit_order;
    bool _isLittleEndain;

    QByteArray _req_frame;
    QByteArray _rsp_frame;

    QList<QStringList> _all_list;
};

#endif // PARSER_H
