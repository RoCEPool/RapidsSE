/*============================================================================*/
/* AlignedObject.cpp -- Base Class for CacheLine Aligned Objects              */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AlignedObject::AlignedObject(eCN eClassNo, i8 nChildSize)
: Object(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
#ifdef WITH_DEBUG_VERSION
  ASSERT(CacheLine::bOnCacheLineBoundary(this));
  ASSERT(CacheLine::bMultipleCacheLines(nChildSize));
#endif
}

/*----------------------------------------------------------------------------*/
/* is address on an nByte boundary?                                           */
/*----------------------------------------------------------------------------*/
bool AlignedObject::bOnBoundary(void* aAddress, i8 nByteBoundary)
{
  if (0 == (u8)aAddress % nByteBoundary)
    return true;
  else
    return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
