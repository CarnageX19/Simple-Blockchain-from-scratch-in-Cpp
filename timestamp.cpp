#include <iostream>
#include <ctime>
#include <string>
using namespace std;

string getTimestamp()
{
    // Get the current date and time
    time_t now = time(nullptr);
    struct tm* now_tm = localtime(&now);

    // Convert each component into a string and format it
    string day = to_string(now_tm->tm_mday);
    string month = to_string(now_tm->tm_mon + 1); // Months are 0-indexed
    string year = to_string(now_tm->tm_year + 1900); // Years since 1900
    string hour = to_string(now_tm->tm_hour);
    string minute = to_string(now_tm->tm_min);
    string second = to_string(now_tm->tm_sec);

    // Ensure that single-digit values are padded with leading zeros
    if (day.length() == 1) day = "0" + day;
    if (month.length() == 1) month = "0" + month;
    if (hour.length() == 1) hour = "0" + hour;
    if (minute.length() == 1) minute = "0" + minute;
    if (second.length() == 1) second = "0" + second;

    // Concatenate the components in the desired format
    string timestamp = day + month + year + hour + minute + second;

    return timestamp;
}

