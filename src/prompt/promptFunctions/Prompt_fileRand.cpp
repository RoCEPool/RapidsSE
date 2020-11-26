/*============================================================================*/
/* Prompt_fileRand.cpp -- file randomise                                      */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_fileRand (
  PF_FILERAND,           // prompt function
  "Prompt_fileRand"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "fileRand"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_fileRand,      // prompt function
  true,                  // enabled

  true,                  // visible
  "fileRand"_Stz,        // help
  ""_Stz                 // description
);

int Prompt::iFileRandCompare(const void* aLeft, const void* aRight)
{
  _fileRand* aFileRandLeft  = (_fileRand*)aLeft;
  _fileRand* aFileRandRight = (_fileRand*)aRight;
  
  if (aFileRandLeft->m_iRandom < aFileRandRight->m_iRandom)
    return -1;
  else
  if (aFileRandLeft->m_iRandom > aFileRandRight->m_iRandom)
    return 1;
  else
    return 0;
}

// fileRand "/Users/Shared/tpch/SF1/nation.csv"   "/tmp/nation_RND.csv" ;
// fileRand "/Users/Shared/tpch/SF1/lineItem.csv" "/tmp/lineItem_RND.csv" ;
/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_fileRand(SePrompt* pPrompt)
{
  s256 sPathIn;
  s256 sPathOut;

  i8 nLines = 0;
  
  Timer    xTimer;
  SP_Timer spTimer;

  
  _fileRand* aFileRand = (_fileRand*)malloc(sizeof(_fileRand) * C_10M);
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("fileRand"_Stz));
  
  sPathIn  = pInpTknVec->pGet(1)->szChars();
  sPathOut = pInpTknVec->pGet(2)->szChars();

  vTrace("fileRand %s into %s\n"_Stz,sPathIn.szStz(),sPathOut.szStz());
  
  File* pFileIn  = new File(nullptr);
  File* pFileOut = new File(nullptr);
  
  bool bOpened = pFileIn->bOpenRead(sPathIn);
  if (!bOpened) {
    vTrace("Unable to open input file %s\n"_Stz,sPathIn.szStz());
    goto L_Exit;
  }

  bOpened = pFileOut->bOpenWrite(sPathOut);
  if (!bOpened) {
    vTrace("Unable to open output file %s\n"_Stz,sPathOut.szStz());
    goto L_Exit;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  xTimer.vStart();
  while (pFileIn->bReadLine((Stz*)aFileRand[nLines].m_caLine))
    nLines++;
  xTimer.vUpdate();
  vTrace("Read %lld lines (%s)\n"_Stz,nLines,spTimer.sz(&xTimer));
  
  xTimer.vStart();
  for (i8 i_Line = 0; i_Line < nLines; i_Line++)
    aFileRand[i_Line].m_iRandom = RndLib::iInteger();
  xTimer.vUpdate();
  vTrace("Set random numbers (%s)\n"_Stz,spTimer.sz(&xTimer));

  xTimer.vStart();
  qsort(aFileRand,
        nLines,
        sizeof(_fileRand),
        Prompt::iFileRandCompare
        );
  xTimer.vUpdate();
  vTrace("Sorted (%s)\n"_Stz,spTimer.sz(&xTimer));

  xTimer.vStart();
  for (i8 i_Line = 0; i_Line < nLines; i_Line++)
    pFileOut->vPrint("%s\n"_Stz,aFileRand[i_Line].m_caLine);
  xTimer.vUpdate();
  vTrace("Written (%s)\n"_Stz,spTimer.sz(&xTimer));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_Exit:
  pFileIn->vClose();
  pFileOut->vClose();

  delete pFileIn;
  delete pFileOut;

  free(aFileRand);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
