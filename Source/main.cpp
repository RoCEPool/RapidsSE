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
  // parse the command line arguments
  if ( ! gxArguments.bParse(argc,(Stz**)argv) )
    exit(EXIT_FAILURE);
  
  // potentially run utilities
  if (RapidsSE::bRunUtilities(argc,(Stz**)argv))
    exit(EXIT_SUCCESS);
  
  // execute
  RapidsSE::vMainEntry();

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
