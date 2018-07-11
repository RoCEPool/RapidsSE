/*============================================================================*/
/* PromptFun.hpp -- a description of a prompt function                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class PromptFun
  {
  public:
    s32  m_sName;     // name of the prompt function
    s256 m_sComment;  // comment about the function
    s32  m_sCommand1; // first word of prompt command
    s32  m_sCommand2; // optional second word of prompt command
  
    bool (*m_fPrompt)(TokenVec* pTokenVec);
  
  public:
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
