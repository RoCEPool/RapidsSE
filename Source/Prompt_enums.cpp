/*============================================================================*/
/* Prompt_enums.cpp -- enums list                                             */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_enums (
  PF_ENUMS,             // prompt function
  "Prompt_enums"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "enums"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_enums,        // prompt function
  true,                 // enabled

  true,                 // visible
  "enums"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_enums(SePrompt* pPrompt)
{
  vTrace("Enums:\n"_Stz);

  vTrace("  eAC: api call kind\n"_Stz);
  vTrace("  eAK: arg kind\n"_Stz);
  vTrace("  eAT: access technique\n"_Stz);
  vTrace("  eCN: class number\n"_Stz);
  vTrace("  eCT: column type\n"_Stz);
  vTrace("  eCT: comment type\n"_Stz);
  vTrace("  eDC: date components\n"_Stz);
  vTrace("  eDM: datum type\n"_Stz);
  vTrace("  eDS: data source\n"_Stz);
  vTrace("  eEC: error code\n"_Stz);
  vTrace("  eHT: heap type\n"_Stz);
  //vTrace("  eIF: index form\n"_Stz);
  vTrace("  eIS: input source\n"_Stz);
  vTrace("  eIT: index type\n"_Stz);
  vTrace("  eJK: job kind\n"_Stz);
  vTrace("  eJL: job list\n"_Stz);
  vTrace("  eJP: job phase\n"_Stz);
  vTrace("  eJS: job state\n"_Stz);
  vTrace("  eKC: key compare\n"_Stz);
  vTrace("  eLK: lock kind\n"_Stz);
  vTrace("  ePK: predicate kind\n"_Stz);
  vTrace("  eQK: queue kinds\n"_Stz);
  vTrace("  eLE: record end\n"_Stz);
  vTrace("  eRO: relational op\n"_Stz);
  vTrace("  eSC: system call\n"_Stz);
  vTrace("  eRK: system resource\n"_Stz);
  vTrace("  eSS: seed string\n"_Stz);
  vTrace("  eHS: thread state\n"_Stz);
  vTrace("  eTS: task state\n"_Stz);
  vTrace("  eTN: token type\n"_Stz);
  vTrace("  eXP: transaction phase\n"_Stz);
  vTrace("  eTT: table type\n"_Stz);
  vTrace("  eWK: word type\n"_Stz);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
