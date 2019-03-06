/*============================================================================*/
/* PromptNote.cpp -- the class which populates the prompt catalog             */
/*============================================================================*/
#pragma once

namespace Prompt
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class PromptNote
  {
  public:

  public:
    PromptNote(ePF  ePromptFun,
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
