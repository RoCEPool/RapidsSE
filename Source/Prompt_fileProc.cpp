/*============================================================================*/
/* Prompt_fileProc.cpp -- file proc                                           */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_fileProc (
  PF_FILEPROC,           // prompt function
  "Prompt_fileProc"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "fileProc"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_fileProc,      // prompt function
  true,                  // enabled

  true,               // visible
  "fileProc"_Stz,     // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_fileProc(SePrompt* pPrompt)
{
  s256 sFileInp;
  s256 sFileOut;

  TokenVec* pTknVec = pPrompt->pTknVec();
  
  ASSERT((*pTknVec)[0]->bSame("fileProc"_Stz));
  
  sFileInp = (*pTknVec)[1]->aText();
  sFileOut = (*pTknVec)[2]->aText();
  i8 nLines = pTknVec->nItems() > 3 ? pTknVec->pGet(3)->iToI8() : 0;

  File oFileInp;
  File oFileOut;

  if (! oFileInp.bOpenRead(sFileInp)) {
    vTrace("No such input file\n"_Stz);
    return true;
  }
  if (! oFileOut.bOpenWrite(sFileOut)) {
    vTrace("Unable to open output file\n"_Stz);
    return true;
  }

  s16KB sLine_1;
  s16KB sLine_2;
  int i_Line = 0;
  while (oFileInp.bReadLine(sLine_1)) {
    
    if (! oFileInp.bReadLine(sLine_2))
      break;

    if (nLines && ++i_Line > nLines)
      break;
    
    vTrace("%lld: %s%s\n"_Stz,i_Line,sLine_1.aStz(),sLine_2.aStz());
    
    oFileOut.vPrint("%s%s\n"_Stz,sLine_1.aStz(),sLine_2.aStz());
  }
  
  oFileInp.vClose();
  oFileOut.vClose();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
