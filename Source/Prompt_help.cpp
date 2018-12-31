/*============================================================================*/
/* Prompt_help.cpp -- print prompt help                                       */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_help (
  "Prompt_help"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "help"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_help,      // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_helpJob(SePrompt* pPrompt)
{
  i8 nTokens = pPrompt->pTknVec()->nTokens();

  s32 sCmd;
  
  Stz* aCommand0 = pPrompt->pTknVec()->pGet(1)->aText();
  Stz* aCommand1 = nTokens > 2 ? pPrompt->pTknVec()->pGet(2)->aText() : ""_Stz;
  
  JobClass* pJobClass = JobClass::pFind(aCommand0, aCommand1);

  if (! pJobClass) {
    vTrace("No such command\n"_Stz);
    return false;
  }
  
  vTrace("Syntax : %s\n"_Stz,pJobClass->m_aHelp);
  vTrace("Example:\n%s\n"_Stz,pJobClass->m_aExample);
  
  vTrace("\n"_Stz);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_helpJobs(SePrompt* pPrompt)
{
  s32 sCmd;
  
  JobClass* pJobClass = nullptr;
  
  for (pJOBCLASSFUN pfJobClass = JobClass::pFirstFun(); pfJobClass; ) {
    
    pJobClass = pfJobClass((pFUN**)&pfJobClass);
    
    if ( ! pJobClass->m_bEnabled )
      continue;

    sCmd = pJobClass->m_aCommand1;
    if (pJobClass->m_aCommand2) {
      sCmd.vConcat(" "_Stz);
      sCmd.vConcat(pJobClass->m_aCommand2);
    }

    vTrace("  %14s -- %s\n"_Stz,sCmd.aStz(),pJobClass->m_aComment);
  }

  vTrace("\n"_Stz);
  
//  for (i8 i_Fun = 0; i_Fun < gxPromptCatalog.m_nFunctions; i_Fun++) {
//    
//    if (! gxPromptCatalog.m_xPromptFun[i_Fun].m_bEnabled)
//      continue;
//
//    vTrace("%s\n"_Stz,gxPromptCatalog.m_xPromptFun[i_Fun].m_aComment);
//  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_help(SePrompt* pPrompt)
{
  i8 nTokens = pPrompt->pTknVec()->nTokens();
  
  if (nTokens > 1)
    bPrompt_helpJob(pPrompt);
  else
    bPrompt_helpJobs(pPrompt);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
