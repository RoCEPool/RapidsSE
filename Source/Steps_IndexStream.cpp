/*============================================================================*/
/* Steps_IndexStream.cpp - index stream step group                            */
/*============================================================================*/

#include "Steps.h"

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
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].close                                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamClose(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(*m_pRegSet->m_xpIndexStream);
  
  pIndexStream->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].delete                                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamDelete(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(*m_pRegSet->m_xpIndexStream);
  
  delete pIndexStream;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].deleteNext                                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamDeleteNext(void)
{
  ENTRY();
  
  IndexStream* pIndexStream = _pIndexStream(*m_pRegSet->m_xpIndexStream);
  
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
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpIndexStream);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexStream[].isNull -- is indexStream null ?                              */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpIndexStream.bIsNull() ? 1 : 0);
  
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
    
  IndexStream* pIndexStream = _pIndexStream(*m_pRegSet->m_xpIndexStream);
  
  bool bOpened = pIndexStream->bOpen();
  ASSERT(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].read                                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamRead(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(*m_pRegSet->m_xpIndexStream);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the rowOnce skipList
  RowOnce* pRowOnce = _pRowOnce(m_pDataStack->pPopPtr(__func__));
  
  CrwBuffer* pCrwBuffer = pIndexStream->pRead(pRowOnce);

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexStream[].set -- set partition stream                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamSet(void)
{
  ENTRY();
  
  m_pRegSet->m_xpIndexStream = _pIndexStream(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream[].seek                                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamSeek(void)
{
  ENTRY();
  
  IndexStream* pIndexStream = _pIndexStream(*m_pRegSet->m_xpIndexStream);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the planner
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the upper bound key
  Datum xUpperVariable;
  m_pDataStack->vPopVal(xUpperVariable,__func__);
  Stz* szUpperKey = xUpperVariable.szStz();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the lower bound key
  Datum xLowerVariable;
  m_pDataStack->vPopVal(xLowerVariable,__func__);
  Stz* szLowerKey = xLowerVariable.szStz();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the phrase
  i8 iPhrase = m_pDataStack->iPopInt(__func__);
  eRO eLowerRelnOp = RO_NONE;
  eRO eUpperRelnOp = RO_NONE;
  
  if ( ! StrLib::bSame(szLowerKey->azChar(), "") ) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the lower relational OP
    eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  }
  
  if (eLowerRelnOp == RO_EQUAL_TO) {
    eLowerRelnOp = RO_GREATER_EQUAL;
    eUpperRelnOp = RO_LESS_EQUAL;
  }
  else
    if ( ! StrLib::bSame(szUpperKey->azChar(), "") ) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // get the lower relational OP
      eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
    }
  
  pIndexStream->vSeek(eLowerRelnOp, szLowerKey, eUpperRelnOp, szUpperKey);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
