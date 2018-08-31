/*============================================================================*/
/* JobNote.cpp -- the class which populates the job catalog                   */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobNote
  {
  public:
    JobClass_o* m_pJobClass_o; // the job class created by this note
  
  public:
    JobNote(Stz* aName,
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
            );
  
    JobClass_o* pJobClass(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
