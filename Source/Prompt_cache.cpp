/*============================================================================*/
/* Prompt_cache.cpp -- cache helper                                           */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_cache (
  PF_CACHE,           // prompt function
  "Prompt_cache"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "cache"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_cache,      // prompt function
  true,               // enabled

  true,               // visible
  "cache"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_cache(SePrompt* pPrompt)
{
  CacheLine xCacheLine;
  
  
  w8 wVal = 1;
  
  vTrace("Ones\n"_Stz);
  wVal = 0x01;
  xCacheLine.vSetAll(0xFF);
  for (i8 i_Offset = 0; i_Offset < C_BYTES_PER_CACHE_LINE-0; i_Offset += 1) {
    xCacheLine.v1B(i_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,i_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
  }
  
  vTrace("Twos\n"_Stz);
  wVal = 0x0201;
  xCacheLine.vSetAll(0xFF);
  for (i8 i_Offset = 0; i_Offset < C_BYTES_PER_CACHE_LINE-1; i_Offset += 2) {
    xCacheLine.v2B(i_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,i_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
  }
  
  vTrace("Threes\n"_Stz);
  wVal = 0x030201;
  xCacheLine.vSetAll(0xFF);
  for (i8 i_Offset = 0; i_Offset < C_BYTES_PER_CACHE_LINE-2; i_Offset += 3) {
    xCacheLine.v3B(i_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,i_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
  }
  
  vTrace("Fours\n"_Stz);
  wVal = 0x04030201;
  xCacheLine.vSetAll(0xFF);
  for (i8 i_Offset = 0; i_Offset < C_BYTES_PER_CACHE_LINE-3; i_Offset += 4) {
    xCacheLine.v4B(i_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,i_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
  }
  
  vTrace("Fives\n"_Stz);
  wVal = 0x0504030201;
  xCacheLine.vSetAll(0xFF);
  for (i8 i_Offset = 0; i_Offset < C_BYTES_PER_CACHE_LINE-4; i_Offset += 5) {
    xCacheLine.v5B(i_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,i_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
  }
  
  vTrace("Sixes\n"_Stz);
  wVal = 0x060504030201;
  xCacheLine.vSetAll(0xFF);
  for (i8 i_Offset = 0; i_Offset < C_BYTES_PER_CACHE_LINE-5; i_Offset += 6) {
    xCacheLine.v6B(i_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,i_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
  }
  
  vTrace("Sevens\n"_Stz);
  wVal = 0x07060504030201;
  xCacheLine.vSetAll(0xFF);
  for (i8 i_Offset = 0; i_Offset < C_BYTES_PER_CACHE_LINE-6; i_Offset += 7) {
    xCacheLine.v7B(i_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,i_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
  }
  
  vTrace("Eights\n"_Stz);
  wVal = 0x0807060504030201;
  xCacheLine.vSetAll(0xFF);
  for (i8 i_Offset = 0; i_Offset < C_BYTES_PER_CACHE_LINE-7; i_Offset += 8) {
    xCacheLine.v8B(i_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,i_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
