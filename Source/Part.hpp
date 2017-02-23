/*============================================================================*/
/* Part.hpp -- part of an instantiable class                                  */
/*============================================================================*/

/*
 
 Description:
 
   Parts are things which will always be members of other classes.  We do not
   expect to directly instantiate them (allocate them in a heap), nor do we 
   expect them to be pushed onto a thread stack.
 
   On their own, they are not self-describing.  Consequently they do not have 
   a header or a footer.  They are ued within other classes which form 
   instantiable, self-describing classes
 
 */

#pragma once

#include "Platform.h"

namespace Platform
{
  class Part
  {
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
