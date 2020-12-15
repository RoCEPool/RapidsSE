/*============================================================================*/
/* Prompt_ascii.cpp -- Ascii Helper                                           */
/*============================================================================*/

#include "Prompt.h"

#include "eOC.h"

PromptFun __Prompt_ascii (
  PF_ASCII,           // prompt function
  "Prompt_ascii"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "ascii"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_ascii,      // prompt function
  true,               // enabled

  true,               // visible
  "ascii"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ascii(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();

  if (1 == iNumTokens) {
    
    for (i4 i_Char = 0; i_Char < 128; i_Char++) {
      if (0 == i_Char % 8)
        vTrace("| "_Stz);
      vTrace("%02x %-3s | "_Stz,i_Char,StrLib::azAscii(i_Char));
      if (i_Char && 0 == (i_Char+1) % 8)
        vTrace("\n"_Stz);
    }

//    vTrace("\n"_Stz);
//    for (i4 i_Char = 0; i_Char < 32; i_Char++) {
//      vTrace("%02x %-3s %c\n"_Stz,
//             i_Char,
//             StrLib::azAscii(i_Char),
//             128 + i_Char);
//    }

    vTrace("\n"_Stz);
    for (i4 i_Char = 0; i_Char < 32; i_Char++)
      vTrace("%s"_Stz,aOC[i_Char]);
    vTrace("\n"_Stz);

    return true;
  }
  
  Token* pTkn1 = pInpTknVec->pGet(1);

  char* aChar = (char*)pTkn1->szChars();
  vTrace("Ascii: "_Stz);
  while (*aChar)
    vTrace("%02x "_Stz,*aChar++);
  vTrace("\n"_Stz);

  aChar = (char*)pTkn1->szChars();
  vTrace("     : "_Stz);
  while (*aChar)
    vTrace(" %c "_Stz,*aChar++);
  vTrace("\n"_Stz);
    
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
