//
// Created by lorenzodonini on 06.09.16.
//

#include "../core/TransportDaemon.h"

#include <memory>

void print_usage()
{
    std::cout << "Invalid arguments.\n";
    std::cout << "Usage:\n";
    std::cout << "  s3tpd <unix_path> fire <fire_tcp_port>\n";
    std::cout << "  s3tpd <unix_path> spi <device_path> <interrupt_pinid> <unix_control_path>\n";
}

int main(int argc, char ** argv) {
    if (argc < 4) {
        print_usage();
        return -1;
    }
    s3tp_daemon daemon;

    TRANSCEIVER_CONFIG config;
    config.control = nullptr;

    int argi = 1;

    socket_path = argv[argi++];

    //Creating spi interface
    char * transceiverType = argv[argi++];

    if (strcmp(transceiverType, "spi") == 0) {
        if (argc != 6)
        {
            print_usage();
            return -1;
        }

        char* device_path = argv[argi++];
        char* interrupt_pinid = argv[argi++];
        char* unix_control_path = argv[argi++];

        config.type = SPI;
        Transceiver::SPIDescriptor desc;
        config.descriptor.spi = device_path;
        config.descriptor.interrupt = PinMapper::find(interrupt_pinid);

        config.control = new Transceiver::SimpleControl(unix_control_path);
    } else if (strcmp(transceiverType, "fire") == 0) {
        if (argc != 4)
        {
            print_usage();
            return -1;
        }

        int start_port = atoi(argv[argi++]);
        config.type = FIRE;
        Transceiver::FireTcpPair pairs[S3TP_VIRTUAL_CHANNELS];
        std::cout << "Created config for [port:channel]: ";
        for (int i=0; i < S3TP_VIRTUAL_CHANNELS; i++) {
            pairs[i].port = start_port + (i*2);
            pairs[i].channel = i;
            config.mappings.push_back(pairs[i]);
            std::cout << start_port + (i*2) << ":" << i << " ";
        }
        std::cout << std::endl;
    } else {
        print_usage();
        return -2;
    }

    daemon.init(&config);
    daemon.startDaemon();
}
