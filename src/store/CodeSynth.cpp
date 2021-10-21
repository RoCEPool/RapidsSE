
/*============================================================================*/
/* CodeSynth.cpp -- Code Synthesizer                                          */
/*============================================================================*/

#include "BASE.h"

#include "Stream.hpp"
#include "StreamDef.hpp"
#include "TokenVec.hpp"
#include "Token.hpp"
#include "LogicUnit.hpp"

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
void CodeSynth::vSynthFilter(Stz* sFilterExpr, eSR eStencilRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;

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

  s256 sCastRow;
  sCastRow.vSprint("   p%s = (struct %s*)aRow;\n   if ( "_Stz, xRowRef[eStencilRow], xRowRef[eStencilRow].m_szInclude);

  StrLib::vAppendString(&m_azCodeCrs, "\n\nL_FilterRow:\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n\n  if (bTrace)");
  StrLib::vAppendString(&m_azCodeCrs, "\n     vRowSingle_Print(pRowSingle);\n");
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());
  StrLib::vAppendString(&m_azCodeCrs, sInfix.azChar());

  StrLib::vAppendString(&m_azCodeCrs, " ) { \n");
  StrLib::vAppendString(&m_azCodeCrs, "     goto L_ProjectRow;\n");
  StrLib::vAppendString(&m_azCodeCrs, "   } else { \n");
  StrLib::vAppendString(&m_azCodeCrs, "     vRowSingle_DiscardStr(pRowSingle);\n");
  StrLib::vAppendString(&m_azCodeCrs, "     goto L_AdvanceNextRow;\n");
  StrLib::vAppendString(&m_azCodeCrs, "   }\n");

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the inflate                                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthInflate(StreamDef* pStreamDefOut)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, "   \n\nL_InflateRow:");

  for (i8 i_Col = 0; i_Col < pStreamDefOut->nColumns(); i_Col++) {
    StrLib::vSprintCat((Stz*)m_caCode, "\n"_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n  i_Col = %d;"_Stz , i_Col);
    StrLib::vSprintCat((Stz*)m_caCode, "\n\n  uMetaCode = *auMetaCode++;"_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n  uHigh = uMetaCode >> 4;"_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n  uLow  = uMetaCode & 0x0F;"_Stz);

    if (i_Col < pStreamDefOut->nColumns() - 1) {
      StrLib::vSprintCat((Stz*)m_caCode, "\n\n  aSubReturnLabel = && L_RetsubInflateColumn%d;"_Stz, i_Col+1);
      StrLib::vSprintCat((Stz*)m_caCode, "\n  goto L_InflateAColumn;"_Stz);
      StrLib::vSprintCat((Stz*)m_caCode, "\n\nL_RetsubInflateColumn%d:"_Stz, i_Col+1);
    }
    else {
      StrLib::vSprintCat((Stz*)m_caCode, "\n  aSubReturnLabel = && L_FilterRow;"_Stz);
      StrLib::vSprintCat((Stz*)m_caCode, "\n  goto L_InflateAColumn;"_Stz);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* synthesize the projection                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthProject(StreamDef* pStreamDefOut, eSR eStencilFrom, eSR eStencilTo,
                              IntStack* pAggTypeStack, ePJ eProjectKind, Stz* sProjectExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s256 sCopy;
  i8 i_Col;
  i8 nColumns = pStreamDefOut->nColumns();

  // generate the infix from eStencilFrom
  vTrace("Postfix: %s\n"_Stz, sProjectExpr);
  vToInfix((char*)sProjectExpr, eStencilFrom, pAggTypeStack, eProjectKind);
  RASSERT(m_pExprStack->nValues() == pStreamDefOut->nColumns());

  // code for allocating a row in the tub
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n   i8 i_Ofs;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   m8 amCellAddress;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   char* racProjectArea;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   byte* aProjectRow;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   i8 nSrcSize;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   Stz* szSrcStz;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   Stz* szDstStz;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   static i8 iProjectRow = 0;");

  if (eProjectKind == PJ_EXP) {

    StrLib::vAppendString(&m_azCodeCrs, "\nL_ProjectRow:");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n   if (bTrace)");
    StrLib::vAppendString(&m_azCodeCrs, "\n     printf(\"\\nProjecting row\\n\");\n");

    StrLib::vAppendString(&m_azCodeCrs, "\n   // allocate a projectRow from the tub");
    StrLib::vAppendString(&m_azCodeCrs, "\n   aProjectRow = (byte*)aTub_AllotRow(pTub,&racProjectArea);");
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    // cast the ProjectRow to eStencilTo structure
    sCastRow.vSprint("\n   p%s = (struct %s*)aProjectRow;\n"_Stz,xRowRef[eStencilTo].m_szInclude, xRowRef[eStencilTo].m_szInclude);
    StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

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
        sCopy.vSprint("\n   p%s->%s = %s;\n"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      }
    }
  }
  else if (eProjectKind == PJ_AGG) {

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\nL_ProjectRow:");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n   if (bTrace)");
    StrLib::vAppendString(&m_azCodeCrs, "\n     printf(\"\\nProjecting row\\n\");\n");

    StrLib::vAppendString(&m_azCodeCrs, "\n   if ( iProjectRow == 0 ) {");
    StrLib::vAppendString(&m_azCodeCrs, "\n     // allocate a projectRow from the tub");
    StrLib::vAppendString(&m_azCodeCrs, "\n     aProjectRow = (byte*)aTub_AllotRow(pTub,&racProjectArea);");

    // cast the ProjectRow to eStencilTo structure
    sCastRow.vSprint("\n   p%s = (struct %s*)aProjectRow;\n"_Stz,xRowRef[eStencilTo].m_szInclude, xRowRef[eStencilTo].m_szInclude);
    StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

    // generate init code for each projected agg column
    i_Col= 0;
    for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {
      StrLib::vAppendString(&m_azCodeCrs, "\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
      Column *pColumn = pStreamDefOut->pGetColumn(i_Col++);

      eAG eAggType = (eAG) pAggTypeStack->iPop();

      switch(eAggType) {
        case AG_SUM:
          sCopy.vSprint("\n   // init agg_sum column %d"_Stz, i_Col - 1);
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n");

          sCopy.vSprint("\n   p%s->%s = %d;\n"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm(), m_pInitStack->iPopInt(__func__));
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

          break;
        default:
          NEVER_REACH;
      }
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n     iProjectRow = 1;");
    StrLib::vAppendString(&m_azCodeCrs, "\n   }");
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    // for each projected column
    i_Col = 0;
    for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

      //eAG eAggType = (eAG) pAggTypeStack->iPop();

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

      RASSERT(pColumn->eColumnType() != CT_VARCHAR);
      RASSERT(pColumn->eColumnType() != CT_CHAR);

      sCopy.vSprint("\n   p%s->%s += %s;\n"_Stz, xRowRef[eStencilTo].m_szInclude, pColumn->szColNm(), sInfix.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
  }
  else {
    RASSERT(eProjectKind == PJ_GBY);

    // not implemented yet
    RASSERT(0);
  }
  StrLib::vAppendString(&m_azCodeCrs, "\n   goto L_AdvanceNextRow;\n");

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the streamDef into a struct                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthStruct(StreamDef* pStreamDefOut, eSR eStencilRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vSprint((Stz*)m_caCode, "\n    struct %s { \n"_Stz, xRowRef[eStencilRow]);

  for (i8 i_Col = 0; i_Col < pStreamDefOut->nColumns(); i_Col++) {
    Column *pColumn = pStreamDefOut->pGetColumn(i_Col);
    StrLib::vSprintCat((Stz*)m_caCode, "     i8 %s;\n"_Stz, pColumn->szColNm());
  }

  StrLib::vSprintCat((Stz*)m_caCode, "    };"_Stz);
  StrLib::vSprintCat((Stz*)m_caCode, "\n   struct %s* p%s;"_Stz, xRowRef[eStencilRow], xRowRef[eStencilRow]);
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
/* add the init aggregate                                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vInitAgg(eAG eAggType, eCT eExprType)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  switch(eAggType) {
    case AG_SUM:
      switch(eExprType) {
        case CT_INTEGER:
          m_pInitStack->vPushInt(0);
          break;
        default:
          NEVER_REACH;
      }
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

  eCT eExprType = CT_NONE;
  eAG eAggType  = AG_NONE;

  TypedCell *pTypedCell;

  TokenVec *pTknVec = TokenVec::pTokenise((Stz*)sExpr);
  Token* pTkn;
  i8 i_Col = 0;

  for (i8 i_Tkn = 0; i_Tkn < pTknVec->nItems(); i_Tkn++) {

    pTkn = pTknVec->pGet(i_Tkn);

    vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame("type"_Stz)) {
      // get the data type
      i_Tkn++;
      pTkn = pTknVec->pGet(i_Tkn);
      eExprType = StreamDefBase::eColumn(pTkn->szChars());
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
    if (pTkn->bIsArithmetic() || pTkn->bIsLogical() || pTkn->bIsRelational() ) {

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
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
