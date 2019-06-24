/*============================================================================*/
/* Transform.h -- Transform namespace                                         */
/*============================================================================*/

#pragma once

#include "MetaData.h"

//-----------------------------------------------------------------------------

namespace Transform
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

using namespace Transform;

//-----------------------------------------------------------------------------

#include "DtmVal.hpp"

#include "Scan.hpp"
#include "Sprint.hpp"

#include "Id.hpp"
#include "Row.hpp"
#include "Key.hpp"
#include "Ent.hpp"
#include "EntWrk.hpp"

#include "Record.hpp"
#include "IrwBuffer.hpp"

#include "hpRow.hpp"

#include "snBCD.hpp"
#include "snBOL.hpp"
#include "snDTE.hpp"
#include "snFLT.hpp"
#include "snINT.hpp"
#include "snIVL.hpp"
#include "snTME.hpp"
#include "snTSP.hpp"
#include "snVCH.hpp"

#include "spBCD.hpp"
#include "spBOL.hpp"
#include "spDTE.hpp"
#include "spFLT.hpp"
#include "spINT.hpp"
#include "spIVL.hpp"
#include "spTME.hpp"
#include "spTSP.hpp"
#include "spVCH.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
