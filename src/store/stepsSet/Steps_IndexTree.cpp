/*============================================================================*/
/* Steps_IndexTree.cpp - indexTree[] step group                                */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexTree(void)
{
  vAddStep("IndexTree[]"_Stz,           &Step::vIndexTreeGet);
  vAddStep("IndexTree[].asAlt"_Stz,     &Step::vIndexTreeAsAlt);
  vAddStep("IndexTree[].asSrc"_Stz,     &Step::vIndexTreeAsSrc);
  vAddStep("IndexTree[].asTmp"_Stz,     &Step::vIndexTreeAsTmp);
  vAddStep("IndexTree[].delete"_Stz,    &Step::vIndexTreeDelete);
  vAddStep("IndexTree[].isNull"_Stz,    &Step::vIndexTreeIsNull);
  vAddStep("IndexTree[].new"_Stz,       &Step::vIndexTreeNew);
  vAddStep("IndexTree[].set"_Stz,       &Step::vIndexTreeSet);
  
  vAddStep("IndexTree.insEntBuffer"_Stz,    &Step::vIndexTreeInsEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].asAlt -- set this index tree as m_pAltIdxTree                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeAsAlt(void)
{
  ENTRY();
  
  ( m_pJob->pLogicUnit_job() )->m_pDevAltIxt->m_pIdxTree = * ( m_pJob->pLogicUnit_job() )->m_xpIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].asSrc -- set this index tree as m_pSrcIdxTree                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeAsSrc(void)
{
  ENTRY();
  
  ( m_pJob->pLogicUnit_job() )->m_pDevSrcIxt->m_pIdxTree = * ( m_pJob->pLogicUnit_job() )->m_xpIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].asTmp -- set this index tree as m_pIndexTree                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeAsTmp(void)
{
  ENTRY();
  
  ( m_pJob->pLogicUnit_job() )->m_pDevTmpIxt->m_pIdxTree = * ( m_pJob->pLogicUnit_job() )->m_xpIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].delete -- delete index tree                                    */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeDelete(void)
{
  ENTRY();
  
  IndexTree* pIndexTree = * ( m_pJob->pLogicUnit_job() )->m_xpIndexTree;
  
  ( m_pJob->pLogicUnit_job() )->m_xpIndexTree = nullptr;
  
  delete pIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].get -- get index tree                                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpIndexTree);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].isNull -- is indexTree null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(( m_pJob->pLogicUnit_job() )->m_xpIndexTree.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].new -- new a index tree                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeNew(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));
  ( m_pJob->pLogicUnit_job() )->m_xpIndexTree = new IndexTree(nullptr, pIndexDef, C_UNKNOWN);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].set -- set index tree                                          */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeSet(void)
{
  ENTRY();
  
  ( m_pJob->pLogicUnit_job() )->m_xpIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree.insEntBuffer -- insert all entries from given ent buffer          */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeInsEntBuffer(void)
{
  ENTRY();
  
  IndexTree* pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));
  
  EntBuffer* pEntBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nEntries = 0;
  
  for (Ent* aEnt = pEntBuffer->aFirstItem();
       aEnt;
       aEnt = pEntBuffer->aNextEntry(aEnt,0)) {
    
    // ..add code here to insert entry into index tree..
    
    bool bInserted = pIndexTree->bInsertEntry(aEnt);
    
    nEntries++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushInt(nEntries);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
