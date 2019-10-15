/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "Oblique.h"

extern Arguments  gxArguments;

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
  printf("sizeof(Column)       %lld\n", (i8)sizeof(Column));
  printf("sizeof(LinkedBuffer) %lld\n", (i8)sizeof(LinkedBuffer));
  printf("sizeof(HeapBuffer)   %lld\n", (i8)sizeof(HeapBuffer));
  
  s32 sRightTest;
  StrLib::vRuler(sRightTest.aChar(),30);
  vTrace("%s\n"_Stz,sRightTest.aChar());

  StrLib::vRight(sRightTest.aChar(),(char*)"abcde"_Stz,29);
  vTrace("%s\n"_Stz,sRightTest.aChar());
  
  Process::vEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // parse the command line arguments
  if (! gxArguments.bParse(argc,(Stz**)argv))
    Process::vExitFailure();
  
  // potentially run utilities
  if (Engine::bRunUtilities(argc,(Stz**)argv))
    Process::vExitSuccess();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // execute
  Engine::vMainEntry();

  return EXIT_SUCCESS;
}

/*
 
 Desktop/rapidsse/OS X/Build/Products/Debug
 
 ./SE --start --killPrev --verbose --subloaders 1 --size 10GB --name NODE0 --node NODE0 127.0.0.1 5550 --log
 
 to run prompt:

 ./SE --prompt --name NODE0 --log
 
 */

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
