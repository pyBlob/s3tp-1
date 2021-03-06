//
// Created by Lorenzo Donini on 07/09/16.
//

#ifndef S3TP_PRIORITYCOMPARATOR_H
#define S3TP_PRIORITYCOMPARATOR_H

template <typename T>
class PolicyActor {
public:
    virtual int comparePriority(T element1, T element2) = 0;
    virtual bool isElementValid(T element) = 0;
    virtual bool maximumWindowExceeded(T queueHead, T newElement) = 0;
};

#endif //S3TP_PRIORITYCOMPARATOR_H
