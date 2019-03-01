/*============================================================================*/
/* EnumGroups.cpp -- Enum Groups                                              */
/*============================================================================*/

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* dump enums                                                                 */
/*----------------------------------------------------------------------------*/
Stz* EnumGroups::szEnumText(i8 iEnumGroup, i8 i_Enum)
{
  switch (iEnumGroup) {
      
    case _EG_AC: return aAC[i_Enum];
    case _EG_AK: return aAK[i_Enum];
    case _EG_AN: return aAN[i_Enum];
    case _EG_AT: return aAT[i_Enum];
    case _EG_CN: return aCN[i_Enum];
    case _EG_CT: return aCT[i_Enum];
    case _EG_DC: return aDC[i_Enum];
    case _EG_DL: return aDL[i_Enum];
    case _EG_DM: return aDM[i_Enum];
    case _EG_DS: return aDS[i_Enum];
    case _EG_EC: return aEC[i_Enum];
    case _EG_HT: return aHT[i_Enum];
    case _EG_IF: return aIF[i_Enum];
    case _EG_IS: return aIS[i_Enum];
    case _EG_IT: return aIT[i_Enum];
    case _EG_JF: return aJF[i_Enum];
    case _EG_JK: return aJK[i_Enum];
    case _EG_JL: return aJL[i_Enum];
    case _EG_JP: return aJP[i_Enum];
    case _EG_JS: return aJS[i_Enum];
    case _EG_JT: return aJT[i_Enum];
    case _EG_KC: return aKC[i_Enum];
    case _EG_LK: return aLK[i_Enum];
    case _EG_PK: return aPK[i_Enum];
    case _EG_PY: return aPY[i_Enum];
    case _EG_QK: return aQK[i_Enum];
      //case _EG_QR: return aQR[i_Enum];
    case _EG_RE: return aRE[i_Enum];
    case _EG_RO: return aRO[i_Enum];
    case _EG_SC: return aSC[i_Enum];
    case _EG_SR: return aSR_Short[i_Enum];
    case _EG_SS: return aSS[i_Enum];
    case _EG_TD: return aTD[i_Enum];
    case _EG_TK: return aTK[i_Enum];
    case _EG_TN: return aTN[i_Enum];
    case _EG_TP: return aTP[i_Enum];
    case _EG_TT: return aTT[i_Enum];
    case _EG_WT: return aWT[i_Enum];
      
    default:
      vTrace("EnumGroups::szEnumText: Unknown enum group %lld\n"_Stz, iEnumGroup);
      return nullptr;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
