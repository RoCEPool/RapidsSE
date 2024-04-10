/*============================================================================*/
/* EnumValidator.cpp -- Validates that the string arrays associated with enums*/
/* that frequently change are properly initialised.                           */
/*============================================================================*/

#include "BASE.h"
#include "EnumValidator.hpp"

#include "eAA.h"
#include "eAC.h"
#include "eAD.h"
#include "eAG.h"
#include "eAK.h"
#include "eAL.h"
#include "eAR.h"
#include "eAT.h"
#include "eBK.h"
#include "eBO.h"
#include "eCC.h"
#include "eCF.h"
#include "eCK.h"
#include "eCM.h"
#include "eCS.h"
#include "eCT.h"
#include "eCX.h"
#include "eDA.h"
#include "eDC.h"
#include "eDK.h"
#include "eDL.h"
#include "eDM.h"
#include "eDR.h"
#include "eDS.h"
#include "eDU.h"
#include "eEC.h"
#include "eED.h"
#include "eEK.h"
#include "eEV.h"
#include "eFK.h"
#include "eFM.h"
#include "eFN.h"
#include "eFU.h"
#include "eFX.h"
#include "eGA.h"
#include "eGC.h"
#include "eHA.h"
#include "eHK.h"
#include "eHS.h"
#include "eHT.h"
#include "eII.h"
#include "eIK.h"
#include "eIP.h"
#include "eIS.h"
#include "eIT.h"
#include "eIU.h"
#include "eIV.h"
#include "eJF.h"
#include "eJL.h"
#include "eJP.h"
#include "eJT.h"
#include "eKC.h"
#include "eKE.h"
#include "eKK.h"
#include "eLA.h"
#include "eLE.h"
#include "eLG.h"
#include "eLH.h"
#include "eLR.h"
#include "eMA.h"
#include "eMD.h"
#include "eMC.h"
#include "eMG.h"
#include "eMM.h"
#include "eMS.h"
#include "eMT.h"
#include "eNE.h"
#include "eNS.h"
#include "eOA.h"
#include "eOH.h"
#include "eOK.h"
#include "eOM.h"
#include "eOR.h"
#include "eOT.h"
#include "ePA.h"
#include "ePD.h"
#include "ePE.h"
#include "ePF.h"
#include "ePK.h"
#include "ePL.h"
#include "ePR.h"
#include "ePT.h"
#include "eQC.h"
#include "eQD.h"
#include "eQF.h"
#include "eQK.h"
#include "eQN.h"
#include "eQO.h"
#include "eRA.h"
#include "eRF.h"
#include "eRG.h"
#include "eRK.h"
#include "eRN.h"
#include "eRO.h"
#include "eRS.h"
#include "eRU.h"
#include "eRV.h"
#include "eSC.h"
#include "eSD.h"
#include "eSF.h"
#include "eSI.h"
#include "eSK.h"
#include "eSR.h"
#include "eST.h"
#include "eSY.h"
#include "eTA.h"
#include "eTE.h"
#include "eTI.h"
#include "eTN.h"
#include "eTP.h"
#include "eTQ.h"
#include "eTR.h"
#include "eTS.h"
#include "eTT.h"
#include "eUF.h"
#include "eUK.h"
#include "eUT.h"
#include "eVA.h"
#include "eVB.h"
#include "eWC.h"
#include "eWK.h"
#include "eXA.h"
#include "eXC.h"
#include "eXF.h"
#include "eXK.h"
#include "eXP.h"
#include "eYF.h"
#include "eYG.h"
#include "eYS.h"
#include "eYX.h"
#include "DbGenInfo.hpp"

using namespace BASE;

