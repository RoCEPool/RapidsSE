/*============================================================================*/
/* EnumGroups.hpp -- the master list of all enums in the system               */
/*============================================================================*/
#pragma once

#include "Enums.h"

namespace Enums
{

/*----------------------------------------------------------------------------*/
/* enum groups                                                                */
/*----------------------------------------------------------------------------*/
enum e_EG // enum groups
{
  _EG_NONE = 0,
  
  _EG_AC,
  _EG_AK,
  _EG_AN,
  _EG_AT,
  _EG_CN,
  _EG_CT,
  _EG_DC,
  _EG_DL,
  _EG_DM,
  _EG_DS,
  _EG_EC,
  _EG_EK,
  _EG_HT,
  _EG_IS,
  _EG_IT,
  _EG_JF,
  _EG_JK,
  _EG_JL,
  _EG_JP,
  _EG_JS,
  _EG_JT,
  _EG_KC,
  _EG_LK,
  _EG_OK,
  _EG_PF,
  _EG_PK,
  _EG_PY,
  _EG_QK,
  _EG_RA,
  _EG_RE,
  _EG_RK,
  _EG_RO,
  _EG_RS,
  _EG_RU,
  _EG_SC,
  _EG_SS,
  _EG_TD,
  _EG_TK,
  _EG_TN,
  _EG_TP,
  _EG_TR,
  _EG_TS,
  _EG_TT,
  _EG_WT,
  
  _EG_ENUMS
};
  
static Stz* a_EG[]
{
  "_EG_NONE"_Stz,
  
  "_EG_AC"_Stz,
  "_EG_AK"_Stz,
  "_EG_AN"_Stz,
  "_EG_AT"_Stz,
  "_EG_CN"_Stz,
  "_EG_CT"_Stz,
  "_EG_DC"_Stz,
  "_EG_DL"_Stz,
  "_EG_DM"_Stz,
  "_EG_DS"_Stz,
  "_EG_EC"_Stz,
  "_EG_EK"_Stz,
  "_EG_HT"_Stz,
  "_EG_IS"_Stz,
  "_EG_IT"_Stz,
  "_EG_JF"_Stz,
  "_EG_JK"_Stz,
  "_EG_JL"_Stz,
  "_EG_JP"_Stz,
  "_EG_JS"_Stz,
  "_EG_JT"_Stz,
  "_EG_KC"_Stz,
  "_EG_LK"_Stz,
  "_EG_LN"_Stz,
  "_EG_OK"_Stz,
  "_EG_PK"_Stz,
  "_EG_PY"_Stz,
  "_EG_QK"_Stz,
  "_EG_RA"_Stz,
  "_EG_RE"_Stz,
  "_EG_RK"_Stz,
  "_EG_RO"_Stz,
  "_EG_RS"_Stz,
  "_EG_RU"_Stz,
  "_EG_SC"_Stz,
  "_EG_SS"_Stz,
  "_EG_TD"_Stz,
  "_EG_TK"_Stz,
  "_EG_TN"_Stz,
  "_EG_TP"_Stz,
  "_EG_TR"_Stz,
  "_EG_TS"_Stz,
  "_EG_TT"_Stz,
  "_EG_WT"_Stz,
  
  "_EG_ENUMS"_Stz,
};
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class EnumGroups
  {
  public:
    static Stz* szEnumText(i8 iEnumGroup, i8 i_Enum);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
