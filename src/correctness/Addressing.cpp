/*============================================================================*/
/* Addressing.cpp -- Functions for Manipulating Addresses                     */
/*============================================================================*/

#include "Correctness.h"

extern BufferPool* gpBufferPool;


/*----------------------------------------------------------------------------*/
/* compute absolute byte difference between two addresses                     */
/*----------------------------------------------------------------------------*/
i8 Resources::nDifference(void* aAddress0, void* aAddress1)
{
  byte* ayAddr0 = reinterpret_cast<byte*>(aAddress0);
  byte* ayAddr1 = reinterpret_cast<byte*>(aAddress1);
 
  i8 nBytes = 0;
  
  if (ayAddr0 >= ayAddr1)
    nBytes = ayAddr0 - ayAddr1;
  else
    nBytes = ayAddr1 - ayAddr0;

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* vCheckAddress is defined in BufferPool.cpp                                 */
/*----------------------------------------------------------------------------*/

// refer: BufferPool.cpp

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
