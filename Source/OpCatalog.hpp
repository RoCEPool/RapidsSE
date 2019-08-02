/*============================================================================*/
/* PromptCatalog.hpp -- the catalog of prompt functions                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class OpCatalog
  {
  public:
    i8      m_nOpDefns;     // number of op definitions
    OpDefn  m_xOpDefn[128]; // array of op definitions
  
  public:
    //PromptFun* pFindFun(Stz* aCommand1, Stz* aCommand2);
    //void vFuns(ePF ePromptFun);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
