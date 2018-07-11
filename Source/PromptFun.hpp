/*============================================================================*/
/* PromptFun.hpp -- a description of a prompt function                        */
/*============================================================================*/
#pragma once

namespace Prompt
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class PromptFun
  {
  public:
    Stz* m_aName;     // name of the prompt function
    Stz* m_aComment;  // comment about the function
    Stz* m_aCommand1; // first word of prompt command
    Stz* m_aCommand2; // optional second word of prompt command
  
    void (*m_fPrompt)(SePrompt* pPrompt);
  
  public:
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
