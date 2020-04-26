/*============================================================================*/
/* LexicalAnalysis.h -- LexicalAnalysis namespace                             */
/*============================================================================*/
#pragma once

#include "Arguments.h"

//------------------------------------------------------------------------------

namespace LexicalAnalysis
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#define __WITH_CHARS_BYTE_SPACE EndDefine


#include "AnnChar.hpp"
#include "Token.hpp"
#include "TokenVec.hpp"

#include "Charser.hpp"
#include "Input.hpp"

#include "SP_AnnChar.hpp"
#include "SP_Charser.hpp"
#include "SP_Token.hpp"
#include "SP_TknVecStr.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
