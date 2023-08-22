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

  m_iSynthKey  = 0;

  RowBuffer* pRowBuffer = new RowBuffer;

  pRowBuffer->vResolveRowPix(pStreamDef);

  m_pPdsOfBufferIds = new Pds_t<PdsItem_BufferId>(nullptr);

  PdsItem_BufferId oItem;
  oItem.iKey = ++m_iSynthKey;
  oItem.oBufferId = pRowBuffer->oGetBufferId();
  SkipListBlock* pBlock = m_pPdsOfBufferIds->pInsert((void*)&oItem);
  RASSERT(pBlock != nullptr);

  *(PdsItem_BufferId*)(pBlock->aItem()) = oItem;

  
  _checkInitialised_(CN_CRWTOROW);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CrwToRow::~CrwToRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  delete m_pPdsOfBufferIds;
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
  Pds* pPds = pCrwToRow->m_pPdsOfBufferIds;

  SkipListBlock* pCurrBlock = pPds->pFirstBlock();

  while(pCurrBlock) {
    SkipListBlock* pNextBlock = pPds->pNextSLBlock(pCurrBlock, 0);

    PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pCurrBlock->aItem();

    RowBuffer* pRowBuffer = _pRowBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));
    pPds->bDelete((void*)&oBufferIdStruct.iKey);
    AM::vDeleteOperand(pRowBuffer,nullptr);

    pCurrBlock = pNextBlock;
  }

  RASSERT(pPds->iNumEntries(0) == 0);

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

  RASSERT(m_pPdsOfBufferIds->iNumEntries(0) <= 1);

  if(m_pPdsOfBufferIds->iNumEntries(0) == 0)
    return nullptr;

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();
  RASSERT(pCurrBlock != nullptr);

  PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pCurrBlock->aItem();

  RowBuffer* pRowBuffer = _pRowBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));

  m_pPdsOfBufferIds->bDelete((void*)&oBufferIdStruct.iKey);

  RASSERT(m_pPdsOfBufferIds->iNumEntries(0) == 0);

  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full rowBuffer (if any)                                           */
/*----------------------------------------------------------------------------*/
RowBuffer* CrwToRow::pFullRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  if(m_pPdsOfBufferIds->iNumEntries(0) == 0)
    return nullptr;

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();
  RASSERT(pCurrBlock != nullptr);

  PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pCurrBlock->aItem();

  RowBuffer* pRowBuffer = _pRowBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));

  m_pPdsOfBufferIds->bDelete((void*)&oBufferIdStruct.iKey);

  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* return tail of chain (most recently added buffer)                          */
/*----------------------------------------------------------------------------*/
RowBuffer* CrwToRow::pRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTOROW,_FOOTER_);

  PdsItem_BufferId oKey;
  oKey.iKey = C_INT_MAX;

  SkipListBlock* pSearchBlock = m_pPdsOfBufferIds->pFindLessThan((void *) &oKey);

  if(pSearchBlock != nullptr) {
    PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pSearchBlock->aItem();

    RowBuffer* pRowBuffer = _pRowBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));
    return pRowBuffer;
  }

  return nullptr;
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


  PdsItem_BufferId oItem;
  oItem.iKey = ++m_iSynthKey;
  oItem.oBufferId = pNewRowBuffer->oGetBufferId();

  SkipListBlock* pBlock = m_pPdsOfBufferIds->pInsert((void*) &oItem);
  RASSERT(pBlock != nullptr);
  *(PdsItem_BufferId*)(pBlock->aItem()) = oItem;

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
