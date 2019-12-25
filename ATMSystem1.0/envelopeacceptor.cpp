#include "envelopeacceptor.h"
#include "mainwindow.h"
void EnvelopeAcceptor::acceptEnvelope()
{
    MainWindow & w1=get_global<MainWindow>();
    bool inserted = w1.acceptEnvelope();
    if (inserted)
        _log.logEnvelopeAccepted();
    else
        throw  Cancelled();
}
