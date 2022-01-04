/*============================================================================*/
/* Steps_IndexStream.cpp - index stream step group                            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Index.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"
#include "AccessPath.hpp"
#include "PathIdentifier.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexStream(void)
{
  vAddStep("indexStream::new"_Stz,       &Step::vIndexStreamNew);

  vAddStep("indexStream[]"_Stz,          &Step::vIndexStreamGet);
  vAddStep("indexStream[].close"_Stz,    &Step::vIndexStreamClose);
  vAddStep("indexStream[].delete"_Stz,   &Step::vIndexStreamDelete);
  vAddStep("indexStream[].deleteNext"_Stz, &Step::vIndexStreamDeleteNext);
  vAddStep("indexStream[].get"_Stz,      &Step::vIndexStreamGet);
  vAddStep("indexStream[].isNull"_Stz,   &Step::vIndexStreamIsNull);
  vAddStep("indexStream[].open"_Stz,     &Step::vIndexStreamOpen);
  vAddStep("indexStream[].read"_Stz,     &Step::vIndexStreamRead);
  vAddStep("indexStream[].seek"_Stz,     &Step::vIndexStreamSeek);
  vAddStep("indexStream[].set"_Stz,      &Step::vIndexStreamSet);

  vAddStep("indexStream::scanNew"_Stz,    &Step::vIndexStreamScanNew);

  vAddStep("indexStream[].populate"_Stz,  &Step::vIndexStreamScanPopulate);
  vAddStep("indexStream[].populateTop"_Stz,&Step::vIndexStreamScanPopulateTop);
  vAddStep("indexStream[].scanBegin"_Stz, &Step::vIndexStreamScanBegin);
  vAddStep("indexStream[].scanNext"_Stz,  &Step::vIndexStreamScanNext);
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].close                                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamClose(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);
  
  pIndexStream->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].delete                                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamDelete(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);
  
  delete pIndexStream;

  m_p_TaskLogicUnit->m_xpIndexStream = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].deleteNext                                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamDeleteNext(void)
{
  ENTRY();
  
  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);
  
  bool bMoreToDelete = pIndexStream->bDelete();
  
  m_pDataStack->vPushInt(bMoreToDelete);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* indexStream[] -- get partition stream                                      */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpIndexStream);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexStream[].isNull -- is indexStream null ?                              */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpIndexStream.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream::new -- new a partition stream                                 */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamNew(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  IndexStream* pIndexStream = new IndexStream(pTablePtn, pIndex);
  
  m_pDataStack->vPushPtr(pIndexStream);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].open                                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamOpen(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);
  
  bool bOpened = pIndexStream->bOpen();
  ASSERT(bOpened);
  UNUSED(bOpened);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].read                                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamRead(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the rowOnce skipList
  RowOnce* pRowOnce = _pRowOnce(m_pDataStack->pPopPtr(__func__));
  
  CrwBuffer* pCrwBuffer = pIndexStream->pRead_idxstm(pRowOnce);

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexStream[].set -- set partition stream                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamSet(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpIndexStream = _pIndexStream(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].seek                                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamSeek(void)
{
  ENTRY();
  
  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the pathIdentifier 
  PathIdentifier* pPathIdentifier = _pPathIdentifier(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the upper bound key
  CellSingle xUpperBound;
  m_pDataStack->vPopItem(xUpperBound,__func__);
  Stz* szUpperKey = xUpperBound.szStz();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the lower bound key
  CellSingle xLowerBound;
  m_pDataStack->vPopItem(xLowerBound,__func__);
  Stz* szLowerKey = xLowerBound.szStz();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the phrase
  i8 iPhrase = m_pDataStack->iPopInt(__func__);
  eRO eLowerRelnOp = RO_NONE;
  eRO eUpperRelnOp = RO_NONE;
  
  if ( ! StrLib::bSame(szLowerKey->azChar(), "") ) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the lower relational OP
    eLowerRelnOp = pPathIdentifier->eGetFirstLowerPredicateRO(iPhrase);
  }
  
  if (eLowerRelnOp == RO_EQUAL_TO) {
    eLowerRelnOp = RO_GREATER_EQUAL;
    eUpperRelnOp = RO_LESS_EQUAL;
  }
  else
    if ( ! StrLib::bSame(szUpperKey->azChar(), "") ) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // get the lower relational OP
      eUpperRelnOp = pPathIdentifier->eGetFirstUpperPredicateRO(iPhrase);
    }
  
  pIndexStream->vSeek(eLowerRelnOp, szLowerKey, eUpperRelnOp, szUpperKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].ScanBegin -- begin indexTree Scan                            */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamScanBegin(void)
{
  ENTRY();

  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);

  pIndexStream->vScanBegin();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[]::ScanNew -- new a partition stream for indexTree Scan        */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamScanNew(void)
{
  ENTRY();

  IndexDef* pIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  IndexStream* pIndexStream = new IndexStream(pStreamDef, pIndexDef);

  m_pDataStack->vPushPtr(pIndexStream);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream::ScanNext -- get next buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamScanNext(void)
{
  ENTRY();

  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);

  RowBuffer* pRowBuffer = pIndexStream->pScanNext();

  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].Populate -- populate the indexTree of the indexStream        */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamScanPopulate(void)
{
  ENTRY();

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pRowBuffer->vStreamDef(pStreamDef);
  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);

  pIndexStream->vScanPopulate(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].PopulateTop -- populate indexTree with top limit entries     */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamScanPopulateTop(void)
{
  ENTRY();

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));
  i8 iTop = m_pDataStack->iPopInt(__func__);

  pRowBuffer->vStreamDef(pStreamDef);
  IndexStream* pIndexStream = _pIndexStream(*m_p_TaskLogicUnit->m_xpIndexStream);

  pIndexStream->vScanPopulateTop(pRowBuffer, iTop);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
