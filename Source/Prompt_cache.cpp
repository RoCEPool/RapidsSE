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
  
  
  w8 wVal = 0;
  
  xCacheLine.vSetAll(0xFF);
  vTrace("Ones\n"_Stz);
  wVal = 0x01;
  for (u8 u_Offset = 0; u_Offset < C_BYTES_PER_CACHE_LINE-0; u_Offset += 1) {
    RASSERT(0xFF == xCacheLine.w8_1B(u_Offset));
    xCacheLine.v1B(u_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,u_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
    RASSERT(wVal == xCacheLine.w8_1B(u_Offset));
  }
  
  xCacheLine.vSetAll(0xFF);
  vTrace("Twos\n"_Stz);
  wVal = 0x0201;
  for (u8 u_Offset = 0; u_Offset < C_BYTES_PER_CACHE_LINE-1; u_Offset += 2) {
    RASSERT(0xFFFF == xCacheLine.w8_2B(u_Offset));
    xCacheLine.v2B(u_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,u_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
    RASSERT(wVal == xCacheLine.w8_2B(u_Offset));
  }
  
  xCacheLine.vSetAll(0xFF);
  vTrace("Threes\n"_Stz);
  wVal = 0x030201;
  for (u8 u_Offset = 0; u_Offset < C_BYTES_PER_CACHE_LINE-2; u_Offset += 3) {
    RASSERT(0xFFFFFF == xCacheLine.w8_3B(u_Offset));
    xCacheLine.v3B(u_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,u_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
    RASSERT(wVal == xCacheLine.w8_3B(u_Offset));
  }
  
  xCacheLine.vSetAll(0xFF);
  vTrace("Fours\n"_Stz);
  wVal = 0x04030201;
  for (u8 u_Offset = 0; u_Offset < C_BYTES_PER_CACHE_LINE-3; u_Offset += 4) {
    RASSERT(0xFFFFFFFF == xCacheLine.w8_4B(u_Offset));
    xCacheLine.v4B(u_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,u_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
    RASSERT(wVal == xCacheLine.w8_4B(u_Offset));
  }
  
  xCacheLine.vSetAll(0xFF);
  vTrace("Fives\n"_Stz);
  wVal = 0x0504030201;
  for (u8 u_Offset = 0; u_Offset < C_BYTES_PER_CACHE_LINE-4; u_Offset += 5) {
    RASSERT(0xFFFFFFFFFF == xCacheLine.w8_5B(u_Offset));
    xCacheLine.v5B(u_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,u_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
    RASSERT(wVal == xCacheLine.w8_5B(u_Offset));
  }
  
  xCacheLine.vSetAll(0xFF);
  vTrace("Sixes\n"_Stz);
  wVal = 0x060504030201;
  for (u8 u_Offset = 0; u_Offset < C_BYTES_PER_CACHE_LINE-5; u_Offset += 6) {
    RASSERT(0xFFFFFFFFFFFF == xCacheLine.w8_6B(u_Offset));
    xCacheLine.v6B(u_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,u_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
    RASSERT(wVal == xCacheLine.w8_6B(u_Offset));
  }
  
  xCacheLine.vSetAll(0xFF);
  vTrace("Sevens\n"_Stz);
  wVal = 0x07060504030201;
  for (u8 u_Offset = 0; u_Offset < C_BYTES_PER_CACHE_LINE-6; u_Offset += 7) {
    RASSERT(0xFFFFFFFFFFFFFF == xCacheLine.w8_7B(u_Offset));
    xCacheLine.v7B(u_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,u_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
    RASSERT(wVal == xCacheLine.w8_7B(u_Offset));
  }
  
  xCacheLine.vSetAll(0xFF);
  vTrace("Eights\n"_Stz);
  wVal = 0x0807060504030201;
  for (u8 u_Offset = 0; u_Offset < C_BYTES_PER_CACHE_LINE-7; u_Offset += 8) {
    RASSERT(0xFFFFFFFFFFFFFFFFull == xCacheLine.w8_8B(u_Offset));
    xCacheLine.v8B(u_Offset,wVal);
    vTrace("\n"_Stz);
    vTrace("%2lld: \n"_Stz,u_Offset);
    MemLib::vWordDump(&xCacheLine,sizeof(xCacheLine));
    RASSERT(wVal == xCacheLine.w8_8B(u_Offset));
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
