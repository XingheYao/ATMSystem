#ifndef ENVELOPEACCEPTOR_H
#define ENVELOPEACCEPTOR_H

#include "log.h"
#include "customerconsole.h"

class EnvelopeAcceptor
{
public:
    EnvelopeAcceptor(Log log)
        : _log(log)
    {}

    void acceptEnvelope();
private:
    Log _log;
};

#endif // ENVELOPEACCEPTOR_H
