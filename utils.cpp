#include "utils.h"

/** Returns true on success, or false if there was an error */
bool set_socket_blocking_socket(int fd, bool is_non_blocking)
{
    if (fd < 0) return false;

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return false;
    flags = is_non_blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
    return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
}


void get_min_max_median(vector<int> stats, int &min, int &max, int &median, int &average) {
    min = stats[0];
    max=stats[0];

    long long sum = 0;

    for (int i = 0 ; i < stats.size(); i++)
    {
        sum = sum + stats[i];
        if (min > stats[i])
        {
            min = stats[i];
        }
        if (max < stats[i])
        {
            max = stats[i];
        }
    }
    median = stats[stats.size()/2];
    average = sum/stats.size();
}


