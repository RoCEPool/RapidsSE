/*============================================================================*/
/* ToHshCrw.cpp -- Convert ptn nst row buffers to hashed crw buffers          */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,ToHshCrw,CN_TOHSHCRW)
__aNewFunction(ToHshCrw,CN_TOHSHCRW)
__vDeleteFunction(ToHshCrw,CN_TOHSHCRW)
__aFooterFunction(ToHshCrw)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToHshCrw::ToHshCrw(StreamDef* pStreamDef)
: Object(CN_TOHSHCRW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);

  // m_oHashSet
  
  m_pTable     = nullptr;
  m_pStreamDef = pStreamDef;
  m_pCrwBuffer = nullptr;

  for (i8 i_Ptn = 0; i_Ptn < AM::nPtnsInGrid(); i_Ptn++)
    m_paRowCompactor[i_Ptn] = new RowCompactor;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToHshCrw::~ToHshCrw(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPtnsInGrid(); i_Ptn++)
    delete m_paRowCompactor[i_Ptn];
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToHshCrw::vConvert(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);
  
  i8 i_Ptn = pRowBuffer->tPtnNo();
  
  //pRowBuffer->pRowBuffer()->vDumpNst();
  
  m_pCrwBuffer = m_oHashSet.pBuffer(i_Ptn) ?
                  _pCrwBuffer(m_oHashSet.pBuffer(i_Ptn)) : nullptr;
  
  if (! m_pCrwBuffer) {
    m_pCrwBuffer = pNewCrwBuffer(i_Ptn);
    vAddNst(pRowBuffer);
  }
  
  vAddRowsToBuffer(i_Ptn,pRowBuffer);
  
  delete pRowBuffer;
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
CrwBuffer* ToHshCrw::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);
  
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
CrwBuffer* ToHshCrw::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_oHashSet.pDanglingBuffer(i_Ptn));

  return pCrwBuffer;}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add normalised strings into new crw buffer                                 */
/*----------------------------------------------------------------------------*/
void ToHshCrw::vAddNst(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);
  
  ASSERT(0 == m_pCrwBuffer->nRowCount());

  NmlStr* pNmlStr = pRowBuffer->pNmlStr();

  Stz* szNmlStr = nullptr;
  while ( (szNmlStr = pNmlStr->szEach(szNmlStr)) ) {
    //vTrace("vAddNst : %s\n"_Stz,szNmlStr);

    bool bAdded = m_pCrwBuffer->m_pRowCompactor->bCompactTail(szNmlStr);
    ASSERT(bAdded);
  }


  //vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* create a new crw buffer                                                    */
/*----------------------------------------------------------------------------*/
void ToHshCrw::vAddRowsToBuffer(i8 i_Ptn, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);
    
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
CrwBuffer* ToHshCrw::pNewCrwBuffer(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOHSHCRW,_FOOTER_);
  
  //  create a new buffer
  CrwBuffer* pCrwBuffer = new CrwBuffer();
  
  pCrwBuffer->vStreamDef(m_pStreamDef);
  pCrwBuffer->vPtnNo(i_Ptn);

  // open buffer for append
  pCrwBuffer->vAppendOpen(m_paRowCompactor[i_Ptn],false);

  m_oHashSet.vAddBuffer(i_Ptn,pCrwBuffer);
  
  return pCrwBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
