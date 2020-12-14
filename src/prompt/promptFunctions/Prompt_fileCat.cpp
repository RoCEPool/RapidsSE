/*============================================================================*/
/* Prompt_fileCat.cpp -- file cat                                             */
/*============================================================================*/

#include "Prompt.h"

#include "File.hpp" //?

PromptFun __Prompt_fileCat (
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

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("fileCat"_Stz));
  
  sFile = pInpTknVec->pGet(1)->szChars();
  i8 nLines = pInpTknVec->pGet(2)->iToI8();

  File* pFile = new File(pPrompt);
  
  if (! pFile->bOpenRead(sFile)) {
    vTrace("No such file\n"_Stz);
    return true;
  }
  
  s16KB sLine;
  int i_Line = 0;
  while (pFile->bReadLine(sLine)) {
    if (++i_Line > nLines)
      break;
    vTrace("%s\n"_Stz,sLine.szStz());
  }
  
  pFile->vClose();
  
  delete pFile;

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
