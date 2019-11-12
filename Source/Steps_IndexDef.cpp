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
  ENTRY("vIndexDefParse"_Stz);
  
  s4K sCmd;
  if (! m_pDataStack->bPopS4K(sCmd))
    THROW_DSTACK();

  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);

  IndexDef* pIndexDef = new IndexDef;
  
  eEC eResult = pIndexDef->eParse(sCmd);
  
  delete pIndexDef;
  pIndexDef = nullptr;
  
  m_pDataStack->vPushPointer(pIndexDef);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
