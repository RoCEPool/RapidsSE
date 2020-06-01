/*============================================================================*/
/* Steps_WriteHash.cpp - Write Row Hasher                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_WriteHash(void)
{
  vAddStep("WriteHash[].create"_Stz,         &Step::vWriteHashCreate);
  vAddStep("WriteHash[].delete"_Stz,         &Step::vWriteHashDelete);
}

/*----------------------------------------------------------------------------*/
/* WriteHash[].create -- create the writeHash                                 */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashCreate(void)
{
  ENTRY();
  
  WriteHash* pWriteHash = new WriteHash(*m_pRegSet->m_xpTable);
  
  m_pRegSet->m_xpWriteHash = pWriteHash;
}

/*----------------------------------------------------------------------------*/
/* WriteHash[].delete -- delete the writeHash                                 */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashDelete(void)
{
  ENTRY();
  
  WriteHash* pWriteHash = *m_pRegSet->m_xpWriteHash;
  
  delete pWriteHash;
  
  m_pRegSet->m_xpWriteHash = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
