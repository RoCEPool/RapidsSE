/*============================================================================*/
/* CrwToRow.cpp -- Crw to Row                                                 */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"

#include "RowSingle.hpp"
#include "RowBuffer.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowInflator.hpp"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CrwToRow,CN_CRWTOROW)
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

  RowBuffer* pRowBuffer = new RowBuffer;

  pRowBuffer->vResolveRowPix(pStreamDef);

  m_pBufferIdQueue = new PdsBufferIdQueue();

  m_pBufferIdQueue->vAddBuffer(pRowBuffer);

  _checkInitialised_(CN_CRWTOROW);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CrwToRow::~CrwToRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  delete m_pBufferIdQueue;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* repair the file                                                            */
/*----------------------------------------------------------------------------*/
void CrwToRow::vRepair(Object* pObject)
{
  _static_

  CrwToRow* pCrwToRow = _pCrwToRow(pObject);
  PdsBufferIdQueue::vRepair(pCrwToRow->m_pBufferIdQueue);

  // delete the CrwToRow
  delete pCrwToRow;
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
    
  i8 nCrws = pCrwBuffer->nRows();
  for (i8 i_Crw = 0; i_Crw <nCrws; i_Crw++) {
    
    HeadLine* pCrwHead = pCrwBuffer->aGetHead(i_Crw);

    vAllotNewRow();
    
    RowInflator::vInflate_AllColumns_into_RowBuffer(pRowBuffer(), pCrwHead);
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* return last remaining dangling rowBuffer (if any)                          */
/*----------------------------------------------------------------------------*/
RowBuffer* CrwToRow::pDanglingRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  return _pRowBuffer(m_pBufferIdQueue->pDanglingBuffer());
}

/*----------------------------------------------------------------------------*/
/* return a full rowBuffer (if any)                                           */
/*----------------------------------------------------------------------------*/
RowBuffer* CrwToRow::pFullRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  return _pRowBuffer(m_pBufferIdQueue->pCompletedBuffer());
}

/*----------------------------------------------------------------------------*/
/* return tail of chain (most recently added buffer)                          */
/*----------------------------------------------------------------------------*/
RowBuffer* CrwToRow::pRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  return _pRowBuffer(m_pBufferIdQueue->pDataBuffer());
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
  
  if (pRowBuffer()->nFreeBytes() < C_MAX_RECORD)
    vNewRowBuffer();
  
  Row* pRow = pRowBuffer()->pAllotRow();
  UNUSED(pRow);
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


  m_pBufferIdQueue->vAddBuffer(pNewRowBuffer);

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
