/*============================================================================*/
/* Prompt_ascii.cpp -- Ascii Helper                                           */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_ascii (
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
  TokenVec* pTokenVec = pPrompt->pTknVec();
  
  i8 iNumTokens = pTokenVec->nItems();

  if (1 == iNumTokens) {

    for (i4 i_Char = 0; i_Char < 128; i_Char++) {
      if (isprint(i_Char))
        vTrace("%02x %c  "_Stz,i_Char,i_Char);
      else
        vTrace("%02x .  "_Stz,i_Char,i_Char);
      
      if (i_Char && 0 == (i_Char+1) % 8)
        vTrace("\n"_Stz);
    }
    
    return true;
  }
  
  Token* pTkn1 = pTokenVec->pGet(1);

  char* aChar = (char*)pTkn1->aText();
  vTrace("Ascii: "_Stz);
  while (*aChar)
    vTrace("%02x "_Stz,*aChar++);
  vTrace("\n"_Stz);

  aChar = (char*)pTkn1->aText();
  vTrace("     : "_Stz);
  while (*aChar)
    vTrace(" %c "_Stz,*aChar++);
  vTrace("\n"_Stz);
    
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
