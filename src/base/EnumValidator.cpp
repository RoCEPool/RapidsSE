/*============================================================================*/
/* EnumValidator.cpp -- Validates that the string arrays associated with enums*/
/* that frequently change are properly initialised.                           */
/*============================================================================*/

#include "BASE.h"
#include "EnumValidator.hpp"

#include "eAA.h"
#include "eCT.h"
#include "eEC.h"
#include "eEV.h"
#include "eFM.h"
#include "eLR.h"
#include "eOI.h"
#include "eOM.h"
#include "eSF.h"
#include "eXF.h"

using namespace BASE;

/*----------------------------------------------------------------------------*/
/* enum groups                                                                */
/*----------------------------------------------------------------------------*/
void EnumValidator::vValidateEnumStzArrays(void)
{

  struct Validator {
    Stz* m_pArrayElement;
    Stz* m_pArrayName;
    Stz* m_pEnumName;
  };

  // The list of enums that we wish to validate.
  struct Validator validations[] = {
      {       aAA[AA_ENUMS - 1],       "aAA[]"_Stz,   "eAA"_Stz },


      {       aEC[EC_ENUMS - 1],       "aEC[]"_Stz,   "eEC"_Stz },
      {      aEC2[EC_ENUMS - 1],      "aEC2[]"_Stz,   "eEC"_Stz },
      { aErrorMsg[EC_ENUMS - 1], "aErrorMsg[]"_Stz,   "eEC"_Stz },

      {       aCT[CT_ENUMS - 1],       "aCT[]"_Stz,   "eCT"_Stz },
      { szCT_char[CT_ENUMS - 1], "szCT_char[]"_Stz,   "eCT"_Stz },

      {       aEV[EV_ENUMS - 1],       "aEV[]"_Stz,   "eEV"_Stz },
      {       aFM[FM_ENUMS - 1],       "aFM[]"_Stz,   "eFM"_Stz },
      {       aLR[LR_ENUMS - 1],       "aLR[]"_Stz,   "eLR"_Stz },
      {       aOI[OI_ENUMS - 1],       "aOI[]"_Stz,   "eOI"_Stz },
      {       aOM[OM_ENUMS - 1],       "aOM[]"_Stz,   "eOM"_Stz },
      {       aSF[SF_ENUMS - 1],       "aSF[]"_Stz,   "eSF"_Stz },
      {       aXF[XF_ENUMS - 1],       "aXF[]"_Stz,   "eXF"_Stz },
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
  // eEC

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
