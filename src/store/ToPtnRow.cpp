/*============================================================================*/
/* ToPtnRow.cpp -- Convert row buffers to partitioned row slab buffers        */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ToPtnRow,CN_TOPTNROW)
__aNewFunction(ToPtnRow,CN_TOPTNROW)
__vDeleteFunction(ToPtnRow,CN_TOPTNROW)
__aFooterFunction(ToPtnRow)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToPtnRow::ToPtnRow(void)
: Object(CN_TOPTNROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNROW,_FOOTER_);

  // m_oHashSet

  m_pFromRowBuffer = nullptr;

  m_pIntoRowBuffer   = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOPTNROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToPtnRow::~ToPtnRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the toPtnRow                                                        */
/*----------------------------------------------------------------------------*/
void ToPtnRow::vRepairHashSet(Object* pObject)
{
  _static_

  ToPtnRow* pToPtnRow = _pToPtnRow(pObject);

  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++) {

    // delete all completed buffers
    RowBuffer* pRowBuffer = pToPtnRow->pCompleted(i_Ptn);
    while(pRowBuffer) {
      AM::vDeleteOperand(pRowBuffer);
      pRowBuffer = pToPtnRow->pCompleted(i_Ptn);
    }

    // delete dangling buffer
    pRowBuffer = pToPtnRow->pDangling(i_Ptn);
    delete pRowBuffer;
  }

  // delete the toPtnRow
  delete pToPtnRow;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToPtnRow::vConvert(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNROW,_FOOTER_);
  
  m_pFromRowBuffer = pRowBuffer;
      
  i8 nRows = pRowBuffer->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Row* aRowSrc = pRowBuffer->pRowAddress(i_Row);
    
#ifdef WITH_KEEP_ROW_HASH
    i8 iHash = pRowBuffer->iHashOfRow(aRowSrc);
    
    i8 i_Ptn = AM::PTNMAP::tHashToDtdPtnNo(iHash);
#else
    i8 i_Ptn = pRowBuffer->iPtnOfRow(aRowSrc);
#endif
    
    vCopyRow(i_Ptn,aRowSrc);
  }
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
RowBuffer* ToPtnRow::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNROW,_FOOTER_);
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_oHashSet.pCompletedBuffer(i_Ptn));
  
  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
RowBuffer* ToPtnRow::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNROW,_FOOTER_);
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_oHashSet.pDanglingBuffer(i_Ptn));
  
  return pRowBuffer;}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source row into target partition row slab                             */
/*----------------------------------------------------------------------------*/
void ToPtnRow::vCopyRow(i8 i_Ptn, Row* aRowSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNROW,_FOOTER_);
    
  // new a destination row in target partition row slab
  vNewDstRow(i_Ptn);
  
  // copy that
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;
  m_pIntoRowBuffer->vCopyRow(aRowSrc,bSrcHasPtnHash,bCopyPtnHash);
}

/*----------------------------------------------------------------------------*/
/* new a destination row in row slab in hash set                              */
/*----------------------------------------------------------------------------*/
void ToPtnRow::vNewDstRow(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNROW,_FOOTER_);
    
  // get row buffer for given partition (new a first buffer if ncessary)
  RowBuffer* pDstRowBuffer = _pRowBuffer(m_oHashSet.pBuffer(i_Ptn));
  if (! pDstRowBuffer) {
    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(m_pFromRowBuffer->pRowPix());
    pDstRowBuffer->pRowPix()->vRemovePtnHash();
    m_oHashSet.vAddBuffer(i_Ptn,pDstRowBuffer);
  }
  
  // new a row in row buffer (new another row buffer if necessary)
  Row* aNewRow = pDstRowBuffer->pAllotRow();
  if (! aNewRow) {
    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(m_pFromRowBuffer->pRowPix());
    pDstRowBuffer->pRowPix()->vRemovePtnHash();
    m_oHashSet.vAddBuffer(i_Ptn,pDstRowBuffer);
    aNewRow = pDstRowBuffer->pAllotRow();
    ASSERT(aNewRow);
  }
  
  pDstRowBuffer->vPtnNo(i_Ptn);
  
  m_pIntoRowBuffer = pDstRowBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
