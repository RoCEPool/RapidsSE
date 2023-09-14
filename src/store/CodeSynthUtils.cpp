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
                                String* sCode, Column* pColumn, i8 iIndexColumn)
{
  _static_

  s4K sLine;
  s4K sReplacedLine;
  bool bFoundFunc = false;
  bool bFoundStart = false;

  //if ( ! sLabel->bSame(""_Stz) )
  //  sCode->vCatSprint("\n%s:\n"_Stz, sLabel->azChar());

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

      vReplaceOperatorName(sLine, pOpSketch);

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
          sCode->vCatSprint("%s\n"_Stz, sLine.azChar());
        }
      }
    }
    else {
      if (bFoundStart) {

        if (bGenerateExprStack(sLine, sCode, pOpSketch)) {
          sLine = ""_Stz;
          continue;
        }

        vReplaceOperatorName(sLine, pOpSketch);

        if (pColumn)
          vReplaceColumnName(sLine, pColumn);

        if (iIndexColumn != C_UNKNOWN)
          vReplaceIndexColumn(sLine, iIndexColumn);

        vReplaceStructTo(sLine, pOpSketch);
        vReplaceStructFrom(sLine, pOpSketch);

        vReplaceColumnsInflated(sLine, pOpSketch);
        vReplaceColumnsToInflate(sLine, pOpSketch);
        vReplaceNumColumns(sLine, pOpSketch);

        vReplaceLabel(sLine, pOpSketch);
        vReplaceLabelNext(sLine, pOpSketch);
        vReplaceLabelLoop(sLine, pOpSketch);
        vReplaceLabelIndex(sLine, pOpSketch);
				vReplaceLabelJoinNulls(sLine, pOpSketch);

        vReplacePayloadBytes(sLine, pOpSketch);
        vReplaceKeyKind(sLine, pOpSketch);

        vReplaceInsertEnt(sLine, pOpSketch);

        vReplaceTubPot(sLine, pOpSketch);

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
        sCode->vCatSprint("%s\n"_Stz, sLine.azChar());
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
          sCode->vCatSprint("%s\n"_Stz, sLine.azChar());
        }
      }
    }
    else {
      if (bFoundStart) {

        vReplaceColumnNo(sLine, pColumn);
        vReplaceReturnToLabel(sLine, sReturnToLabel);

        // copy line into sCode
        sCode->vCatSprint("%s\n"_Stz, sLine.azChar());
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
bool CodeSynthUtils::bGenerateExprStack(Stz* sLine, String* sCode, OpSketch* pOpSketch)
{
  _static_

  //s4K sCopy;
  TypeValueArea* pTypedCell;

  if (StrLib::bContains(sLine->azChar(), "__NULLSTACK__")) {
    TypedStack* pTypedStack = pOpSketch->m_pNullStack;

    i8 nExpr = pTypedStack ? pTypedStack->nValues() - 1 : C_UNKNOWN ;

    while ( nExpr >= 0) {
      pTypedCell = pTypedStack->aPeekItem(nExpr--);
      Stz* sExpr = pTypedCell->szStz();
      sCode->vCatSprint("\n %s"_Stz, sExpr->azChar());
    }
    return true;
  }

  if (StrLib::bContains(sLine->azChar(), "__EXPRSTACK__")) {
    TypedStack* pTypedStack = pOpSketch->m_pExprStack;

    i8 nExpr = pTypedStack ? pTypedStack->nValues() - 1 : C_UNKNOWN ;

    while ( nExpr >= 0) {
      pTypedCell = pTypedStack->aPeekItem(nExpr--);
      Stz* sExpr = pTypedCell->szStz();
      sCode->vCatSprint("\n %s"_Stz, sExpr->azChar());
    }
    return true;
  }

  if (StrLib::bContains(sLine->azChar(), "__CONSTRAINTSTACK__")) {
    TypedStack* pTypedStack = pOpSketch->m_pConstraintStack;

    i8 nExpr = pTypedStack ? pTypedStack->nValues() - 1 : C_UNKNOWN ;

    while ( nExpr >= 0) {
      pTypedCell = pTypedStack->aPeekItem(nExpr--);
      Stz* sExpr = pTypedCell->szStz();
      sCode->vCatSprint("\n %s"_Stz, sExpr->azChar());
    }
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceColumnNo(String* sLine, Column* pColumn) {

  _static_

  if (StrLib::bContains(sLine->azStr(), "__COLUMN_NO__")) {
    s256 sColumn;
    sColumn.vSprint("%lld"_Stz, pColumn->iColumnNo());
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__COLUMN_NO__", sColumn.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceTubPot(String* sLine, OpSketch* pOpSketch) {

  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azStr(), "__EXTERN_ROWPIX__")) {
    if (pOpSketch->m_eSketchKind == SK_TUB) {
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__EXTERN_ROWPIX__", "vExtern_TubRowPix");
      RASSERT(bResult);
   }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_POT);
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__EXTERN_ROWPIX__", "vExtern_PotRowPix");
      RASSERT(bResult);
    }
  }

  if (StrLib::bContains(sLine->azStr(), "__EXTERN_ALLOTROW__()")) {
    if (pOpSketch->m_eSketchKind == SK_TUB) {
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__EXTERN_ALLOTROW__()",
                                 "aExtern_TubAllotRow((void*)aDynamicT->m_pTub,&aDynamicT->m_xRowInfoTO.m_acStrArea,&aDynamicT->m_iErrorCode)");
      RASSERT(bResult);
    }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_POT);
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__EXTERN_ALLOTROW__()",
                                 "aExtern_PotAllotRow((void*)aDynamicT->m_pPot,iKeyHash,&aDynamicT->m_xRowInfoPO.m_acStrArea,&aDynamicT->m_iErrorCode)");

      RASSERT(bResult);
    }
  }

  if (StrLib::bContains(sLine->azStr(), "__EXTERN_ROWSTRSIZE")) {
    if (pOpSketch->m_eSketchKind == SK_TUB) {
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__EXTERN_ROWSTRSIZE__",
                                 "vExtern_TubRowStrSize(aDynamicT->m_pTub,nSrcSize)");
      RASSERT(bResult);
    }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_POT);
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__EXTERN_ROWSTRSIZE__",
                                 "vExtern_PotRowStrSize(aDynamicT->m_pPot,iKeyHash, nSrcSize)");
      RASSERT(bResult);
    }
  }

  if (StrLib::bContains(sLine->azStr(), "__TUB_POT__")) {
    if (pOpSketch->m_eSketchKind == SK_TUB) {
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__TUB_POT__", "aDynamicT->m_pTub");
      RASSERT(bResult);
    }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_POT);
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__TUB_POT__", "aDynamicT->m_pPot");
      RASSERT(bResult);
    }
  }

  if (StrLib::bContains(sLine->azStr(), "__POT__")) {
    if (pOpSketch->m_eSketchKind == SK_TUB) {
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__POT__", "false");
      RASSERT(bResult);
    }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_POT);
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__POT__", "true");
      RASSERT(bResult);
    }
  }

}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceInsertEnt(String* sLine, OpSketch* pOpSketch) {

  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azStr(), "__EXTERN_INSERTENT__")) {
    if (pOpSketch->m_eSketchKind == SK_GBYAGG ||
        pOpSketch->m_eSketchKind == SK_SORT) {
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__EXTERN_INSERTENT__()",
         "aExtern_IndexTreeInsertEnt(aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntInt, &aDynamicT->m_iErrorCode);");
      RASSERT(bResult);
    }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_TOP);

      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__EXTERN_INSERTENT__()",
        "aExtern_IndexTreeInsertEntTop(aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntInt, "
           "&aDynamicT->m_xIndexInfoPL.m_aEntBottom, __TOP_COUNT__, &aDynamicT->m_iErrorCode)");
      RASSERT(bResult);

      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__TOP_COUNT__",pOpSketch->m_sLimitExpr.azChar());
      RASSERT(bResult);
    }


  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceColumnName(String* sLine, Column* pColumn) {

  _static_

  bool bResult;
  s256 sColNm;
  sColNm = pColumn->szColNm();
  StrLib::vToUpper(sColNm.azChar(), sColNm.nAreaBytes());

  if (StrLib::bContains(sLine->azStr(), "__UNION_TYPE__")) {
    switch(pColumn->eColumnType()) {
      case CT_BOOLEAN:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Bol_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_DATE:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Dte_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_DECIMAL:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Bcd_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_FLOAT:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Flt_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_INTEGER:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Int_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_INTERVAL_DT:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Ivl_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_INTERVAL_YM:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Ivl_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_TIME:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Tme_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_TIMESTAMP:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Tsp_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_UNKNOWN:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Unk_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      case CT_CHAR:
      case CT_VARCHAR:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__UNION_TYPE__", "  union Vch_T __COLUMN_TO__");
        RASSERT(bResult);
        break;
      default:
        NEVER_REACH
    }

    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__COLUMN_TO__")) {
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__COLUMN_TO__", sColNm.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__COLUMN_FROM__")) {
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(), "__COLUMN_FROM__", sColNm.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceIndexPosition(String* sLine, OpSketch* pOpSketch)
{
  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azStr(), "__INDEX_POSITION_LOWER__")) {
    switch(pOpSketch->m_eLowerIP) {
      case IP_GT:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                   "__INDEX_POSITION_LOWER__", "aExtern_IndexTreePositionGT");
        break;
      case IP_GE:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                   "__INDEX_POSITION_LOWER__", "aExtern_IndexTreePositionGE");
        break;
      default:
        RASSERT(0);
    }

    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__INDEX_POSITION_UPPER__")) {
    switch(pOpSketch->m_eUpperIP) {
      case IP_LT:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                   "__INDEX_POSITION_UPPER__", "aExtern_IndexTreePositionLT");
        RASSERT(bResult);
        break;
      case IP_LE:
        bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                   "__INDEX_POSITION_UPPER__", "aExtern_IndexTreePositionLE");
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
bool CodeSynthUtils::bReplaceIndexInfo(String* sLine, String* sReplacedLine, OpSketch* pOpSketch)
{
  _static_

  bool bResult;
  UNUSED(bResult);

  if (StrLib::bContains(sLine->azStr(), "__TABLENAME__")) {
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__TABLENAME__", pOpSketch->m_pTable->szTableNm()->azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__INDEXNAME__")) {
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__INDEXNAME__", pOpSketch->m_pIndex->szIndexNm()->azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__INDEX_INFO__")) {
    switch(pOpSketch->m_eIndexInfo) {
      case II_ST:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__INDEX_INFO__", "aDynamicT->m_xIndexInfoT");
        break;
      case II_JJ:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__INDEX_INFO__", "aDynamicT->m_xIndexInfoJ");
        break;
      case II_PL:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__INDEX_INFO__", "aDynamicT->m_xIndexInfoPL");
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
void CodeSynthUtils::vReplaceKeyKind(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__KEY_KIND__")) {
    s256 sKey;
    StrLib::vToString(sKey.azChar(), sKey.nAreaBytes(), (i8) pOpSketch->m_pIndexDef->eKeyKind());
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__KEY_KIND__", sKey.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceNumColumns(String* sLine, OpSketch* pOpSketch) {

  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azStr(), "__LIMIT_ROWS__")) {
    if (pOpSketch->m_eSketchKind == SK_LIMIT)
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                 "__LIMIT_ROWS__", pOpSketch->m_sExprString.azChar());
    else
      bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                 "__LIMIT_ROWS__", pOpSketch->m_sLimitExpr.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__COLUMNS_IN__")) {
    s256 sColumn;
    sColumn.vSprint("%lld"_Stz, pOpSketch->m_pStreamDefLft->nColumns());
    bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                               "__COLUMNS_IN__", sColumn.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__COLUMNS_OUT__")) {
    s256 sColumn;
    sColumn.vSprint("%lld"_Stz, pOpSketch->m_pStreamDefOut->nColumns());
    bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                               "__COLUMNS_OUT__", sColumn.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__INDEX_COLUMNS__")) {
    s256 sColumn;
    sColumn.vSprint("%lld"_Stz, pOpSketch->m_pIndexDef->nColumns());
    bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                               "__INDEX_COLUMNS__", sColumn.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceIndexColumn(String* sLine, i8 iIndexColumn)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__INDEX_COLUMN__")) {
    s256 sIndexColumn;
    sIndexColumn.vSprint("%lld"_Stz,iIndexColumn);
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__INDEX_COLUMN__", sIndexColumn.azChar());
    RASSERT(bResult);
  }

}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplacePayloadBytes(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__PAYLOADBYTES__")) {
    //RASSERT( pOpSketch->m_nPayloadBytes );
    s256 sPayLoadBytes;
    StrLib::vToString(sPayLoadBytes.azChar(), sPayLoadBytes.nAreaBytes(), pOpSketch->m_nPayloadBytes);
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__PAYLOADBYTES__", sPayLoadBytes.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceReturnToLabel(String* sLine, Stz* sReturnToLabel)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__L_RETURN_TO_LABEL__")) {
    RASSERT(!sReturnToLabel->bSame(""_Stz));
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__L_RETURN_TO_LABEL__", sReturnToLabel->azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceStructTo(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "p__STRUCT_TO__")) {
    RASSERT( pOpSketch->m_eStencilTo != SR_NONE );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__STRUCT_TO__", (char*)aSR[pOpSketch->m_eStencilTo]);
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__STRUCT_TO__")) {
    RASSERT( pOpSketch->m_eStencilTo != SR_NONE );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__STRUCT_TO__", (char*)aSR[pOpSketch->m_eStencilTo]);
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceStructFrom(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "p__STRUCT_FROM__")) {
    RASSERT( pOpSketch->m_eStencilFrom != SR_NONE );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__STRUCT_FROM__", (char*)aSR[pOpSketch->m_eStencilFrom]);
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__STRUCT_FROM__")) {
    RASSERT( pOpSketch->m_eStencilFrom != SR_NONE );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__STRUCT_FROM__", (char*)aSR[pOpSketch->m_eStencilFrom]);
    RASSERT(bResult);
  }

}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceLabel(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__L_LABEL__")) {
    RASSERT( ! pOpSketch->m_sLabel.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__L_LABEL__", pOpSketch->m_sLabel.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceLabelIndex(String* sLine, OpSketch* pOpSketch)
{
  _static_

  s256 sLabel;

  if (StrLib::bContains(sLine->azStr(), "__L_LABEL_INDEX_DEREF__")) {
    sLabel.vSprint("L_IndexDeref_%d"_Stz, pOpSketch->m_iOperatorNo);
    RASSERT( ! pOpSketch->m_sLabelLoop.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__L_LABEL_INDEX_DEREF__", sLabel.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__L_LABEL_INDEX_RETURN__")) {
    sLabel.vSprint("L_IndexReturn_%d"_Stz, pOpSketch->m_iOperatorNo);
    RASSERT( ! pOpSketch->m_sLabelLoop.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__L_LABEL_INDEX_RETURN__", sLabel.azChar());
    RASSERT(bResult);
  }

}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceLabelNext(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__L_LABEL_NEXT__")) {
    RASSERT( ! pOpSketch->m_sLabelNext.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__L_LABEL_NEXT__", pOpSketch->m_sLabelNext.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceLabelJoinNulls(String* sLine, OpSketch* pOpSketch)
{
	_static_

	s256 sLabel;

	if (StrLib::bContains(sLine->azStr(), "__L_LABEL_JOIN_NULLS__")) {
		sLabel.vSprint("L_JoinNulls_%d"_Stz, pOpSketch->m_iOperatorNo);
		bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__L_LABEL_JOIN_NULLS__", sLabel.azChar());
		RASSERT(bResult);
	}
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceLabelLoop(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__L_LABEL_LOOP__")) {
    //RASSERT( ! pOpSketch->m_sLabelLoop.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__L_LABEL_LOOP__", pOpSketch->m_sLabelLoop.azChar());
    RASSERT(bResult);
  }

  if (StrLib::bContains(sLine->azStr(), "__L_LABEL_OUTER_LOOP__")) {
    RASSERT( ! pOpSketch->m_sLabelOuterLoop.bSame(""_Stz) );
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__L_LABEL_OUTER_LOOP__", pOpSketch->m_sLabelOuterLoop.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceColumnsInflated(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__COLUMNS_INFLATED__")) {

    s256 sColumnsInflated;
    if (pOpSketch->m_eSketchKind == SK_INDEXJOIN) {
      RASSERT( pOpSketch->m_pStreamDefLft );
      sColumnsInflated.vSprint("%lld"_Stz,pOpSketch->m_pStreamDefLft->nColumns());
    }
    else if (pOpSketch->m_eSketchKind == SK_SEMIJOIN ||
             pOpSketch->m_eSketchKind == SK_ANTIJOIN) {
      sColumnsInflated.vSprint("%lld"_Stz,0);
    }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_INDEXSCAN);
      sColumnsInflated.vSprint("0"_Stz);
    }
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__COLUMNS_INFLATED__", sColumnsInflated.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceColumnsToInflate(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__COLUMNS_TO_INFLATE_")) {

    s256 sColumnsToInflate;
    if ( pOpSketch->m_eSketchKind == SK_INDEXJOIN ||
         pOpSketch->m_eSketchKind == SK_SEMIJOIN  ||
         pOpSketch->m_eSketchKind == SK_ANTIJOIN ) {
      RASSERT(pOpSketch->m_pStreamDefRht);
      sColumnsToInflate.vSprint("%lld"_Stz, pOpSketch->m_pStreamDefRht->nColumns());
    }
    else {
      RASSERT(pOpSketch->m_eSketchKind == SK_INDEXSCAN);
      sColumnsToInflate.vSprint("%lld"_Stz,pOpSketch->m_pStreamDefLft->nColumns());
    }

    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__COLUMNS_TO_INFLATE_", sColumnsToInflate.azChar());
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
void CodeSynthUtils::vReplaceOperatorName(String* sLine, OpSketch* pOpSketch)
{
  _static_

  if (StrLib::bContains(sLine->azStr(), "__OPERATOR_NAME__")) {
    bool bResult = StrLib::bReplace(sLine->azStr(), sLine->nAreaBytes(),
                                    "__OPERATOR_NAME__", (char*)aSK[pOpSketch->m_eSketchKind]);
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Extract code for function from file                                        */
/*----------------------------------------------------------------------------*/
bool CodeSynthUtils::bReplaceRowInfoFrom(String* sLine, String* sReplacedLine, OpSketch* pOpSketch)
{
  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azStr(), "__ROW_INFO_FROM__")) {
    RASSERT( pOpSketch->m_eStencilFrom != SR_NONE );
    switch(pOpSketch->m_eStencilFrom) {
      case SR_AT:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoAT");
        RASSERT(bResult);
        break;
      case SR_IJ:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoIJ");
        RASSERT(bResult);
        break;
      case SR_IT:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoIT");
        RASSERT(bResult);
        break;
      case SR_PL:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoPL");
        RASSERT(bResult);
        break;
      case SR_PO:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoPO");
        RASSERT(bResult);
        break;
      case SR_PT:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoPT");
        RASSERT(bResult);
        break;
      case SR_TO:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoTO");
        RASSERT(bResult);
        break;
      case SR_TJ:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_FROM__", "aDynamicT->m_xRowInfoTJ");
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
bool CodeSynthUtils::bReplaceRowInfoTo(String* sLine, String* sReplacedLine, OpSketch* pOpSketch)
{
  _static_

  bool bResult;

  if (StrLib::bContains(sLine->azStr(), "__ROW_INFO_TO__")) {
    RASSERT( pOpSketch->m_eStencilTo != SR_NONE );
    switch(pOpSketch->m_eStencilTo) {
      case SR_AT:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoAT");
        RASSERT(bResult);
        break;
      case SR_IJ:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoIJ");
        RASSERT(bResult);
        break;
      case SR_IT:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoIT");
        RASSERT(bResult);
        break;
      case SR_PL:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoPL");
        RASSERT(bResult);
        break;
      case SR_PO:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoPO");
        RASSERT(bResult);
        break;
      case SR_PT:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoPT");
        RASSERT(bResult);
        break;
      case SR_TO:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoTO");
        RASSERT(bResult);
        break;
      case SR_TJ:
        bResult = StrLib::bReplace(sReplacedLine->azStr(), sReplacedLine->nAreaBytes(),
                                   sLine->azStr(), "__ROW_INFO_TO__", "aDynamicT->m_xRowInfoTJ");
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
