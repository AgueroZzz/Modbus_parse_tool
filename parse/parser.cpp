#include "parser.h"

parser::parser(const QStringList &deploy, QObject *parent)
    : QObject{parent}
{
    _protocol_type = deploy[deploy_type];
    _byte_order = deploy[deploy_byte_order];
    _bit_order = deploy[deploy_bit_order];

    _isLittleEndain = (_byte_order.contains("BADC") || _byte_order.contains("DCBA") || _byte_order.contains("FEHG") || _byte_order.contains("HGFEDCBA"));
}
