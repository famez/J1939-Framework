/*
 * Utils.cpp
 *
 *  Created on: Apr 5, 2018
 *      Author: famez
 */


#include "Utils.h"


namespace Utils {

u32 getElapsedMillis(timespec* start, timespec* end)
{
    return (end->tv_sec - start->tv_sec) * 1000 + (end->tv_nsec - start->tv_nsec) / 1000000;
}

}
