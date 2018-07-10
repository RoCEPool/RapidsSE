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
            bool (*pre)(TokenVec* pTokenVec),
            void (*post)(void));
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
