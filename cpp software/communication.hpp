#pragma once 

/*
||---------------------------------------------------------------------------------------------||
||---------------------------------------------------------------------------------------------||
||                                                                                             ||
|| This software is a private part of the project "waste sorting by artificial intelligence".  ||
|| The software is provided "as is" without warranty of any kind.                              ||
|| The author of the software is not responsible for any damage caused by the software.        ||
|| You will be unable to use this software without the agreement of the autor                  ||
|| It's forbidden to use it without the agreement of the autor                                 ||
|| This software use a opencv librabry which have been boosted using cuda                      ||
|| You will be unable to use this software without recompiling opencv binaries                 ||
||                                                                                             ||
||   ";)/""                                                                                    ||
||---------------------------------------------------------------------------------------------||
||---------------------------------------------------------------------------------------------||
*/
#ifndef __include_termios_h__
#define __include_termios_h__
#include <termios.h>
#endif

#ifndef __include_string_h__
#define __include_string_h__
#include <string.h>
#endif

#ifndef __include_IOStream_h__
#define __include_IOStream_h__
#include <iostream>
#endif

#ifndef __include_Vector__
#define __include_Vector__
#include <vector>
#endif

using namespace std;

#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP


namespace communication {

    extern int config;
    extern struct termios configuration;

    class TransfertDataToArduino {
    private:
        std::string message;
        std::string connectionPort = "/dev/ttyUSB0";
        int baudRate = 9600;
        int fd = -1;

    public:
        TransfertDataToArduino();
        ~TransfertDataToArduino();
        void init();
        void send(std::string message);
        void close();
    };
}

#endif /* COMMUNICATION_HPP */


