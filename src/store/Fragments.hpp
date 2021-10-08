/*============================================================================*/
/* Fragments.hpp -- Code fragments utility                                    */
/*============================================================================*/
#pragma once

namespace STORE
{

  //----------------------------------------------------------------------------

  class Fragments
  {
  private:

  public:

    static void vAppendConcluding  (char *azStr, i8* piLineNumber);
    static void vAppendCrwIteration(char *azStr, i8* piLineNumber);
    static void vAppendPreliminary (char *azStr, i8* piLineNumber);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
