#ifndef _ANDROID22_NO_STREAMS_
#include <iostream>
#include <iomanip>
#endif//_ANDROID22_NO_STREAMS_

#include "basic_type.h"
#include "base/log.h"

#ifndef _ANDROID22_NO_STREAMS_

void DumpBinaryLine(const uint8_t* binary, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    std::cout << std::hex << std::setfill('0') << std::setw(2) <<
        (int)*(binary + i);
    if (i == size - 1)
      break;
    if (i % 8 == 7)
      std::cout << "  ";
    else if (i % 4 == 3)
      std::cout << "_";
    else
      std::cout << " ";
  }
  std::cout << "  ";

  for (size_t i = 0; i < size; ++i) {
    uint8_t ch = *(binary + i);
    if (ch == '\r' || ch == '\n' || ch == '\t' || ch == 0xff || ch == 0)
      std::cout << ".";
    else
      std::cout << ch;
  }
  std::cout << "\r\n";
}

void DumpBinary(const uint8_t* binary, size_t size) {
  const size_t line_number = 16;
  for (size_t i = 0; i <= size / line_number; ++i) {
    if (i == size / line_number) {
      if (size % line_number) {
        std::cout << std::hex <<
          std::setfill('0') << std::setw(2) << i << "  ";
        DumpBinaryLine(binary + i * line_number, size % line_number);
      }
    } else {
      std::cout << std::hex << std::setfill('0') << std::setw(2) << i << "  ";
      DumpBinaryLine(binary + i * line_number, line_number);
    }
  }
}

void DumpBinary(const uint8_t* binary, size_t size, const char* message) {
  if (message)
    std::cout << message << "\r\n";
  DumpBinary(binary, size);
}
#else
void DumpBinaryLine(const uint8_t* binary, uint32_t size) {

}
void DumpBinary(const uint8_t* binary, uint32_t size) {
  const size_t line_number = 16;
  for (size_t i = 0; i <= size / line_number; ++i) {
    if (i == size / line_number) {
      if (size % line_number) {
        LOGI(OUT_HEX << i);
        //std::cout << std::hex << std::setfill('0') << std::setw(2) << i << "  ";
        DumpBinaryLine(binary + i * line_number, size % line_number);
      }
    } else {
      //std::cout << std::hex << std::setfill('0') << std::setw(2) << i << "  ";
      DumpBinaryLine(binary + i * line_number, line_number);
    }
  }
}
void DumpBinary(const uint8_t* binary, uint32_t size, const char* message) {
  if (message) {
    LOGI(message);
  }
  DumpBinary(binary, size);
}

#endif//_ANDROID22_NO_STREAMS_

void DumpBingaryToLogCat(const uint8_t* binary,
  size_t size,
  const char* message) {
  std::string log;
  for (size_t i = 0; i < size; ++i) {
    char t[32] = {};
    sprintf(t, "%d ", binary[i]);
    log += t;
  }
  LOGI(message << log);
}

