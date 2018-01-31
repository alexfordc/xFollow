#ifndef MY_LOG_H
#define MY_LOG_H

#include <stdio.h>
#include <string>
#include "XNYSTools/Log.h"

#define KN_FINACIAL_LOG_DEBUG(format, ...) do { KN_DEBUG(##format, ##__VA_ARGS__);} while(0)
#define KN_FINACIAL_LOG_TRACE(format, ...) do { KN_TRACE(##format, ##__VA_ARGS__);} while(0)
#define KN_FINACIAL_LOG_WARN(format, ...)  do { KN_WARNING(##format, ##__VA_ARGS__);} while(0)
#define KN_FINACIAL_LOG_ERROR(format, ...) do { KN_ERROR(##format, ##__VA_ARGS__);} while(0)


#endif // MY_LOG_H
