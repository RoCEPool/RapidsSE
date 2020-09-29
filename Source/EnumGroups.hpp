/*============================================================================*/
/* EnumGroups.hpp -- the master list of all enums in the system               */
/*============================================================================*/
#pragma once


#include "EnumGroups.h"

namespace Enums
{
#include "Enums.h"

  
static Stz* aEG[]
{
  "EG_NONE"_Stz,
  
  "EG_AA"_Stz,
  "EG_AC"_Stz,
  "EG_AK"_Stz,
  "EG_AN"_Stz,
  "EG_AT"_Stz,

  "EG_BO"_Stz,

  "EG_CC"_Stz,
  "EG_CK"_Stz,
  "EG_CM"_Stz,
  "EG_CN"_Stz,
  "EG_CP"_Stz,
  "EG_CS"_Stz,
  "EG_CT"_Stz,
  "EG_CX"_Stz,

  "EG_DA"_Stz,
  "EG_DC"_Stz,
  "EG_DG"_Stz,
  "EG_DL"_Stz,
  "EG_DM"_Stz,
  "EG_DS"_Stz,
  
  "EG_EA"_Stz,
  "EG_EC"_Stz,
  "EG_ED"_Stz,
  "EG_EK"_Stz,
  "EG_EO"_Stz,
  "EG_EP"_Stz,

  "EG_FA"_Stz,
  "EG_FN"_Stz,
  "EG_FU"_Stz,

  "EG_GA"_Stz,
  
  "EG_HT"_Stz,
  "EG_HS"_Stz,
  "EG_HT"_Stz,

  "EG_IK"_Stz,
  "EG_IS"_Stz,
  "EG_IT"_Stz,

  "EG_JA"_Stz,
  "EG_JF"_Stz,
  "EG_JK"_Stz,
  "EG_JL"_Stz,
  "EG_JP"_Stz,
  "EG_JS"_Stz,
  
  "EG_KC"_Stz,
  "EG_KE"_Stz,
  "EG_KK"_Stz,

  "EG_LA"_Stz,
  "EG_LK"_Stz,
  "EG_LN"_Stz,
  
  "EG_MA"_Stz,
  "EG_MC"_Stz,
  "EG_MK"_Stz,
  "EG_MM"_Stz,
  "EG_MN"_Stz,
  "EG_MT"_Stz,

  "EG_NE"_Stz,
  "EG_NK"_Stz,
  "EG_NS"_Stz,
  
  "EG_OA"_Stz,
  "EG_OC"_Stz,
  "EG_OI"_Stz,
  "EG_OK"_Stz,
  "EG_OM"_Stz,
  "EG_OP"_Stz,
  "EG_OR"_Stz,

  "EG_PA"_Stz,
  "EG_PF"_Stz,
  "EG_PK"_Stz,
  "EG_PO"_Stz,
  "EG_PP"_Stz,
  "EG_PT"_Stz,
  "EG_PX"_Stz,
  "EG_PY"_Stz,
  
  "EG_QA"_Stz,
  "EG_QK"_Stz,
  "EG_QN"_Stz,

  "EG_RA"_Stz,
  "EG_RE"_Stz,
  "EG_RG"_Stz,
  "EG_RO"_Stz,
  "EG_RS"_Stz,
  "EG_RU"_Stz,
  
  "EG_SC"_Stz,
  "EG_SI"_Stz,
  "EG_SK"_Stz,
  "EG_SL"_Stz,
  "EG_SS"_Stz,
  "EG_ST"_Stz,

  "EG_TA"_Stz,
  "EG_TN"_Stz,
  "EG_TP"_Stz,
  "EG_TQ"_Stz,
  "EG_TR"_Stz,
  "EG_TS"_Stz,
  "EG_TT"_Stz,
  
  "EG_UF"_Stz,
  "EG_UK"_Stz,

  "EG_VA"_Stz,
  "EG_VL"_Stz,
  "EG_VP"_Stz,

  "EG_WK"_Stz,

  "EG_XA"_Stz,
  "EG_XK"_Stz,
  "EG_XP"_Stz,

  "EG_YA"_Stz,
  
  "EG_ZK"_Stz,
  
  "EG_ENUMS"_Stz,
};
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class EnumGroups
  {
  public:
    static Stz* szEnumText(i8 i_EnumGroup, i8 i_Enum);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
