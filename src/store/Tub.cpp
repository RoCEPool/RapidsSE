/*============================================================================*/
/* Tub.cpp -- Chain of Buffers with dangling support                          */
/*============================================================================*/

#include "BASE.h"

#include "Tub.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Tub,CN_TUB)
__aNewFunction(Tub,CN_TUB)
__vDeleteFunction(Tub,CN_TUB)
__aFooterFunction(Tub)

/*----------------------------------------------------------------------------*/
/*-- Externs -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
extern "C" void* aExtern_TubAllotRow(void* pTub, char** aazStrArea, i8* piErrorCode) {
  return Tub::aAllotRow((Tub*)pTub, (char**)aazStrArea, piErrorCode);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_TubRowStrSize(void* pTub, i8 nRowStrSize) {
  Tub::vRowStrSize((Tub*)pTub, nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_TubRowPix(void* pTub, void* pRowPix) {
  Tub::vSetRowPix((Tub*)pTub, (RowPix*)pRowPix);
}

/*----------------------------------------------------------------------------*/
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
void* aTub_AllotRow(void* pTub, char** aazStrArea, i8* piErrorCode) {
  return Tub::aAllotRow((Tub*)pTub, (char**)aazStrArea, piErrorCode);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void vTub_RowStrSize(void* pTub, i8 nRowStrSize) {
  Tub::vRowStrSize((Tub*)pTub, nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void vTub_RowPix(void* pTub, void* pRowPix) {
  Tub::vSetRowPix((Tub*)pTub, (RowPix*)pRowPix);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tub::Tub(void)
: Object(CN_TUB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  m_iSynthKey = 0;
  m_pPdsOfBufferIds = new Pds_t<PdsItem_BufferId>(nullptr);

  m_pRowPix      = nullptr;
  
  _checkInitialised_(CN_TUB);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tub::~Tub(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  delete m_pRowPix;

  delete m_pPdsOfBufferIds;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair tubby                                                               */
/*----------------------------------------------------------------------------*/
void Tub::vRepair(Object* pObject)
{
  _static_
  
  UNUSED(pObject);
}

/*----------------------------------------------------------------------------*/
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
void* Tub::aAllotRow(Tub* pTub, char** aazStrArea, i8* piErrorCode)
{
  _static_

  try {
    RASSERT(CN_TUB == pTub->eGetClassNo());

    RowPix* pRowPix = pTub->pRowPix();

    RowBuffer* pRowBuffer = _pRowBuffer(pTub->pWorkingBuffer());
    if (! pRowBuffer) {
      pRowBuffer = new RowBuffer;
      pRowBuffer->vCopyRowPix(pRowPix);
      pTub->vAddBuffer(pRowBuffer);
    }

    Row* pRow = pRowBuffer->pAllotRow();

    if (! pRow) {
      pRowBuffer = new RowBuffer;
      pTub->vAddBuffer(pRowBuffer);
      pRowBuffer->vCopyRowPix(pRowPix);
      pRow = pRowBuffer->pAllotRow();
      ASSERT(pRow);
    }

    *aazStrArea = pRowBuffer->azDepositString_begin();
    return pRow->aCellArray();
  }
  catch (LowMemory xLowMemory) {
    *piErrorCode = EC_LOW_MEMORY;
    return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void Tub::vRowStrSize(Tub* pTub, i8 nRowStrSize)
{
  _static_
  
  RASSERT(CN_TUB == pTub->eGetClassNo());

  RowBuffer* pRowBuffer = _pRowBuffer(pTub->pWorkingBuffer());

  pRowBuffer->vTakeHead(nRowStrSize);

}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void Tub::vSetRowPix(Tub* pTub, RowPix* pRowPix)
{
  _static_

  RASSERT(CN_TUB == pTub->eGetClassNo());

  pTub->vRowPix(pRowPix);

}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given buffer as first into buffer chain                                */
/*----------------------------------------------------------------------------*/
void Tub::vAddBuffer(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  PdsItem_BufferId oItem;
  oItem.iKey = ++m_iSynthKey;
  oItem.oBufferId = pDataBuffer->oGetBufferId();

  SkipListBlock* pBlock = m_pPdsOfBufferIds->pInsert((void*) &oItem);
  RASSERT(pBlock != nullptr);
  *(PdsItem_BufferId*)(pBlock->aItem()) = oItem;

  return;
}

/*----------------------------------------------------------------------------*/
/* return a non-first/full buffer (if any)                                    */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pCompletedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  // don't return a buffer if only one exists in the pds
  // since it may not be full. Only the buffer at the end
  // of the pds chain may not be not full.
  // [Root-Block]-->[Full-Buffer]-->[Full-Buffer]-->[Non-Full-Buffer]-->NULL
  if(m_pPdsOfBufferIds->iNumEntries(0) <= 1)
    return nullptr;

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();
  RASSERT(pCurrBlock != nullptr);

  PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pCurrBlock->aItem();

  DataBuffer* pDataBuffer = _pDataBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));

  m_pPdsOfBufferIds->bDelete((void*)&oBufferIdStruct.iKey);

  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* return any first/final dangling buffer                                     */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pDanglingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  RASSERT(m_pPdsOfBufferIds->iNumEntries(0) <= 1);

  if(m_pPdsOfBufferIds->iNumEntries(0) == 0)
    return nullptr;

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();
  RASSERT(pCurrBlock != nullptr);

  PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pCurrBlock->aItem();

  DataBuffer* pDataBuffer = _pDataBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));

  m_pPdsOfBufferIds->bDelete((void*)&oBufferIdStruct.iKey);

  RASSERT(m_pPdsOfBufferIds->iNumEntries(0) == 0);

  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* is this tub empty?                                                         */
/*----------------------------------------------------------------------------*/
bool Tub::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  return m_pPdsOfBufferIds->iNumEntries(0) == 0;
}

/*----------------------------------------------------------------------------*/
/* get row pix                                                                */
/*----------------------------------------------------------------------------*/
RowPix* Tub::pRowPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
    
  return m_pRowPix;
}

/*----------------------------------------------------------------------------*/
/* set row pix                                                                */
/*----------------------------------------------------------------------------*/
void Tub::vRowPix(RowPix* pRowPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  m_pRowPix = pRowPix;
}

/*----------------------------------------------------------------------------*/
/* return working buffer (tail)                                               */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pWorkingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  if(m_pPdsOfBufferIds->iNumEntries(0) == 0)
    return nullptr;

  PdsItem_BufferId oKey;
  oKey.iKey = C_INT_MAX;

  // Find the last block less than MAX
  SkipListBlock* pSearchBlock = m_pPdsOfBufferIds->pFindLessThan((void *) &oKey);

  if(pSearchBlock != nullptr) {
    PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pSearchBlock->aItem();

    DataBuffer* pDataBuffer = _pDataBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));
    return pDataBuffer;
  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
