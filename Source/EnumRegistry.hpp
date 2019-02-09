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
enum e_EG // enum registry
{
  _EG_NONE = 0,
  
  _EG_AC,
  _EG_AK,
  _EG_AN,
  _EG_AT,
  _EG_CN,
  _EG_CT,
  _EG_DC,
  _EG_DM,
  _EG_DS,
  _EG_EC,
  _EG_HT,
  _EG_IF,
  _EG_IS,
  _EG_IT,
  _EG_JC,
  _EG_JK,
  _EG_JL,
  _EG_JP,
  _EG_JS,
  _EG_JT,
  _EG_LK,
  _EG_PK,
  _EG_QK,
  _EG_QR,
  _EG_RE,
  _EG_RO,
  _EG_SC,
  _EG_SR,
  _EG_SS,
  _EG_TD,
  _EG_TK,
  _EG_TN,
  _EG_TP,
  _EG_TT,
  _EG_WT,
  
  _EG_ENUMS
};
  
static Stz* a_EG[]
{
  "_EG_NONE"_Stz,
  
  "_EG_AC"_Stz,
  "_EG_AK"_Stz,
  "_EG_AT"_Stz,
  "_EG_CN"_Stz,
  "_EG_CT"_Stz,
  "_EG_DC"_Stz,
  "_EG_DM"_Stz,
  "_EG_DS"_Stz,
  "_EG_EC"_Stz,
  "_EG_HT"_Stz,
  "_EG_IF"_Stz,
  "_EG_IS"_Stz,
  "_EG_IT"_Stz,
  "_EG_JC"_Stz,
  "_EG_JK"_Stz,
  "_EG_JL"_Stz,
  "_EG_JP"_Stz,
  "_EG_JS"_Stz,
  "_EG_JT"_Stz,
  "_EG_LK"_Stz,
  "_EG_PK"_Stz,
  "_EG_QK"_Stz,
  "_EG_QR"_Stz,
  "_EG_RE"_Stz,
  "_EG_RO"_Stz,
  "_EG_SC"_Stz,
  "_EG_SR"_Stz,
  "_EG_SS"_Stz,
  "_EG_TD"_Stz,
  "_EG_TK"_Stz,
  "_EG_TN"_Stz,
  "_EG_TP"_Stz,
  "_EG_TT"_Stz,
  "_EG_WT"_Stz,
  
  "_EG_ENUMS"_Stz,
};
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class EnumGroup
  {
  public:
    static Stz* szEnumText(i8 iEnumGroup, i8 i_Enum);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
