#include "YYZT_MatrixKey.h"

/**
 * 函数功能：设置行列引脚和行列数
 * 参数1：[_rowsPin] [uint8_t*] 行引脚
 * 参数2：[_columnsPin] [uint8_t*] 列引脚
 * 参数3：[_numberOfRows] [uint8_t] 行数
 * 参数4：[_numberOfColumns] [uint8_t] 列数
 * 返回值：无
 * 注意事项：无
 */
void MatrixKey::setPinsAndNumbers(uint8_t* _rowsPin, uint8_t* _columnsPin, uint8_t _numberOfRows, uint8_t _numberOfColumns) {
  rowsPin = _rowsPin;
  columnsPin = _columnsPin;
  numberOfRows = _numberOfRows;
  numberOfColumns = _numberOfColumns;
  for (uint8_t i = 0; i < numberOfRows; i++) {  //行引脚输入
    pinMode(rowsPin[i], rowPinsInputMode);
  }
  for (uint8_t i = 0; i < numberOfColumns; i++) {  //列引脚输出
    pinMode(columnsPin[i], OUTPUT);
  }
}

/**
 * 函数功能：设置返回键值
 * 参数1：[_returnKeyValue] [String*] 返回键值
 * 返回值：无
 * 注意事项：无
 */
void MatrixKey::setReturnKeyValue(String* _returnKeyValue) {
  returnKeyValue = _returnKeyValue;
}

/**
 * 函数功能：设置消抖时间
 * 参数1：[_debouncingTime] [uint32_t] 消抖时间，默认为10ms
 * 返回值：无
 * 注意事项：无
 */
void MatrixKey::setDebouncingTime(uint32_t _debouncingTime) {
  debouncingTime = _debouncingTime;
}

/**
 * 函数功能：设置松开时间
 * 参数1：[_releaseTime] [uint32_t] 松开时间，默认为40ms
 * 返回值：无
 * 注意事项：无
 */
void MatrixKey::setReleaseTime(uint32_t _releaseTime) {
  releaseTime = _releaseTime;
}

/**
 * 函数功能：设置持续按下的状态
 * 参数1：[_continuePressedState] [bool] 持续按下的状态，默认为false，即按下按键不松开时，只返回1次键值
 * 返回值：无
 * 注意事项：无
 */
void MatrixKey::setContinuePressedState(bool _continuePressedState) {
  continuePressedState = _continuePressedState;
}

/**
 * 函数功能：获取按键按下的键值
 * 参数：无
 * 返回值：[String] 按键按下的键值
 * 注意事项：无
 */
String MatrixKey::getKeyValue() {
  String keyValue = MatrixKeyNone;
  i = 0, j = 0, m = 0;
  //列线输出低电平0
  for (i = 0; i < numberOfColumns; i++) {
    digitalWrite(columnsPin[i], LOW);
  }

  //扫描行按键是否被按下
  for (i = 0; i < numberOfRows; i++) {
    if (digitalRead(rowsPin[i]) == LOW) {  //行按键被按下
      //消抖
      if (YYZT_abs(millis() - recordDebouncingTime) < debouncingTime) {
        keyValue = MatrixKeyNone;
        return keyValue;
      }
      recordDebouncingTime = millis();

      if (digitalRead(rowsPin[i]) == LOW) {  //行按键被按下
        //扫描 列按键是否被按下
        for (i = 0; i < numberOfColumns; i++) {
          if (i == 0) {  //第1列为低电平，其他列为高电平
            digitalWrite(columnsPin[i], LOW);
            for (m = 1; m < numberOfColumns; m++) {
              digitalWrite(columnsPin[m], HIGH);
            }
            keyValue = computeKeyValue();
            if (keyValue != MatrixKeyNone) {
              return keyValue;
            }
          } else if (i == numberOfColumns - 1) {  //最后1列为低电平，其他列为高电平
            digitalWrite(columnsPin[numberOfColumns - 1], LOW);
            for (m = 0; m < numberOfColumns - 1; m++) {
              digitalWrite(columnsPin[m], HIGH);
            }
            keyValue = computeKeyValue();
            if (keyValue != MatrixKeyNone) {
              return keyValue;
            }
          } else {  //第i列为低电平，其他列为高电平
            digitalWrite(columnsPin[i], LOW);
            for (m = 0; m < numberOfColumns; m++) {
              if (m != i) {
                digitalWrite(columnsPin[m], HIGH);
              }
            }
            keyValue = computeKeyValue();
            if (keyValue != MatrixKeyNone) {
              return keyValue;
            }
          }
        }
      }
    }
  }

  return keyValue;
}

/**
 * 函数功能：计算键值
 * 参数：无
 * 返回值：[String] 计算好的键值
 * 注意事项：无
 */
String MatrixKey::computeKeyValue() {
  String keyValue = MatrixKeyNone;
  for (j = 0; j < numberOfRows; j++) {
    if (digitalRead(rowsPin[j]) == LOW) {
      if (continuePressedState == false) {
        if (YYZT_abs(millis() - recordReleaseTime) < releaseTime) {
          recordReleaseTime = millis();
          keyValue = MatrixKeyNone;
          return keyValue;
        }
        recordReleaseTime = millis();
      }

      // keyValue = String(j + 1) + String(i + 1);
      // Serial.print("行rowPin：");
      // Serial.println(j + 1);
      // Serial.print("列colPin：");
      // Serial.println(i + 1);
      String* pReturnKeyValue = returnKeyValue;
      keyValue = *(pReturnKeyValue += (j * numberOfColumns + i));

      return keyValue;
    }
  }
  return keyValue;
}