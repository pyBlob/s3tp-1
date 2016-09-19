//
// Created by Lorenzo Donini on 23/08/16.
//

#ifndef S3TP_TXMODULE_H
#define S3TP_TXMODULE_H

#include "Constants.h"
#include "Buffer.h"
#include "utilities.h"
#include <map>
#include <trctrl/LinkInterface.h>

#define TX_PARAM_RECOVERY 0x01
#define TX_PARAM_CUSTOM 0x02
#define CODE_INACTIVE_ERROR -1

class TxModule : public PriorityComparator<S3TP_PACKET *> {
public:
    enum STATE {
        RUNNING,
        BLOCKED,
        WAITING
    };

    TxModule();
    ~TxModule();

    STATE getCurrentState();
    void startRoutine(Transceiver::LinkInterface * spi_if);
    void stopRoutine();
    int enqueuePacket(S3TP_PACKET * packet, uint8_t frag_no, bool more_fragments, uint8_t spi_channel, uint8_t options);
    void notifyLinkAvailability(bool available);
    bool isQueueAvailable(uint8_t port, uint8_t no_packets);
    void reset();
    void synchronizeStatus();
private:
    STATE state;
    bool active;
    pthread_t tx_thread;
    pthread_mutex_t tx_mutex;
    pthread_cond_t tx_cond;
    uint8_t global_seq_num;
    uint8_t to_consume_global_seq;
    uint8_t to_consume_sub_seq;
    Transceiver::LinkInterface * linkInterface;
    bool scheduled_sync;
    S3TP_SYNC syncStructure;

    std::map<uint8_t, uint8_t> to_consume_port_seq;
    std::map<uint8_t, uint8_t> port_sequence;
    Buffer * outBuffer;

    void txRoutine();
    static void * staticTxRoutine(void * args);
    virtual int comparePriority(S3TP_PACKET* element1, S3TP_PACKET* element2);
};

#endif //S3TP_TXMODULE_H
