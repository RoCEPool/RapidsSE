/*============================================================================*/
/* Prompt_time.cpp -- show time                                               */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_time (
  "Prompt_time"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "time"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_time,      // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_time(SePrompt* pPrompt)
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
