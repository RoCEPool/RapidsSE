/*============================================================================*/
/* Prefetch.cpp -- Prefetch test                                              */
/*============================================================================*/

#include "Utilities.h"
#include "Asm.h"

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
  const i8 iSize = C_1GB * 2;
  const i8 nWords = iSize / 8;
  
  vTrace("Prefetch Utility\n"_Stz);
  
  Timer oTimer;
  
  vTrace("Allocating %lld chunk\n"_Stz,iSize);
  i8* aWord = (i8*)MemLib::aMallocBlock(iSize);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fill all words
  oTimer.vStart();
  vFill(aWord,nWords);
  oTimer.vUpdate();
  
  vTrace("Time to fill: total %lldns; perWord %lldns\n"_Stz,
         oTimer.iNanoseconds(),
         oTimer.iNanoseconds() / nWords
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sum randomly
  oTimer.vStart();

  iSum = 0;
  
  for (i8 i_WordNo = 0; i_WordNo < nWords; i_WordNo++) {
    i8 i_Word = Random::iRandom(nWords);
    iSum += aWord[i_Word];
  }

  oTimer.vUpdate();
  vTrace("Time to randomly sum: total %lldns; perWord %lldns (sum %lld)\n"_Stz,
         oTimer.iNanoseconds(),
         oTimer.iNanoseconds() / nWords,
         iSum
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fill all words
  oTimer.vStart();
  vFill(aWord,nWords);
  oTimer.vUpdate();
  
  vTrace("Time to fill: total %lldns; perWord %lldns\n"_Stz,
         oTimer.iNanoseconds(),
         oTimer.iNanoseconds() / nWords
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sum randomly with prefetch
  oTimer.vStart();
  
  iSum = 0;
  i8 i_Word = Random::iRandom(nWords);
  for (i8 i_WordNo = 0; i_WordNo < nWords; i_WordNo++) {
    i8 i_Word2 = Random::iRandom(nWords);
    __builtin_prefetch(&aWord[i_Word2]);
    //vPrefetch((void*)(&aWord[i_Word2]));
    iSum += aWord[i_Word];
    i_Word = i_Word2;
  }
  
  oTimer.vUpdate();
  vTrace("Time to randomly sum with prefetch: total %lldns; perWord %lldns (sum %lld)\n"_Stz,
         oTimer.iNanoseconds(),
         oTimer.iNanoseconds() / nWords,
         iSum
         );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fill all words
  oTimer.vStart();
  vFill(aWord,nWords);
  oTimer.vUpdate();
  
  vTrace("Time to fill: total %lldns; perWord %lldns\n"_Stz,
         oTimer.iNanoseconds(),
         oTimer.iNanoseconds() / nWords
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sum linearily
  oTimer.vStart();
  
  iSum = 0;
  for (i8 i_WordNo = 0; i_WordNo < nWords; i_WordNo++) {
    Random::iRandom(nWords);
    i8 i_Word = i_WordNo;
    iSum += aWord[i_Word];
  }
  
  oTimer.vUpdate();
  vTrace("Time to linearily sum(a): total %lldns; perWord %lldns (sum %lld)\n"_Stz,
         oTimer.iNanoseconds(),
         oTimer.iNanoseconds() / nWords,
         iSum
         );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fill all words
  oTimer.vStart();
  vFill(aWord,nWords);
  oTimer.vUpdate();
  
  vTrace("Time to fill: total %lldns; perWord %lldns\n"_Stz,
         oTimer.iNanoseconds(),
         oTimer.iNanoseconds() / nWords
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sum linearily
  oTimer.vStart();
  
  iSum = 0;
  for (i8 i_WordNo = 0; i_WordNo < nWords; i_WordNo++) {
    i8 i_Word = i_WordNo;
    iSum += aWord[i_Word];
  }
  
  oTimer.vUpdate();
  vTrace("Time to linearily sum(b): total %lldns; perWord %lldns (sum %lld)\n"_Stz,
         oTimer.iNanoseconds(),
         oTimer.iNanoseconds() / nWords,
         iSum
         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("Freeing chunk\n"_Stz);
  //MemLib::vFreeBlock(aWord);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
