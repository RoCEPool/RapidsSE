/*============================================================================*/
/* Steps_IndexStream.cpp - index stream step group                            */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexStream(void)
{
  vAddStep("indexStream[]"_Stz,          &Step::vIndexStreamGet);
  vAddStep("indexStream[].get"_Stz,      &Step::vIndexStreamGet);
  vAddStep("indexStream[].set"_Stz,      &Step::vIndexStreamSet);

  vAddStep("indexStream::new"_Stz,       &Step::vIndexStreamNew);
  
  vAddStep("indexStream.bufferNo"_Stz,   &Step::vIndexStreamBufferNo);
  vAddStep("indexStream.close"_Stz,      &Step::vIndexStreamClose);
  vAddStep("indexStream.delete"_Stz,     &Step::vIndexStreamDelete);
  vAddStep("indexStream.open"_Stz,       &Step::vIndexStreamOpen);
  vAddStep("indexStream.read"_Stz,       &Step::vIndexStreamRead);
  vAddStep("indexStream.seek"_Stz,       &Step::vIndexStreamSeek);
}

/*----------------------------------------------------------------------------*/
/* IndexStream::bufferNumber                                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamBufferNo(void)
{
  ENTRY();
  
  IndexStream* pIndexStream = _pIndexStream(m_pDataStack->pPopPtr(__func__));
  
  i8 iBufferNo = pIndexStream->iBufferNo();
  
  m_pDataStack->vPushInt(iBufferNo);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream::close                                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamClose(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(m_pDataStack->pPopPtr(__func__));
  
  pIndexStream->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream::delete                                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamDelete(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(m_pDataStack->pPopPtr(__func__));
  
  delete pIndexStream;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexStream[] -- get partition stream                                      */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpIndexStream);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream::new -- new a partition stream                                 */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamNew(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  IndexStream* pIndexStream = new IndexStream(pTablePtn);
  
  m_pDataStack->vPushPtr(pIndexStream);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream::open                                                          */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamOpen(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(m_pDataStack->pPopPtr(__func__));
  
  bool bOpened = pIndexStream->bOpen();
  
  m_pDataStack->vPushInt(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream::read                                                            */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamRead(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(m_pDataStack->pPopPtr(__func__));
  
  CrwBuffer* pCrwBuffer = pIndexStream->pRead();

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexStream::seek                                                            */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamSeek(void)
{
  ENTRY();
    
  IndexStream* pIndexStream = _pIndexStream(m_pDataStack->pPopPtr(__func__));
  
  i8 iBufferNo = m_pDataStack->iPopInt(__func__);

  bool bSeeked = pIndexStream->bSeek(iBufferNo);
  
  m_pDataStack->vPushInt(bSeeked);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexStream[].set -- set partition stream                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexStreamSet(void)
{
  ENTRY();
  
  m_pRegSet->m_xpIndexStream = _pIndexStream(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
