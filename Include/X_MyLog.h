#ifndef X_MY_LOG_H
#define X_MY_LOG_H

#include "../XNYSTools/Interface/ILog.h"

#define FOLLOW_LOG_DEBUG(format, ...) do { LOG_DEBUG(##format, ##__VA_ARGS__);} while(0)
#define FOLLOW_LOG_TRACE(format, ...) do { LOG_TRACE(##format, ##__VA_ARGS__);} while(0)
#define FOLLOW_LOG_WARN(format, ...)  do { LOG_WARNN(##format, ##__VA_ARGS__);} while(0)
#define FOLLOW_LOG_ERROR(format, ...) do { LOG_ERROR(##format, ##__VA_ARGS__);} while(0)


#endif // X_MY_LOG_H
