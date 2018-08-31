/*============================================================================*/
/* JobNote.cpp -- the class which populates the job catalog                   */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

extern JobCatalog gxJobCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobNote::JobNote(Stz* aName,
                 eCN  eClass,
                 eJK  eKind,
                 Stz* aComment,
                 Stz* aCommand1,
                 Stz* aCommand2,
                 Stz* aSeedRun,
                 Stz* aSeedReq,
                 Stz* aSeedRcv,
                 Stz* aSeedExe,
                 Stz* aSeedEnd,
                 Stz* aSeedCan,
                 bool bResources,
                 bool bDetached,
                 bool bBuffer,
                 eJT  eTrace,
                 Job* (*fPre)(Interface::SEitf* pSEitf),
                 void (*fPost)(Interface::SEitf* pSEitf, Job* pJob),
                 Job* (*fNew)(Job* pArg),
                 void (*fDelete)(Interface::SEitf* pSEitf, Job* pJob),
                 void (*fSend)(Job* pJob),
                 void (*fRecv)(Job* pJob),
                 void (*fShow)(Job* pJob),
                 bool bEnabled,
                 Stz* aHelp,
                 Stz* aExample
                 )
{
  //vTrace("JobNote: %s %s %s %s\n"_Stz,
  //       aName, aComment, aCommand1, aSeedReq);
  
  // add the job class to the catalog
  i8 i_Job = gxJobCatalog.m_nClasses;
  
  if (aName)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aName,aName->aChar());
  
  gxJobCatalog.m_xJobClass[i_Job].m_eClass      = eClass;
  gxJobCatalog.m_xJobClass[i_Job].m_eKind       = eKind;
  
  if (aComment)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aComment,aComment->aChar());
  
  if (aCommand1)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aCommand1,aCommand1->aChar());
  
  if (aCommand2)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aCommand2,aCommand2->aChar());
  
  if (aSeedRun)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aSeedRun,aSeedRun->aChar());
  
  if (aSeedReq)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aSeedReq,aSeedReq->aChar());
  
  if (aSeedRcv)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aSeedRcv,aSeedRcv->aChar());
  
  if (aSeedExe)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aSeedExe,aSeedExe->aChar());
  
  if (aSeedEnd)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aSeedEnd,aSeedEnd->aChar());
  
  if (aSeedCan)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aSeedCan,aSeedCan->aChar());
  
  gxJobCatalog.m_xJobClass[i_Job].m_bResources  = bResources;
  gxJobCatalog.m_xJobClass[i_Job].m_bDetached   = bDetached;
  gxJobCatalog.m_xJobClass[i_Job].m_bBuffer     = bBuffer;
  gxJobCatalog.m_xJobClass[i_Job].m_eTrace      = eTrace;
  gxJobCatalog.m_xJobClass[i_Job].m_fPre        = fPre;
  gxJobCatalog.m_xJobClass[i_Job].m_fPost       = fPost;
  gxJobCatalog.m_xJobClass[i_Job].m_fNew        = fNew;
  gxJobCatalog.m_xJobClass[i_Job].m_fDelete     = fDelete;
  gxJobCatalog.m_xJobClass[i_Job].m_fSend       = fSend;
  gxJobCatalog.m_xJobClass[i_Job].m_fRecv       = fRecv;
  gxJobCatalog.m_xJobClass[i_Job].m_fShow       = fShow;
  gxJobCatalog.m_xJobClass[i_Job].m_bEnabled    = bEnabled;
  
  if (aHelp)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aHelp,aHelp->aChar());
  
  if (aExample)
    StrLib::vCopy(gxJobCatalog.m_xJobClass[i_Job].m_aExample,aExample->aChar());
  
  // record the job class
  m_pJobClass_o = &gxJobCatalog.m_xJobClass[i_Job];

  // we have one more class in the job catalog
  gxJobCatalog.m_nClasses++;
}

/*----------------------------------------------------------------------------*/
/* get job class                                                              */
/*----------------------------------------------------------------------------*/
JobClass_o* JobNote::pJobClass(void)
{
  return m_pJobClass_o;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
