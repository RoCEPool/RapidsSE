/*============================================================================*/
/* Indexes.h -- Indexes namespace                                             */
/*============================================================================*/
#pragma once

#include "Transform.h"

namespace Indexes
{  
  void vCheckAssumptions(void);
  
  enum eKC // KeyCompare
  {
  /*  0 */ KC_NONE=0,
  /*  1 */ KC_BEFORE,
  /*  2 */ KC_SAME,
  /*  3 */ KC_AFTER,
  /*  4 */ KC_ENUMS
  };
  
  static Stz* aKC[]
    {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "BEFORE"_Stz,
    /*  2 */ "SAME"_Stz,
    /*  3 */ "AFTER"_Stz,
    /*  4 */ "ENUMS"_Stz,
    };

}

using namespace Indexes;

#include "Id.hpp"
#include "Key.hpp"
#include "Ent.hpp"
#include "EntWrk.hpp"
#include "SkipRoots.hpp"

#include "EntBuffer.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
