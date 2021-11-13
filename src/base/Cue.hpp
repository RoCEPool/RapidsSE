/*============================================================================*/
/* Cue.hpp -- Cue (litle prompt)                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Cue
  {
  public:
    static void vGetInput(Stz* szPrompt, String* pString);
    static void vGetReturn(void);
    static void vGetReturn(Stz* szPrompt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
