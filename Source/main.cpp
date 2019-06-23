/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "StorageEngine.h"

extern Arguments  gxArguments;

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
//  vTrace("Word %lld DoubleWord %lld QuadWord %lld CacheLine %lld\n"_Stz,
//         (i8)sizeof(Word),
//         (i8)sizeof(DoubleWord),
//         (i8)sizeof(QuadWord),
//         (i8)sizeof(CacheLine)
//         );

//  Timer xTimer;
//  
//  xTimer.vStart();
//  i8 iSum = 0;
//  d8 dProd = 1.0;
//  for (i8 i = 1; i < C_1G; i++) {
//    iSum  += i;
//    dProd *= 10;
//    if (dProd > C_1G)
//      dProd = 1.0;
//  }
//  xTimer.vUpdate();
//  vTrace("Time %lld; Sum %lld; iProd %g\n"_Stz,xTimer.iSeconds(),iSum,dProd);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // parse the command line arguments
  if ( ! gxArguments.bParse(argc,(Stz**)argv) )
    Process::vExit(EXIT_FAILURE);
  
  // potentially run utilities
  if (Engine::bRunUtilities(argc,(Stz**)argv))
    Process::vExit(EXIT_SUCCESS);
  
  // execute
  Engine::vMainEntry();

  return EXIT_SUCCESS;
}

/*
 
 ./SE --start --killPrev --verbose --subloaders 1 --size 10GB --name NODE1 --node NODE1 127.0.0.1 5441
 
 to run prompt:

 ./SE --prompt --name NODE1
 
 */

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
