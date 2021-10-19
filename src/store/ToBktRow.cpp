/*============================================================================*/
/* ToBktRow.cpp -- Convert row buffers to partitioned row slab buffers        */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"
#include "ToBktRow.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ToBktRow,CN_TOBKTROW)
__aNewFunction(ToBktRow,CN_TOBKTROW)
__vDeleteFunction(ToBktRow,CN_TOBKTROW)
__aFooterFunction(ToBktRow)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToBktRow::ToBktRow(void)
: Object(CN_TOBKTROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);

  // m_oHashSet

  m_pFromRowBuffer = nullptr;

  m_pIntoRowBuffer   = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToBktRow::~ToBktRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ToBktCrw                                                        */
/*----------------------------------------------------------------------------*/
void ToBktRow::vRepairHashSet(Object* pObject)
{
  _static_

  ToBktRow* pToBktRow = _pToBktRow(pObject);

  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++) {

    // delete all completed buffers
    RowBuffer* pRowBuffer = pToBktRow->pCompleted(i_Ptn);
    while(pRowBuffer) {
      AM::vDeleteOperand(pRowBuffer);
      pRowBuffer = pToBktRow->pCompleted(i_Ptn);
    }

    // delete dangling buffer
    pRowBuffer = pToBktRow->pDangling(i_Ptn);
    delete pRowBuffer;
  }

  // delete the toBktRow
  delete pToBktRow;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToBktRow::vConvert(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);
  NEVER_REACH;
  m_pFromRowBuffer = pRowBuffer;
      
  i8 nRows = pRowBuffer->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Row* aRowSrc = pRowBuffer->pRowAddress(i_Row);

    i8 iHash1 = pRowBuffer->iHashOfRow(aRowSrc);
    i8 i_Bkt1 = iHash1 % C_MAX_BUCKETS;

    PtnId i_Ptn1;
    i_Ptn1 = pRowBuffer->pTable()->tBktToPtn(i_Bkt1);  //AM::PTNMAP::tToPtn(i_Bkt1);
    if (pRowBuffer->pTable()) {
      i_Ptn1 = pRowBuffer->pTable()->tBktToPtn(i_Bkt1);
      vTrace("ToBktRow::vConvert Row:%lld Bkt:%lld  i_Ptn:%lld\n"_Stz, i_Row, i_Bkt1, i_Ptn1.iAsI8());
    }
    vCopyRow(i_Ptn1.iAsI8(),aRowSrc);

  }
}

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToBktRow::vConvert(Table* pTable, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);

  m_pFromRowBuffer = pRowBuffer;

  i8 nRows = pRowBuffer->nRows();
  vTrace("ToBktRow::vConvert2 table:%s Total Rows:%lld \n"_Stz,pTable->szTableNm()->azChar(), nRows);
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {

    Row* aRowSrc = pRowBuffer->pRowAddress(i_Row);

    i8 iHash = pRowBuffer->iHashOfRow(aRowSrc);
    i8 i_Bkt = iHash % C_MAX_BUCKETS;

    // convert Bkt No to Ptn No
    PtnId tPtnId = pTable->tBktToPtn(i_Bkt);

    vTrace("ToBktRow::vConvert2 table:%s Row:%lld Bkt:%lld  ==>  Ptn:%lld\n"_Stz, pTable->szTableNm()->azChar(),   i_Row, i_Bkt, tPtnId.iAsI8());
    vCopyRow(tPtnId.iAsI8(),i_Bkt,aRowSrc);

/*
#ifdef WITH_KEEP_ROW_HASH
    i8 iHash = pRowBuffer->iHashOfRow(aRowSrc);

    PtnId tPtn = AM::PTNMAP::tHashToDtdPtnId(iHash);

    i8 i_Bkt = pTable->iPtnToBkt(tPtn);   //AM::PTNMAP::iToBkt(i_Ptn);

    vTrace("WITH_KEEP_ROW_HASH ToBktRow::vConvert Row:%lld Bkt:%lld  i_Ptn:%lld\n"_Stz, i_Row, i_Bkt, tPtn.iAsI8());
    vCopyRow(i_Bkt,aRowSrc);
#endif
 */


  }
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
RowBuffer* ToBktRow::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_oHashSet.pCompletedBuffer(i_Ptn));
  
  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
RowBuffer* ToBktRow::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_oHashSet.pDanglingBuffer(i_Ptn));
  
  return pRowBuffer;}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source row into target partition row slab                             */
/*----------------------------------------------------------------------------*/
void ToBktRow::vCopyRow(i8 i_Ptn, Row* aRowSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);
  NEVER_REACH;
  // new a destination row in target partition row slab
  vNewDstRow(i_Ptn);
  
  // copy that
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;
  m_pIntoRowBuffer->vCopyRow(aRowSrc,bSrcHasPtnHash,bCopyPtnHash);
}

/*----------------------------------------------------------------------------*/
/* copy source row into target partition row slab                             */
/*----------------------------------------------------------------------------*/
void ToBktRow::vCopyRow(i8 i_Ptn, i8 iBkt, Row* aRowSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);

  // new a destination row in target partition row slab
  vNewDstRow(i_Ptn, iBkt);

  // copy that
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;
  m_pIntoRowBuffer->vCopyRow(aRowSrc,bSrcHasPtnHash,bCopyPtnHash);
}

/*----------------------------------------------------------------------------*/
/* new a destination row in row slab in hash set                              */
/*----------------------------------------------------------------------------*/
void ToBktRow::vNewDstRow(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);
  NEVER_REACH;
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

  PtnId tPtnId;
  tPtnId = i_Ptn;
  pDstRowBuffer->vPtnId(tPtnId);

  m_pIntoRowBuffer = pDstRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* new a destination row in row slab in hash set                              */
/*----------------------------------------------------------------------------*/
void ToBktRow::vNewDstRow(i8 i_Ptn, i8 i_Bkt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTROW,_FOOTER_);

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

  PtnId tPtnId;
  tPtnId = i_Ptn;
  pDstRowBuffer->vPtnId(tPtnId);
  pDstRowBuffer->vBucketNo(i_Bkt);

  m_pIntoRowBuffer = pDstRowBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
