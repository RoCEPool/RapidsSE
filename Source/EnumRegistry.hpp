/*============================================================================*/
/* EnumRegistery.hpp -- the master list of all enums in the system            */
/*============================================================================*/
#pragma once

#include "Base.h"
#include "Stz.hpp"

namespace StorageEngine
{

/*----------------------------------------------------------------------------*/
/* enum registry                                                              */
/*----------------------------------------------------------------------------*/
enum e_ER // enum registry
{
  _ER_NONE = 0,
  
  _ER_AC,
  _ER_AK,
  _ER_AN,
  _ER_AT,
  _ER_CN,
  _ER_CT,
  _ER_DC,
  _ER_DM,
  _ER_DS,
  _ER_EC,
  _ER_HT,
  _ER_IF,
  _ER_IS,
  _ER_IT,
  _ER_JC,
  _ER_JK,
  _ER_JL,
  _ER_JP,
  _ER_JS,
  _ER_JT,
  _ER_LK,
  _ER_PK,
  _ER_QK,
  _ER_QR,
  _ER_RE,
  _ER_RO,
  _ER_SC,
  _ER_SR,
  _ER_SS,
  _ER_TD,
  _ER_TK,
  _ER_TN,
  _ER_TP,
  _ER_TT,
  _ER_WT,
  
  _ER_ENUMS
};
  
static Stz* a_ER[]
{
  "_ER_NONE"_Stz,
  
  "_ER_AC"_Stz,
  "_ER_AK"_Stz,
  "_ER_AT"_Stz,
  "_ER_CN"_Stz,
  "_ER_CT"_Stz,
  "_ER_DC"_Stz,
  "_ER_DM"_Stz,
  "_ER_DS"_Stz,
  "_ER_EC"_Stz,
  "_ER_HT"_Stz,
  "_ER_IF"_Stz,
  "_ER_IS"_Stz,
  "_ER_IT"_Stz,
  "_ER_JC"_Stz,
  "_ER_JK"_Stz,
  "_ER_JL"_Stz,
  "_ER_JP"_Stz,
  "_ER_JS"_Stz,
  "_ER_JT"_Stz,
  "_ER_LK"_Stz,
  "_ER_PK"_Stz,
  "_ER_QK"_Stz,
  "_ER_QR"_Stz,
  "_ER_RE"_Stz,
  "_ER_RO"_Stz,
  "_ER_SC"_Stz,
  "_ER_SR"_Stz,
  "_ER_SS"_Stz,
  "_ER_TD"_Stz,
  "_ER_TK"_Stz,
  "_ER_TN"_Stz,
  "_ER_TP"_Stz,
  "_ER_TT"_Stz,
  "_ER_WT"_Stz,
  
  "_ER_ENUMS"_Stz,
};
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class EnumReg
  {
  public:
    static Stz* szEnumText(i8 iEnumCategory, i8 i_Enum);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
