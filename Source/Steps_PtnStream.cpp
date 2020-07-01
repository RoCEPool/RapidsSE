/*============================================================================*/
/* Steps_PtnStream.cpp - partition stream step group                          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PtnStream(void)
{
  vAddStep("ptnStream[]"_Stz,           &Step::vPtnStreamGet);
  vAddStep("ptnStream[].get"_Stz,       &Step::vPtnStreamGet);
  vAddStep("ptnStream[].set"_Stz,       &Step::vPtnStreamSet);

  vAddStep("ptnStream::new"_Stz,        &Step::vPtnStreamNew);
  
  vAddStep("ptnStream.bufferNo"_Stz,    &Step::vPtnStreamBufferNo);
  vAddStep("ptnStream.close"_Stz,       &Step::vPtnStreamClose);
  vAddStep("ptnStream.delete"_Stz,      &Step::vPtnStreamDelete);
  vAddStep("ptnStream.open"_Stz,        &Step::vPtnStreamOpen);
  vAddStep("ptnStream.read"_Stz,        &Step::vPtnStreamRead);
  vAddStep("ptnStream.seek"_Stz,        &Step::vPtnStreamSeek);
}

/*----------------------------------------------------------------------------*/
/* ptnStream::bufferNumber                                                    */
/*----------------------------------------------------------------------------*/
void Step::vPtnStreamBufferNo(void)
{
  ENTRY();
  
  PtnStream* pPtnStream = _pPtnStream(m_pDataStack->pPopPtr(__func__));
  
  i8 iBufferNo = pPtnStream->iBufferNo();
  
  m_pDataStack->vPushInt(iBufferNo);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnStream::close                                                           */
/*----------------------------------------------------------------------------*/
void Step::vPtnStreamClose(void)
{
  ENTRY();
    
  PtnStream* pPtnStream = _pPtnStream(m_pDataStack->pPopPtr(__func__));
  
  pPtnStream->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnStream::delete                                                          */
/*----------------------------------------------------------------------------*/
void Step::vPtnStreamDelete(void)
{
  ENTRY();
    
  PtnStream* pPtnStream = _pPtnStream(m_pDataStack->pPopPtr(__func__));
  
  delete pPtnStream;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnStream[] -- get partition stream                                        */
/*----------------------------------------------------------------------------*/
void Step::vPtnStreamGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpPtnStream);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnStream::new -- new a partition stream                                   */
/*----------------------------------------------------------------------------*/
void Step::vPtnStreamNew(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  PtnStream* pPtnStream = new PtnStream(pTablePtn);
  
  m_pDataStack->vPushPtr(pPtnStream);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnStream::open                                                            */
/*----------------------------------------------------------------------------*/
void Step::vPtnStreamOpen(void)
{
  ENTRY();
    
  PtnStream* pPtnStream = _pPtnStream(m_pDataStack->pPopPtr(__func__));
  
  bool bOpened = pPtnStream->bOpen();
  
  m_pDataStack->vPushInt(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnStream::read                                                            */
/*----------------------------------------------------------------------------*/
void Step::vPtnStreamRead(void)
{
  ENTRY();
    
  PtnStream* pPtnStream = _pPtnStream(m_pDataStack->pPopPtr(__func__));
  
  CrwBuffer* pCrwBuffer = pPtnStream->pRead();

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnStream::seek                                                            */
/*----------------------------------------------------------------------------*/
void Step::vPtnStreamSeek(void)
{
  ENTRY();
    
  PtnStream* pPtnStream = _pPtnStream(m_pDataStack->pPopPtr(__func__));
  
  i8 iBufferNo = m_pDataStack->iPopInt(__func__);

  bool bSeeked = pPtnStream->bSeek(iBufferNo);
  
  m_pDataStack->vPushInt(bSeeked);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ptnStream[].set -- set partition stream                                    */
/*----------------------------------------------------------------------------*/
void Step::vPtnStreamSet(void)
{
  ENTRY();
  
  m_pRegSet->m_xpPtnStream = _pPtnStream(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
