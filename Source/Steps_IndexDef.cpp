/*============================================================================*/
/* Steps_IndexDef.cpp - index definition step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexDef(void)
{
  vAddStep("indexDef::parse"_Stz, &Step::vIndexDefParse);
}

/*----------------------------------------------------------------------------*/
/* indexDef::parse -- parse Create Index statement and produce a IndexDef     */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefParse(void)
{
  ENTRY(); // ENTRY("vIndexDefParse"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job* pJob_CreateIndex = _pJob(pJobPtr);
  
  Object* pPtrTable; m_pDataStack->vPopPtr(pPtrTable,__func__);
  Table* pTable = _pTable(pPtrTable);

  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);

  IndexDef* pIndexDef = new IndexDef;
  
  eEC eResult = pIndexDef->eParse(pTable,sCmd);
  
  if (eResult != EC_NONE) {
    
    // set the error code in the job
    pJob_CreateIndex->m_bHasError = true;
    pJob_CreateIndex->m_xResponse.m_eErrorCode = eResult;
    
    // delete IndexDef
    delete pIndexDef;
    
    pIndexDef = nullptr;
  }
  
  m_pDataStack->vPushPtr(pIndexDef);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
