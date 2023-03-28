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



#ifndef __include_termios.h__
#define __include_termios.h__
#include <termios.h>
#endif

#include"communication.hpp"

namespace communication;

{
    arduino::TransfertDataToArduino(){}

    arduino::~TransfertDataToArduino(){
        close();
    }


    void TransfertDataToArduino::init(){
        fd=open(connectionPort.c_str(),O_RDWR | O_NOCTTY);
        if(fd==-1){
            cout<<"Error opening port"<<endl;  //error opening port
            return;
        }

        if(tcgetattr(fd,&config)<0){
            cout<<"Error getting config"<<endl;  //error getting config
            return;
        }
        
        tcgettattr(fd,&config);
        config.c_iflag &=config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
        config.c_oflag &= ~OPOST;
        config.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        config.c_cflag &= ~(CSIZE | PARENB);
        config.c_cflag |= CS8;
        cfsetispeed(&config, baudRate);
        cfsetospeed(&config, baudRate);
        tcsetattr(fd, TCSANOW, &config);
        }
    }

    void TransfertDataToArduino::send(string message){

        if (fd==-1){
            cerr << "Error: port is not open" << endl;
            return;
        }
        write(fd,message.c_str(),message.size());
    }


    void TransfertDataToArduino::close(){
        if(fd!=-1){
            close(fd);
            fd=-1;
        }
    }
}

