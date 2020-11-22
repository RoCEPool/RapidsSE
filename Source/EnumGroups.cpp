/*============================================================================*/
/* EnumGroups.cpp -- Enums helper                                             */
/*============================================================================*/

#include "Enums.h"

/*----------------------------------------------------------------------------*/
/* get enum text                                                              */
/*----------------------------------------------------------------------------*/
Stz* EnumGroups::szEnumText(i8 i_EnumGroup, i8 i_Enum)
{
  switch (i_EnumGroup) {
      
    case EG_AA: return aAA[i_Enum];
    case EG_AC: return aAC[i_Enum];
    case EG_AK: return aAK[i_Enum];
    //case EG_AN: return aAN[i_Enum];
    case EG_AT: return aAT[i_Enum];
             
    case EG_BO: return aBO[i_Enum];
      
    case EG_CC: return aCC[i_Enum];
    case EG_CK: return aCK[i_Enum];
    case EG_CM: return aCM[i_Enum];
    case EG_CN: return aCN[i_Enum];
    case EG_CP: return aCP[i_Enum];
    //case EG_CS: return aCS[i_Enum];
    case EG_CT: return aCT[i_Enum];
    case EG_CX: return aCX[i_Enum];

    case EG_DA: return aDA[i_Enum];
    case EG_DC: return aDC[i_Enum];
    case EG_DG: return aDG[i_Enum];
    case EG_DL: return aDL[i_Enum];
    case EG_DM: return aDM[i_Enum];
    case EG_DS: return aDS[i_Enum];
      
    case EG_EA: return aEA[i_Enum];
    case EG_EC: return aEC[i_Enum];
    case EG_ED: return aED[i_Enum];
    case EG_EO: return aEO[i_Enum];
    case EG_EP: return aEP[i_Enum];

    case EG_FA: return aFA[i_Enum];
    case EG_FN: return aFN[i_Enum];
    case EG_FU: return aFU[i_Enum];

    case EG_GA: return aGA[i_Enum];

    case EG_HA: return aHA[i_Enum];
    case EG_HS: return aHS[i_Enum];
    case EG_HT: return aHT[i_Enum];
      
    case EG_IK: return aIK[i_Enum];
    case EG_IS: return aIS[i_Enum];
    case EG_IT: return aIT[i_Enum];
      
    case EG_JA: return aJA[i_Enum];
    case EG_JF: return aJF[i_Enum];
    case EG_JK: return aJK[i_Enum];
    case EG_JL: return aJL[i_Enum];
    case EG_JP: return aJP[i_Enum];
    case EG_JS: return aJS[i_Enum];
      
    case EG_KC: return aKC[i_Enum];
    case EG_KE: return aKE[i_Enum];
    case EG_KK: return aKK[i_Enum];

    case EG_LA: return aLA[i_Enum];
    case EG_LG: return aLG[i_Enum];
    case EG_LH: return aLH[i_Enum];
    case EG_LK: return aLK[i_Enum];
      
    case EG_MA: return aMA[i_Enum];
    case EG_MC: return aMC[i_Enum];
    case EG_MK: return aMK[i_Enum];
    case EG_MM: return aMM[i_Enum];
    case EG_MT: return aMT[i_Enum];

    case EG_NE: return aNE[i_Enum];
    case EG_NK: return aNK[i_Enum];
    case EG_NS: return aNS[i_Enum];
      
    case EG_OA: return aOA[i_Enum];
    case EG_OC: return aOC[i_Enum];
    case EG_OI: return aOI[i_Enum];
    case EG_OK: return aOK[i_Enum];
    case EG_OM: return aOM[i_Enum];
    case EG_OP: return aOP[i_Enum];
    case EG_OR: return aOR[i_Enum];

    case EG_PA: return aPA[i_Enum];
    case EG_PF: return aPF[i_Enum];
    case EG_PK: return aPK[i_Enum];
    case EG_PO: return aPO[i_Enum];
    case EG_PP: return aPP[i_Enum];
    case EG_PT: return aPT[i_Enum];
    case EG_PX: return aPX[i_Enum];
    case EG_PY: return aPY[i_Enum];

    case EG_QA: return aQA[i_Enum];
    case EG_QK: return aQK[i_Enum];
    case EG_QN: return aQN[i_Enum];

    case EG_RA: return aRA[i_Enum];
    case EG_RE: return aRE[i_Enum];
    case EG_RG: return aRG[i_Enum];
    case EG_RO: return aRO[i_Enum];
    case EG_RS: return aRS[i_Enum];
    case EG_RU: return aRU[i_Enum];
      
    case EG_SC: return aSC[i_Enum];
    case EG_SI: return aSI[i_Enum];
    case EG_SK: return aSK[i_Enum];
    case EG_SL: return aSL[i_Enum];
    case EG_SS: return aSS[i_Enum];
    case EG_ST: return aST[i_Enum];

    case EG_TA: return aTA[i_Enum];
    case EG_TN: return aTN[i_Enum];
    case EG_TP: return aTP[i_Enum];
    case EG_TQ: return aTQ[i_Enum];
    case EG_TR: return aTR[i_Enum];
    case EG_TS: return aTS[i_Enum];
    case EG_TT: return aTT[i_Enum];
      
    case EG_UF: return aUF[i_Enum];
    case EG_UK: return aUK[i_Enum];

    case EG_VA: return aVA[i_Enum];
    case EG_VL: return aVL[i_Enum];
    case EG_VP: return aVP[i_Enum];

    case EG_WK: return aWK[i_Enum];

    case EG_XA: return aXA[i_Enum];
    case EG_XK: return aXK[i_Enum];
    case EG_XP: return aXP[i_Enum];

    case EG_YA: return aYA[i_Enum];

    case EG_ZK: return aZK[i_Enum];

    default:
      vTrace("AM::szEnumText: Unknown enum group %lld\n"_Stz, i_EnumGroup);
      return nullptr;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
