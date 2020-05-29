/*============================================================================*/
/* PromptDefn.hpp -- a definition of a prompt function                        */
/*============================================================================*/
#pragma once

namespace Prompt
{
  class PromptDefn
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
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
