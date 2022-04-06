/*============================================================================*/
/* ToBktCrw.cpp -- Convert ptn nst row buffers to ptn crw buffers             */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"
#include "ToLglPtnCrw.hpp"

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

__pCastFunction(STORE,ToLglPtnCrw,CN_TOLGLPTNCRW)
__aNewFunction(ToLglPtnCrw,CN_TOLGLPTNCRW)
__vDeleteFunction(ToLglPtnCrw,CN_TOLGLPTNCRW)
__aFooterFunction(ToLglPtnCrw)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToLglPtnCrw::ToLglPtnCrw(StreamDef* pStreamDef)
: Object(CN_TOLGLPTNCRW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);

  // m_oHashSet
  
  m_pTable     = nullptr;
  m_pStreamDef = pStreamDef;
  m_pCrwBuffer = nullptr;

  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++)
    m_paRowCompactor[i_Ptn] = new RowCompactor;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToLglPtnCrw::~ToLglPtnCrw(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);
  
  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++)
    delete m_paRowCompactor[i_Ptn];
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* repair the ToBktCrw                                                        */
/*----------------------------------------------------------------------------*/
void ToLglPtnCrw::vRepairHashSet(Object* pObject)
{
  _static_

  ToLglPtnCrw* pToLglPtnCrw = _pToLglPtnCrw(pObject);

  for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsInGrid(); i_Ptn++) {

    // delete all completed buffers
    CrwBuffer* pCrwBuffer = pToLglPtnCrw->pCompleted(i_Ptn);
    while(pCrwBuffer) {
      AM::vDeleteOperand(pCrwBuffer);
      pCrwBuffer = pToLglPtnCrw->pCompleted(i_Ptn);
    }

    // delete dangling buffer
    pCrwBuffer = pToLglPtnCrw->pDangling(i_Ptn);
    delete pCrwBuffer;
  }

  // delete the toBktCrw
  delete pToLglPtnCrw;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToLglPtnCrw::vConvert(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);

   LglPtn tLglPtn = pRowBuffer->tLglPtn();
   //i8 iBktNo = pRowBuffer->iBucketNo();
   //vTrace("ToBktCrw::vConvert PtnID:%lld Bkt:%lld\n"_Stz, tLglPtn.iAsI8(), iBktNo);
  //pRowBuffer->pRowBuffer()->vDumpNst();
  
  m_pCrwBuffer = m_oHashSet.pBuffer(tLglPtn.iAsI8()) ?
      _pCrwBuffer(m_oHashSet.pBuffer(tLglPtn.iAsI8())) : nullptr;
  
  if (! m_pCrwBuffer) {
    //vTrace("ToBktCrw::vAddRowsToBuffer iBkt:%lld \n"_Stz,pRowBuffer->iBucketNo());
    m_pCrwBuffer = pNewCrwBuffer(tLglPtn, pRowBuffer->iBucketNo());
    vAddNst(pRowBuffer);
  }
  
  vAddRowsToBuffer(tLglPtn,pRowBuffer);
  
  //delete pRowBuffer;
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
CrwBuffer* ToLglPtnCrw::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);
  
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
CrwBuffer* ToLglPtnCrw::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_oHashSet.pDanglingBuffer(i_Ptn));

  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add normalised strings into new crw buffer                                 */
/*----------------------------------------------------------------------------*/
void ToLglPtnCrw::vAddNst(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);
  
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
void ToLglPtnCrw::vAddRowsToBuffer(LglPtn tLglPtn, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);
    
  i8 nRows = pRowBuffer->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Row* aRow = pRowBuffer->pRowAddress(i_Row);
    
    bool bAppended = m_pCrwBuffer->bAppendRow(aRow);
    
    if (bAppended)
      continue;

    m_pCrwBuffer->vAppendClose();
    
    //m_pCrwBuffer->vPrintRows(m_pStreamDef);
    //vTrace("ToBktCrw::vAddRowsToBuffer iBkt:%lld \n"_Stz,pRowBuffer->iBucketNo());
    m_pCrwBuffer = pNewCrwBuffer(tLglPtn, pRowBuffer->iBucketNo());
    vAddNst(pRowBuffer);

    bAppended = m_pCrwBuffer->bAppendRow(aRow);
    ASSERT(bAppended);
  }
}

/*----------------------------------------------------------------------------*/
/* create a new crw buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* ToLglPtnCrw::pNewCrwBuffer(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);
  
  //  create a new buffer
  CrwBuffer* pCrwBuffer = new CrwBuffer();
  
  pCrwBuffer->vStreamDef(m_pStreamDef);

  pCrwBuffer->vPtnId(tLglPtn);

  // Set the bucket number
  i8 iBktNo = 0;

  pCrwBuffer->vBucketNo(iBktNo);

  // open buffer for append
  pCrwBuffer->vAppendOpen(m_paRowCompactor[tLglPtn.iAsI8()],false);

  m_oHashSet.vAddBuffer(tLglPtn.iAsI8(),pCrwBuffer);
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* create a new crw buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* ToLglPtnCrw::pNewCrwBuffer(LglPtn tLglPtn, i8 iBkt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOLGLPTNCRW,_FOOTER_);

  //  create a new buffer
  CrwBuffer* pCrwBuffer = new CrwBuffer();

  pCrwBuffer->vStreamDef(m_pStreamDef);

  pCrwBuffer->vPtnId(tLglPtn);

  pCrwBuffer->vBucketNo(iBkt);

  // open buffer for append
  pCrwBuffer->vAppendOpen(m_paRowCompactor[tLglPtn.iAsI8()],false);

  m_oHashSet.vAddBuffer(tLglPtn.iAsI8(),pCrwBuffer);

  return pCrwBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
