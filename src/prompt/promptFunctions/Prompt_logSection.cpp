/*============================================================================*/
/* Prompt_logSection.cpp -- write a section line into the log                 */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_logSection (
  PF_LOGSECTION,           // prompt function
  "Prompt_logSection"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the function
  "logSection"_Stz,        // the prompt command
  nullptr,                 // the second word
  bPrompt_logSection,      // prompt function
  true,                    // enabled

  true,                    // visible
  "logSection"_Stz,        // help
  ""_Stz                   // description
);

extern Log* gpLog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_logSection(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  if (pInpTknVec->nTokens() < 1)
    return true;
  
  Stz* szSectionNm = pInpTknVec->pGet(1)->szChars();
  i8 iSectNmLen = szSectionNm->nLength();

  char caSection[256] = { '\0' };
  
  StrLib::vPad(caSection,127-iSectNmLen-4,'#');
  
  StrLib::vConcat(caSection,' ');
  StrLib::vConcat(caSection,szSectionNm->azChar());
  StrLib::vConcat(caSection,' ');
  StrLib::vConcat(caSection,'#');
  StrLib::vConcat(caSection,'#');

  AM::vLogLog(SL_INF,gpLog,GA_LOG_SECTION,
              "%s"_Stz, //!format-LogLog
              (Stz*)caSection);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
