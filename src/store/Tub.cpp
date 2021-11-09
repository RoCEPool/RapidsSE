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
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
void* aTub_AllotRow(void* pTub, char** aazStrArea) {
  return Tub::aAllotRow((Tub*)pTub, (char**)aazStrArea);
}

/*----------------------------------------------------------------------------*/
/* discard last allotted row                                                  */
/*----------------------------------------------------------------------------*/
void vTub_DiscardRow(void* pTub)
{
  Tub::vDiscardRow((Tub*)pTub);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void vTub_RowStrSize(void* pTub, i8 nRowStrSize) {
  Tub::vRowStrSize((Tub*)pTub, nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tub::Tub(void)
: Object(CN_TUB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  m_pBufferChain = nullptr;
  
  m_pInCrwBuffer = nullptr;
  m_pInRowBuffer = nullptr;

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

  // assert we don't have any dangling buffers
  RASSERT(! m_pBufferChain);
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
void* Tub::aAllotRow(Tub* pTub, char** aazStrArea)
{
  _static_
  
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

/*----------------------------------------------------------------------------*/
/* discard last allotted row                                                  */
/*----------------------------------------------------------------------------*/
void Tub::vDiscardRow(Tub* pTub)
{
  _static_
  
  RASSERT(CN_TUB == pTub->eGetClassNo());

  RowBuffer* pRowBuffer = _pRowBuffer(pTub->pWorkingBuffer());

  pRowBuffer->vDiscardInvalidRow();
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
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given buffer as first into buffer chain                                */
/*----------------------------------------------------------------------------*/
void Tub::vAddBuffer(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  // link the old with this
  pDataBuffer->vNextLinkedBuffer(m_pBufferChain);

  if (m_pBufferChain)
    m_pBufferChain->vPrevLinkedBuffer(pDataBuffer);

  m_pBufferChain = pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a non-first/full buffer (if any)                                    */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pCompletedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  DataBuffer* pCompletedBuffer = nullptr;
  
  DataBuffer* pDataBuffer = (DataBuffer*)
                              (m_pBufferChain
                                 ? m_pBufferChain->pNextLinkedBuffer()
                                 : nullptr);
  while (pDataBuffer) {
    pCompletedBuffer = pDataBuffer;
    pDataBuffer = (DataBuffer*)pDataBuffer->pNextLinkedBuffer();
  }
  
  if (pCompletedBuffer) {
    DataBuffer* pPrevBuffer = (DataBuffer*)pCompletedBuffer->pPrevLinkedBuffer();
    pPrevBuffer->vNextLinkedBuffer(nullptr);
  }
  
  return pCompletedBuffer;
}

/*----------------------------------------------------------------------------*/
/* return any first/final dangling buffer                                     */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pDanglingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  DataBuffer* pDataBuffer = m_pBufferChain;
  
  m_pBufferChain = nullptr;
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* get input crw buffer                                                       */
/*----------------------------------------------------------------------------*/
CrwBuffer* Tub::pInCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
    
  return m_pInCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* set input crw buffer                                                       */
/*----------------------------------------------------------------------------*/
void Tub::vInCrwBuffer(CrwBuffer* pInCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
    
  m_pInCrwBuffer = pInCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* get input row buffer                                                       */
/*----------------------------------------------------------------------------*/
RowBuffer* Tub::pInRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
    
  return m_pInRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set input row buffer                                                       */
/*----------------------------------------------------------------------------*/
void Tub::vInRowBuffer(RowBuffer* pInRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
    
  m_pInRowBuffer = pInRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* is this tub empty?                                                         */
/*----------------------------------------------------------------------------*/
bool Tub::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  bool bEmpty = m_pBufferChain ? false : true;
    
  return bEmpty;
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
/* return first/working buffer                                                */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pWorkingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  DataBuffer* pBuffer = m_pBufferChain;

  return pBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
