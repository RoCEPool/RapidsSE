/*============================================================================*/
/* Steps_IndexTreeRlu.cpp - indexTreeRlu[] step group                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexTreeRlu(void)
{
  vAddStep("IndexTreeRlu[]"_Stz,           &Step::vIndexTreeRluGet);
  vAddStep("IndexTreeRlu[].asAlt"_Stz,     &Step::vIndexTreeRluAsAlt);
  vAddStep("IndexTreeRlu[].asSrc"_Stz,     &Step::vIndexTreeRluAsSrc);
  vAddStep("IndexTreeRlu[].asTmp"_Stz,     &Step::vIndexTreeRluAsTmp);
  vAddStep("IndexTreeRlu[].delete"_Stz,    &Step::vIndexTreeRluDelete);
  vAddStep("IndexTreeRlu[].isNull"_Stz,    &Step::vIndexTreeRluIsNull);
  vAddStep("IndexTreeRlu[].new"_Stz,       &Step::vIndexTreeRluNew);
  vAddStep("IndexTreeRlu[].set"_Stz,       &Step::vIndexTreeRluSet);
  
  vAddStep("IndexTree.insEntBuffer"_Stz,    &Step::vIndexTreeRluInsEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* IndexTreeRlu[].asAlt -- set this index tree as m_pAltIdxTree               */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRluAsAlt(void)
{
  ENTRY();

  IndexTree* pIndexTree = _pIndexTree(m_pLogicUnit->m_xRluCommon.m_pIndexTree);

  m_pLogicUnit->m_pDevAltIxt->m_pIdxTree = pIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTreeRlu[].asSrc -- set this index tree as m_pSrcIdxTree               */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRluAsSrc(void)
{
  ENTRY();

  IndexTree* pIndexTree = _pIndexTree(m_pLogicUnit->m_xRluCommon.m_pIndexTree);

  m_pLogicUnit->m_pDevLftIxt->m_pIdxTree = pIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTreeRlu[].asTmp -- set this index tree as m_pIndexTree                */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRluAsTmp(void)
{
  ENTRY();

  IndexTree* pIndexTree = _pIndexTree(m_pLogicUnit->m_xRluCommon.m_pIndexTree);

  m_pLogicUnit->m_pDevTmpIxt->m_pIdxTree = pIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTreeRlu[].delete -- delete index tree                                 */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRluDelete(void)
{
  ENTRY();

  IndexTree* pIndexTree = _pIndexTree(m_pLogicUnit->m_xRluCommon.m_pIndexTree);

  m_pLogicUnit->m_xRluCommon.m_pIndexTree = nullptr;
  
  delete pIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTreeRlu[].get -- get index tree                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRluGet(void)
{
  ENTRY();

  IndexTree* pIndexTree = _pIndexTree(m_pLogicUnit->m_xRluCommon.m_pIndexTree);

  m_pDataStack->vPushPtr(pIndexTree);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTreeRlu[].isNull -- is indexTree null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRluIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xRluCommon.m_pIndexTree ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTreeRlu[].new -- new a index tree                                     */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRluNew(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));
  m_pLogicUnit->m_xRluCommon.m_pIndexTree = new IndexTree(nullptr, pIndexDef, C_UNKNOWN);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTreeRlu[].set -- set index tree                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRluSet(void)
{
  ENTRY();

  m_pLogicUnit->m_xRluCommon.m_pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree.insEntBuffer -- insert all entries from given ent buffer         */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRluInsEntBuffer(void)
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
    UNUSED(bInserted);

    nEntries++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushInt(nEntries);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
