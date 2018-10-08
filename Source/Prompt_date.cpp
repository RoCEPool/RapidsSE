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
  Since xNow;
  
  xNow.vSet();

  Time xTime;
  
  xTime.vSet(&xNow);
  
  vTrace("%lld/%02lld/%02lld %02lld:%02lld:%02lld (utc)\n"_Stz,
         (i8)xTime.m_iYear + 1900,
         (i8)xTime.m_iMonth,
         (i8)xTime.m_iDay,
         (i8)xTime.m_iHour,
         (i8)xTime.m_iMinute,
         (i8)xTime.m_iSecond
         );
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
