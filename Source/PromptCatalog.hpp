/*============================================================================*/
/* PromptCatalog.hpp -- the catalog of prompt functions                       */
/*============================================================================*/
#pragma once

namespace Prompt
{
  class PromptDesc;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class PromptCatalog
  {
  public:
    i8          m_nFunctions;       // number of prompt functions
    PromptDesc* m_aaDesc[128];      // array of prompt function descriptions

  public:
    PromptDesc* pFindFun(Stz* aCommand1, Stz* aCommand2);
    void vFuns(ePF ePromptFun);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
