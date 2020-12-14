/*============================================================================*/
/* CrwToRow.cpp -- Crw to Row                                                 */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,CrwToRow,CN_CRWTOROW)
__aNewFunction(CrwToRow,CN_CRWTOROW)
__vDeleteFunction(CrwToRow,CN_CRWTOROW)
__aFooterFunction(CrwToRow)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CrwToRow::CrwToRow(StreamDef* pStreamDef)
: Object(CN_CRWTOROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  m_pStreamDef = pStreamDef;
  m_pRowBuffer = new RowBuffer;
  
  m_pRowBuffer->vResolveStreamPix(pStreamDef);
  
  _checkInitialised_(CN_CRWTOROW);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CrwToRow::~CrwToRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  ASSERT(! m_pRowBuffer);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert a crwbuffer into a rowBuffer                                       */
/*----------------------------------------------------------------------------*/
bool CrwToRow::bConvertCrwToRow(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);
    
  i8 nCrws = pCrwBuffer->nRowCount();
  for (i8 i_Crw = 0; i_Crw <nCrws; i_Crw++) {
    
    HeadLine* pCrwHead = pCrwBuffer->aGetHead(i_Crw);

    vAllotNewRow();
    
    RowInflator::vInflate_AllColumns_into_RowBuffer(m_pRowBuffer,pCrwHead);
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* return last remaining dangling rowBuffer (if any)                          */
/*----------------------------------------------------------------------------*/
RowBuffer* CrwToRow::pDanglingRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);
  
  RowBuffer* pDanglingRowBuffer = m_pRowBuffer;
  
  m_pRowBuffer = nullptr;
  
  return pDanglingRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full rowBuffer (if any)                                           */
/*----------------------------------------------------------------------------*/
RowBuffer* CrwToRow::pFullRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  if (! m_pRowBuffer)
    return nullptr;
  
  RowBuffer* pScanRowBuffer = m_pRowBuffer;
  if (! pScanRowBuffer->pNextBuffer())
    return nullptr;

  while (pScanRowBuffer->pNextBuffer()->pNextBuffer())
    pScanRowBuffer = _pRowBuffer(pScanRowBuffer->pNextBuffer());
  
  RowBuffer* pLastRowBuffer  = _pRowBuffer(pScanRowBuffer->pNextBuffer());
  
  pScanRowBuffer->vNextBuffer(nullptr);

  return pLastRowBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a new row in rowBuffer; create neww buffer if needed                 */
/*----------------------------------------------------------------------------*/
void CrwToRow::vAllotNewRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);
  
  if (m_pRowBuffer->nFreeBytes() < C_MAX_RECORD)
    vNewRowBuffer();
  
  Row* pRow = m_pRowBuffer->aAllotNewRow();
}

/*----------------------------------------------------------------------------*/
/* create a new rowBuffer, add as head of row buffer chain                    */
/*----------------------------------------------------------------------------*/
void CrwToRow::vNewRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);
  
  //  create a new buffer
  RowBuffer* pNewRowBuffer = new RowBuffer;
  
  // initialise the column pix inside the new row buffer
  pNewRowBuffer->vInitPix(m_pStreamDef,false);

  if (! m_pRowBuffer) {
    // set as head of empty row buffer chain
    m_pRowBuffer = pNewRowBuffer;
    return;
  }

  // add as head of existing row buffer chain
  pNewRowBuffer->vNextBuffer(m_pRowBuffer);
  m_pRowBuffer = pNewRowBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
