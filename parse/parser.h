#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QByteArray>
#include <QPair>
#include <QRegularExpression>
#include <QDebug>
#include <QMap>

class parser : public QObject
{
    Q_OBJECT
public:
    explicit parser(const QStringList& deploy, QObject *parent = nullptr);

    enum idx_deploy{
        deploy_type = 0,
        deploy_byte_order,
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
        if(_isLittleEndain){
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

    virtual void get_addr2data() = 0;

signals:

public:
    QString _protocol_type;
    QString _byte_order;
    QString _bit_order;
    bool _isLittleEndain;
};

#endif // PARSER_H
