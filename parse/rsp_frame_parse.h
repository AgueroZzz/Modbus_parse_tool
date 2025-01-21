#ifndef RSP_FRAME_PARSE_H
#define RSP_FRAME_PARSE_H

#include "parse/parser.h"
#include <QObject>
class rsp_frame_parse : public parser
{
    Q_OBJECT
public:
    explicit rsp_frame_parse(const QStringList& deploy, const QString& rsp_frame, QObject *parent = nullptr);

    inline QList<QStringList> get_list(){
        return no2hex2dec;
    }

signals:

private:
    QByteArray _rsp_frame;

    QList<QStringList> no2hex2dec;

    // parser interface
public:
    void get_addr2data() override;
};

#endif // RSP_FRAME_PARSE_H
