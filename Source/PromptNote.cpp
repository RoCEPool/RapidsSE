/*============================================================================*/
/* PromptNote.cpp -- the class which populates the prompt catalog             */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PromptNote::PromptNote(ePF  ePromptFun,
                       Stz* aName,
                       Stz* aComment,
                       Stz* aCommand1,
                       Stz* aCommand2,
                       bool (*fPrompt)(SePrompt* pPrompt),
                       bool bEnabled,
                       
                       bool bVisible,
                       Stz* aHelp)
{
  //vTrace("PromptNote: %s %s %s\n"_Stz,
  //       aName, aComment, aCommand1);
  
  // add the prompt class to the catalog
  i8 i_Prompt = gxPromptCatalog.m_nFunctions;
  
  gxPromptCatalog.m_xPromptFun[i_Prompt].m_aName     = aName;
  gxPromptCatalog.m_xPromptFun[i_Prompt].m_aComment  = aComment;
  gxPromptCatalog.m_xPromptFun[i_Prompt].m_aCommand1 = aCommand1;
  if (aCommand2)
    gxPromptCatalog.m_xPromptFun[i_Prompt].m_aCommand2 = aCommand2;
  gxPromptCatalog.m_xPromptFun[i_Prompt].m_fPrompt     = fPrompt;
  gxPromptCatalog.m_xPromptFun[i_Prompt].m_bEnabled    = bEnabled;
  gxPromptCatalog.m_xPromptFun[i_Prompt].m_bVisible    = bVisible;

  gxPromptCatalog.m_xPromptFun[i_Prompt].m_aHelp = aHelp;

  // we have one more class in the prompt catalog
  gxPromptCatalog.m_nFunctions++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
