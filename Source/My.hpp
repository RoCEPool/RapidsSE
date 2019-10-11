/*============================================================================*/
/* My.hpp -- My helper                                                        */
/*============================================================================*/
#pragma once

//!                    ** Do not include StorageEngine.hpp **

#include "eAA.h"
#include "eDA.h"
#include "eGA.h"
#include "eHA.h"
#include "eJA.h"
#include "eLA.h"
#include "eCA.h"
#include "eMA.h"
#include "eOA.h"
#include "ePA.h"
#include "eQA.h"
#include "eRA.h"
#include "eSL.h"
#include "eTA.h"
#include "eVA.h"
#include "eXA.h"
#include "eYA.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace Oblique
{
  class My
  {
  public:
    static Stz* szJobNm_curr(void);
    static Stz* szGridNm_curr(void);
    static Stz* szWordNm_curr(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
