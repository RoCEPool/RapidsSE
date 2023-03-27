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
bool CodeSynthUtils::bExtract(File* pFile, Stz* sLabel, Stz* sLabelNext, Stz* sLabelLoop, Stz* szFunMkr, Stz* szStartMkr, Stz* szEndMkr, String* sCode)
{

  i8 nFileSize = pFile->iSize();
  i8 nStringSize = sCode->nArea();

  s4K sLine;
  bool bFoundFunc = false;
  bool bFoundStart = false;

  //if ( ! sLabel->bSame(""_Stz) )
  //  StrLib::vSprintCat(sCode->szStz(), "\n%s:\n"_Stz, sLabel->azChar());

  while (pFile->bReadLine(sLine)) {

    // look for comments
    if (StrLib::bContains(sLine.azChar(), "//")) {

      // look for function marker
      if ( ! bFoundFunc) {
        if (StrLib::bContains(sLine.azChar(), szFunMkr->azChar())) {
          //vTrace("FOUND FUNCTION MARKER - %s\n"_Stz, szFunMkr);
          bFoundFunc = true;
          continue;
        }
      }

      // look for start marker
      if (bFoundFunc && !bFoundStart) {
        if (StrLib::bContains(sLine.azChar(), szStartMkr->azChar())) {
          //vTrace("FOUND FUNCTION START MARKER - %s\n"_Stz, szStartMkr);
          bFoundStart = true;
          continue;
        }
      }

      // look for end marker
      if (bFoundStart) {
        if (StrLib::bContains(sLine.azChar(), szEndMkr->azChar())) {
          //vTrace("FOUND FUNCTION END MARKER - %s\n"_Stz, szEndMkr);
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

        if (StrLib::bContains(sLine.azChar(), "__L_LABEL__")) {
          RASSERT( ! sLabelLoop->bSame(""_Stz) );
          bool bResult = StrLib::bReplace(sLine.azChar(), "__L_LABEL__", sLabel->azChar());
          RASSERT(bResult);
        }

        if (StrLib::bContains(sLine.azChar(), "__L_LABEL_NEXT__")) {
          RASSERT( ! sLabelLoop->bSame(""_Stz) );
          bool bResult = StrLib::bReplace(sLine.azChar(), "__L_LABEL_NEXT__", sLabelNext->azChar());
          RASSERT(bResult);
        }

        if (StrLib::bContains(sLine.azChar(), "__L_LABEL_LOOP__")) {
          RASSERT( ! sLabelLoop->bSame(""_Stz) );
          bool bResult = StrLib::bReplace(sLine.azChar(), "__L_LABEL_LOOP__", sLabelLoop->azChar());
          RASSERT(bResult);
        }

        // copy line into sCode
        StrLib::vSprintCat(sCode->szStz(), "%s\n"_Stz, sLine.azChar());
      }
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
bool CodeSynthUtils::bExtractInflate(File* pFile, Stz* sColumn, Stz* sReturnToLabel, Stz* szFunMkr, Stz* szStartMkr, Stz* szEndMkr, String* sCode)
{

  i8 nFileSize = pFile->iSize();
  i8 nStringSize = sCode->nArea();

  s4K sLine;
  bool bFoundFunc = false;
  bool bFoundStart = false;

  while (pFile->bReadLine(sLine)) {

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

        if (StrLib::bContains(sLine.azChar(), "__COLUMN__")) {
          RASSERT( ! sColumn->bSame(""_Stz) );
          bool bResult = StrLib::bReplace(sLine.azChar(), "__COLUMN__", sColumn->azChar());
          RASSERT(bResult);
        }

        if (StrLib::bContains(sLine.azChar(), "__L_ReturnToColumn__")) {
          RASSERT( ! sReturnToLabel->bSame(""_Stz) );
          bool bResult = StrLib::bReplace(sLine.azChar(), "__L_ReturnToColumn__", sReturnToLabel->azChar());
          RASSERT(bResult);
        }

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
