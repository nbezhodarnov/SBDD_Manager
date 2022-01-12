#ifndef ABSTRACTSBDDEXPORTER_H
#define ABSTRACTSBDDEXPORTER_H

#include "SBDD/sbdd.h"

class AbstractSBDDExporter
{
public:
    AbstractSBDDExporter() = default;
    virtual bool Export(const SBDD &sbdd) const = 0;
    virtual ~AbstractSBDDExporter() = default;
};

#endif // ABSTRACTSBDDEXPORTER_H
