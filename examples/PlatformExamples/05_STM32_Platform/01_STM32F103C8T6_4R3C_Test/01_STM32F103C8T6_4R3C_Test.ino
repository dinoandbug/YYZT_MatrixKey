/**
 * 程序功能：获取矩阵按键的键值
 * 使用教程：https://blog.yyzt.site/
 * 注意事项：1.行引脚作为输入（默认为上拉输入），列引脚作为输出
 *          2.如果行引脚没有做硬件上拉，那么行引脚需要具有软件上拉功能
 * 参考资料：https://www.arduino.cn/thread-91244-1-1.html
 */

#include <Arduino.h>         // 导入Arduino库文件
#include <YYZT_MatrixKey.h>  // 导入YYZT_MatrixKey库文件

#define debugHardwareSerialRxPin PA10                                                    // STM32F103C8T6的A10引脚（PA10）,串口接收引脚（RX）
#define debugHardwareSerialTxPin PA9                                                     // STM32F103C8T6的A9引脚（PA9）,串口发送引脚（TX）
HardwareSerial debugHardwareSerial(debugHardwareSerialRxPin, debugHardwareSerialTxPin);  // 创建调试串口实例
#define debugState true                                                                  // 串口调试打印状态，true--开启调试打印，false--关闭调试打印
#define debugSerial debugHardwareSerial                                                  // 调试打印的串口
#define debugSerialBuadrate 115200                                                       // 调试打印的串口波特率

#define continuePressedState false                        // 矩阵按键持续按下的状态，为false，则按下按键不松开时只返回1次键值，为true，则按下按键不松开时一直返回键值，默认为false
#define numberOfRows 4                                    // 矩阵按键的行数
#define numberOfColumns 3                                 // 矩阵按键的列数
uint8_t rowsPin[numberOfRows] = {PB11, PB10, PB1, PB0};   // STM32F103C8T6开发板的B11、B10、B1、B0引脚接矩阵键盘的行引脚（1-4）
uint8_t columnsPin[numberOfColumns] = {PA7, PA6, PA5};    // STM32F103C8T6开发板的A7、A6、A5引脚接矩阵键盘的列引脚（1-3）
String returnKeyValue[numberOfRows][numberOfColumns] = {  // 矩阵键盘按下后的返回键值，支持任意字符串
    {"1", "2", "3"},
    {"4", "5", "6"},
    {"7", "8", "9"},
    {"*", "0", "#"}};

MatrixKey matrixKey;  // 创建矩阵按键对象

void setup() {
  delay(1000);  // 等待设备上电稳定

#if debugState
  debugSerial.begin(debugSerialBuadrate);  // 初始化调试打印的串口
#endif

  matrixKey.setPinsAndNumbers(rowsPin, columnsPin, numberOfRows, numberOfColumns);  // 设置行列引脚和行列数
  matrixKey.setReturnKeyValue(returnKeyValue[0]);                                   // 设置返回键值
  matrixKey.setContinuePressedState(continuePressedState);                          // 设置持续按下的状态

#if debugState
  debugSerial.println(F("\r\n程序初始化完成！"));
#endif
}

void loop() {
  String keyValue = matrixKey.getKeyValue();  // 读取键值
  if (keyValue != MatrixKeyNone) {            // 如果键值不为MatrixKeyNone，即表示按下了按键
#if debugState
    debugSerial.print(F("键值："));
    debugSerial.println(keyValue);  // 打印对应的键值
#endif
  }
}
