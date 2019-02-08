/*============================================================================*/
/* EnumRegistry.cpp -- Enum Helper                                            */
/*============================================================================*/

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* dump enums                                                                 */
/*----------------------------------------------------------------------------*/
Stz* EnumReg::szEnumText(i8 iEnumCategory, i8 i_Enum)
{
  switch (iEnumCategory) {
      
  case _ER_AC: return aAC[i_Enum];
  case _ER_AK: return aAK[i_Enum];
  case _ER_AT: return aAT[i_Enum];
  case _ER_CN: return aCN[i_Enum];
  case _ER_CT: return aCT[i_Enum];
  case _ER_DC: return aDC[i_Enum];
  case _ER_DM: return aDM[i_Enum];
  case _ER_DS: return aDS[i_Enum];
  case _ER_EC: return aEC[i_Enum];
  case _ER_HT: return aHT[i_Enum];
  case _ER_IF: return aIF[i_Enum];
  case _ER_IS: return aIS[i_Enum];
  case _ER_IT: return aIT[i_Enum];
  case _ER_JC: return aJC[i_Enum];
  case _ER_JK: return aJK[i_Enum];
  case _ER_JL: return aJL[i_Enum];
  case _ER_JP: return aJP[i_Enum];
  case _ER_JS: return aJS[i_Enum];
  case _ER_JT: return aJT[i_Enum];
  case _ER_LK: return aLK[i_Enum];
  case _ER_PK: return aPK[i_Enum];
  case _ER_QK: return aQK[i_Enum];
  //case _ER_QR: return aQR[i_Enum];
  case _ER_RE: return aRE[i_Enum];
  case _ER_RO: return aRO[i_Enum];
  case _ER_SC: return aSC[i_Enum];
  case _ER_SR: return aSR[i_Enum];
  case _ER_SS: return aSS[i_Enum];
  case _ER_TD: return aTD[i_Enum];
  case _ER_TK: return aTK[i_Enum];
  case _ER_TN: return aTN[i_Enum];
  case _ER_TP: return aTP[i_Enum];
  case _ER_TT: return aTT[i_Enum];
  case _ER_WT: return aWT[i_Enum];
      
  default:
    vTrace("Unknown enum category %lld\n"_Stz, iEnumCategory);
    return nullptr;
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
