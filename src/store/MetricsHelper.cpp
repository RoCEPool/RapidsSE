/*============================================================================*/
/* MetricsHelper.cpp -- Metrics helper                                        */
/*============================================================================*/

#include "BASE.h"

#include "MetricsHelper.hpp"
#include "Record.hpp"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void MetricsHelper::vWriteCsvPds(Pds* pPdsRowBuffer, StreamDef* pStreamDef, File* pFile)
{
  _static_

  HV_Record hvRecord(pStreamDef);

  PdsCursor* pCursor = new PdsCursor(pPdsRowBuffer);

  while( ! pCursor->bIsCurrNull()) {
    Row* pInfoRow = pCursor->pCurrRow();

    vTrace("MetricsHelper: %s\n"_Stz,hvRecord->sz(pInfoRow));

    pFile->vPrint("%s\n"_Stz,hvRecord->sz(pInfoRow));

    pCursor->vNext();
  }

  pCursor->m_pPds = nullptr;
  delete pCursor;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
