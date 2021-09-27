/*============================================================================*/
/* ToPtnCrw.cpp -- Convert ptn nst row buffers to ptn crw buffers             */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

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

__pCastFunction(STORE,ToPtnCrw,CN_TOPTNCRW)
__aNewFunction(ToPtnCrw,CN_TOPTNCRW)
__vDeleteFunction(ToPtnCrw,CN_TOPTNCRW)
__aFooterFunction(ToPtnCrw)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToPtnCrw::ToPtnCrw(StreamDef* pStreamDef)
: Object(CN_TOPTNCRW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);

  // m_oHashSet
  
  m_pTable     = nullptr;
  m_pStreamDef = pStreamDef;
  m_pCrwBuffer = nullptr;

  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++)
    m_paRowCompactor[i_Ptn] = new RowCompactor;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToPtnCrw::~ToPtnCrw(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);
  
  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++)
    delete m_paRowCompactor[i_Ptn];
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the toPtnCrw                                                        */
/*----------------------------------------------------------------------------*/
void ToPtnCrw::vRepairHashSet(Object* pObject)
{
  _static_

  ToPtnCrw* pToPtnCrw = _pToPtnCrw(pObject);

  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++) {

    // delete all completed buffers
    CrwBuffer* pCrwBuffer = pToPtnCrw->pCompleted(i_Ptn);
    while(pCrwBuffer) {
      AM::vDeleteOperand(pCrwBuffer);
      pCrwBuffer = pToPtnCrw->pCompleted(i_Ptn);
    }

    // delete dangling buffer
    pCrwBuffer = pToPtnCrw->pDangling(i_Ptn);
    delete pCrwBuffer;
  }

  // delete the toPtnCrw
  delete pToPtnCrw;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToPtnCrw::vConvert(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);

  PtnId tPtn = pRowBuffer->tPtnId();
  m_pCrwBuffer = m_oHashSet.pBuffer(tPtn.iAsI8()) ?
      _pCrwBuffer(m_oHashSet.pBuffer(tPtn.iAsI8())) : nullptr;

  if (! m_pCrwBuffer) {
    m_pCrwBuffer = pNewCrwBuffer(tPtn.iAsI8());
    vAddNst(pRowBuffer);
  }

  vAddRowsToBuffer(tPtn.iAsI8(),pRowBuffer);
  
  //delete pRowBuffer;
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
CrwBuffer* ToPtnCrw::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);
  
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
CrwBuffer* ToPtnCrw::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_oHashSet.pDanglingBuffer(i_Ptn));

  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add normalised strings into new crw buffer                                 */
/*----------------------------------------------------------------------------*/
void ToPtnCrw::vAddNst(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);
  
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
void ToPtnCrw::vAddRowsToBuffer(i8 i_Ptn, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);
    
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
CrwBuffer* ToPtnCrw::pNewCrwBuffer(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNCRW,_FOOTER_);

  //  create a new buffer
  CrwBuffer* pCrwBuffer = new CrwBuffer();
  
  pCrwBuffer->vStreamDef(m_pStreamDef);

  PtnId tPtnId;
  tPtnId = i_Ptn;
  pCrwBuffer->vPtnId(tPtnId);
  // open buffer for append
  pCrwBuffer->vAppendOpen(m_paRowCompactor[i_Ptn],false);

  m_oHashSet.vAddBuffer(i_Ptn,pCrwBuffer);
  
  return pCrwBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
