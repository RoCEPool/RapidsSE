/*============================================================================*/
/* ToLglPtnRow.cpp -- Convert row buff to logicl partitioned row buff         */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"
#include "ToLglPtnRow.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "BucketMap.hpp"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ToLglPtnRow,CN_TOLGLPTNROW)
__aNewFunction(ToLglPtnRow,CN_TOLGLPTNROW)
__vDeleteFunction(ToLglPtnRow,CN_TOLGLPTNROW)
__aFooterFunction(ToLglPtnRow)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToLglPtnRow::ToLglPtnRow(void)
: Object(CN_TOLGLPTNROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);

  // m_oHashSet

  m_pFromRowBuffer = nullptr;

  m_pIntoRowBuffer   = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToLglPtnRow::~ToLglPtnRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ToBktCrw                                                        */
/*----------------------------------------------------------------------------*/
void ToLglPtnRow::vRepairHashSet(Object* pObject)
{
  _static_

  ToLglPtnRow* pToLglPtnRow = _pToLglPtnRow(pObject);

  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++) {

    // delete all completed buffers
    RowBuffer* pRowBuffer = pToLglPtnRow->pCompleted(i_Ptn);
    while(pRowBuffer) {
      AM::vDeleteOperand(pRowBuffer);
      pRowBuffer = pToLglPtnRow->pCompleted(i_Ptn);
    }

    // delete dangling buffer
    pRowBuffer = pToLglPtnRow->pDangling(i_Ptn);
    delete pRowBuffer;
  }

  // delete the toBktRow
  delete pToLglPtnRow;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToLglPtnRow::vConvert(Table* pTable, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);

  m_pFromRowBuffer = pRowBuffer;
  
  RowPix* pRowPix = pRowBuffer->pRowPix();
  i8 i_KeyHshCol = pRowPix->i_KeyHsh();

  i8 nRows = pRowBuffer->nRows();
  //vTrace("ToBktRow::vConvert2 table:%s Total Rows:%lld \n"_Stz,pTable->szTableNm()->azChar(), nRows);
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {

    Row* aRowSrc = pRowBuffer->pRowAddress(i_Row);

    i8 iKeyHsh = aRowSrc->iKeyHsh(i_KeyHshCol);
    
    KeyBkt tKeyBkt = KeyBkt::tToKeyBkt(iKeyHsh);

    // convert keyBkt to lglPtn
    LglPtn tPtnId = pTable->tBktToLglPtn(tKeyBkt);

    //vTrace("ToBktRow::vConvert2 table:%s Row:%lld Bkt:%lld  ==>  Ptn:%lld\n"_Stz, pTable->szTableNm()->azChar(),   i_Row, i_Bkt, tPtnId.iAsI8());
    vCopyRow(tPtnId,tKeyBkt,aRowSrc);

  }
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
RowBuffer* ToLglPtnRow::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_oHashSet.pCompletedBuffer(i_Ptn));
  
  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
RowBuffer* ToLglPtnRow::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_oHashSet.pDanglingBuffer(i_Ptn));
  
  return pRowBuffer;}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source row into target partition row slab                             */
/*----------------------------------------------------------------------------*/
void ToLglPtnRow::vCopyRow(LglPtn tPtnId, i8 i_Bkt, Row* aRowSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);

  // new a destination row in target partition row slab
  vNewDstRow(tPtnId, i_Bkt);

  // copy that
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;
  m_pIntoRowBuffer->vCopyRow(aRowSrc,bSrcHasPtnHash,bCopyPtnHash);
}

/*----------------------------------------------------------------------------*/
/* new a destination row in row slab in hash set                              */
/*----------------------------------------------------------------------------*/
void ToLglPtnRow::vNewDstRow(LglPtn tPtnId, i8 i_Bkt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);

  i8 i_Ptn = tPtnId.iAsI8();

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

  pDstRowBuffer->vPtnId(tPtnId);
  pDstRowBuffer->vBucketNo(i_Bkt);

  m_pIntoRowBuffer = pDstRowBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
