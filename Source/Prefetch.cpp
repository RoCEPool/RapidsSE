/*============================================================================*/
/* Prefetch.cpp -- Prefetch test                                              */
/*============================================================================*/

#include "Utilities.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fill memory                                                                 */
/*----------------------------------------------------------------------------*/
void Prefetch::vFill(i8* aWord, i8 nWords)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write to all words
  i8 iRndValue = Random::iRandom();
  for (i8 i_Word = 0; i_Word < nWords; i_Word++) {
    aWord[i_Word] = iRndValue;
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
void Prefetch::vTest(void)
{
  i8 iSum = 0;
  const i8 iSize = C_1GB * 16;
  const i8 nWords = iSize / 8;
  
  vTrace("Prefetch Utility\n"_Stz);
  
  Timer xTimer;
  
  vTrace("Allocating %lld chunk\n"_Stz,iSize);
  i8* aWord = (i8*)malloc(iSize);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fill all words
  xTimer.vStart();
  vFill(aWord,nWords);
  xTimer.vUpdate();
  
  vTrace("Time to fill: total %lldns; perWord %lldns\n"_Stz,
         xTimer.iNanoseconds(),
         xTimer.iNanoseconds() / nWords
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sum randomly
  xTimer.vStart();
  
  iSum = 0;
  for (i8 i_WordNo = 0; i_WordNo < nWords; i_WordNo++) {
    i8 i_Word = Random::iRandom(nWords);
    iSum += aWord[i_Word];
  }

  xTimer.vUpdate();
  vTrace("Time to randomly sum: total %lldns; perWord %lldns (sum %lld)\n"_Stz,
         xTimer.iNanoseconds(),
         xTimer.iNanoseconds() / nWords,
         iSum
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fill all words
  xTimer.vStart();
  vFill(aWord,nWords);
  xTimer.vUpdate();
  
  vTrace("Time to fill: total %lldns; perWord %lldns\n"_Stz,
         xTimer.iNanoseconds(),
         xTimer.iNanoseconds() / nWords
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sum linearily
  xTimer.vStart();
  
  iSum = 0;
  for (i8 i_WordNo = 0; i_WordNo < nWords; i_WordNo++) {
    Random::iRandom(nWords);
    i8 i_Word = i_WordNo;
    iSum += aWord[i_Word];
  }
  
  xTimer.vUpdate();
  vTrace("Time to linearily sum(a): total %lldns; perWord %lldns (sum %lld)\n"_Stz,
         xTimer.iNanoseconds(),
         xTimer.iNanoseconds() / nWords,
         iSum
         );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fill all words
  xTimer.vStart();
  vFill(aWord,nWords);
  xTimer.vUpdate();
  
  vTrace("Time to fill: total %lldns; perWord %lldns\n"_Stz,
         xTimer.iNanoseconds(),
         xTimer.iNanoseconds() / nWords
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sum linearily
  xTimer.vStart();
  
  iSum = 0;
  for (i8 i_WordNo = 0; i_WordNo < nWords; i_WordNo++) {
    i8 i_Word = i_WordNo;
    iSum += aWord[i_Word];
  }
  
  xTimer.vUpdate();
  vTrace("Time to linearily sum(b): total %lldns; perWord %lldns (sum %lld)\n"_Stz,
         xTimer.iNanoseconds(),
         xTimer.iNanoseconds() / nWords,
         iSum
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("Freeing chunk\n"_Stz);
  //free(aWord);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
