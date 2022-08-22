/**
 * 程序功能：获取矩阵按键按下后的键值
 * 使用教程：https://blog.yyzt.site/
 * 注意事项：1.行引脚作为输入（默认为上拉输入），列引脚作为输出
 *          2.如果行引脚没有做硬件上拉，那么行引脚需要具有软件上拉功能
 * 参考资料：https://www.arduino.cn/thread-91244-1-1.html
 */

#ifndef YYZT_MATRIXKEY_H
#define YYZT_MATRIXKEY_H

#include <Arduino.h>

#define YYZT_MatrixKeyLibraryVersion 0.0    // 库版本
#define YYZT_abs(x) ((x) > 0 ? (x) : -(x))  // 计算绝对值
#define MatrixKeyNone "MKN"                 // 键盘没有按下，返回的空值

class MatrixKey {
 public:
  /**
   * 函数功能：设置行列引脚和行列数
   * 参数1：[_rowsPin] [uint8_t*] 行引脚
   * 参数2：[_columnsPin] [uint8_t*] 列引脚
   * 参数3：[_numberOfRows] [uint8_t] 行数
   * 参数4：[_numberOfColumns] [uint8_t] 列数
   * 返回值：无
   * 注意事项：无
   */
  void setPinsAndNumbers(uint8_t* _rowsPin, uint8_t* _columnsPin, uint8_t _numberOfRows, uint8_t _numberOfColumns);

  /**
   * 函数功能：设置返回键值
   * 参数1：[_returnKeyValue] [String*] 返回键值
   * 返回值：无
   * 注意事项：无
   */
  void setReturnKeyValue(String* _returnKeyValue);

  /**
   * 函数功能：设置消抖时间
   * 参数1：[_debouncingTime] [uint32_t] 消抖时间，默认为10ms
   * 返回值：无
   * 注意事项：无
   */
  void setDebouncingTime(uint32_t _debouncingTime);

  /**
   * 函数功能：设置松开时间
   * 参数1：[_releaseTime] [uint32_t] 松开时间，默认为40ms
   * 返回值：无
   * 注意事项：无
   */
  void setReleaseTime(uint32_t _releaseTime);

  /**
   * 函数功能：设置持续按下的状态
   * 参数1：[_continuePressedState] [bool] 持续按下的状态，默认为false，即按下按键不松开时，只返回1次键值
   * 返回值：无
   * 注意事项：无
   */
  void setContinuePressedState(bool _continuePressedState);

  /**
   * 函数功能：设置行引脚输入模式
   * 参数1：[_rowPinsInputMode] [uint8_t] 行引脚输入模式，默认为上拉输入
   * 返回值：无
   * 注意事项：无
   */
  void setRowPinsInputMode(uint8_t _rowPinsInputMode);

  /**
   * 函数功能：获取按键按下的键值
   * 参数：无
   * 返回值：[String] 按键按下的键值
   * 注意事项：无
   */
  String getKeyValue();

 private:
  /**
   * 函数功能：计算键值
   * 参数：无
   * 返回值：[String] 计算好的键值
   * 注意事项：无
   */
  String computeKeyValue();

  uint8_t* rowsPin = NULL;                  // 行引脚
  uint8_t* columnsPin = NULL;               // 列引脚
  uint8_t numberOfRows = 0;                 // 行数
  uint8_t numberOfColumns = 0;              // 列数
  String* returnKeyValue = NULL;            // 返回键值
  uint32_t debouncingTime = 10;             // 消抖时间，默认为10ms
  uint32_t releaseTime = 40;                // 松开时间，默认为40ms
  bool continuePressedState = false;        // 持续按下的状态，默认为false，即按下按键不松开时，只返回1次键值
  uint32_t recordDebouncingTime = 0;        // 记录消抖时间
  uint32_t recordReleaseTime = 0;           // 记录松开时间
  uint8_t rowPinsInputMode = INPUT_PULLUP;  // 行引脚输入模式，默认为上拉输入
  uint8_t i = 0, j = 0, m = 0;
};

#endif