/*----------------------------------------------------------------------------*/
/* enum groups                                                                */
/*----------------------------------------------------------------------------*/
void EnumValidator::vValidateEnumStzArrays(void)
{
  // A struct to hold the 3 bits of information we need to validate
  // an enum string array and produce an error message.
  struct Validator {
    Stz* m_pArrayElement;
    Stz* m_pArrayName;
    Stz* m_pEnumName;
  };

  // The list of enums that we wish to validate.
  // See note below about how this was generated.
  //
  // Please add any new enums to this list.
  struct Validator validations[] = {

      {           aAA[AA_ENUMS - 1],          "aAA[]"_Stz,     "eAA"_Stz },
      {     aAA_short[AA_ENUMS - 1],    "aAA_short[]"_Stz,     "eAA"_Stz },
      {           aAC[AC_ENUMS - 1],          "aAC[]"_Stz,     "eAC"_Stz },
      {           aAD[AD_ENUMS - 1],          "aAD[]"_Stz,     "eAD"_Stz },
      {           aAG[AG_ENUMS - 1],          "aAG[]"_Stz,     "eAG"_Stz },
      {           aAK[AK_ENUMS - 1],          "aAK[]"_Stz,     "eAK"_Stz },
      {           aAL[AL_ENUMS - 1],          "aAL[]"_Stz,     "eAL"_Stz },
      {           aAR[AR_ENUMS - 1],          "aAR[]"_Stz,     "eAR"_Stz },
      {           aAT[AT_ENUMS - 1],          "aAT[]"_Stz,     "eAT"_Stz },
      {           aBK[BK_ENUMS - 1],          "aBK[]"_Stz,     "eBK"_Stz },
      {           aBO[BO_ENUMS - 1],          "aBO[]"_Stz,     "eBO"_Stz },
			{           aBT[BO_ENUMS - 1],          "aBT[]"_Stz,     "eBT"_Stz },
      {           aCC[CC_ENUMS - 1],          "aCC[]"_Stz,     "eCC"_Stz },
      {           aCF[CF_ENUMS - 1],          "aCF[]"_Stz,     "eCF"_Stz },
      {           aCK[CK_ENUMS - 1],          "aCK[]"_Stz,     "eCK"_Stz },
      {     aCK_short[CK_ENUMS - 1],    "aCK_short[]"_Stz,     "eCK"_Stz },
      {           aCM[CM_ENUMS - 1],          "aCM[]"_Stz,     "eCM"_Stz },
      {           aCT[CT_ENUMS - 1],          "aCT[]"_Stz,     "eCT"_Stz },
      {     szCT_char[CT_ENUMS - 1],    "szCT_char[]"_Stz,     "eCT"_Stz },
      {           aCX[CX_ENUMS - 1],          "aCX[]"_Stz,     "eCX"_Stz },
      {           aDA[DA_ENUMS - 1],          "aDA[]"_Stz,     "eDA"_Stz },
      {     aDA_short[DA_ENUMS - 1],    "aDA_short[]"_Stz,     "eDA"_Stz },
      {    aDBG_TblNme[DBG_ENUMS-1],  "aDBG_TblNme[]"_Stz,     "eDBG"_Stz},
      //{      aDBG_Info[DBG_ENUMS-1][DBG_MAX_COL],    "aDBG_Info[][]"_Stz,     "eDBG"_Stz},
      {           aDC[DC_ENUMS - 1],          "aDC[]"_Stz,     "eDC"_Stz },
      {           aDK[DK_ENUMS - 1],          "aDK[]"_Stz,     "eDK"_Stz },
      {           aDL[DL_ENUMS - 1],          "aDL[]"_Stz,     "eDL"_Stz },
      {           aDM[DM_ENUMS - 1],          "aDM[]"_Stz,     "eDM"_Stz },
      {           aDR[DR_ENUMS - 1],          "aDR[]"_Stz,     "eDR"_Stz },
      {     aDR_short[DR_ENUMS - 1],    "aDR_short[]"_Stz,     "eDR"_Stz },
      {           aDS[DS_ENUMS - 1],          "aDS[]"_Stz,     "eDS"_Stz },
      {           aDU[DU_ENUMS - 1],          "aDU[]"_Stz,     "eDU"_Stz },
      {           aEC[EC_ENUMS - 1],          "aEC[]"_Stz,     "eEC"_Stz },
      {          aEC2[EC_ENUMS - 1],         "aEC2[]"_Stz,     "eEC"_Stz },
      {     aErrorMsg[EC_ENUMS - 1],    "aErrorMsg[]"_Stz,     "eEC"_Stz },
      {           aED[ED_ENUMS - 1],          "aED[]"_Stz,     "eED"_Stz },
      {           aEK[EK_ENUMS - 1],          "aEK[]"_Stz,     "eEK"_Stz },
      {           aEV[EV_ENUMS - 1],          "aEV[]"_Stz,     "eEV"_Stz },
      {           aFK[FK_ENUMS - 1],          "aFK[]"_Stz,     "eFK"_Stz },
      {           aFM[FM_ENUMS - 1],          "aFM[]"_Stz,     "eFM"_Stz },
      {           aFN[FN_ENUMS - 1],          "aFN[]"_Stz,     "eFN"_Stz },
      {           aFU[FU_ENUMS - 1],          "aFU[]"_Stz,     "eFU"_Stz },
      {           aFX[FX_ENUMS - 1],          "aFX[]"_Stz,     "eFX"_Stz },
      {           aGA[GA_ENUMS - 1],          "aGA[]"_Stz,     "eGA"_Stz },
      {     aGA_short[GA_ENUMS - 1],    "aGA_short[]"_Stz,     "eGA"_Stz },
      {           aGC[GC_ENUMS - 1],          "aGC[]"_Stz,     "eGC"_Stz },
      {           aHA[HA_ENUMS - 1],          "aHA[]"_Stz,     "eHA"_Stz },
      {     aHA_short[HA_ENUMS - 1],    "aHA_short[]"_Stz,     "eHA"_Stz },
      {           aHK[HK_ENUMS - 1],          "aHK[]"_Stz,     "eHK"_Stz },
      {           aHS[HS_ENUMS - 1],          "aHS[]"_Stz,     "eHS"_Stz },
      {           aHT[HT_ENUMS - 1],          "aHT[]"_Stz,     "eHT"_Stz },
      {     aHT_short[HT_ENUMS - 1],    "aHT_short[]"_Stz,     "eHT"_Stz },
      {           aII[II_ENUMS - 1],          "aII[]"_Stz,     "eII"_Stz },
      {           aIK[IK_ENUMS - 1],          "aIK[]"_Stz,     "eIK"_Stz },
      {        aIK_3c[IK_ENUMS - 1],       "aIK_3c[]"_Stz,     "eIK"_Stz },
      {            aIP[IP_ENUM - 1],          "aIP[]"_Stz,     "eIP"_Stz },
      {           aIS[IS_ENUMS - 1],          "aIS[]"_Stz,     "eIS"_Stz },
      {           aIT[IT_ENUMS - 1],          "aIT[]"_Stz,     "eIT"_Stz },
      {           aIU[IU_ENUMS - 1],          "aIU[]"_Stz,     "eIU"_Stz },
      {           aIV[IV_ENUMS - 1],          "aIV[]"_Stz,     "eIV"_Stz },
      {     aIV_field[IV_ENUMS - 1],    "aIV_field[]"_Stz,     "eIV"_Stz },
      {           aJF[JF_ENUMS - 1],          "aJF[]"_Stz,     "eJF"_Stz },
      {           aJL[JL_ENUMS - 1],          "aJL[]"_Stz,     "eJL"_Stz },
      {           aJP[JP_ENUMS - 1],          "aJP[]"_Stz,     "eJP"_Stz },
      {          aJT[JT_NENUMS - 1],          "aJT[]"_Stz,     "eJT"_Stz },
      {   aJT_pretty[JT_NENUMS - 1],   "aJT_pretty[]"_Stz,     "eJT"_Stz },
      {           aKC[KC_ENUMS - 1],          "aKC[]"_Stz,     "eKC"_Stz },
      {           aKE[KE_ENUMS - 1],          "aKE[]"_Stz,     "eKE"_Stz },
      {           aKK[KK_ENUMS - 1],          "aKK[]"_Stz,     "eKK"_Stz },
      {       aLA[LA_LCK_ENUMS - 1],          "aLA[]"_Stz,     "eLA"_Stz },
      { aLA_short[LA_LCK_ENUMS - 1],    "aLA_short[]"_Stz,     "eLA"_Stz },
      {           aLE[LE_ENUMS - 1],          "aLE[]"_Stz,     "eLE"_Stz },
      {           aLG[LG_ENUMS - 1],          "aLG[]"_Stz,     "eLG"_Stz },
      {           aLH[LH_ENUMS - 1],          "aLH[]"_Stz,     "eLH"_Stz },
      {           aLR[LR_ENUMS - 1],          "aLR[]"_Stz,     "eLR"_Stz },
      {       aMA[MA_MNG_ENUMS - 1],          "aMA[]"_Stz,     "eMA"_Stz },
      { aMA_short[MA_MNG_ENUMS - 1],    "aMA_short[]"_Stz,     "eMA"_Stz },
      {           aMC[MC_ENUMS - 1],          "aMC[]"_Stz,     "eMC"_Stz },
      {           aMG[MG_ENUMS - 1],          "aMG[]"_Stz,     "eMG"_Stz },
      {           aMM[MM_ENUMS - 1],          "aMM[]"_Stz,     "eMM"_Stz },
      {           aMS[MS_ENUMS - 1],          "aMS[]"_Stz,     "eMS"_Stz },
      {           aMT[MT_ENUMS - 1],          "aMT[]"_Stz,     "eMT"_Stz },
      {          aNE[NE_ENUMT_ - 1],          "aNE[]"_Stz,     "eNE"_Stz },
      {           aNS[NS_ENUMS - 1],          "aNS[]"_Stz,     "eNS"_Stz },
      {           aOA[OA_ENUMS - 1],          "aOA[]"_Stz,     "eOA"_Stz },
      {           aOH[OH_ENUMS - 1],          "aOH[]"_Stz,     "eOH"_Stz },
      {     aOH_short[OH_ENUMS - 1],    "aOH_short[]"_Stz,     "eOH"_Stz },
      {           aOK[OK_ENUMS - 1],          "aOK[]"_Stz,     "eOK"_Stz },
      {           aOM[OM_ENUMS - 1],          "aOM[]"_Stz,     "eOM"_Stz },
      {           aOR[OR_ENUMS - 1],          "aOR[]"_Stz,     "eOR"_Stz },
      {           aOT[OT_ENUMS - 1],          "aOT[]"_Stz,     "eOT"_Stz },
      {       aPA[PA_PRM_ENUMS - 1],          "aPA[]"_Stz,     "ePA"_Stz },
      { aPA_short[PA_PRM_ENUMS - 1],    "aPA_short[]"_Stz,     "ePA"_Stz },
      {           aPD[PD_ENUMS - 1],          "aPD[]"_Stz,     "ePD"_Stz },
      {           aPE[PE_ENUMS - 1],          "aPE[]"_Stz,     "ePE"_Stz },
      {          aPF[PF_ENUMT_ - 1],          "aPF[]"_Stz,     "ePF"_Stz },
      {           aPK[PK_ENUMS - 1],          "aPK[]"_Stz,     "ePK"_Stz },
      {           aPL[PL_ENUMS - 1],          "aPL[]"_Stz,     "ePL"_Stz },
      {           aPR[PR_ENUMS - 1],          "aPR[]"_Stz,     "ePR"_Stz },
      {           aPT[PT_ENUMS - 1],          "aPT[]"_Stz,     "ePT"_Stz },
      {           aQC[QC_ENUMS - 1],          "aQC[]"_Stz,     "eQC"_Stz },
      {           aQD[QD_ENUMS - 1],          "aQD[]"_Stz,     "eQD"_Stz },
      {           aQF[QF_ENUMS - 1],          "aQF[]"_Stz,     "eQF"_Stz },
      {           aQK[QK_ENUMS - 1],          "aQK[]"_Stz,     "eQK"_Stz },
      {           aQN[QN_ENUMS - 1],          "aQN[]"_Stz,     "eQN"_Stz },
      {           aQO[QO_ENUMS - 1],          "aQO[]"_Stz,     "eQO"_Stz },
      {           aRA[RA_ENUMS - 1],          "aRA[]"_Stz,     "eRA"_Stz },
      {     aRA_short[RA_ENUMS - 1],    "aRA_short[]"_Stz,     "eRA"_Stz },
      {           aRF[RF_ENUMS - 1],          "aRF[]"_Stz,     "eRF"_Stz },
      {           aRG[RG_ENUMS - 1],          "aRG[]"_Stz,     "eRG"_Stz },
      {           aRK[RK_ENUMS - 1],          "aRK[]"_Stz,     "eRK"_Stz },
      {     aRK_short[RK_ENUMS - 1],    "aRK_short[]"_Stz,     "eRK"_Stz },
      {           aRN[RN_ENUMS - 1],          "aRN[]"_Stz,     "eRN"_Stz },
      {      aRN_desc[RN_ENUMS - 1],     "aRN_desc[]"_Stz,     "eRN"_Stz },
      {           aRO[RO_ENUMS - 1],          "aRO[]"_Stz,     "eRO"_Stz },
      {          aRS[RS_ENUMT_ - 1],          "aRS[]"_Stz,     "eRS"_Stz },
      {           aRU[RU_ENUMS - 1],          "aRU[]"_Stz,     "eRU"_Stz },
      {           aRV[RV_ENUMS - 1],          "aRV[]"_Stz,     "eRV"_Stz },
      {         aSC[C_SC_ENUMS - 1],          "aSC[]"_Stz,     "eSC"_Stz },
      {           aSD[SD_ENUMS - 1],          "aSD[]"_Stz,     "eSD"_Stz },
      {     aSD_short[SD_ENUMS - 1],    "aSD_short[]"_Stz,     "eSD"_Stz },
      {           aSF[SF_ENUMS - 1],          "aSF[]"_Stz,     "eSF"_Stz },
      {           aSI[SI_ENUMS - 1],          "aSI[]"_Stz,     "eSI"_Stz },
      {           aSK[SK_ENUMS - 1],          "aSK[]"_Stz,     "eSK"_Stz },
      {           aSR[SR_ENUMS - 1],          "aSR[]"_Stz,     "eSR"_Stz },
      {           aST[ST_ENUMS - 1],          "aST[]"_Stz,     "eST"_Stz },
      {    aST_string[ST_ENUMS - 1],   "aST_string[]"_Stz,     "eST"_Stz },
      {           aSY[SY_ENUMS - 1],          "aSY[]"_Stz,     "eSY"_Stz },
      {           aTA[TA_ENUMS - 1],          "aTA[]"_Stz,     "eTA"_Stz },
      {     aTA_short[TA_ENUMS - 1],    "aTA_short[]"_Stz,     "eTA"_Stz },
      {           aTE[TE_ENUMS - 1],          "aTE[]"_Stz,     "eTE"_Stz },
      {           aTI[TI_ENUMS - 1],          "aTI[]"_Stz,     "eTI"_Stz },
      {           aTN[TN_ENUMS - 1],          "aTN[]"_Stz,     "eTN"_Stz },
      {           aTP[TP_ENUMS - 1],          "aTP[]"_Stz,     "eTP"_Stz },
      {           aTQ[TQ_ENUMS - 1],          "aTQ[]"_Stz,     "eTQ"_Stz },
      {           aTR[TR_ENUMS - 1],          "aTR[]"_Stz,     "eTR"_Stz },
      {           aTS[TS_ENUMS - 1],          "aTS[]"_Stz,     "eTS"_Stz },
      {           aTT[TT_ENUMS - 1],          "aTT[]"_Stz,     "eTT"_Stz },
      {           aUF[UF_ENUMS - 1],          "aUF[]"_Stz,     "eUF"_Stz },
      {           aUK[UK_ENUMS - 1],          "aUK[]"_Stz,     "eUK"_Stz },
      {           aUT[UT_ENUMS - 1],          "aUT[]"_Stz,     "eUT"_Stz },
      {           aVA[VA_ENUMS - 1],          "aVA[]"_Stz,     "eVA"_Stz },
      {     aVA_short[VA_ENUMS - 1],    "aVA_short[]"_Stz,     "eVA"_Stz },
      {           aVB[VB_ENUMS - 1],          "aVB[]"_Stz,     "eVB"_Stz },
      {           aWC[WC_ENUMS - 1],          "aWC[]"_Stz,     "eWC"_Stz },
      {           aWK[WK_ENUMS - 1],          "aWK[]"_Stz,     "eWK"_Stz },
      {       aXA[XA_TSK_ENUMS - 1],          "aXA[]"_Stz,     "eXA"_Stz },
      { aXA_short[XA_TSK_ENUMS - 1],    "aXA_short[]"_Stz,     "eXA"_Stz },
      {           aXC[XC_ENUMS - 1],          "aXC[]"_Stz,     "eXC"_Stz },
      {      aXC_note[XC_ENUMS - 1],     "aXC_note[]"_Stz,     "eXC"_Stz },
      {           aXF[XF_ENUMS - 1],          "aXF[]"_Stz,     "eXF"_Stz },
      {           aXK[XK_ENUMS - 1],          "aXK[]"_Stz,     "eXK"_Stz },
      {           aXP[XP_ENUMS - 1],          "aXP[]"_Stz,     "eXP"_Stz },
      {           aYF[YF_ENUMS - 1],          "aYF[]"_Stz,     "eYF"_Stz },
      {           aYG[YG_ENUMS - 1],          "aYG[]"_Stz,     "eYG"_Stz },
      {       aYG_15s[YG_ENUMS - 1],      "aYG_15s[]"_Stz,     "eYG"_Stz },
      {     aYG_weave[YG_ENUMS - 1],    "aYG_weave[]"_Stz,     "eYG"_Stz },
      {           aYS[YS_ENUMS - 1],          "aYS[]"_Stz,     "eYS"_Stz },
      {           aYX[YX_ENUMS - 1],          "aYX[]"_Stz,     "eYX"_Stz },
  };

  const i8 nElements = sizeof(validations) / sizeof(validations[0]);

  for (i8 i=0; i<nElements; i++) {
    Validator xValidator = validations[i];

    if (xValidator.m_pArrayElement == nullptr) {
      Stz* szFmt = "Error: The string array %s for enum group %s has not been fully initialised. "
                   "At least the last element is a nullptr. Please check that there is a string "
                   "entry for all enum values.\n"_Stz;
      vTrace(szFmt, xValidator.m_pArrayName, xValidator.m_pEnumName);
      RASSERT(0);
    }
  }

  // NOTE: The above initialisation list was initially produced by a bash script
  //       rather than by hand. The bash script is as follows below:
  //
  //  #!/usr/local/bin/bash
  //
  //  #set -x
  //
  //  INCLUDES=()
  //
  //  for file in base/e*.h; do
  //
  //    ENUM_NAME="$(echo "${file}" | sed -e 's|base/||' -e 's|.h||')"
  //    ENUM_2_LETTERS="${ENUM_NAME:1}"
  //
  //    INCLUDES+=( "$(echo "${file}" | sed -e 's|base/||')"  )
  //
  //    STZ_ARR=()
  //    mapfile -t STZ_ARR < <(grep "extern Stz\* " "${file}")
  //  #  echo "${STZ_ARR[@]}"
  //
  //    for stz in "${STZ_ARR[@]}"; do
  //      ARR_CMD="$(echo "${stz}" | sed -e 's|extern Stz\* ||' -e 's|];||')"
  //      ARR_CMD="${ARR_CMD} - 1]"
  //
  //      ARRAY_NAME="$(echo "${stz}" | sed -e 's|extern Stz\* ||' -e 's|\[.*$||')"
  //
  //  #    echo "${ARR_CMD}"
  //  #    echo "${ARRAY_NAME}"
  //
  //      elem1="${ARR_CMD}"
  //      elem2="\"${ARRAY_NAME}[]\"_Stz"
  //      elem3="\"${ENUM_NAME}\"_Stz"
  //
  //      printf "{ %30s,%30s,%12s },\n" "${elem1}" "${elem2}" "${elem3}"
  //    done
  //  done
  //
  //  echo
  //  echo
  //  echo
  //  echo
  //
  //  for inc in "${INCLUDES[@]}"; do
  //    echo "#include \"${inc}\""
  //  done

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
