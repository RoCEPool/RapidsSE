/*============================================================================*/
/* PromptCatalog.hpp -- the catalog of prompt functions                       */
/*============================================================================*/
#pragma once

namespace Prompt
{
  class PromptNote;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class PromptCatalog
  {
  public:
    i8        m_nFunctions;      // number of prompt functions
    PromptFun m_xPromptFun[128]; // array of prompt function descriptions
  
  public:
    PromptFun* pFindFun(Stz* aCommand1, Stz* aCommand2);
    void vFuns(PromptNote* aPromptNote);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
