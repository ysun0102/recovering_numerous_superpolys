
#include <sys/time.h>
#include <string.h>
#include <chrono>

/* convert timeval to miliseconds */
#define TIMEVAL2F(stamp) ((stamp).tv_sec * 1000.0 + (stamp).tv_usec / 1000.0)

/* get timestamp to the precision of miliseconds since the program starts */
inline double get_timestamp() {
    static double __init_stamp = -1;
    static struct timeval __cur_time;
    if (-1 == __init_stamp) {
        gettimeofday(&__cur_time, NULL);
        __init_stamp = TIMEVAL2F(__cur_time);
    }
    
    gettimeofday(&__cur_time, NULL);
    return ((TIMEVAL2F(__cur_time) - __init_stamp) / 1000.0);
}

/* print msg with timestamp */
#define PRINTF_STAMP(format, ...) \
do { \
    flockfile(stdout); \
    printf("%12.2f - ", get_timestamp()); \
    printf(format, ##__VA_ARGS__); \
    fflush(stdout); \
    funlockfile(stdout); \
} while(0)


inline std::string getCurrentSystemTime() {
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm* ptm = localtime(&tt);
    char date[60] = { 0 };
    sprintf(date, "%d-%02d-%02d-%02d:%02d:%02d", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
    return std::string(date);
}


class Timer {
   
public:
    double start_time;
    double end_time;
    double durable_time;
    
    inline void start() {
        this->start_time = get_timestamp();
    }
    
    inline void end() {
        this->end_time = get_timestamp();
        this->durable_time = this->end_time - this->start_time;
    }
    
    inline double used_time() {
        return get_timestamp() - this->start_time;
    }
    
};
