# Modbus_parse_tool

用于粗略解析Modbus响应帧的每个寄存器的modbus地址-寄存器十六进制-寄存器十进制-寄存器二进制的值

![image](https://github.com/user-attachments/assets/be658273-8175-48f4-ba44-536112d61905)

Version0.0.1版本：
1、可以显示帧每个byte的十六进制-十进制-二进制数据

Version0.0.2版本优化：
1、优化了Modbus协议选择，无需人工选择TCP或者RTU协议，可由程序自行判断
2、增加了寄存器地址转化为Modbus地址显示
3、优化了布局设计
