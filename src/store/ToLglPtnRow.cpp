/*============================================================================*/
/* ToLglPtnRow.cpp -- Convert row buff to logicl partitioned row buff         */
/*============================================================================*/

#include "BASE.h"

#include "ToLglPtnRow.hpp"
#include "LglPtnMap.hpp"
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

  m_pIntoPdsRowBuffer   = nullptr;
  
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
    Pds* pPdsRowBuffer = pToLglPtnRow->pCompleted(i_Ptn);
    while(pPdsRowBuffer) {
      AM::vDeleteOperand(pPdsRowBuffer,nullptr);
      pPdsRowBuffer = pToLglPtnRow->pCompleted(i_Ptn);
    }

    // Delete whole list
    // delete dangling buffer
    pPdsRowBuffer = pToLglPtnRow->pDangling(i_Ptn);
    if (pPdsRowBuffer)
      AM::vDeleteOperand(pPdsRowBuffer,nullptr);
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
void ToLglPtnRow::vConvert(Table* pTable, Pds* pPdsRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);
  UNUSED(pTable);

  RowPix* pRowPix = pPdsRowBuffer->pRowPix();
  i8 i_KeyHshCol = pRowPix->i_KeyHsh();

  LglPtnMap* pPtnMap = AM::pLglPtnMap();
  PdsCursor* pCursor = new PdsCursor(pPdsRowBuffer);

  //vTrace("ToBktRow::vConvert2 table:%s Total Rows:%lld \n"_Stz,pTable->szTableNm()->azChar(), nRows);
  while( ! pCursor->bIsCurrNull()) {
    SkipListBlock* pSkipListBlock = pCursor->m_pCurrentBlock;
    Row* aRowSrc = pSkipListBlock->aRow();

    KeyHsh tKeyHsh = aRowSrc->tKeyHsh(i_KeyHshCol);

    // determine which lglPtn the KeyHsh is currently mapped to
    LglPtn tLglPtn = pPtnMap->tHashToLglPtnId(tKeyHsh);

    //vTrace("ToBktRow::vConvert2 table:%s Row:%lld Bkt:%lld  ==>  Ptn:%lld\n"_Stz, pTable->szTableNm()->azChar(),   i_Row, i_Bkt, tLglPtn.iAsI8());
    vCopyRow(tLglPtn, aRowSrc, pRowPix);

    pCursor->vNext();
  }
  pCursor->m_pPds = nullptr;
  delete pCursor;
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
Pds* ToLglPtnRow::pCompleted(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);

  Pds* pPdsRowBuffer = _pPds(m_oLglBufSet.pCompletedBuffer(tLglPtn));
  
  return pPdsRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
Pds* ToLglPtnRow::pDangling(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);

  Pds* pPdsRowBuffer = _pPds(m_oLglBufSet.pDanglingBuffer(tLglPtn));
  
  return pPdsRowBuffer;
  
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source row into target partition row slab                             */
/*----------------------------------------------------------------------------*/
void ToLglPtnRow::vCopyRow(LglPtn tLglPtn, Row* aRowSrc, RowPix* pFromRowPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNROW,_FOOTER_);

  // copy that
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;

  // get pds row buffer for given partition (new a first buffer if ncessary)

  if (! m_pIntoPdsRowBuffer) {
    m_pIntoPdsRowBuffer = new Pds(pFromRowPix);
    m_pIntoPdsRowBuffer->vLglPtn(tLglPtn);

    m_pIntoPdsRowBuffer->pRowPix()->vRemovePtnHash();
    m_oLglBufSet.vAddBuffer(tLglPtn,m_pIntoPdsRowBuffer);
  }

  // new a row in pds row buffer (new another pds row buffer if necessary)
  Row* aNewRow = m_pIntoPdsRowBuffer->pCopyRow(aRowSrc,bSrcHasPtnHash,bCopyPtnHash);
  if (! aNewRow) {
    m_pIntoPdsRowBuffer = new Pds(pFromRowPix);
    m_pIntoPdsRowBuffer->vLglPtn(tLglPtn);

    m_pIntoPdsRowBuffer->pRowPix()->vRemovePtnHash();
    m_oLglBufSet.vAddBuffer(tLglPtn,m_pIntoPdsRowBuffer);

    aNewRow = m_pIntoPdsRowBuffer->pCopyRow(aRowSrc,bSrcHasPtnHash,bCopyPtnHash);
    RASSERT(aNewRow);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
