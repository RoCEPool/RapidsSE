
/*============================================================================*/
/* CodeSynth.cpp -- Code Synthesizer                                          */
/*============================================================================*/

#include "BASE.h"

#include "Stream.hpp"
#include "StreamDef.hpp"
#include "TokenVec.hpp"
#include "Token.hpp"
#include "LogicUnit.hpp"
#include "Index.hpp"
#include "MathLib.h"

RowRef xRowRef[]
{
  { "none"_Stz },   // SR_NONE

  {"SR_IJ"_Stz},    // SR_IJ
  {"SR_IT"_Stz},    // SR_IT
  {"SR_PJ"_Stz},    // SR_PJ
  {"SR_PT"_Stz},    // SR_PT
  {"SR_TJ"_Stz},    // SR_TJ
  {"SR_H"_Stz},     // SR_H

  {nullptr}
};

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CodeSynth,CN_CODESYNTH)
__aNewFunction(CodeSynth,CN_CODESYNTH)
__vDeleteFunction(CodeSynth,CN_CODESYNTH)
__aFooterFunction(CodeSynth)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CodeSynth::CodeSynth(void)
: Object(CN_CODESYNTH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
  
#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caCode,0xFF,C_FLATTEN_SOURCE,CN_CODESYNTH);
#endif

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_CODESYNTH);
#endif

  m_caCode[0]    = '\0';
  m_azCodeCrs    = m_caCode;

  m_pExprStack  = new TypedStack(KK_DATA_STACK);
  m_pPostExpr   = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_pInFixTknVec = new TokenVec;

  _checkInitialised_(CN_CODESYNTH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CodeSynth::~CodeSynth(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  delete m_pExprStack;
  delete m_pPostExpr;
  delete m_pInitStack;
  delete m_pInFixTknVec;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendFragment(Stz* szFragment, i8* piLineNumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar(), piLineNumber);
#else
  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar());
#endif
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendFragment(CodeSynth* pFragment, i8* piLineNumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
#ifdef WITH_DEBUG_VERSION
  StrLib::vAppendString(&m_azCodeCrs, pFragment->azCode(), piLineNumber);
#else
  StrLib::vAppendString(&m_azCodeCrs, pFragment->azCode());
#endif
}

/*----------------------------------------------------------------------------*/
/* print the synthesized code                                                 */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPrint(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  vTrace("%s\n"_Stz,m_caCode);
}

/*----------------------------------------------------------------------------*/
/* synthesize the filter expression                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthFilter(Stz* sFilterExpr, eSR eStencilRow, Stz* sLabel, Stz* sLabelNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCopy;

  StrLib::vAppendString(&m_azCodeCrs,"\n");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // FILTER-OP ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  // generate inFix from postFix
  vToInfix((char*)sFilterExpr, eStencilRow, nullptr, PJ_NONE);
  RASSERT(m_pExprStack->nValues() == 1);

  // convert postfix to inFix
  TypedCell* pTypedCell = m_pExprStack->aPeekItem(0);
  if (pTypedCell->eType() == CN_PTRVAL) {
    Token *Tkn = _pToken(m_pExprStack->pPopPtr(__func__));
    sInfix = Tkn->szChars();
  }
  else {
    m_pExprStack->vPopStr(sInfix, __func__);
  }

  sCopy.vSprint("\n\n%s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n\n  if (bTrace)");
  StrLib::vAppendString(&m_azCodeCrs, "\n      vRowSingle_Print(pRowSingle);\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  sCopy.vSprint("     p%s = (struct %s*)aRow;\n   if ( "_Stz, xRowRef[eStencilRow], xRowRef[eStencilRow].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, sInfix.azChar());

  StrLib::vAppendString(&m_azCodeCrs, " ) {");
  sCopy.vSprint("\n    goto %s;"_Stz, sLabelNext);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "   } else { \n");
  StrLib::vAppendString(&m_azCodeCrs, "     vRowSingle_DiscardStr(pRowSingle);\n");
  StrLib::vAppendString(&m_azCodeCrs, "     goto L_AdvanceNextRow;\n");
  StrLib::vAppendString(&m_azCodeCrs, "   }\n");

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the index join                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthIndexJoin(StreamDef* pStreamDefOut, StreamDef* pStreamDefLft, StreamDef* pStreamDefRht,
                                IndexDef* pIndexDef, eSR eStencilLeft, eSR eStencilRight,
                                eSR eStencilJoin, Stz* sJoinExpr, Stz* sLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
  s256 sCopy;
  s256 sCastRow;
  s256 sInfix;

  StrLib::vAppendString(&m_azCodeCrs,"\n");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // INDEXJOIN-OP ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n");

  // generate the infix from eStencilFrom
  vTrace("Postfix: %s\n"_Stz, sJoinExpr);
  vToInfix((char*)sJoinExpr, eStencilLeft, nullptr, PJ_NONE);
  RASSERT(m_pExprStack->nValues() == 1);

  StrLib::vAppendString(&m_azCodeCrs, "\n    static i8 iIndexJoinInit = 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void *aEntUpper;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void* pEntBuffer;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    char* racJoinArea;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    byte* aJoinRow;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void* amCellAddress;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 i_Ofs;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void* szSrcStz;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void* szDstStz;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 nSrcSize;");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the label
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n %s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // init index join
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // init indexJoin");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( iIndexJoinInit == 0 ) {");
  sCopy.vSprint("\n      vEntWrk_entKind(aCombinedT->m_pEntWrk,1,%d);"_Stz, pIndexDef->eKeyKind());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n      iIndexJoinInit = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the join key
  if (pIndexDef->eKeyKind()) {

    TypedCell *pTypedCell = m_pExprStack->aPeekItem(0);
    RASSERT(pTypedCell->eType() != CN_PTRVAL);
    sInfix = pTypedCell->szStz();

    StrLib::vAppendString(&m_azCodeCrs, "\n    // populate the entKey from T-table");
    sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKey = %s;"_Stz, sInfix.azChar());
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n    vEntWrk_print(aCombinedT->m_pEntWrk, aCombinedT->m_pIndexDef);");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs, "assert(0)");
  }

  // position at entry greater than equal to the join key (lbound)
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // position at entry >= join key (lbound)");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_aEntInt = aIndexTree_positionGE(aCombinedT->m_pIndexTree, &pEntBuffer, aCombinedT->m_aEntKey);");
  StrLib::vAppendString(&m_azCodeCrs, "\n    vEntWrk_print(aCombinedT->m_pEntWrk, aCombinedT->m_pIndexDef);");

  // find the upper bound entry
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // find the upper bound entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aEntUpper = aIndexTree_positionLE(aCombinedT->m_pIndexTree, aCombinedT->m_aEntKey);");

  // join rows
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    while (aCombinedT->m_aEntInt != aEntUpper) {");

  // get the crw from the entry
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // dereference the entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n      i8 iBuffer = aCombinedT->m_aEntInt->Rrf.m_uBuffer;");
  StrLib::vAppendString(&m_azCodeCrs, "\n      i8 iItem   = aCombinedT->m_aEntInt->Rrf.m_uItem;");
  StrLib::vAppendString(&m_azCodeCrs, "\n      void* aCrw  = ( (char*)C_ADR_REGION + iBuffer * C_1MB+ C_CRW_FIRSTHEAD+ iItem * C_CRW_HEADBYTES );");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // inflate the derefenced entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n      vInflate_into_RowSingle(aCombinedT->m_pRowSingleJ,aCrw);");

  StrLib::vAppendString(&m_azCodeCrs, "\n      if (bTrace)");
  StrLib::vAppendString(&m_azCodeCrs, "\n        vRowSingle_Print(aCombinedT->m_pRowSingleJ);");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the m_pRowSingleJ to eStencilRht structure
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // cast the inflated row to J-Table structure");
  StrLib::vAppendString(&m_azCodeCrs, "\n      void* ayRhtByte = (byte*)aRowSingle_GetCellArray(aCombinedT->m_pRowSingleJ,&racStrArea);");

  sCastRow.vSprint("\n      p%s = (struct %s*)ayRhtByte;\n"_Stz,xRowRef[eStencilRight].m_szInclude,
                   xRowRef[eStencilRight].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n      // allocate a joinRow from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aJoinRow = (byte*)aTub_AllotRow(pTub,&racJoinArea);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilJoin structure
  StrLib::vAppendString(&m_azCodeCrs, "\n      // cast the join row to TJ-Table structure");
  sCastRow.vSprint("\n      p%s = (struct %s*)aJoinRow;\n"_Stz,xRowRef[eStencilJoin].m_szInclude, xRowRef[eStencilJoin].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Join the rows of T and J

  // for each column of row of left table T
  for (i8 i_Col = 0; i_Col < pStreamDefLft->nColumns(); i_Col++) {

    Column *pColumn = pStreamDefLft->pGetColumn(i_Col);
    Column *pJoinColumn = pStreamDefOut->pGetColumn(i_Col);

    StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n      // copy left column %lld to joinRow"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
    StrLib::vAppendString(&m_azCodeCrs, "\n    ");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      StrLib::vAppendString(&m_azCodeCrs, "\n      // get the source string");
      sCopy.vSprint("\n      i_Ofs = p%s->%s;"_Stz, xRowRef[eStencilLeft].m_szInclude, pColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      sCopy.vSprint("\n      amCellAddress = (m8*) &p%s->%s;"_Stz, xRowRef[eStencilLeft].m_szInclude, pColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n      szSrcStz = ADDR_ADD(Stz*, amCellAddress, i_Ofs);");
      StrLib::vAppendString(&m_azCodeCrs, "\n      nSrcSize = strlen(szSrcStz) + 1;");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n      // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n      szDstStz = racJoinArea;");
      StrLib::vAppendString(&m_azCodeCrs, "\n      strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n      // copy the source to the destination");
      sCopy.vSprint("\n      amCellAddress = (m8*) &p%s->%s;"_Stz, xRowRef[eStencilJoin].m_szInclude, pJoinColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n      i_Ofs = - nDifference(racJoinArea,amCellAddress);");
      sCopy.vSprint("\n      p%s->%s = i_Ofs;"_Stz, xRowRef[eStencilJoin].m_szInclude, pJoinColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n      // update the tub string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n      racJoinArea += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n      vTub_RowStrSize(pTub,nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types
      sCopy.vSprint("\n      p%s->%s = p%s->%s;\n"_Stz,
                    xRowRef[eStencilJoin].m_szInclude, pColumn->szColNm(),
                    xRowRef[eStencilLeft].m_szInclude, pColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
  }

    // for each column of row of right table J
    i8 nLeftColumns = pStreamDefLft->nColumns();
    for (i8 i_Col = 0; i_Col < pStreamDefRht->nColumns(); i_Col++) {

      Column *pColumn     = pStreamDefRht->pGetColumn(i_Col);
      Column *pJoinColumn = pStreamDefOut->pGetColumn(i_Col + nLeftColumns);

      StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
      sCopy.vSprint("\n      // copy right column %lld to joinRow);"_Stz, i_Col);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
      StrLib::vAppendString(&m_azCodeCrs, "\n    ");

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // VARCHAR column
      if (pColumn->eColumnType() == CT_VARCHAR ||
          pColumn->eColumnType() == CT_CHAR) {
      StrLib::vAppendString(&m_azCodeCrs, "\n      // get the source string");
      sCopy.vSprint("\n      i_Ofs = p%s->%s;"_Stz, xRowRef[eStencilRight].m_szInclude, pColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      sCopy.vSprint("\n      amCellAddress = (m8*) &p%s->%s;"_Stz, xRowRef[eStencilRight].m_szInclude, pColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n      szSrcStz = ADDR_ADD(Stz*, amCellAddress, i_Ofs);");
      StrLib::vAppendString(&m_azCodeCrs, "\n      nSrcSize = strlen(szSrcStz) + 1;");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n      // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n      szDstStz = racJoinArea;");
      StrLib::vAppendString(&m_azCodeCrs, "\n      strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n      // copy the source to the destination");
      sCopy.vSprint("\n      amCellAddress = (m8*) &p%s->%s;"_Stz, xRowRef[eStencilJoin].m_szInclude, pJoinColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n      i_Ofs = - nDifference(racJoinArea,amCellAddress);");

      sCopy.vSprint("\n      p%s->%s = i_Ofs;"_Stz, xRowRef[eStencilJoin].m_szInclude, pJoinColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n      // update the tub string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n      racJoinArea += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n      vTub_RowStrSize(pTub,nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      } else {
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // all other types
        sCopy.vSprint("\n      p%s->%s = p%s->%s;\n"_Stz,
                      xRowRef[eStencilJoin].m_szInclude, pJoinColumn->szColNm(),
                      xRowRef[eStencilRight].m_szInclude, pColumn->szColNm());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      }
  }

  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n      // move to next entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aEntInt = aEntBuffer_nextEntry(&pEntBuffer, aCombinedT->m_aEntInt);");

  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
  StrLib::vAppendString(&m_azCodeCrs, "\n   goto L_AdvanceNextRow;\n");

  // eof
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\nL_ProcessEof:");
  StrLib::vAppendString(&m_azCodeCrs, "\n   goto L_Return;\n");

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the inflate                                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthInflate(StreamDef* pStreamDefIn, Stz* sTableNm, Stz* sLabel, Stz* sReturnLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sCopy;

  StrLib::vAppendString(&m_azCodeCrs,"\n");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // INFLATE ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n   %s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  for (i8 i_Col = 0; i_Col < pStreamDefIn->nColumns(); i_Col++) {
    StrLib::vSprintCat((Stz*)m_caCode, "\n"_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n    // - - - - - - - - - - - -  "_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n    i_Col = %d;"_Stz , i_Col);
    StrLib::vSprintCat((Stz*)m_caCode, "\n\n    uMetaCode = *auMetaCode++;"_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n    uHigh = uMetaCode >> 4;"_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n    uLow  = uMetaCode & 0x0F;"_Stz);

    if (i_Col < pStreamDefIn->nColumns() - 1) {
      StrLib::vSprintCat((Stz*)m_caCode, "\n\n    aSubReturnLabel = && L_Ret%s_Col%d;"_Stz, sTableNm, i_Col+1);
      StrLib::vSprintCat((Stz*)m_caCode, "\n    goto L_InflateAColumn;"_Stz);
      StrLib::vSprintCat((Stz*)m_caCode, "\n\n    L_Ret%s_Col%d:"_Stz, sTableNm, i_Col+1);
    }
    else {
      StrLib::vSprintCat((Stz*)m_caCode, "\n\n    aSubReturnLabel = && %s;"_Stz, sReturnLabel);
      StrLib::vSprintCat((Stz*)m_caCode, "\n    goto L_InflateAColumn;"_Stz);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* synthesize the projection                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthProject(StreamDef* pStreamDefOut, IndexDef* pIndexDef,
                              eSR eStencilFrom, eSR eStencilTo, ePJ eProjectKind,
                              IntStack* pAggTypeStack, Stz* sProjectExpr, Stz* sLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
  s256 sCopy;

  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // PROJECT-OP ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  // generate the infix from eStencilFrom
  vTrace("Postfix: %s\n"_Stz, sProjectExpr);
  vToInfix((char*)sProjectExpr, eStencilFrom, pAggTypeStack, eProjectKind);
  RASSERT(m_pExprStack->nValues() == pStreamDefOut->nColumns());

  // code for allocating a row in the tub
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 i_Ofs;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 iKind;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 nSrcSize;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    m8 amCellAddress;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    char* racProjectArea;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    byte* aProjectRow;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    Stz* szSrcStz;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    Stz* szDstStz;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void *aEntFound;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8* aiPayload;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void* pEntBuffer;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    static i8 iProjectInit = 0;");


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n%s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n     if (bTrace)");
  StrLib::vAppendString(&m_azCodeCrs, "\n        vRowSingle_Print(pRowSingle);\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  sCopy.vSprint("    p%s = (struct %s*)aRow;"_Stz, xRowRef[eStencilFrom], xRowRef[eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  if (eProjectKind == PJ_EXP) {
    vProjectExp(pStreamDefOut, eStencilTo, pAggTypeStack, sLabel);
    StrLib::vAppendString(&m_azCodeCrs, "\n   goto L_AdvanceNextRow;\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\nL_ProcessEof:");
    StrLib::vAppendString(&m_azCodeCrs, "\n   goto L_Return;\n");
  }
  else if (eProjectKind == PJ_AGG) {
    vProjectAgg(pStreamDefOut, eStencilTo, pAggTypeStack, sLabel);
    StrLib::vAppendString(&m_azCodeCrs, "\n   goto L_AdvanceNextRow;\n");
  }
  else {
    RASSERT(eProjectKind == PJ_GBY);
    vProjectGby(pStreamDefOut, pIndexDef, eStencilFrom, eStencilTo, pAggTypeStack, sLabel);
  }

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the streamDef into a struct                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthStruct(StreamDef* pStreamDefOut, eSR eStencilRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vSprint((Stz*)m_caCode, "\n\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "_Stz);
  switch (eStencilRow) {
    case SR_IJ:
      StrLib::vSprintCat((Stz*)m_caCode, "\n    // Inflate J-Table"_Stz);
      break;
    case SR_IT:
      StrLib::vSprintCat((Stz*)m_caCode, "\n    // Inflate T-Table"_Stz);
      break;
    case SR_PJ:
      StrLib::vSprintCat((Stz*)m_caCode, "\n    // Project J-Table"_Stz);
      break;
    case SR_PT:
      StrLib::vSprintCat((Stz*)m_caCode, "\n    // Project T-Table"_Stz);
      break;
    case SR_TJ:
      StrLib::vSprintCat((Stz*)m_caCode, "\n    // IndexJoin TJ-Table"_Stz);
      break;
    default:
      break;
  }
  StrLib::vSprintCat((Stz*)m_caCode, "\n\n    struct %s { \n"_Stz, xRowRef[eStencilRow]);

  for (i8 i_Col = 0; i_Col < pStreamDefOut->nColumns(); i_Col++) {
    Column *pColumn = pStreamDefOut->pGetColumn(i_Col);
    StrLib::vSprintCat((Stz*)m_caCode, "      i8 %s;\n"_Stz, pColumn->szColNm());
  }

  StrLib::vSprintCat((Stz*)m_caCode, "    };"_Stz);
  StrLib::vSprintCat((Stz*)m_caCode, "\n    struct %s* p%s;\n"_Stz, xRowRef[eStencilRow], xRowRef[eStencilRow]);
}

/*----------------------------------------------------------------------------*/
/* return source code                                                         */
/*----------------------------------------------------------------------------*/
char* CodeSynth::azCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  return m_caCode;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void CodeSynth::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_CODESYNTH);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate code for Project aggregates with groupby                          */
/*----------------------------------------------------------------------------*/
void CodeSynth::vFindKey(StreamDef* pStreamDef, IndexDef* pIndexDef, eSR eStencilFrom, IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // find the key in the indexTree

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // find key in payload index");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aEntFound = aIndexTree_findEntry(aCombinedT->m_pIndexTree, aCombinedT->m_aEntKey);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( !aEntFound ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // key not found");
  vPayloadStore(pStreamDef, pIndexDef, eStencilFrom);

  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n    else {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // key found");
  vPayloadAgg(pStreamDef, pIndexDef, eStencilFrom, pAggTypeStack);
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");

}

/*----------------------------------------------------------------------------*/
/* generate code for creation of group by index                               */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGbyIndex(IndexDef* pIndexDef) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCopy;

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( iProjectInit == 0 ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // create the group by indexTree");

  sCopy.vSprint("\n      aCombinedT->m_pIndexDef = aIndexDef_new(\"%s\");"_Stz, pIndexDef->szIndexNm());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {
    Column *pColumn = pIndexDef->pGetColumn(i_Col);
    sCopy.vSprint("\n      vIndexDef_addColumn(aCombinedT->m_pIndexDef, \"%s\", %d);"_Stz, pColumn->szColNm(), pColumn->eColumnType());
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }

  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_pIndexTree = aIndexTree_new(aCombinedT->m_pIndexDef, &iKind);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  sCopy.vSprint("\n      vEntWrk_entKind(aCombinedT->m_pEntWrk,1,%d);"_Stz, pIndexDef->eKeyKind());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n      iProjectInit = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
}

/*----------------------------------------------------------------------------*/
/* generate code for creation of payload key                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadKey(IndexDef* pIndexDef, eSR eStencilFrom) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCopy;

  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {
    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // populate the payload key");
    if (pIndexDef->eKeyKind()) {
        sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKey = p%s->%s;"_Stz, xRowRef[eStencilFrom].m_szInclude, pColumn->szColNm());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n    vEntWrk_print(aCombinedT->m_pEntWrk, aCombinedT->m_pIndexDef);");
    }
    else {
      StrLib::vAppendString(&m_azCodeCrs, "assert(0)");
    }
  }
}

/*----------------------------------------------------------------------------*/
/* generate code for payload store for groupby                                */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadStore(StreamDef* pStreamDef, IndexDef* pIndexDef, eSR eStencilFrom) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sInfix;
  s256 sCopy;
  i8   i_Col;

  i8 nColumns = pStreamDef->nColumns();

  // create the payload

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // PAYLOAD STORE");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // insert key in the payload");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aEntFound = aIndexTree_insertEnt(aCombinedT->m_pIndexTree, aCombinedT->m_aEntInt);");

  // create the payload
  i8 nPayloadBytes = pStreamDef->nColumns() * 8;
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // create the payload");
  sCopy.vSprint("\n      vEntWrk_payloadCreate(aCombinedT->m_pEntWrk, %d);"_Stz, nPayloadBytes);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  // calculate the payload address
  if (pIndexDef->eKeyKind()) {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aiPayload = (i8 *) ((char *) aEntFound");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + 8);");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs,
    "\n    assert(0);");
    /*aiPayload = (i8 *) ((char *) aEntInsert
                        + sizeof(struct Ent_int)
                        + aEnt->m_iIntElm0
                        + 8
    */
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // populate the payload");
  i_Col = 0;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n       // - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n       // column %d"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    RASSERT(pColumn->eColumnType() != CT_VARCHAR);
    RASSERT(pColumn->eColumnType() != CT_CHAR);

    sCopy.vSprint("\n       aiPayload[%d] = %s;"_Stz, i_Col, sInfix.azChar());
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    i_Col++;
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for payload aggregates for groupby                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadAgg(StreamDef* pStreamDef, IndexDef* pIndexDef, eSR eStencilFrom, IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sInfix;
  s256 sCopy;
  i8   i_Col;

  i8 nColumns = pStreamDef->nColumns();

  // create the payload

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // PAYLOAD AGGREGATE");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  for (i8 i_Project = nColumns - 1, i_Col = 0; i_Project >= 0; i_Project--, i_Col++) {

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    RASSERT(pColumn->eColumnType() != CT_VARCHAR);
    RASSERT(pColumn->eColumnType() != CT_CHAR);

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregate columns
    switch(eAggType) {

      case AG_SUM:
        StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - -  ");
        sCopy.vSprint("\n      // column %d"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");

        sCopy.vSprint("\n      // agg_sum column %d"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      aiPayload[%lld] += %s;\n"_Stz, i_Col, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      default:
        break;
    }
  }

}
/*----------------------------------------------------------------------------*/
/* generate code for processing EOF                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadToBuffer(StreamDef* pStreamDefOut, eSR eStencilTo, IntStack* pAggTypeStack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s256 sCopy;
  i8 nColumns = pStreamDefOut->nColumns();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  sCastRow.vSprint("\n      p%s = (struct %s*)aProjectRow;\n"_Stz,xRowRef[eStencilTo].m_szInclude, xRowRef[eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    Column *pColumn = pStreamDefOut->pGetColumn(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      StrLib::vAppendString(&m_azCodeCrs, "\n  assert(0);");
    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types

      sCopy.vSprint("\n      p%s->%s = aiPayload[%lld];\n"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm(), i_Col);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for processing EOF                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadTraversal(StreamDef* pStreamDefOut, IndexDef* pIndexDef, eSR eStencilTo, IntStack* pAggTypeStack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\nL_ProcessEof:");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // PAYLOAD AGGREGATE");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  //StrLib::vAppendString(&m_azCodeCrs, "\n  aIndexTree = aCombinedT->m_pIndexTree;");

  // begin iterating the payload index
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_aEntInt = aIndexTree_begin(aCombinedT->m_pIndexTree, &pEntBuffer);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n    while (aCombinedT->m_aEntInt) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // allocate a projectRow from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aProjectRow = (byte*)aTub_AllotRow(pTub,&racProjectArea);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // calculate the payload address
  if (pIndexDef->eKeyKind()) {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aiPayload = (i8 *) ((char *) aCombinedT->m_aEntInt");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + 8);");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs,
                          "\n     assert(0);");
    /*aiPayload = (i8 *) ((char *) aCombinedT->m_aEntInt
                        + sizeof(struct Ent_int)
                        + aEnt->m_iIntElm0
                        + 8
    */
  }

  vPayloadToBuffer(pStreamDefOut, eStencilTo, pAggTypeStack);

  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n     aCombinedT->m_aEntInt = aEntBuffer_nextEntry(&pEntBuffer, aCombinedT->m_aEntInt);");

  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
}

