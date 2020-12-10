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
    

  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* return last remaining dangling rowBuffer (if any)                          */
/*----------------------------------------------------------------------------*/
RowBuffer* CrwToRow::pDanglingRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);
  
  RowBuffer* pDanglingRsbBuffer = m_pRowBuffer;
  
  m_pRowBuffer = nullptr;
  
  return pDanglingRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full rowBuffer (if any)                                           */
/*----------------------------------------------------------------------------*/
RowBuffer* CrwToRow::pFullRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  RowBuffer* pFullRowBuffer = nullptr;
  
  RowBuffer* pRowBuffer =
               m_pRowBuffer ? _pRowBuffer(m_pRowBuffer->pNextBuffer())
                            : nullptr;
  
  while (pRowBuffer) {
    pFullRowBuffer = pRowBuffer;
    pRowBuffer = _pRowBuffer(pRowBuffer->pNextBuffer());
  }
  
  if (pFullRowBuffer) {
    RowBuffer* pPrevBuffer = _pRowBuffer(pFullRowBuffer->pPrevBuffer());
    pPrevBuffer->vNextBuffer(nullptr);
  }
  
  return pFullRowBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a new row in rowBuffer; create neww buffer if needed                 */
/*----------------------------------------------------------------------------*/
void CrwToRow::vAllotNewRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* create a new rowBuffer                                                     */
/*----------------------------------------------------------------------------*/
void CrwToRow::vNewRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOROW,_FOOTER_);
  
  //  create a new buffer
  RowBuffer* pNewRowBuffer = new RowBuffer;
  
  // link the new to old
  pNewRowBuffer->vNextBuffer(m_pRowBuffer);
  if (m_pRowBuffer)
    m_pRowBuffer->vPrevBuffer(pNewRowBuffer);
  m_pRowBuffer = pNewRowBuffer;
  
  // initialise the row slab inside the new row buffer
  m_pRowBuffer->vInitPix(m_pStreamDef,false);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
