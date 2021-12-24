#ifndef TXTSBDDEXPORTER_H
#define TXTSBDDEXPORTER_H

#include <QFileDialog>

#include "SBDDExporters/abstractsbddexporter.h"

class TxtSBDDExporter : public AbstractSBDDExporter
{
public:
    TxtSBDDExporter();
    bool Export(const SBDD &sbdd) const;
};

#endif // TXTSBDDEXPORTER_H
