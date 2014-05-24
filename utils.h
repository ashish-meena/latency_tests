#ifndef UTILS_H
#define UTILS_H

#include <fcntl.h>
#include<vector>
using namespace std;
/** Returns true on success, or false if there was an error */
bool set_socket_blocking_socket(int fd, bool is_non_blocking);
void get_min_max_median(vector<int> stats, int &min, int &max, int &median, int &average);
#endif
