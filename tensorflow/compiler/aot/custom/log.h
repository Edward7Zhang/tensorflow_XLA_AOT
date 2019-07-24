#pragma once

#ifndef _ANDROID22_NO_STREAMS_
#include <iostream>
#include <sstream>
#endif//_ANDROID22_NO_STREAMS_

#define TAG "MMCleanNative"

#ifdef _WIN32
#ifndef NOMINMAX
  #define NOMINMAX
#endif//NOMINMAX
#include <WinSock2.h>
#include <Windows.h>
#endif

#include <string>

#define LOG_TO_LOGCAT

#ifdef CMD_TOOL
#undef LOG_TO_LOGCAT
#endif

inline std::string utf8_to_native(std::string const& utf8)
{
#ifdef _WIN32
  int len = MultiByteToWideChar(CP_UTF8, NULL, utf8.c_str(), utf8.size(), NULL, 0);
  wchar_t* result_t = new wchar_t[len + 1];
  MultiByteToWideChar(CP_UTF8, NULL, utf8.c_str(), utf8.size(), result_t, len); //utf-8转换为Unicode  
  result_t[len] = '\0';
  len = WideCharToMultiByte(CP_ACP, NULL, result_t, wcslen(result_t), NULL, 0, NULL, NULL);
  char* result = new char[len + 1];
  WideCharToMultiByte(CP_ACP, NULL, result_t, wcslen(result_t), result, len, NULL, NULL);//Unicode转换为ANSI   
  result[len] = '\0';
  delete[] result_t;
  std::string ret(result);
  delete[]result;
  return ret;
#else
  return utf8;
#endif
}

#ifndef LOG_MAX_LEVEL
#define LOG_MAX_LEVEL 1
#endif//LOG_MAX_LEVEL

const int kLogMaxLevel = LOG_MAX_LEVEL;

#ifdef _ANDROID22_NO_STREAMS_
#ifndef OUT_DEC
  #define OUT_DEC log_stream::dec
#endif//OUT_DEC
#ifndef OUT_HEX
  #define OUT_HEX log_stream::hex
#endif//OUT_HEX
#else//_ANDROID22_NO_STREAMS_
#ifndef OUT_DEC
  #define OUT_DEC std::dec
#endif//OUT_DEC
#ifndef OUT_HEX
  #define OUT_HEX std::hex
#endif//OUT_HEX
#endif//_ANDROID22_NO_STREAMS_

#ifndef ANDROID

#define ENABLE_LOG

typedef enum android_LogPriority {
  ANDROID_LOG_UNKNOWN = 0,
  ANDROID_LOG_DEFAULT,/* only for SetMinPriority() */
  ANDROID_LOG_VERBOSE,
  ANDROID_LOG_DEBUG,
  ANDROID_LOG_INFO,
  ANDROID_LOG_WARN,
  ANDROID_LOG_ERROR,
  ANDROID_LOG_FATAL,
  ANDROID_LOG_SILENT,/* only for SetMinPriority(); must be last */
} android_LogPriority;

#ifdef _WIN32
#include "log_stream.h"
#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>

#define LOG(level, f) { \
  static std::mutex mu; \
  std::unique_lock<std::mutex> lock(mu); \
  do {\
  if(level > kLogMaxLevel){ \
      std::stringstream s; \
      s << "[" << std::this_thread::get_id() << "]" << f; \
      std::cout << utf8_to_native(s.str()) << std::endl; \
    }\
  } while (0, 0); \
}
#else
#define LOG(level, f) do { \
  if(level > kLogMaxLevel){ \
    std::stringstream s; \
    s << "[" << __FUNCTION__ << ":" << __LINE__ << "]\t" << f; \
    std::cout << utf8_to_native(s.str()) << std::endl; \
  } \
} while (0)
#endif // #ifdef _WIN32

#else // #ifndef ANDROID
#ifdef LOG_TO_LOGCAT
#ifdef _ANDROID22_NO_STREAMS_
#include <android/log.h>
#include "log_stream.h"
#define LOG(level, f) do { \
  if(level > kLogMaxLevel){ \
    log_stream s; \
    s << "[" << __FUNCTION__ << ":" << __LINE__ << "]\t" << f << log_stream::endl; \
    __android_log_print(level, TAG, "%s\n", s.get_buf()); \
  }\
} while (0)
#else//_ANDROID22_NO_STREAMS_
#include <android/log.h>
#define LOG(level, f) do { \
  if(level>kLogMaxLevel){ \
    std::stringstream s; \
    s << "[" << __FUNCTION__ << ":" << __LINE__ << "]\t" << f; \
    __android_log_print(level, TAG, "%s\n", s.str().c_str()); \
  }\
} while (0)
#endif//_ANDROID22_NO_STREAMS_
#else//LOG_TO_LOGCAT
#ifndef _ANDROID22_NO_STREAMS_
#include <android/log.h>
#include <iostream>
#define LOG(level, f) do { \
  if(level > kLogMaxLevel){ \
    std::stringstream s; \
    s << f; \
    std::cout << utf8_to_native(s.str()) << std::endl; \
  }\
} while (0, 0) // to avoid warning 4127
#else//_ANDROID22_NO_STREAMS_
#include <android/log.h>
#include "log_stream.h"
#define LOG(level, f) do { \
  if(level > kLogMaxLevel){ \
    log_stream s; \
    s << "[" << __FUNCTION__ << ":" << __LINE__ << 
      "]\t" << f << log_stream::endl; \
    printf("%s",s.get_buf()); \
  }\
} while (0, 0) // to avoid warning 4127
#endif//_ANDROID22_NO_STREAMS_
#endif
#endif // #ifndef ANDROID

#ifdef ENABLE_LOG
#define LOGD(f)  LOG(ANDROID_LOG_DEBUG, f)
#define LOGI(f)  LOG(ANDROID_LOG_INFO, f)
#define LOGW(f)  LOG(ANDROID_LOG_WARN, f)
#define LOGE(f)  LOG(ANDROID_LOG_ERROR, f)
#define LOGF(f)  LOG(ANDROID_LOG_FATAL, f)
#define LOG_DETAIL(f) LOG(ANDROID_LOG_ERROR, f)
#else
#define LOGD(f)
#define LOGI(f)
#define LOGW(f)
#define LOGE(f)
#define LOGF(f)
#define LOG_DETAIL(f) LOG(ANDROID_LOG_ERROR, f)
#endif

#ifdef DEBUG_VERBOSE
#define LOGV(f)  LOG(ANDROID_LOG_INFO, f)
#else
#define LOGV(f)
#endif