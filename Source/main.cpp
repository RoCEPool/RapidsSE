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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
