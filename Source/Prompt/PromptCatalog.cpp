/*============================================================================*/
/* PromptCatalog.cpp -- the catalog of prompt functions                       */
/*============================================================================*/

#include "Prompt.h"

PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
PromptFun* PromptCatalog::pFindFun(Stz* aCommand1, Stz* aCommand2)
{
  for (i8 i_Fun = 0; i_Fun < m_nFunctions; i_Fun++) {
    
    //vTrace("Find: %s v %s\n"_Stz, m_aaFun[i_Fun]->m_aCommand1->azChar(),
    //       aCommand1->azChar());
    
    if (! m_aaFun[i_Fun]->m_aCommand1->bSame(aCommand1))
      continue;
    
    if (m_aaFun[i_Fun]->m_aCommand2) {
      if (! aCommand2->nLength())
        continue;
      if (! m_aaFun[i_Fun]->m_aCommand2->bSame(aCommand2))
        continue;
    }
    
    if (m_aaFun[i_Fun]->m_bEnabled)
      return m_aaFun[i_Fun];
    else
      return nullptr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void PromptCatalog::vFuns(ePF ePromptFun)
{
  
  for (i8 i_Fun = 0; i_Fun < m_nFunctions; i_Fun++) {
    
    if (m_aaFun[i_Fun]->m_ePromptFun != ePromptFun)
      continue;
    
    if (m_aaFun[i_Fun]->m_aHelp)
      vTrace("row: ('%s')\n"_Stz,
             m_aaFun[i_Fun]->m_aHelp);
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
