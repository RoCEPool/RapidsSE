/*============================================================================*/
/* PromptCatalog.cpp -- the catalog of prompt functions                       */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
PromptFun* PromptCatalog::pFindFun(Stz* aCommand1, Stz* aCommand2)
{
  for (i8 i_Fun = 0; i_Fun < m_nFunctions; i_Fun++) {
    
    //vTrace("Find: %s v %s\n"_Stz,m_xPromptFun[i_Fun].m_aCommand1->aChar(),
    //       aCommand1->aChar());
    
    if ( ! m_xPromptFun[i_Fun].m_aCommand1->bSame(aCommand1) )
      continue;
    
    if (m_xPromptFun[i_Fun].m_aCommand2) {
      if ( !aCommand2->iLength())
        continue;
      if ( ! m_xPromptFun[i_Fun].m_aCommand2->bSame(aCommand2) )
        continue;
    }
    
    if (m_xPromptFun[i_Fun].m_bEnabled)
      return &m_xPromptFun[i_Fun];
    else
      return nullptr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void PromptCatalog::vFuns(ePF ePromptFun)
{
  for (i8 i_Fun = 0; i_Fun < m_nFunctions; i_Fun++) {
    
    if (m_xPromptFun[i_Fun].m_ePromptFun != ePromptFun)
      continue;

    if (m_xPromptFun[i_Fun].m_aHelp)
    vTrace("%s\n"_Stz,m_xPromptFun[i_Fun].m_aHelp);
  }

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
