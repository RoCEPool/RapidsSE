/*============================================================================*/
/* AlignedObject.cpp -- Base Class for CacheLine Aligned Objects              */
/*============================================================================*/

#include "Lines.h"

/*----------------------------------------------------------------------------*/
/* initialize object                                                          */
/*----------------------------------------------------------------------------*/
AlignedObject::AlignedObject(eCN eClassNo, i8 nChildSize)
: Object(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  ASSERT(MemLib::bOnCacheLineBoundary(this));
  ASSERT(MemLib::bMultipleCacheLines(nChildSize));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