/*----------------------------------------------------------------------------*/
/* generate code for Project Aggregates                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vProjectAgg(StreamDef* pStreamDefOut, eSR eStencilTo,
                            IntStack* pAggTypeStack, Stz* sLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s256 sCopy;
  i8 i_Col;
  i8 nColumns = pStreamDefOut->nColumns();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate aggregate ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( iProjectInit == 0 ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // allocate a projectRow from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aProjectRow = (byte*)aTub_AllotRow(pTub,&racProjectArea);");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  sCastRow.vSprint("\n     p%s = (struct %s*)aProjectRow;\n"_Stz,xRowRef[eStencilTo].m_szInclude, xRowRef[eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate init code for each projected agg column
  i_Col = 0;

  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    StrLib::vAppendString(&m_azCodeCrs, "\n     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    Column *pColumn = pStreamDefOut->pGetColumn(i_Col++);

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Project);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // initialization of aggregate columns
    switch(eAggType) {

      case AG_SUM:
        sCopy.vSprint("\n     // init agg_sum column %d"_Stz, i_Col - 1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n     p%s->%s = %d;\n"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm(), m_pInitStack->iPopInt(__func__));
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      default:
        break;
    }
  }

  StrLib::vAppendString(&m_azCodeCrs, "\n     iProjectInit = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  i_Col = 0;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n   // column %d"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    Column *pColumn = pStreamDefOut->pGetColumn(i_Col++);

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Project);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregation code for the column
    switch(eAggType) {

      case AG_SUM:
        RASSERT(pColumn->eColumnType() != CT_VARCHAR);
        RASSERT(pColumn->eColumnType() != CT_CHAR);

        sCopy.vSprint("\n   p%s->%s += %s;\n"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      default:
        sCopy.vSprint("\n   p%s->%s = %s;\n"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;
    }
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for Project Expressions                                      */
/*----------------------------------------------------------------------------*/
void CodeSynth::vProjectExp(StreamDef* pStreamDefOut, eSR eStencilTo, IntStack* pAggTypeStack, Stz* sLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s256 sCopy;
  i8 i_Col;
  i8 nColumns = pStreamDefOut->nColumns();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n   // allocate a projectRow from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n   aProjectRow = (byte*)aTub_AllotRow(pTub,&racProjectArea);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  sCastRow.vSprint("\n   p%s = (struct %s*)aProjectRow;\n"_Stz,xRowRef[eStencilTo].m_szInclude, xRowRef[eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column
  i_Col = 0;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n   // column %d"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    Column *pColumn = pStreamDefOut->pGetColumn(i_Col++);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      StrLib::vAppendString(&m_azCodeCrs, "\n   // get the source string");
      sCopy.vSprint("\n   i_Ofs = %s;"_Stz, sInfix.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      sCopy.vSprint("\n   amCellAddress = (m8*) &%s;"_Stz, sInfix.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n   szSrcStz = ADDR_ADD(Stz*, amCellAddress, i_Ofs);");
      StrLib::vAppendString(&m_azCodeCrs, "\n   nSrcSize = strlen(szSrcStz) + 1;");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n   // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n   szDstStz = racProjectArea;");
      StrLib::vAppendString(&m_azCodeCrs, "\n   strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n   // copy the source to the destination");
      sCopy.vSprint("\n   amCellAddress = (m8*) &p%s->%s;"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n   i_Ofs = - nDifference(racProjectArea,amCellAddress);");
      sCopy.vSprint("\n   p%s->%s = i_Ofs;"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n   // update the tub string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n   racProjectArea += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n   vTub_RowStrSize(pTub,nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");
    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types
      sCopy.vSprint("\n   p%s->%s = %s;\n"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm(), sInfix.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* generate code for Project aggregates with groupby                          */
/*----------------------------------------------------------------------------*/
void CodeSynth::vProjectGby(StreamDef* pStreamDefOut, IndexDef* pIndexDef, eSR eStencilFrom, eSR eStencilTo,
                            IntStack* pAggTypeStack, Stz* sLabel) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s256 sCopy;
  i8 i_Col;
  i8 nColumns = pStreamDefOut->nColumns();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the indexTree for the groupby index
  vGbyIndex(pIndexDef);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the payload key
  vPayloadKey(pIndexDef, eStencilFrom);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the key in the indexTree
  vFindKey(pStreamDefOut, pIndexDef, eStencilFrom, pAggTypeStack);

  StrLib::vAppendString(&m_azCodeCrs, "\n    goto L_AdvanceNextRow;\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  vPayloadTraversal(pStreamDefOut, pIndexDef, eStencilTo, pAggTypeStack);
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n   goto L_Return;\n");

  //RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* add the init aggregate                                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vInitAgg(eAG eAggType, eDM eExprType)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s32 sInitValue;
  i8  iInitValue;

  switch(eAggType) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_ALL:
      m_pInitStack->vPushInt(0);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_CNT:
      m_pInitStack->vPushInt(0);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_MIN:
      switch(eExprType) {
        case DM_VCH:
          // get the min value
          //vMathOp_MinValue(sInitValue);
          m_pInitStack->vPushStr(sInitValue);
          break;

        default:
          //iInitValue = iMathOp_MinValue(eExprType);
          m_pInitStack->vPushInt(iInitValue);
          break;
      }
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_MAX:
      switch(eExprType) {
        case DM_VCH:
          // get the min value
          //vMathOp_MaxValue(sInitValue);
          m_pInitStack->vPushStr(sInitValue);
          break;

        default:
          //iInitValue = iMathOp_MaxValue(eExprType);
          m_pInitStack->vPushInt(iInitValue);
          break;
      }
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_SUM:
      RASSERT(eExprType != DM_VCH &&
              eExprType != DM_DTE &&
              eExprType != DM_IVL &&
              eExprType != DM_IVY &&
              eExprType != DM_TME &&
              eExprType != DM_TSP);

          m_pInitStack->vPushInt(0);
      break;

    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* convert postfix to infix                                                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vToInfix(char* sExpr, eSR eStencilRow, IntStack* pAggTypeStack, ePJ eProjectKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sInfix;
  s4K sOperand1;
  s4K sOperand2;

  eDM eExprType = DM_UNK;
  eAG eAggType  = AG_NONE;

  bool bIndexJoin = false;

  TypedCell *pTypedCell;

  TokenVec *pTknVec = TokenVec::pTokenise((Stz*)sExpr);
  Token* pTkn;
  i8 i_Col = 0;

  for (i8 i_Tkn = 0; i_Tkn < pTknVec->nItems(); i_Tkn++) {

    pTkn = pTknVec->pGet(i_Tkn);

    vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame("groupby"_Stz)) {
      break;
    }

    // skip bound
    if (pTkn->bSame("bound"_Stz)) {
      bIndexJoin = true;
      continue;
    }

    // skip $I
    if (pTkn->szChars()->bContains("$"_Stz)) {
      continue;
    }

    if (pTkn->bSame("type"_Stz)) {
      // get the data type
      i_Tkn++;
      pTkn = pTknVec->pGet(i_Tkn);
      eExprType = StreamDefBase::eDatum(pTkn->szChars());
      continue;
    }

    if (pTkn->bSame("as"_Stz)) {
      // skip the alias
      i_Tkn++;
      continue;
    }

    if (pTkn->bSame(","_Stz)) {
      // skip the comma
      i_Col++;
      continue;
    }

    if (pTkn->bSame(")"_Stz)) {
      // aggregate follows
      eAggType = (eAG)pAggTypeStack->iPeek(i_Col);
      vInitAgg(eAggType, eExprType);
      continue;
    }

    if (pTkn->bSame("sum"_Stz)) {
      continue;
    }

    if (pTkn->bIsNumeric()) {
      m_pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("negate"_Stz) ) {
      m_pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bIsDecimalNo()) {
      m_pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("DATE"_Stz) ||  pTkn->bSame("TIME"_Stz) || pTkn->bSame("TIMESTAMP"_Stz)) {
      //m_pExprStack->vPushPtr(pTkn);
      i_Tkn++;
      pTkn = pTknVec->pGet(i_Tkn);
      m_pExprStack->vPushPtr(pTkn);
      continue;
    }

    // operator ?
    if ( (pTkn->bIsArithmetic() || pTkn->bIsLogical() || pTkn->bIsRelational()) ) {

      // pop operand1
      pTypedCell = m_pExprStack->aPeekItem(0);
      if (pTypedCell->eType() == CN_PTRVAL) {
        Token *pOperand1 = _pToken(m_pExprStack->pPopPtr(__func__));
        sOperand1 = pOperand1->szChars();
      }
      else {
        m_pExprStack->vPopStr(sOperand1, __func__);
      }

      // pop operand2
      pTypedCell = m_pExprStack->aPeekItem(0);
      if (pTypedCell->eType() == CN_PTRVAL) {
        Token *pOperand2 = _pToken(m_pExprStack->pPopPtr(__func__));
        sOperand2 = pOperand2->szChars();
      }
      else {
        m_pExprStack->vPopStr(sOperand2, __func__);
      }

      if (pTkn->bSame("AND"_Stz))
        pTkn->vChars("&&"_Stz);
      else
      if (pTkn->bSame("OR"_Stz))
        pTkn->vChars("||"_Stz);
      else
      if (pTkn->bSame("NOT"_Stz))
        pTkn->vChars("!"_Stz);

      // combine the two operatans and operator
      // operand2 op operand1
      sOperand2.vConcat((Stz*)pTkn->azChars());
      sOperand2.vConcat(sOperand1);

      // push new infix string on the TknVec
      m_pExprStack->vPushStr(sOperand2);
    }
    else {
      // save the column name
      s256 sColumn;
      sColumn.vSprint("p%s->%s"_Stz, xRowRef[eStencilRow], pTkn->szChars());
      m_pExprStack->vPushStr(sColumn);

      // hack!!!
      // break out of the loop once column name found
      if (bIndexJoin)
        break;
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
