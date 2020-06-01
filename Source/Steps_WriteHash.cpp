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
  vAddStep("WriteHash[].crwBuffer@"_Stz,     &Step::vWriteHashCrwBufferFetch);
  vAddStep("WriteHash[].crwBuffer!"_Stz,     &Step::vWriteHashCrwBufferStore);
  vAddStep("WriteHash[].delete"_Stz,         &Step::vWriteHashDelete);
  vAddStep("WriteHash[].fullCrwBuffer@"_Stz, &Step::vWriteHashFullCrwBufferFetch);
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

/*----------------------------------------------------------------------------*/
/* WriteHash[].crwBuffer@ -- get crwbuffer for given partition                */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashCrwBufferFetch(void)
{
  ENTRY();
  
  i8 iPartition = m_pDataStack->iPopInt(__func__);
  
  WriteHash* pWriteHash = *m_pRegSet->m_xpWriteHash;

  m_pDataStack->vPushPtr(pWriteHash->pCrwBuffer(iPartition));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* WriteHash[].crwBuffer! -- set crwbuffer for partition                      */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashCrwBufferStore(void)
{
  ENTRY();
  
  i8 iPartition = m_pDataStack->iPopInt(__func__);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));
  
  WriteHash* pWriteHash = *m_pRegSet->m_xpWriteHash;

  pWriteHash->vCrwBuffer(iPartition, pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* WriteHash[].FullCrwBuffer@ -- get full crwbuffer for partition             */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashFullCrwBufferFetch(void)
{
  ENTRY();
  
  i8 iPartition = m_pDataStack->iPopInt(__func__);

  WriteHash* pWriteHash = *m_pRegSet->m_xpWriteHash;

  CrwBuffer* pCrwBuffer = pWriteHash->pFullCrwBuffer(iPartition);
  
  m_pDataStack->vPushPtr(pCrwBuffer);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
