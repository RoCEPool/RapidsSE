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
            Stz* aComment,
            Stz* aCommand1,
            Stz* aCommand2,
            Stz* aSeed,
            bool bDetached,
            Job* (*fPre)(Prompt::SePrompt* pPrompt),
            void (*fPost)(Prompt::SePrompt* pPrompt),
            bool bEnabled=false);
  
    JobClass* pJobClass(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
