/*============================================================================*/
/* PromptDesc.cpp -- the class which populates the prompt catalog             */
/*============================================================================*/

#include "Prompt.h"
#include "AM.hpp"

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PromptDesc::PromptDesc(ePF  ePromptFun,
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
  //vTrace("PromptDesc: %s %s %s\n"_Stz,
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
  
  // add the prompt class to the catalog
  i8 i_Fun = gxPromptCatalog.m_nFunctions;
  
  gxPromptCatalog.m_aaDesc[i_Fun] = this;
  
  // we have one more class in the prompt catalog
  gxPromptCatalog.m_nFunctions++;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
