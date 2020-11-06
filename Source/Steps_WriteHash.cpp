/*============================================================================*/
/* Steps_WriteHash.cpp - Write Row Hasher                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_WriteHash(void)
{
  vAddStep("CrwHash[].create"_Stz,         &Step::vWriteHashCreate);
  vAddStep("CrwHash[].crwBuffer@"_Stz,     &Step::vWriteHashCrwBufferFetch);
  vAddStep("CrwHash[].crwBuffer!"_Stz,     &Step::vWriteHashCrwBufferStore);
  vAddStep("CrwHash[].delete"_Stz,         &Step::vWriteHashDelete);
  vAddStep("CrwHash[].fullCrwBuffer@"_Stz, &Step::vWriteHashFullCrwBufferFetch);
}

/*----------------------------------------------------------------------------*/
/* CrwHash[].create -- create the CrwHash                                     */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashCreate(void)
{
  ENTRY();
  
  CrwHash* pWriteHash = new CrwHash(*m_pLogicUnit->m_xpTable);
  
  m_pLogicUnit->m_xpCrwHash = pWriteHash;
}

/*----------------------------------------------------------------------------*/
/* CrwHash[].delete -- delete the CrwHash                                     */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashDelete(void)
{
  ENTRY();
  
  CrwHash* pWriteHash = *m_pLogicUnit->m_xpCrwHash;
  
  delete pWriteHash;
  
  m_pLogicUnit->m_xpCrwHash = nullptr;
}

/*----------------------------------------------------------------------------*/
/* CrwHash[].crwBuffer@ -- get crwbuffer for given partition                  */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashCrwBufferFetch(void)
{
  ENTRY();
  
  tPN tPtnNo = m_pDataStack->iPopInt(__func__);
  
  CrwHash* pWriteHash = *m_pLogicUnit->m_xpCrwHash;

  m_pDataStack->vPushPtr(pWriteHash->pCrwBuffer(tPtnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwHash[].crwBuffer! -- set crwbuffer for partition                        */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashCrwBufferStore(void)
{
  ENTRY();
  
  tPN tPtnNo = m_pDataStack->iPopInt(__func__);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));
  
  CrwHash* pWriteHash = *m_pLogicUnit->m_xpCrwHash;

  pWriteHash->vCrwBuffer(tPtnNo, pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CrwHash[].FullCrwBuffer@ -- get full crwbuffer for partition               */
/*----------------------------------------------------------------------------*/
void Step::vWriteHashFullCrwBufferFetch(void)
{
  ENTRY();
  
  tPN tPtnNo = m_pDataStack->iPopInt(__func__);

  CrwHash* pWriteHash = *m_pLogicUnit->m_xpCrwHash;

  CrwBuffer* pCrwBuffer = pWriteHash->pFullCrwBuffer(tPtnNo);
  
  m_pDataStack->vPushPtr(pCrwBuffer);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
