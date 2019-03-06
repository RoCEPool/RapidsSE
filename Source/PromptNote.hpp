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
    PromptNote(Stz* aName,
               Stz* aComment,
               Stz* aCommand1,
               Stz* aCommand2,
               bool (*fPrompt)(SePrompt* pPrompt),
               bool bEnabled,
               
               bool bVisible,
               Stz* aHelp
              );
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
