/*============================================================================*/
/* MetricsHelper.cpp -- Metrics helper                                        */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "InfoSet.hpp"

#include "MetricsHelper.hpp"
#include "InfoSet.hpp"
#include "Record.hpp"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MetricsHelper::vWriteCsv(InfoSet* pInfoSet, StreamDef* pStreamDef, File* pFile)
{
  _static_
  
  HV_Record hvRecord(pStreamDef->pRowPix());
  
  for (Row* aInfoRow = pInfoSet->aFirstRow();
       aInfoRow;
       aInfoRow = pInfoSet->aNextRow(aInfoRow)) {
  
    vTrace("MetricsHelper: %s\n"_Stz,hvRecord->sz(aInfoRow));
    
    pFile->vPrint("%s\n"_Stz,hvRecord->sz(aInfoRow));
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
