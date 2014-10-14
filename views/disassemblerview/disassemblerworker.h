#ifndef DISASSEMBLERWORKER_H
#define DISASSEMBLERWORKER_H

#include <QThread>
#include "logwidget/logger.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/processor/processorloader.h"
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

class DisassemblerWorker : public QThread
{
    Q_OBJECT

    public:
        explicit DisassemblerWorker(QHexEditData* hexeditdata, ProcessorLoader* loader, LogWidget* logwidget, QObject *parent = 0);
        DisassemblerListing* listing() const    ;

    protected:
        virtual void run();

    private:
        DisassemblerListing* _listing;
        QHexEditData* _hexeditdata;
        ProcessorLoader* _loader;
        LogWidget* _logwidget;
};

#endif // DISASSEMBLERWORKER_H