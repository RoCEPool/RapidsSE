/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"
#include "ePX.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_WriteBehind(void)
{
  vAddStep("WriteBehind[].create"_Stz,   &Step::vWriteBehindCreate);
  vAddStep("WriteBehind[].get"_Stz,      &Step::vWriteBehindGet);
  vAddStep("WriteBehind[].set"_Stz,      &Step::vWriteBehindset);
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].create -- create the writeBehind                             */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindCreate(void)
{
  ENTRY(); // ENTRY("vWriteBehindGet"_Stz);
  
  m_pRegSet->m_xpWriteBehind = new WriteBehind(DS_INSERT);
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].get -- get the writeBehind                                   */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindGet(void)
{
  ENTRY(); // ENTRY("vWriteBehindGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpWriteBehind);
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].set -- set the writeBehind                                   */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindset(void)
{
  ENTRY(); // ENTRY("vWriteBehindset"_Stz);
  
  m_pRegSet->m_xpWriteBehind = _pWriteBehind(m_pDataStack->pPopPtr(__func__));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
