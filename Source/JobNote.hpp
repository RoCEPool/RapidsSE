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
            Stz* aCommand,
            Stz* aSeed,
            bool (*fPre)(TokenVec* pTokenVec),
            void (*fPost)(void));
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
