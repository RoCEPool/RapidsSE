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
    JobClass* m_pJobClass; // the job class created by this note
  
  public:
    JobNote(Stz* aName,
            eCN  eClass,
            eJK  eKind,
            Stz* aComment,
            Stz* aCommand1,
            Stz* aCommand2,
            Stz* aSeed,
            bool bResources,
            bool bDetached,
            eJT  eTrace,

            Job* (*fPre)(Interface::SEitf* pSEitf),
            void (*fPost)(Interface::SEitf* pSEitf, Job* pJob),
            Job* (*fNew)(Job* pArg),
            void (*fDelete)(Interface::SEitf* pSEitf, Job* pJob),
            void (*fRecv)(Job* pJob),
            void (*fShow)(Job* pJob),

            bool bEnabled,
            Stz* aHelp,
            Stz* aExample
            );
  
    JobClass* pJobClass(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
