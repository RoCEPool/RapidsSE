/*============================================================================*/
/* CodeSynthUtils.cpp -- code synthesiser utilities                           */
/*============================================================================*/

#include "CodeSynthUtils.hpp"
#include "StrLib.hpp"
#include "OpSketch.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
bool CodeSynthUtils::bExtract(File* pFile, OpSketch* pOpSketch, Stz* szFunMkr,
                                String* sCode, Column* pColumn, i8 iPayLoadIndex)
{
  _static_

  i8 nFileSize = pFile->iSize();
  i8 nStringSize = sCode->nArea();

  s4K sLine;
  s4K sReplacedLine;
  bool bFoundFunc = false;
  bool bFoundStart = false;

  //if ( ! sLabel->bSame(""_Stz) )
  //  StrLib::vSprintCat(sCode->szStz(), "\n%s:\n"_Stz, sLabel->azChar());

  sLine = ""_Stz;

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
        if (StrLib::bContains(sLine.azChar(), "marker-start")) {
          //vTrace("FOUND FUNCTION START MARKER - %s\n"_Stz, szStartMkr);
          bFoundStart = true;
          continue;
        }
      }

      if (pColumn)
        vReplaceColumnNo(sLine, pColumn);

      // look for end marker
      if (bFoundStart) {
        if (StrLib::bContains(sLine.azChar(), "marker-end")) {
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

        if (pColumn)
          vReplaceColumnName(sLine, pColumn);

        if (iPayLoadIndex != C_UNKNOWN)
          vReplacePayloadIndex(sLine, iPayLoadIndex);

        vReplaceStructTo(sLine, pOpSketch);
        vReplaceStructFrom(sLine, pOpSketch);

        vReplaceLeftColumns(sLine, pOpSketch);
        vReplaceRightColumns(sLine, pOpSketch);

        vReplaceLabel(sLine, pOpSketch);
        vReplaceLabelNext(sLine, pOpSketch);
        vReplaceLabelLoop(sLine, pOpSketch);

        vReplacePayloadBytes(sLine, pOpSketch);
        vReplaceKeyKind(sLine, pOpSketch);
        vReplaceStringArea(sLine, pOpSketch);

        // cater to multiple replaces of IndexInfo
        if (bReplaceIndexInfo(sLine, sReplacedLine, pOpSketch) )
          sLine = sReplacedLine;

        vReplaceIndexPosition(sLine, pOpSketch);

        // copy line into sCode
        StrLib::vSprintCat(sCode->szStz(), "%s\n"_Stz, sLine.azChar());
      }
    }
    sLine = ""_Stz;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
