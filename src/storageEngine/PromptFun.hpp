/*============================================================================*/
/* PromptFun.hpp -- description of a prompt function                          */
/*============================================================================*/
#pragma once

#include "ePF.h"

namespace Prompt
{
  class PromptFun
  {
  public:
    ePF  m_ePromptFun;   // prompt function
    Stz* m_aName;        // name of the prompt function
    Stz* m_aComment;     // comment about the function
    Stz* m_aCommand1;    // first word of prompt command
    Stz* m_aCommand2;    // optional second word of prompt command
  
    bool (*m_fPrompt)(SePrompt* pPrompt);
 
    bool m_bEnabled;     // is this function enabled
    bool m_bVisible;     // is this function visible
    Stz* m_aHelp;        // help about the function
    Stz* m_aDesc;        // description

  public:
    PromptFun(ePF  ePromptFun,
               Stz* aName,
               Stz* aComment,
               Stz* aCommand1,
               Stz* aCommand2,
               
               bool (*fPrompt)(SePrompt* pPrompt),
               
               bool bEnabled,
               bool bVisible,
               Stz* aHelp,
               Stz* aDescription
              );
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
