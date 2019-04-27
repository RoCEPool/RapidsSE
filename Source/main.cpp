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
//  printf("sizeof(Nibbler1) %ld\n",sizeof(Nibbler1));
//  printf("sizeof(Nibbler2) %ld\n",sizeof(Nibbler2));

//  printf("sizeof(Numbat) %ld\n",sizeof(Numbat));
  
  //       FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210
//  u8 u = 0b1111111111011111111101111111101111111011111101111101111011101101;
//  printf("%llx %llo %llu\n",u,u,u);
//  
//  printf("%llx\n",IntLib::uByte(u,0));
//  printf("%llx\n",IntLib::uByte(u,1));
//  printf("%llx\n",IntLib::uByte(u,7));
//
//  printf("%llx\n",IntLib::uBits(u,0,1));
//  printf("%llx\n",IntLib::uBits(u,1,2));
//  printf("%llx\n",IntLib::uBits(u,2,3));

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
