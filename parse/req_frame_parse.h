#ifndef REQ_FRAME_PARSE_H
#define REQ_FRAME_PARSE_H

#include "parse/parser.h"
#include <QObject>

class req_frame_parse : public parser
{
    Q_OBJECT
public:
    explicit req_frame_parse(const QStringList& deploy, const QString& req_frame, QObject *parent = nullptr);

    enum idx_req_frame_rtu{
        req_rtu_addr = 0,
        req_rtu_func_code = 1,
        req_rtu_start_addr = 2,
        req_rtu_register_num = 4,
        req_rtu_crc = 6
    };

    enum idx_req_frame_tcp{
        req_tcp_no = 0,
        req_tcp_protocol = 2,
        req_tcp_length = 4,
        req_tcp_addr = 6,
        req_tcp_func_code = 7,
        req_tcp_start_addr = 8,
        req_tcp_register_num = 10
    };

    inline QList<QStringList> get_list(){
        return no2hex2dec;
    }

signals:
    void sig_display_req_frame(const QList<QStringList>& data);

private:
    QByteArray _req_frame;

    QList<QStringList> no2hex2dec;


    // quint16 _tcp_mbap_no;
    // quint16 _tcp_protocol_type;
    // quint16 _tcp_length;
    // quint8 _modbus_addr;
    // quint8 _modbus_func;
    // quint16 _modbus_start_addr;
    // quint16 _modbus_register_num;
    // QMap<quint16, quint16> _modbus_register_data;
    // quint16 _rtu_crc;

    // parser interface
public:
    void get_addr2data() override;
};

#endif // REQ_FRAME_PARSE_H
