#ifndef DOTSBDDEXPORTER_H
#define DOTSBDDEXPORTER_H

#include <QFileDialog>

#include "SBDDExporters/abstractsbddexporter.h"

class DotSBDDExporter : public AbstractSBDDExporter
{
public:
    DotSBDDExporter();
    bool Export(const SBDD &sbdd) const;
};

#endif // DOTSBDDEXPORTER_H
