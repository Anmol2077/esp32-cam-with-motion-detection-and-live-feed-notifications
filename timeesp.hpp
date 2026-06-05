
#ifndef TIMEESP_HPP_
#define TIMEESP_HPP_

// Define the custom time structure for your application
typedef struct {
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    bool     is_valid; // Tells other tasks if the time is actually accurate yet
} app_time_t;


app_time_t get_global_time(void);

#endif /* TIMEESP_HPP_ */
