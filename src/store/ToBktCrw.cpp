/*============================================================================*/
/* ToBktCrw.cpp -- Convert ptn nst row buffers to ptn crw buffers             */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"
#include "ToBktCrw.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ToBktCrw,CN_TOBKTCRW)
__aNewFunction(ToBktCrw,CN_TOBKTCRW)
__vDeleteFunction(ToBktCrw,CN_TOBKTCRW)
__aFooterFunction(ToBktCrw)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToBktCrw::ToBktCrw(StreamDef* pStreamDef)
: Object(CN_TOBKTCRW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);

  // m_oHashSet
  
  m_pTable     = nullptr;
  m_pStreamDef = pStreamDef;
  m_pCrwBuffer = nullptr;

  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++)
    m_paRowCompactor[i_Ptn] = new RowCompactor;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToBktCrw::~ToBktCrw(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);
  
  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++)
    delete m_paRowCompactor[i_Ptn];
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* repair the ToBktCrw                                                        */
/*----------------------------------------------------------------------------*/
void ToBktCrw::vRepairHashSet(Object* pObject)
{
  _static_

  ToBktCrw* pToBktCrw = _pToBktCrw(pObject);

  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++) {

    // delete all completed buffers
    CrwBuffer* pCrwBuffer = pToBktCrw->pCompleted(i_Ptn);
    while(pCrwBuffer) {
      AM::vDeleteOperand(pCrwBuffer);
      pCrwBuffer = pToBktCrw->pCompleted(i_Ptn);
    }

    // delete dangling buffer
    pCrwBuffer = pToBktCrw->pDangling(i_Ptn);
    delete pCrwBuffer;
  }

  // delete the toBktCrw
  delete pToBktCrw;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToBktCrw::vConvert(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);

  i8 i_Ptn = pRowBuffer->tPtnId().iAsI8();

  //pRowBuffer->pRowBuffer()->vDumpNst();
  
  m_pCrwBuffer = m_oHashSet.pBuffer(i_Ptn) ?
                  _pCrwBuffer(m_oHashSet.pBuffer(i_Ptn)) : nullptr;
  
  if (! m_pCrwBuffer) {
    m_pCrwBuffer = pNewCrwBuffer(i_Ptn);
    vAddNst(pRowBuffer);
  }
  
  vAddRowsToBuffer(i_Ptn,pRowBuffer);
  
  //delete pRowBuffer;
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
CrwBuffer* ToBktCrw::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_oHashSet.pCompletedBuffer(i_Ptn));
  
//  if (pCrwBuffer) {
//    pCrwBuffer->vDump();
//    vDebugPoint();
//  }
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
CrwBuffer* ToBktCrw::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_oHashSet.pDanglingBuffer(i_Ptn));

  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add normalised strings into new crw buffer                                 */
/*----------------------------------------------------------------------------*/
void ToBktCrw::vAddNst(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);
  
  ASSERT(0 == m_pCrwBuffer->nRows());

  NmlStr* pNmlStr = pRowBuffer->pNmlStr();

  Stz* szNmlStr = nullptr;
  while ( (szNmlStr = pNmlStr->szEach(szNmlStr)) ) {
    //vTrace("vAddNst : %s\n"_Stz,szNmlStr);

    bool bAdded = m_pCrwBuffer->pRowCompactor()->bCompactTail(szNmlStr);
    ASSERT(bAdded);
    UNUSED(bAdded);
  }


  //vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* create a new crw buffer                                                    */
/*----------------------------------------------------------------------------*/
void ToBktCrw::vAddRowsToBuffer(i8 i_Ptn, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);
    
  i8 nRows = pRowBuffer->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Row* aRow = pRowBuffer->pRowAddress(i_Row);
    
    bool bAppended = m_pCrwBuffer->bAppendRow(aRow);
    
    if (bAppended)
      continue;

    m_pCrwBuffer->vAppendClose();
    
    //m_pCrwBuffer->vPrintRows(m_pStreamDef);
    
    m_pCrwBuffer = pNewCrwBuffer(i_Ptn);
    vAddNst(pRowBuffer);

    bAppended = m_pCrwBuffer->bAppendRow(aRow);
    ASSERT(bAppended);
  }
}

/*----------------------------------------------------------------------------*/
/* create a new crw buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* ToBktCrw::pNewCrwBuffer(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOBKTCRW,_FOOTER_);
  
  //  create a new buffer
  CrwBuffer* pCrwBuffer = new CrwBuffer();
  
  pCrwBuffer->vStreamDef(m_pStreamDef);

  PtnId tPtnId;
  tPtnId = i_Ptn;
  pCrwBuffer->vPtnId(tPtnId);

  // Set the bucket number
  i8 iBktNo = AM::PTNMAP::iToBkt(tPtnId);

  pCrwBuffer->vBucketNo(iBktNo);

  // open buffer for append
  pCrwBuffer->vAppendOpen(m_paRowCompactor[i_Ptn],false);

  m_oHashSet.vAddBuffer(i_Ptn,pCrwBuffer);
  
  return pCrwBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
