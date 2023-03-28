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

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "communication.hpp"

namespace communication {

    int config;
    struct termios configuration;

    TransfertDataToArduino::TransfertDataToArduino() {}

    TransfertDataToArduino::~TransfertDataToArduino() {
        close();
    }

    void TransfertDataToArduino::init() {
        fd = open(connectionPort.c_str(), O_RDWR | O_NOCTTY);
        if (fd == -1) {
            std::cout << "Error opening port" << std::endl;
            return;
        }

        if (tcgetattr(fd, &configuration) < 0) {
            std::cout << "Error getting config" << std::endl;
            return;
        }

        tcgetattr(fd, &configuration);
        configuration.c_iflag &= configuration.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
        configuration.c_oflag &= ~OPOST;
        configuration.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        configuration.c_cflag &= ~(CSIZE | PARENB);
        configuration.c_cflag |= CS8;
        cfsetispeed(&configuration, baudRate);
        cfsetospeed(&configuration, baudRate);
        tcsetattr(fd, TCSANOW, &configuration);
    }

    void TransfertDataToArduino::send(std::string message) {
        if (fd == -1) {
            std::cerr << "Error: port is not open" << std::endl;
            return;
        }
        const char *buf = message.c_str();
        size_t count = message.size();
        write(fd, buf, count);
    }

    void TransfertDataToArduino::close() {
        if (fd != -1) {
            ::close(fd);
            fd = -1;
        }
    }
}

