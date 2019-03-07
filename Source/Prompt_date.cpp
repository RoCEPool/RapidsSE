/*============================================================================*/
/* Prompt_date.cpp -- show date and time                                      */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_date (
  PF_DATE,           // prompt function
  "Prompt_date"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "date"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_date,      // prompt function
  true,              // enabled

  true,              // visible
  "date"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_date(SePrompt* pPrompt)
{
  Time oTime;
  oTime.vSetLocalNow();
  
  s32 sDate;
  char* z = sDate.aChar();
  
  oTime.vAppendLocal(&z);

  vTrace("%s\n"_Stz,sDate.aChar());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