bool CodeSynthUtils::bExtractInflate(File* pFile, Column* pColumn, Stz* sReturnToLabel, Stz* szFunMkr, String* sCode)
{

  _static_

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
          //vTrace("FOUND FUNCTION MARKER - %s\n"_Stz, szFunMkr);
          bFoundFunc = true;
          continue;
        }
      }

      // look for start marker
      if (bFoundFunc && !bFoundStart) {
        if (StrLib::bContains(sLine.azChar(), "marker-start")) {
          //vTrace("FOUND FUNCTION START MARKER - marker-start\n"_Stz);
          bFoundStart = true;
          continue;
        }
      }

      // look for end marker
      if (bFoundStart) {
        if (StrLib::bContains(sLine.azChar(), "marker-end")) {
          //vTrace("FOUND FUNCTION END MARKER - marker-end\n"_Stz);
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

        vReplaceColumnNo(sLine, pColumn);
        vReplaceReturnToLabel(sLine, sReturnToLabel);

        // copy line into sCode
        StrLib::vSprintCat(sCode->szStz(), "%s\n"_Stz, sLine.azChar());
      }
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceColumnNo(Stz* sLine, Column* pColumn) {

  _static_

  if (StrLib::bContains(sLine->azChar(), "__COLUMN_NO__")) {
    s256 sColumn;
    StrLib::vSprint(sColumn, "%lld"_Stz, pColumn->iColumnNo());
    bool bResult = StrLib::bReplace(sLine->azChar(), "__COLUMN_NO__", sColumn.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceColumnName(Stz* sLine, Column* pColumn) {

  _static_

  s256 sColNm;
  sColNm = pColumn->szColNm();
  StrLib::vToUpper((char *) sColNm.azChar());

  if (StrLib::bContains(sLine->azChar(), "__COLUMN_TO__")) {
    bool bResult = StrLib::bReplace(sLine->azChar(), "__COLUMN_TO__", sColNm.azChar());
    RASSERT(bResult);
  }
  if (StrLib::bContains(sLine->azChar(), "__COLUMN_FROM__")) {
    bool bResult = StrLib::bReplace(sLine->azChar(), "__COLUMN_FROM__", sColNm.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceIndexPosition(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azChar(), "__INDEX_POSITION_LOWER__")) {
    switch(pOpSketch->m_eLowerIP) {
      case IP_GT:
        bResult = StrLib::bReplace(sLine->azChar(), "__INDEX_POSITION_LOWER__", "aIndexTree_positionGT");
        break;
      case IP_GE:
        bResult = StrLib::bReplace(sLine->azChar(), "__INDEX_POSITION_LOWER__", "aIndexTree_positionGE");
        break;
      default:
        RASSERT(0);
    }

    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__INDEX_POSITION_UPPER__")) {
    switch(pOpSketch->m_eUpperIP) {
      case IP_LT:
        bResult = StrLib::bReplace(sLine->azChar(), "__INDEX_POSITION_UPPER__", "aIndexTree_positionLT");
        RASSERT(bResult);
        break;
      case IP_LE:
        bResult = StrLib::bReplace(sLine->azChar(), "__INDEX_POSITION_UPPER__", "aIndexTree_positionLE");
        RASSERT(bResult);
        break;
      default:
        RASSERT(0);
    }

  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
bool CodeSynthUtils::bReplaceIndexInfo(Stz* sLine, Stz* sReplacedLine, OpSketch* pOpSketch)
{
  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azChar(), "__INDEX_INFO__")) {
    switch(pOpSketch->m_eIndexInfo) {
      case II_ST:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__INDEX_INFO__", "m_xIndexInfoT");
        break;
      case II_JJ:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__INDEX_INFO__", "m_xIndexInfoJ");
        break;
      case II_PL:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__INDEX_INFO__", "m_xIndexInfo");
        break;
      default:
        RASSERT(0);
    }
    return true;
  }

  return false;
}
/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceKeyKind(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__KEY_KIND__")) {
    s256 sKey;
    StrLib::wToString((i8) pOpSketch->m_pIndexDef->eKeyKind(), sKey.azChar());
    bool bResult = StrLib::bReplace(sLine->azChar(), "__KEY_KIND__", sKey.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplacePayloadIndex(Stz* sLine, i8 iPayLoadIndex)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__PAYLOAD_INDEX__")) {
    s256 sPayLoadIndex;
    StrLib::vSprint(sPayLoadIndex,"%lld"_Stz,iPayLoadIndex);
    bool bResult = StrLib::bReplace(sLine->azChar(), "__PAYLOAD_INDEX__", sPayLoadIndex.azChar());
    RASSERT(bResult);
  }

}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplacePayloadBytes(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__PAYLOADBYTES__")) {
    RASSERT( pOpSketch->m_nPayloadBytes );
    s256 sPayLoadBytes;
    StrLib::wToString(pOpSketch->m_nPayloadBytes, sPayLoadBytes.azChar());
    bool bResult = StrLib::bReplace(sLine->azChar(), "__PAYLOADBYTES__", sPayLoadBytes.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceReturnToLabel(Stz* sLine, Stz* sReturnToLabel)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__L_RETURN_TO_LABEL__")) {
    RASSERT(!sReturnToLabel->bSame(""_Stz));
    bool bResult = StrLib::bReplace(sLine->azChar(), "__L_RETURN_TO_LABEL__", sReturnToLabel->azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceStructTo(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "p__STRUCT_TO__")) {
    RASSERT( pOpSketch->m_eStencilTo != SR_NONE );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__STRUCT_TO__", (char*)aSR[pOpSketch->m_eStencilTo]);
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__STRUCT_TO__")) {
    RASSERT( pOpSketch->m_eStencilTo != SR_NONE );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__STRUCT_TO__", (char*)aSR[pOpSketch->m_eStencilTo]);
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceStructFrom(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "p__STRUCT_FROM__")) {
    RASSERT( pOpSketch->m_eStencilFrom != SR_NONE );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__STRUCT_FROM__", (char*)aSR[pOpSketch->m_eStencilFrom]);
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__STRUCT_FROM__")) {
    RASSERT( pOpSketch->m_eStencilFrom != SR_NONE );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__STRUCT_FROM__", (char*)aSR[pOpSketch->m_eStencilFrom]);
    RASSERT(bResult);
  }

}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceLabel(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__L_LABEL__")) {
    RASSERT( ! pOpSketch->m_sLabel.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__L_LABEL__", pOpSketch->m_sLabel.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceLabelNext(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__L_LABEL_NEXT__")) {
    RASSERT( ! pOpSketch->m_sLabelNext.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__L_LABEL_NEXT__", pOpSketch->m_sLabelNext.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceLabelLoop(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__L_LABEL_LOOP__")) {
    RASSERT( ! pOpSketch->m_sLabelLoop.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__L_LABEL_LOOP__", pOpSketch->m_sLabelLoop.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceLeftColumns(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__LEFT_COLUMNS__")) {
    RASSERT( pOpSketch->m_pStreamDefLft );
    s256 sLeftColumns;
    StrLib::vSprint(sLeftColumns,"%lld"_Stz,pOpSketch->m_pStreamDefLft->nColumns());
    bool bResult = StrLib::bReplace(sLine->azChar(), "__LEFT_COLUMNS__", sLeftColumns.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceRightColumns(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__RIGHT_COLUMNS__")) {
    RASSERT( pOpSketch->m_pStreamDefRht );
    s256 sRightColumns;
    StrLib::vSprint(sRightColumns,"%lld"_Stz,pOpSketch->m_pStreamDefRht->nColumns());
    bool bResult = StrLib::bReplace(sLine->azChar(), "__RIGHT_COLUMNS__", sRightColumns.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceStringArea(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__STRING_AREA__")) {
    RASSERT( ! pOpSketch->m_sLabelLoop.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__STRING_AREA__", (char*)aSRArea[pOpSketch->m_eStencilTo]);
    RASSERT(bResult);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
