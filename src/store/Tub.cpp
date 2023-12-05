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
extern "C" void* aExtern_TubAllotRowPds(void* pTub, char** aazStrArea, i8* piErrorCode) {
  return Tub::aAllotRowPds((Tub*)pTub, aazStrArea, piErrorCode);
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

  m_pBufferIdQueue = new PdsBufferIdQueue();

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

  delete m_pBufferIdQueue;
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
/* allot a new row in a pds                                                   */
/*----------------------------------------------------------------------------*/
void* Tub::aAllotRowPds(Tub* pTub, char** aazStrArea, i8* piErrorCode)
{
  _static_

  try {
    RASSERT(CN_TUB == pTub->eGetClassNo());

    RowPix* pRowPix = pTub->pRowPix();

    Pds* pPdsRowBuffer = _pPds(pTub->pWorkingBuffer());

    if (! pPdsRowBuffer) {
      pPdsRowBuffer = new Pds(pRowPix);
      pTub->vAddBuffer(pPdsRowBuffer);
    }

    Row* pRowDst = pPdsRowBuffer->pAllotRowFull(aazStrArea);

    if (pRowDst == nullptr) {
      pPdsRowBuffer = new Pds(pRowPix);
      pTub->vAddBuffer(pPdsRowBuffer);
      pRowDst = pPdsRowBuffer->pAllotRowFull(aazStrArea);
      RASSERT(pRowDst);
    }

    return pRowDst->aCellArray();
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

  m_pBufferIdQueue->vAddBuffer(pDataBuffer);

  return;
}

/*----------------------------------------------------------------------------*/
/* return a non-first/full buffer (if any)                                    */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pCompletedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  return m_pBufferIdQueue->pCompletedBuffer();
}

/*----------------------------------------------------------------------------*/
/* return any first/final dangling buffer                                     */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pDanglingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  return m_pBufferIdQueue->pDanglingBuffer();
}

/*----------------------------------------------------------------------------*/
/* is this tub empty?                                                         */
/*----------------------------------------------------------------------------*/
bool Tub::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  return m_pBufferIdQueue->bIsEmpty();
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

  return m_pBufferIdQueue->pDataBuffer();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
