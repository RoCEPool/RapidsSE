/*============================================================================*/
/* MetricsHelper.cpp -- Metrics helper                                        */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "MetricsHelper.hpp"
#include "Record.hpp"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void MetricsHelper::vWriteCsv(RowBuffer* pRowBuffer, StreamDef* pStreamDef, File* pFile)
{
	_static_

	HV_Record hvRecord(pStreamDef->pRowPix());

	for (Row* pInfoRow = pRowBuffer->pFirstRow();
			 pInfoRow;
			 pInfoRow = pRowBuffer->pNextRow(pInfoRow)) {

		vTrace("MetricsHelper: %s\n"_Stz,hvRecord->sz(pInfoRow));

		pFile->vPrint("%s\n"_Stz,hvRecord->sz(pInfoRow));
	}
}

/*----------------------------------------------------------------------------*/
void MetricsHelper::vWriteCsvPds(Pds* pPdsRowBuffer, StreamDef* pStreamDef, File* pFile)
{
  _static_

  HV_Record hvRecord(pStreamDef->pRowPix());

  PdsCursor* pCursor = new PdsCursor(pPdsRowBuffer);

  while( ! pCursor->bIsCurrNull()) {
    SkipListBlock* pSkipListBlock = pCursor->m_pCurrentBlock;
    Row* pInfoRow = pSkipListBlock->aRow();

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
