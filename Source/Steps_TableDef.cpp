/*============================================================================*/
/* Steps_TableDef.cpp - table definition step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TableDef(void)
{
  vAddStep("tableDef::parse"_Stz, &Step::vTableDefParse);
}

/*----------------------------------------------------------------------------*/
/* tableDef::parse -- parse Create Table statement and produce a TableDef     */
/*----------------------------------------------------------------------------*/
void Step::vTableDefParse(void)
{
  ENTRY(); // ENTRY("vTableDefParse"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopPtr(pJobPtr) ) THROW_DSTACK();
  Job* pJob_CreateTable = _pJob(pJobPtr);
  
  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  //vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);

  TableDef* pTableDef = new TableDef;
  
  eEC eResult = pTableDef->eParse(sCmd);
  
  if (eResult != EC_NONE) {
    
    // set the error code in the job
    pJob_CreateTable->m_bHasError = true;
    pJob_CreateTable->m_xResponse.m_eErrorCode = eResult;
    
    // delete tabledef
    delete pTableDef;
    
    pTableDef = nullptr;
  }
  
  m_pDataStack->vPushPtr(pTableDef);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
