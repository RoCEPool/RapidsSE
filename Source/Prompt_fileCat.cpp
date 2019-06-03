/*============================================================================*/
/* Prompt_fileCat.cpp -- file cat                                             */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_fileCat (
  PF_FILECAT,           // prompt function
  "Prompt_fileCat"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "fileCat"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_fileCat,      // prompt function
  true,                 // enabled

  true,              // visible
  "fileCat"_Stz,     // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_fileCat(SePrompt* pPrompt)
{
  s256 sFile;

  TokenVec* pTokenVec = pPrompt->pTknVec();
  
  ASSERT((*pTokenVec)[0]->bSame("fileCat"_Stz));
  
  sFile = (*pTokenVec)[1]->aText();
  i8 nLines = pTokenVec->pGet(2)->iToI8();

  File oFile;
  
  if (! oFile.bOpenRead(sFile)) {
    vTrace("No such file\n"_Stz);
    return true;
  }
  
  s16KB sLine;
  int i_Line = 0;
  while (oFile.bReadLine(sLine)) {
    if (++i_Line > nLines)
      break;
    vTrace("%s\n"_Stz,sLine.aStz());
  }
  
  oFile.vClose();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
