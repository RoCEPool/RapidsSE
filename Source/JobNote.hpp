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

  public:
    JobNote(Stz* aName,
            Stz* aComment,
            Stz* aCommand1,
            Stz* aCommand2,
            Stz* aSeed,
            Job* (*fPre)(SePrompt* pPrompt),
            void (*fPost)(SePrompt* pPrompt),
            bool bEnabled=false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
