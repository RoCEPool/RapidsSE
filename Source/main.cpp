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
  printf("sizeof(Nibbler1) %ld\n",sizeof(Nibbler1));
  printf("sizeof(Nibbler2) %ld\n",sizeof(Nibbler2));
  printf("C_2_BYTES %llu\n",C_2_BYTES);

  // parse the command line arguments
  if ( ! gxArguments.bParse(argc,(Stz**)argv) )
    exit(EXIT_FAILURE);
  
  // potentially run utilities
  if (Engine::bRunUtilities(argc,(Stz**)argv))
    exit(EXIT_SUCCESS);
  
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
