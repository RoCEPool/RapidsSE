/*============================================================================*/
/* PromptFun.cpp -- description of a prompt function                          */
/*============================================================================*/

#include "StorageEngine.h"

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PromptFun::PromptFun(ePF  ePromptFun,
                     Stz* aName,
                     Stz* aComment,
                     Stz* aCommand1,
                     Stz* aCommand2,
                     bool (*fPrompt)(SePrompt* pPrompt),
                     bool bEnabled,
                     bool bVisible,
                     Stz* aHelp,
                     Stz* aDesc)
{
  //vTrace("PromptFun: %s %s %s\n"_Stz,
  //       aName, aComment, aCommand1);
  
  m_ePromptFun  = ePromptFun;
  m_aName       = aName;
  m_aComment    = aComment;
  m_aCommand1   = aCommand1;
  m_aCommand2   = aCommand2;
  m_fPrompt     = fPrompt;
  m_bEnabled    = bEnabled;
  m_bVisible    = bVisible;
  m_aHelp       = aHelp;
  m_aDesc       = aDesc;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // add the prompt function to the catalog
  i8 i_Fun = gxPromptCatalog.m_nFunctions++;
  gxPromptCatalog.m_aaFun[i_Fun] = this;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
