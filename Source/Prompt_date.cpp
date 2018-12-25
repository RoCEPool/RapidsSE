/*============================================================================*/
/* Prompt_date.cpp -- show date and time                                      */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_date (
  "Prompt_date"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "date"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_date,      // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_date(SePrompt* pPrompt)
{
  Time oTime;
  oTime.vSetNowLocal();
  
  s32 sDate;
  char* z = sDate.aChar();
  
  oTime.vAppendLocal(&z);

  vTrace("%s\n"_Stz,sDate.aChar());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
