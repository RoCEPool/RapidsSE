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
  
  for (Row* pInfoRow = pInfoSet->pFirstRow();
       pInfoRow;
       pInfoRow = pInfoSet->pNextRow(pInfoRow)) {
  
    vTrace("MetricsHelper: %s\n"_Stz,hvRecord->sz(pInfoRow));
    
    pFile->vPrint("%s\n"_Stz,hvRecord->sz(pInfoRow));
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
