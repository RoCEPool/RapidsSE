/*============================================================================*/
/* PromptDesc.cpp -- the class which populates the prompt catalog             */
/*============================================================================*/
#pragma once

namespace Prompt
{
  class PromptDesc
  {
  public:

  public:
    PromptDesc(ePF  ePromptFun,
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
