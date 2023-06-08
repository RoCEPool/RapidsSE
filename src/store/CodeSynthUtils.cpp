/*============================================================================*/
/* CodeSynthUtils.cpp -- code synthesiser utilities                           */
/*============================================================================*/

#include "CodeSynthUtils.hpp"
#include "StrLib.hpp"
#include "OpSketch.hpp"
#include "Table.hpp"
#include "Index.hpp"

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

        vReplaceColumnsInflated(sLine, pOpSketch);
        vReplaceColumnsToInflate(sLine, pOpSketch);
        vReplaceNumColumns(sLine, pOpSketch);

        vReplaceLabel(sLine, pOpSketch);
        vReplaceLabelNext(sLine, pOpSketch);
        vReplaceLabelLoop(sLine, pOpSketch);
        vReplaceLabelIndex(sLine, pOpSketch);

        vReplacePayloadBytes(sLine, pOpSketch);
        vReplaceKeyKind(sLine, pOpSketch);
        // cater to multiple replaces of IndexInfo
        if (bReplaceRowInfoTo(sLine, sReplacedLine, pOpSketch) )
          sLine = sReplacedLine;

        // cater to multiple replaces of IndexInfo
        if (bReplaceRowInfoFrom(sLine, sReplacedLine, pOpSketch) )
          sLine = sReplacedLine;

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

  bool bResult;
  s256 sColNm;
  sColNm = pColumn->szColNm();
  StrLib::vToUpper((char *) sColNm.azChar());

  if (StrLib::bContains(sLine->azChar(), "__UNION_TYPE__")) {
    switch(pColumn->eColumnType()) {
      case CT_BOOLEAN:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Bol_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_DATE:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Dte_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_DECIMAL:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Bcd_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_FLOAT:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Flt_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_INTEGER:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Int_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_INTERVAL_DT:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Ivl_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_INTERVAL_YM:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Ivl_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_TIME:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Tme_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_TIMESTAMP:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Tsp_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_UNKNOWN:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Unk_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_CHAR:
      case CT_VARCHAR:
        bResult = StrLib::bReplace(sLine->azChar(), "__UNION_TYPE__", "  union Vch_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      default:
        NEVER_REACH
    }

    RASSERT(bResult);
  }

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
        bResult = StrLib::bReplace(sLine->azChar(), "__INDEX_POSITION_LOWER__", "aExtern_IndexTreePositionGT");
        break;
      case IP_GE:
        bResult = StrLib::bReplace(sLine->azChar(), "__INDEX_POSITION_LOWER__", "aExtern_IndexTreePositionGE");
        break;
      default:
        RASSERT(0);
    }

    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__INDEX_POSITION_UPPER__")) {
    switch(pOpSketch->m_eUpperIP) {
      case IP_LT:
        bResult = StrLib::bReplace(sLine->azChar(), "__INDEX_POSITION_UPPER__", "aExtern_IndexTreePositionLT");
        RASSERT(bResult);
        break;
      case IP_LE:
        bResult = StrLib::bReplace(sLine->azChar(), "__INDEX_POSITION_UPPER__", "aExtern_IndexTreePositionLE");
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

  if (StrLib::bContains(sLine->azChar(), "__TABLENAME__")) {
    bool bResult = StrLib::bReplace(sLine->azChar(), "__TABLENAME__", pOpSketch->m_pTable->szTableNm()->azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__INDEXNAME__")) {
    bool bResult = StrLib::bReplace(sLine->azChar(), "__INDEXNAME__", pOpSketch->m_pIndex->szIndexNm()->azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__INDEX_INFO__")) {
    switch(pOpSketch->m_eIndexInfo) {
      case II_ST:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__INDEX_INFO__", "aDynamicT->m_xIndexInfoT");
        break;
      case II_JJ:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__INDEX_INFO__", "aDynamicT->m_xIndexInfoJ");
        break;
      case II_PL:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__INDEX_INFO__", "aDynamicT->m_xIndexInfoPL");
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
void CodeSynthUtils::vReplaceNumColumns(Stz* sLine, OpSketch* pOpSketch) {

  _static_

  if (StrLib::bContains(sLine->azChar(), "__LIMIT_ROWS__")) {
    bool bResult = StrLib::bReplace(sLine->azChar(), "__LIMIT_ROWS__", pOpSketch->m_sExprString.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__COLUMNS_IN__")) {
    s256 sColumn;
    StrLib::vSprint(sColumn, "%lld"_Stz, pOpSketch->m_pStreamDefLft->nColumns());
    bool bResult = StrLib::bReplace(sLine->azChar(), "__COLUMNS_IN__", sColumn.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__COLUMNS_OUT__")) {
    s256 sColumn;
    StrLib::vSprint(sColumn, "%lld"_Stz, pOpSketch->m_pStreamDefOut->nColumns());
    bool bResult = StrLib::bReplace(sLine->azChar(), "__COLUMNS_OUT__", sColumn.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__INDEX_COLUMNS__")) {
    s256 sColumn;
    StrLib::vSprint(sColumn, "%lld"_Stz, pOpSketch->m_pIndexDef->nColumns());
    bool bResult = StrLib::bReplace(sLine->azChar(), "__INDEX_COLUMNS__", sColumn.azChar());
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
void CodeSynthUtils::vReplaceLabelIndex(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  s256 sLabel;

  if (StrLib::bContains(sLine->azChar(), "__L_LABEL_INDEX_DEREF__")) {
    sLabel.vSprint("L_IndexDeref_%d"_Stz, pOpSketch->m_iOperatorNo);
    RASSERT( ! pOpSketch->m_sLabelLoop.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__L_LABEL_INDEX_DEREF__", sLabel.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azChar(), "__L_LABEL_INDEX_RETURN__")) {
    sLabel.vSprint("L_IndexReturn_%d"_Stz, pOpSketch->m_iOperatorNo);
    RASSERT( ! pOpSketch->m_sLabelLoop.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__L_LABEL_INDEX_RETURN__", sLabel.azChar());
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

  if (StrLib::bContains(sLine->azChar(), "__L_LABEL_OUTER_LOOP__")) {
    RASSERT( ! pOpSketch->m_sLabelOuterLoop.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azChar(), "__L_LABEL_OUTER_LOOP__", pOpSketch->m_sLabelOuterLoop.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceColumnsInflated(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__COLUMNS_INFLATED__")) {

    s256 sColumnsInflated;
    if (pOpSketch->m_eSketchKind == SK_INDEXJOIN) {
      RASSERT( pOpSketch->m_pStreamDefLft );
      StrLib::vSprint(sColumnsInflated,"%lld"_Stz,pOpSketch->m_pStreamDefLft->nColumns());
    }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_INDEXSCAN);
      StrLib::vSprint(sColumnsInflated,"0"_Stz);
    }
    bool bResult = StrLib::bReplace(sLine->azChar(), "__COLUMNS_INFLATED__", sColumnsInflated.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceColumnsToInflate(Stz* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azChar(), "__COLUMNS_TO_INFLATE_")) {

    s256 sColumnsToInflate;
    if (pOpSketch->m_eSketchKind == SK_INDEXJOIN) {
      RASSERT(pOpSketch->m_pStreamDefRht);
      StrLib::vSprint(sColumnsToInflate, "%lld"_Stz, pOpSketch->m_pStreamDefRht->nColumns());
    }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_INDEXSCAN);
      StrLib::vSprint(sColumnsToInflate,"%lld"_Stz,pOpSketch->m_pStreamDefLft->nColumns());
    }

    bool bResult = StrLib::bReplace(sLine->azChar(), "__COLUMNS_TO_INFLATE_", sColumnsToInflate.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
bool CodeSynthUtils::bReplaceRowInfoFrom(Stz* sLine, Stz* sReplacedLine, OpSketch* pOpSketch)
{
  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azChar(), "__ROW_INFO_FROM__")) {
    RASSERT( pOpSketch->m_eStencilFrom != SR_NONE );
    switch(pOpSketch->m_eStencilFrom) {
      case SR_AT:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoAT");
        RASSERT(bResult);
        break;
      case SR_IJ:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoIJ");
        RASSERT(bResult);
        break;
      case SR_IT:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoIT");
        RASSERT(bResult);
        break;
      case SR_PL:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoPL");
        RASSERT(bResult);
        break;
      case SR_PO:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoTO");
        RASSERT(bResult);
        break;
      case SR_PT:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoTO");
        RASSERT(bResult);
        break;
      case SR_TO:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoTO");
        RASSERT(bResult);
        break;
      case SR_TJ:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoTJ");
        RASSERT(bResult);
        break;
      default:
        NEVER_REACH;
    }
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
bool CodeSynthUtils::bReplaceRowInfoTo(Stz* sLine, Stz* sReplacedLine, OpSketch* pOpSketch)
{
  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azChar(), "__ROW_INFO_TO__")) {
    RASSERT( pOpSketch->m_eStencilTo != SR_NONE );
    switch(pOpSketch->m_eStencilTo) {
      case SR_AT:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoAT");
        RASSERT(bResult);
        break;
      case SR_IJ:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoIJ");
        RASSERT(bResult);
        break;
      case SR_IT:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoIT");
        RASSERT(bResult);
        break;
      case SR_PL:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoPL");
        RASSERT(bResult);
        break;
      case SR_PO:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoPO");
        RASSERT(bResult);
        break;
      case SR_PT:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoPT");
        RASSERT(bResult);
        break;
      case SR_TO:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoTO");
        RASSERT(bResult);
        break;
      case SR_TJ:
        bResult = StrLib::bReplace(sReplacedLine->azChar(), sLine->azChar(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoTJ");
        RASSERT(bResult);
        break;
      default:
        NEVER_REACH;
    }
    return true;
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
