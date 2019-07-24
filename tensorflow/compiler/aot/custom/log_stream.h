#pragma once
#include <vector>
#include "basic_type.h"

class log_stream {
public:
  enum showmode{
    dec = 1,//out put as dec
    hex,
    endl,
  };
  log_stream() : mode_(dec) {
  }
  void insert(const char* val,bool end = false);
  const char* get_buf() { return &buf_[0]; }
  std::vector<char> buf_;
  showmode get_mode() {
    return mode_;
  }
  void set_mode(showmode mode) {
    mode_ = mode;
  }
private:
  showmode mode_;
};

inline void log_stream::insert(const char* val,bool end /*= false*/) {
  if (val) {
    size_t size = strlen(val);
    buf_.insert(buf_.end(),val,val+size);
    if (end) {
      buf_.push_back('\0');
    }
  }
}

inline log_stream& operator << (log_stream& obj, const char* val) {
  if (val) {
    obj.insert(val);
  }
  return obj;
}
inline log_stream& operator << (log_stream& obj, int val) {
  char str_val[64] = {};
  if (obj.get_mode() == log_stream::dec) {
    sprintf(str_val, "%d", val);
  } else if (obj.get_mode() == log_stream::hex) {
    sprintf(str_val, "%x", val);
  }
  obj.insert(str_val);
  return obj;
}
inline log_stream& operator << (log_stream& obj, double val) {
  char str_val[64] = {};
  sprintf(str_val, "%f", val);
  obj.insert(str_val);
  return obj;
}

//inline log_stream& operator << (log_stream& obj, unsigned long val) {
//  char str_val[64] = {};
//  if (obj.get_mode() == log_stream::dec) {
//    sprintf(str_val, "%llu", static_cast<long long unsigned int>(val));
//  }
//  else if (obj.get_mode() == log_stream::hex) {
//    sprintf(str_val, "%x", (uint32_t)val);
//  }
//  obj.insert(str_val);
//  return obj;
//}

inline log_stream& operator << (log_stream& obj, uint32_t val) {
  char str_val[64] = {};
  if (obj.get_mode() == log_stream::dec) {
    sprintf(str_val, "%d", val);
  } else if (obj.get_mode() == log_stream::hex) {
    sprintf(str_val, "%x", val);
  }
  obj.insert(str_val);
  return obj;
}
inline log_stream& operator << (log_stream& obj, uint64_t val) {
  char str_val[128] = {};
  if (obj.get_mode() == log_stream::dec) {
    sprintf(str_val, "%llu", static_cast<long long unsigned int>(val));
  } else if (obj.get_mode() == log_stream::hex) {
    sprintf(str_val, "%llx", static_cast<long long unsigned int>(val));
  }
  obj.insert(str_val);
  return obj;
}
inline log_stream& operator << (log_stream& obj, time_t val) {
  char str_val[128] = {};
  if (obj.get_mode() == log_stream::dec) {
    sprintf(str_val, "%llu", static_cast<long long unsigned int>(val));
  }
  else if (obj.get_mode() == log_stream::hex) {
    sprintf(str_val, "%llx", static_cast<long long unsigned int>(val));
  }
  obj.insert(str_val);
  return obj;
}
inline log_stream& operator << (log_stream& obj, const std::string& val) {
  obj.insert(val.c_str());
  return obj;
}
inline log_stream& operator << (log_stream& obj, log_stream::showmode mode) {
  obj.set_mode(mode);
  if(mode == log_stream::endl) {
    obj.insert("\n",true);
  }
  return obj;
}

