/*============================================================================*/
/* CodeSynthUtils.cpp -- code synthesiser utilities                           */
/*============================================================================*/

#include "CodeSynthUtils.hpp"
//#include "Fragments/Code.hpp"
#include "StrLib.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
bool CodeSynthUtils::bExtract(File* pFile, Stz* szFunMkr, Stz* szStartMkr, Stz* szEndMkr, String* sCode)
{

  i8 nFileSize = pFile->iSize();
  i8 nStringSize = sCode->nArea();

  s4K sLine;
  bool bFoundFunc = false;
  bool bFoundStart = false;

  while (pFile->bReadLine(sLine)) {
    //vTrace("%s\n"_Stz,sLine.szStz());

    // look for comments
    if (StrLib::bContains(sLine.azChar(), "//")) {

      // look for function marker
      if ( ! bFoundFunc) {
        if (StrLib::bContains(sLine.azChar(), szFunMkr->azChar())) {
          vTrace("FOUND FUNCTION MARKER - %s\n"_Stz, szFunMkr);
          bFoundFunc = true;
          continue;
        }
      }

      // look for start marker
      if (bFoundFunc && !bFoundStart) {
        if (StrLib::bContains(sLine.azChar(), szStartMkr->azChar())) {
          vTrace("FOUND FUNCTION START MARKER - %s\n"_Stz, szStartMkr);
          bFoundStart = true;
          continue;
        }
      }

      // look for end marker
      if (bFoundStart) {
        if (StrLib::bContains(sLine.azChar(), szEndMkr->azChar())) {
          vTrace("FOUND FUNCTION END MARKER - %s\n"_Stz, szEndMkr);
          return true;
        }
        else {

          // copy line into sCode
          StrLib::vSprintCat(sCode->szStz(), "%s\n"_Stz, sLine.azChar());
        }
      }
    }
    else {
      if (bFoundStart) {
        // copy line into sCode
        StrLib::vSprintCat(sCode->szStz(), "%s\n"_Stz, sLine.azChar());
      }
    }
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
