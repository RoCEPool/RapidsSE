/*============================================================================*/
/* Prompt_markApi.cpp -- the mark api option                                  */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_markApi (
  "Prompt_markApi"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "markApi"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_markApi,   // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

extern PromptCatalog gxPromptCatalog;
extern AttachMap*    gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markApi(SePrompt* pPrompt)
{
  i8 nTokens = pPrompt->pTknVec()->nTokens();

  Stz* aTkn1 = nullptr;

  if (nTokens == 1) {
    for (i8 i_Api = 0; i_Api < AC_ENUMS; i_Api++)
      if (gpAttachMap->m_bMarkApi[i_Api])
        vTrace("%s : %s\n"_Stz,
               aAC[i_Api],
               gpAttachMap->m_bMarkApi[i_Api] ? "on"_Stz : "off"_Stz);
    return true;
  }
  
  if (nTokens == 2) {
    aTkn1 = pPrompt->pTknVec()->pGet(1)->aText();
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
