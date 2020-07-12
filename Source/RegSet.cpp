/*============================================================================*/
/* RegSet.hpp -- Task Register Set                                            */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,RegSet,CN_REGSET)
__aNewFunction(RegSet,CN_REGSET)
__vDeleteFunction(RegSet,CN_REGSET)
__aFooterFunction(RegSet)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RegSet::RegSet(Task* pTask)
: Object(CN_REGSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_pTask = pTask;
  
  m_eDdlLocked = LH_HAVE_NO_LOCK;
  m_i_NextRnd  = AM::bDeveloper() ? 0 : C_UNKNOWN;
  m_pSendNode  = nullptr;
  
  m_xpHandler  = nullptr;
  m_bCallAgain = false;
  m_iDebugFlag = 0;

  m_xpNow = new Now;
  
  m_iCounter0 = 0;
  m_iCounter1 = 0;
  m_iCounter2 = 0;
  
  m_aLowerEntry = nullptr;
  m_aUpperEntry = nullptr;
  m_pEntBuffer  = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pPot        = nullptr;
  m_aDstRsb     = nullptr;
  m_pDstRowSlab = nullptr;

  m_i_SrcRow    = C_UNKNOWN;
  m_nSrcRows    = C_UNKNOWN;
  m_aSrcRsb     = nullptr;
  m_pSrcRowSlab = nullptr;
  
  m_i_AltRow    = C_UNKNOWN;
  m_nAltRows    = C_UNKNOWN;
  m_aAltRsb     = nullptr;
  m_pAltRowSlab = nullptr;

  m_pEnt        = nullptr;
  // m_oEntWrk
  m_pEntBuffer  = nullptr;
  m_pIndexTree  = nullptr;
  m_aPayRsb     = nullptr;
  
  m_i_Code = 0;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_eaCode,sizeof(m_eaCode),CN_REGSET);
  MemLib::vZero(m_iaDataStack,sizeof(m_iaDataStack),CN_REGSET);
  MemLib::vZero(m_aaReturnStack,sizeof(m_aaReturnStack),CN_REGSET);
#endif
  
  m_nLabels = 0;
  for (i8 i_Label = 0; i_Label < C_CALC_LABEL; i_Label++) {
    m_xaJumpLabel[i_Label].m_bDefined = false;
    m_xaJumpLabel[i_Label].m_i_Chain = C_UNKNOWN;
  }

  _checkInitialised_(CN_REGSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RegSet::~RegSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);

  RASSERT(LH_HAVE_NO_LOCK == m_eDdlLocked);
  
  if (! m_xpAggTimer.bIsNull())
    delete *m_xpAggTimer;
  
  if (! m_xpNow.bIsNull())
    delete *m_xpNow;
  
  if (! m_xpTimer.bIsNull())
    delete *m_xpTimer;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void RegSet::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("RegSet\n"_Stz);
  vTrace("------\n"_Stz);

  vTrace("%p:\n"
         "m_i_NextRnd=%lld\n"
         "m_pSendNode=%p %s\n"_Stz,

         this,
         
         m_i_NextRnd,
         
         m_pSendNode,
         m_pSendNode ? m_pSendNode->szNodeNm() : ""_Stz,
         
         C_VARGS_END
         );

}

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
Calculator* RegSet::pCalculator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  return & m_oCalculator;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


//
// ..from Calculator
//

extern EoArg EoArgs[];

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//! TEMP : TO GO

/*----------------------------------------------------------------------------*/
/* generate agg group by                                                      */
/*----------------------------------------------------------------------------*/
void RegSet::vGenerateAggCpy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_i_Code = 0;
  
  // begin index tree traversal
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMPFALSE,"END_IXT_SRC:"_Stz);
  
  vDefineLabel("NEXT_IXT_SRC:"_Stz);
  
  // new a dst rsb row
  vAddCode(EO_RSB_DST_NEW);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)0);
  vAddCodeArg(EO_DEPOSITCOLI,(i8)0);
  vAddCode(EO_RSB_DST_PRINT);
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMPTRUE,"NEXT_IXT_SRC:"_Stz);
  
  //vAddCode(EO_IXT_TRV_NEXT);
  //vAddCodeLabel(EO_JMPTRUE,"NEXT_IXT_SRC:"_Stz);
  
  vDefineLabel("END_IXT_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  //vDumpCode1();
  vDumpCode2();
}

/*----------------------------------------------------------------------------*/
/* generate agg group by                                                      */
/*----------------------------------------------------------------------------*/
void RegSet::vGenerateAggGby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
    // append the key element to entwrk
  vAddCode(EO_ENTWRK_RESET);
  vAddCodeArg(EO_ENTWRK_APPENDELMI,(i8)0);
  
  // find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
  vAddCode(EO_ENT_PUSH);
  vAddCode(EO_PUSH0);
  vAddCode(EO_NOTEQUAL);
  vAddCodeLabel(EO_JMPFALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMPFALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_EQUAL);
  vAddCodeLabel(EO_JMPTRUE,"FOUND"_Stz);
  
  // NOT_FOUND:
  vDefineLabel("NOT_FOUND"_Stz);
  vAddCodeArg(EO_ENT_CREATEPAY, 8);
  vAddCode(EO_ENT_INSERT);
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)0);
  
  // FOUND:
  vDefineLabel("FOUND"_Stz);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)0);
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)0);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)0);
  vAddCode(EO_RETURN);
  
  //vDumpCode1();
  vDumpCode2();
}

/*----------------------------------------------------------------------------*/
/* generate sort test code                                                    */
/*----------------------------------------------------------------------------*/
void RegSet::vGenerateSort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);

  // -- -- -- -- -- -- -- -- -- -- --
  // insert each rsb
  
  // forearch src rsb
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NEXT_RSB_SRC:"_Stz);

  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMPFALSE,"END_RSB_SRC:"_Stz);

  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);

  // add key
  vAddCodeArg(EO_PUSHSRCCOLI,(i8)0);
  vAddCodeArg(EO_ENTWRK_APPENDELMI,(i8)0);

  vAddCodeArg(EO_PUSHSRCCOLI,(i8)3);
  vAddCodeArg(EO_ENTWRK_APPENDELMI,(i8)3);

  // add offset
  vAddCode(EO_RSB_SRC_PUSHOFS);
  vAddCode(EO_ENTWRK_POPREF);

  // insert entry into index tree
  vAddCode(EO_ENT_INSERT);

  vAddCodeLabel(EO_JMP,"NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);

  
  // -- -- -- -- -- -- -- -- -- -- --

  // copy src to dst rsb in index order

  // forearch src rsb
  vAddCode(EO_IXT_TRV_BEGIN);

  vDefineLabel("NEXT_IXT_ENT:"_Stz);

  vAddCode(EO_IXT_TRV_NEXT);
  vAddCodeLabel(EO_JMPFALSE,"END_IXT_ENT:"_Stz);
  

  // ...
  vAddCode(EO_RSB_DST_NEW);
  vAddCode(EO_REF_SRCRSB);
  vAddCode(EO_RSB_DST_COPYSRC);

  vAddCodeLabel(EO_JMP,"NEXT_IXT_ENT:"_Stz);

  
  // -- -- -- -- -- -- -- -- -- -- --

  vDefineLabel("END_IXT_ENT:"_Stz);
  vAddCode(EO_RETURN);

  
  //vDumpCode1();
  vDumpCode2();
}

/*----------------------------------------------------------------------------*/
/* aggregate compute                                                          */
/*----------------------------------------------------------------------------*/
i8 RegSet::iAggregate(RowSlab* pDstRowSlab, Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
    
  m_pDstRowSlab = pDstRowSlab;
  
  m_aSrcRsb      = aSrcRsb;
  m_pSrcRowSlab  = pSrcRowSlab;
  
  m_aAltRsb     = nullptr;
  m_pAltRowSlab = nullptr;
  
  m_pEnt              = nullptr;
  //m_oEntWrk
  m_pEntBuffer        = nullptr;
  m_pIndexTree        = nullptr;
  
  i8 iResult = iCompute();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* group by aggregate compute                                                 */
/*----------------------------------------------------------------------------*/
i8 RegSet::iAggregateGby(Pot* pPot, IndexTree* pIndexTree, RowSlab* pDstRowSlab,
                             Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_pPot        = pPot;
  m_pDstRowSlab = pDstRowSlab;
  
  m_aSrcRsb      = aSrcRsb;
  m_pSrcRowSlab  = pSrcRowSlab;
  
  m_aAltRsb     = nullptr;
  m_pAltRowSlab = nullptr;
  
  m_pEnt        = nullptr;
  //m_oEntWrk
  m_pEntBuffer  = nullptr;
  
  m_pIndexTree  = pIndexTree;
  
  i8 iResult = iCompute();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool RegSet::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  TokenVec* pTknVec = TokenVec::pTokenise(m_sSource);
  
  bool b = bCompile(pTknVec);
  
  delete pTknVec;
  
  return b;
}


/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool RegSet::bCompile(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  bool b = bCompileCode(pTknVec);
  return b;
}

/*----------------------------------------------------------------------------*/
/* calc compute                                                               */
/*----------------------------------------------------------------------------*/
i8 RegSet::iCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_pDstRowSlab = nullptr;

  m_aSrcRsb      = nullptr;
  m_pSrcRowSlab  = nullptr;

  m_aAltRsb     = nullptr;
  m_pAltRowSlab = nullptr;
  
  i8 iResult = iCompute();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* filter compute                                                             */
/*----------------------------------------------------------------------------*/
i8 RegSet::iFilter(Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_pDstRowSlab = nullptr;

  m_aSrcRsb      = aSrcRsb;
  m_pSrcRowSlab  = pSrcRowSlab;

  m_aAltRsb     = nullptr;
  m_pAltRowSlab = nullptr;

  i8 iResult = iCompute();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* project compute                                                            */
/*----------------------------------------------------------------------------*/
i8 RegSet::iProject(Pot* pPot, RowSlab* pDstRowSlab,
                        Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_pPot = pPot;
  m_pDstRowSlab = pDstRowSlab;
  
  m_aSrcRsb      = aSrcRsb;
  m_pSrcRowSlab  = pSrcRowSlab;
  m_nSrcRows = m_pSrcRowSlab->nRows();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  i8 iResult = iCompute();
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pPot = nullptr;
  m_pDstRowSlab = nullptr;

  m_nSrcRows          = C_UNKNOWN;
  m_aSrcRsb      = nullptr;
  m_pSrcRowSlab  = nullptr;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* joint compute                                                              */
/*----------------------------------------------------------------------------*/
i8 RegSet::iJoin(RowSlab* pDstRowSlab,
                     Rsb* aSrcRsb_left,  RowSlab* pSrcRowSlab_left,
                     Rsb* aSrcRsb_right, RowSlab* pSrcRowSlab_right)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_pDstRowSlab = pDstRowSlab;

  m_aSrcRsb      = aSrcRsb_left;
  m_pSrcRowSlab  = pSrcRowSlab_left;

  m_aAltRsb     = aSrcRsb_right;
  m_pAltRowSlab = pSrcRowSlab_right;

  i8 iResult = iCompute();
  
  return iResult;
}


/*----------------------------------------------------------------------------*/
/* -- private ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add code                                                                   */
/*----------------------------------------------------------------------------*/
void RegSet::vAddCode(eEO eCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_eaCode[m_i_Code++] = eCode;
}

/*----------------------------------------------------------------------------*/
/* add code with argument                                                     */
/*----------------------------------------------------------------------------*/
void RegSet::vAddCodeArg(eEO eCode, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_eaCode[m_i_Code++] = eCode;

  i8* piArg = (i8*) & m_eaCode[m_i_Code];
  * piArg = iArg;
  
  m_i_Code += sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* add code with argument                                                     */
/*----------------------------------------------------------------------------*/
void RegSet::vAddCodeArg(eEO eCode,Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  i8 iArg = pTkn->iToI8();
  
  vAddCodeArg(eCode,iArg);
}

/*----------------------------------------------------------------------------*/
/* add code with named label                                                  */
/*----------------------------------------------------------------------------*/
void RegSet::vAddCodeLabel(eEO eCode, Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label) {
    i_Label = iAddLabel(szLabelNm,false);
    m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code + 1;
    vAddCodeArg(eCode,C_UNKNOWN);
    vPrintLabels();
    return;
  }

  if (m_xaJumpLabel[i_Label].m_bDefined) {
    vAddCodeArg(eCode,m_xaJumpLabel[i_Label].m_i_Code);
    return;
  }
  
  i8 i_Chain = m_xaJumpLabel[i_Label].m_i_Chain;
  m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code + 1;
  vAddCodeArg(eCode,i_Chain);
  vPrintLabels();
}

/*----------------------------------------------------------------------------*/
/* add code with label name in given token                                    */
/*----------------------------------------------------------------------------*/
void RegSet::vAddCodeLabel(eEO eCode, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);

  vAddCodeLabel(eCode,pTkn->szChars());
}

/*----------------------------------------------------------------------------*/
/* add label                                                                  */
/*----------------------------------------------------------------------------*/
i8 RegSet::iAddLabel(Stz* szLabelNm, bool bKnown)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  i8 i_Label = m_nLabels;
  
  m_xaJumpLabel[i_Label].m_sLabel = szLabelNm;
  m_xaJumpLabel[i_Label].m_bDefined = bKnown;
  m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code;
  m_nLabels++;
  
  return i_Label;
}

/*----------------------------------------------------------------------------*/
/* correct label chain                                                        */
/*----------------------------------------------------------------------------*/
void RegSet::vCorrectLabel(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  ASSERT(! m_xaJumpLabel[i_Label].m_bDefined);
  
  i8 i_Chain = m_xaJumpLabel[i_Label].m_i_Chain;
  while (C_UNKNOWN != i_Chain) {
    
    i8* piArg = (i8*) & m_eaCode[i_Chain];
 
    i_Chain = * piArg;

    * piArg = m_i_Code;
  }

  m_xaJumpLabel[i_Label].m_bDefined = true;
}

/*----------------------------------------------------------------------------*/
/* decode label into subscript                                                */
/*----------------------------------------------------------------------------*/
i8 RegSet::iDecodeLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  i8 i_Label = iFindLabel(szLabelNm);

  if (C_UNKNOWN == i_Label)
    i_Label = iAddLabel(szLabelNm,false);
  
  i8 i_Code = m_xaJumpLabel[i_Label].m_i_Chain;
 
  m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code;
  
  vPrintLabels();
  
  return i_Code;
}

/*----------------------------------------------------------------------------*/
/* define label                                                               */
/*----------------------------------------------------------------------------*/
void RegSet::vDefineLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  // has this label been used yet?
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label)
    i_Label = iAddLabel(szLabelNm,true);
  else
    vCorrectLabel(i_Label);
  
  vPrintLabels();
}

/*----------------------------------------------------------------------------*/
/* add label                                                                  */
/*----------------------------------------------------------------------------*/
i8 RegSet::iFindLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (m_xaJumpLabel[i_Label].m_sLabel.bSame(szLabelNm))
      return i_Label;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* print labels                                                               */
/*----------------------------------------------------------------------------*/
void RegSet::vPrintLabels(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    vTrace("  %s %lld: %4lld\n"_Stz,
           m_xaJumpLabel[i_Label].m_sLabel.szStz(),
           m_xaJumpLabel[i_Label].m_bDefined,
           m_xaJumpLabel[i_Label].m_i_Chain);
}

/*----------------------------------------------------------------------------*/
/* get source                                                                 */
/*----------------------------------------------------------------------------*/
Stz* RegSet::szSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  return m_sSource;
}

/*----------------------------------------------------------------------------*/
/* set source                                                                 */
/*----------------------------------------------------------------------------*/
void RegSet::vSource(Stz* szSource)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_sSource = szSource;
}


// calc L AGAIN 1 ;

// calc 10 L AGAIN dup . dec dup jt aAGAIN ;

// calc 1 L AGAIN dup . inc dup 10 > jf AGAIN;

// calc 1 L AGAIN call PRINT inc dup 10 > jf AGAIN ret L PRINT dup . ret;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool RegSet::bCompileCode(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  //bCompileTest(pTknVec);
  
  i8* piArg;
  i8  iArg = 0;

  i8 nTkns = pTknVec->nTokens();
  
  for (i8 i_Tkn = 0; i_Tkn < nTkns; i_Tkn++) {
    
    Token* pTkn = pTknVec->pGet(i_Tkn);
    vTrace(" >%s<\n"_Stz,pTkn->szChars());
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // arithmetic operators
    if (pTkn->bSame("+"_Stz)) { vAddCode(EO_ADD);      continue; }
    if (pTkn->bSame("/"_Stz)) { vAddCode(EO_DIVIDE);   continue; }
    if (pTkn->bSame("%"_Stz)) { vAddCode(EO_MODULO);   continue; }
    if (pTkn->bSame("*"_Stz)) { vAddCode(EO_MULTIPLY); continue; }
    if (pTkn->bSame("-"_Stz)) { vAddCode(EO_SUBTRACT); continue; }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // entry relational operators
    if (pTkn->bSame("<"_Stz))  { vAddCode(EO_LESSTHAN);     continue; }
    if (pTkn->bSame("<="_Stz)) { vAddCode(EO_LESSEQUAL);    continue; }
    if (pTkn->bSame("=="_Stz)) { vAddCode(EO_EQUAL);        continue; }
    if (pTkn->bSame("!="_Stz)) { vAddCode(EO_NOTEQUAL);     continue; }
    if (pTkn->bSame(">="_Stz)) { vAddCode(EO_GREATEREQUAL); continue; }
    if (pTkn->bSame(">"_Stz))  { vAddCode(EO_GREATERTHAN);  continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // logical operators
    if (pTkn->bSame("and"_Stz)) { vAddCode(EO_AND); continue; }
    if (pTkn->bSame("or"_Stz))  { vAddCode(EO_OR);  continue; }
    if (pTkn->bSame("not"_Stz)) { vAddCode(EO_NOT); continue; }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // null test operators
    if (pTkn->bSame("isNull"_Stz))    { vAddCode(EO_ISNULL);    continue; }
    if (pTkn->bSame("isNotNull"_Stz)) { vAddCode(EO_ISNOTNULL); continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // stack operators
    if (pTkn->bSame("++"_Stz))   { vAddCode(EO_INC);           continue; }
    if (pTkn->bSame("inc"_Stz))  { vAddCode(EO_INC);           continue; }
    if (pTkn->bSame("--"_Stz))   { vAddCode(EO_DEC);           continue; }
    if (pTkn->bSame("dec"_Stz))  { vAddCode(EO_DEC);           continue; }
    if (pTkn->bSame("dup"_Stz))  { vAddCode(EO_DUP);           continue; }
    if (pTkn->bSame("pop"_Stz))  { vAddCode(EO_POP);           continue; }
    if (pTkn->bSame("."_Stz))    { vAddCode(EO_PRINT);         continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // rsb operators
    if (pTkn->bSame("bs"_Stz))   { vAddCode(EO_RSB_SRC_BEGIN); continue; }
    if (pTkn->bSame("gs"_Stz))   { vAddCode(EO_RSB_SRC_NEXT);  continue; }
    if (pTkn->bSame("nd"_Stz))   { vAddCode(EO_RSB_DST_NEW);   continue; }

    if (pTkn->bSame(".d"_Stz))   { vAddCode(EO_RSB_DST_PRINT); continue; }
    if (pTkn->bSame(".s"_Stz))   { vAddCode(EO_RSB_SRC_PRINT); continue; }
    if (pTkn->bSame(".a"_Stz))   { vAddCode(EO_RSB_ALT_PRINT); continue; }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // subroutine operators
    if (pTkn->bSame("ret"_Stz)) { vAddCode(EO_RETURN); continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//    if (pTkn->bIsName()) {
//      vAddCode(EO_PUSHCOLI;
//      i8 iColVec;
//      piArg = (i8*) & m_eaCode[m_i_Code];
//      * piArg = pStreamDef->iFindColumn(pTkn->azChars(), iColVec);
//      m_i_Code += sizeof(i8);
//      continue;
//    }
    
    // push src column value
    if (pTkn->bSame("@"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_PUSHSRCCOLI,pTkn);
      continue;
    }

    // push dst column value
    if (pTkn->bSame("#"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_PUSHDSTCOLI,pTkn);
      continue;
    }
    
    // deposit dst column value
    if (pTkn->bSame("!"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_DEPOSITCOLI,pTkn);
      continue;
    }

    // copy src row into dst
    if (pTkn->bSame("="_Stz)) {
      vAddCode(EO_RSB_DST_COPYSRC);
      continue;
    }
    
    // call subroutine
    if (pTkn->bSame("call"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_CALL,pTkn);
      continue;
    }

    // label/location
    if (pTkn->bSame("L"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vDefineLabel(pTkn->szChars());
      continue;
    }

    // fetch pay col
    if (pTkn->bSame("pf"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_PAY_FETCH_COL,pTkn);
      continue;
    }
    
    // store pay col
    if (pTkn->bSame("ps"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_PAY_STORE_COL,pTkn);
      continue;
    }
    
    // jump
    if (pTkn->bSame("j"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMP,pTkn);
      continue;
    }

    // jump on false
    if (pTkn->bSame("jf"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMPFALSE,pTkn);
      continue;
    }
    
    // jump on true
    if (pTkn->bSame("jt"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMPTRUE,pTkn);
      continue;
    }

    if (pTkn->bIsInteger()) {
      iArg = pTkn->iToI8();
      vAddCodeArg(EO_INTEGER,iArg);
//      iArg = pTkn->iToI8();
//      piArg = (i8*) & m_eaCode[m_i_Code];
//      * piArg = iArg;
//      m_i_Code += sizeof(i8);
      continue;
    }
    
    if (pTkn->bSame(C_NULL)) {
      vAddCodeArg(EO_INTEGER,C_DATUM_NULL);
      continue;
    }
    
    if (pTkn->bSame("0"_Stz)) {
      vAddCodeArg(EO_INTEGER,(i8)0);
      continue;
    }
    
    if (pTkn->bSame("1"_Stz)) {
      vAddCodeArg(EO_INTEGER,1);
      continue;
    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    vAddCode(EO_RETURN);
    vTrace("Error: %s\n"_Stz,pTkn->szChars());
    return false;
  }

  vAddCode(EO_RETURN);
  vTrace("\n"_Stz);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (! m_xaJumpLabel[i_Label].m_bDefined) {
      vTrace("Error: undefined label %s\n"_Stz,
             m_xaJumpLabel[i_Label].m_sLabel.szStz());
      return false;
    }
  
  //vDumpCode1();
  vDumpCode2();

  return true;
}

/*----------------------------------------------------------------------------*/
/* dump code 2                                                                */
/*----------------------------------------------------------------------------*/
void RegSet::vDumpCode2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
    
  i8* piArg;
  i8  iArg = 0;
  
  for (i8 i_Code = 0; i_Code < m_i_Code; ) {
    
    eEO eCode = m_eaCode[i_Code];
    
    if (EoArgs[eCode].m_nArgBytes)
      vDumpCode(eCode,i_Code);
    else
      vDumpCode(i_Code,eCode);
  }

}
/*----------------------------------------------------------------------------*/
/* dump code                                                                  */
/*----------------------------------------------------------------------------*/
void RegSet::vDumpCode(i8& i_Code, eEO eCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  vTrace("%04lld: %18s\n"_Stz,i_Code,aEO[eCode]);
  
  i_Code++;
}

/*----------------------------------------------------------------------------*/
/* dump code                                                                  */
/*----------------------------------------------------------------------------*/
void RegSet::vDumpCode(eEO eCode, i8& i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  i8* piArg = (i8*) & m_eaCode[i_Code+1];
  i8 iArg = * piArg;

  vTrace("%04lld: %18s (%lld)\n"_Stz,i_Code,aEO[eCode],iArg);

  i_Code += 1 + sizeof(i8);
}

#if 0

  #define dCC3(eCode,iA,iB,iC) \
    if (bTrace) \
      vTrace("%lld : %12s %lld %lld %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA,iB,iC);

  #define dCC2(eCode,iA,iB) \
    if (bTrace) \
      vTrace("%lld : %12s %lld %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA,iB);

  #define dCC1(eCode,iA) \
    if (bTrace) \
      vTrace("%lld : %12s %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA);

  #define dCC0(eCode) \
    if (bTrace) \
      vTrace("%lld : %12s\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode]);

#else

  #define dCC3(eCode,iA,iB,iC)
  #define dCC2(eCode,iA,iB)
  #define dCC1(eCode,iA)
  #define dCC0(eCode)

#endif


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 RegSet::iCompute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  i8 iArg;
  i8 iTopA;
  i8 iTopB;
  i8 iRes;
  i8 iCmp;
  i8 i_Cell_ntv;
  
  bool bTrace = true;

  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;

  eEO  eCode;
  eEO* aeCode = & m_eaCode[0];
  i8*  aiDstk = &m_iaDataStack[0];
  
  IndexDef* pIndexDef = m_pIndexTree ? m_pIndexTree->pIndexDef() : nullptr;

  eEO** aaeRstk = &m_aaReturnStack[0];

  for (;;) {
  
    eCode = *aeCode++;
    
    switch (eCode) {
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // arithmetic operators
      case EO_ADD: // add top two int's push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA + iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_DIVIDE: // divide top int by second push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA / iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_MODULO: // modulo top int be second push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA % iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_MULTIPLY: // multiply top two int's push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA * iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_NEGATE: // negate the top value
        iTopA = *(aiDstk--);
        iRes  = - iTopA;
        dCC2(eCode,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_SUBTRACT: // subtract second int from top push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA - iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // branch operators
      case EO_JMP: // jump to location
        iArg = * (i8*)aeCode;
        dCC1(eCode,iArg);
        aeCode += 8;
        aeCode = & m_eaCode[iArg];
        break;
        
      case EO_JMPFALSE: // pop top and jump to location if false
        iArg = * (i8*)aeCode;
        dCC1(eCode,iArg);
        aeCode += 8;
        iTopA = *(aiDstk--);
        if (! iTopA)
          aeCode = & m_eaCode[iArg];
        break;
        
      case EO_JMPTRUE: // pop top and jump to location if true
        iArg = * (i8*)aeCode;
        dCC1(eCode,iArg);
        aeCode += 8;
        iTopA = *(aiDstk--);
        if (iTopA)
          aeCode = & m_eaCode[iArg];
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry
      case EO_ENT_CREATEPAY: // create a payload in entwrk
        iArg = * (i8*)aeCode;
        dCC1(eCode,iArg);
        aeCode += 8;
        m_oEntWrk.vPayload(true);
        m_oEntWrk.vPayloadBytes(iArg);
        break;
        
      case EO_ENT_INSERT: // insert an entry into the indexTree
      {
        dCC0(eCode);
        SP_Ent spEnt;
        vTrace("Inserting entry"_Stz);
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pIndexTree->pIndexDef()));
        m_pEnt = m_pIndexTree->aInsertEntry(m_oEntWrk.aEnt());
        vTrace("\n\nEnt : %s\n"_Stz,spEnt.sz(m_pEnt,m_pIndexTree->pIndexDef()));
        ASSERT(m_pEnt);
      }
        break;
        
      case EO_ENT_INSERTPAY: NEVER_REACH; // not yet implemented
        
      case EO_ENT_NEXT: // move to next m_pEnt
        dCC0(eCode);
        m_pEnt = EntBuffer::pMoveToNextEntry(m_pEnt, &m_pEntBuffer);
        if (m_pEnt)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;
        
      case EO_ENT_POSPRIOR: // position m_pEnt prior using m_oEntWrk.key
        dCC0(eCode);
        m_pEnt = m_pIndexTree->aPositionPriorEntry(m_oEntWrk.aKey(), &m_pEntBuffer);
        break;
        
      case EO_ENT_PUSH: // push entry on the stack
        dCC0(eCode);
        *(++aiDstk) = (i8)m_pEnt;
        break;
        
      case EO_ENT_PUSHREF: NEVER_REACH; // not yet implemented
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry relational operators
      case EO_ENT_EQUAL: {
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pIndexTree->pIndexDef()));
        vTrace("\n\nEnt : %s\n"_Stz,spEnt.sz(m_pEnt,m_pIndexTree->pIndexDef()));
        iRes  = KC_SAME == Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dCC1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
      }
        
      case EO_ENT_NOTEQUAL:
        iRes  = KC_SAME != Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dCC1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_GREATEREQUAL:
        iCmp = Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey());
        iRes = (KC_SAME == iCmp || KC_AFTER == iCmp ) ? 1 : 0;
        dCC1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_GREATERTHAN:
        iRes  = KC_AFTER == Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dCC1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_LESSTHAN: // test if m_pEnt.key <  m_oEntWrk.key
        iRes  = KC_BEFORE == Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dCC1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_LESSEQUAL: // test if m_pEnt.key <= m_oEntWrk.key
        iCmp = Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey());
        iRes  = (KC_SAME == iCmp || KC_BEFORE == iCmp ) ? 1 : 0;
        dCC1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entWrk
      case EO_ENTWRK_APPENDELMI: // append next element value (int)
      {
        iArg = * (i8*)aeCode;
        dCC1(eCode,iArg);
        aeCode += 8;
        m_oEntWrk.vAppendElement(m_aSrcRsb, pIndexDef->pGetColumn(iArg));
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pIndexTree->pIndexDef()));
        break;
      }
        
      case EO_ENTWRK_POPREF: NEVER_REACH; // not yet implemented

      case EO_ENTWRK_RESET: // reset m_oEntWrk
        dCC0(eCode);
        m_oEntWrk.vReset();
        break;
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // index tree operators
      case EO_IXT_INS_ENT:      // insert ent
        RASSERT(0);
        
      case EO_IXT_TRV_BEGIN:    // begin linear traverse
        m_pEntBuffer = m_pIndexTree->pFirstH0Buffer();
        if (m_pEntBuffer) {
          m_pEnt = m_pEntBuffer->aLeadEntry();
          if (m_pEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;
        
      case EO_IXT_TRV_NEXT:     // move to next entry
        m_pEntBuffer = m_pEntBuffer->pNextBuffer();
        if (m_pEntBuffer) {
          m_pEnt = m_pEntBuffer->aLeadEntry();
          if (m_pEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // logical operators
      case EO_AND: // logical and top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA && iTopB) ? 1 : 0;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_OR: // logical or top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA || iTopB) ? 1 : 0;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_NOT: // logical not top int, push result
        iTopA = *(aiDstk--);
        iRes  = (! iTopA) ? 1 : 0;
        dCC2(eCode,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // null test operators
      case EO_ISNULL: // test if top is null, push result
        iTopA = *(aiDstk--);
        iRes  = (C_DATUM_NULL == iTopA) ? 1 : 0;
        dCC2(eCode,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ISNOTNULL: // test if top is not null, push result
        iTopA = *(aiDstk--);
        iRes  = (C_DATUM_NULL != iTopA) ? 1 : 0;
        dCC2(eCode,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pay load
      case EO_PAY_FETCH_COL: // fetch a column from the entry payload
        iArg = * (i8*)aeCode;
        aeCode += 8;
        m_aPayRsb = (Rsb*)m_pEnt->ayPayload();
        i_Cell_ntv = m_aPayRsb->i_ColToCell_ntv(iArg);
        iRes = m_aPayRsb->iVal_ntv(i_Cell_ntv);
        dCC2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_PAY_STORE_COL: // store a column of the entry payload
        iArg = * (i8*)aeCode;
        aeCode += 8;
        iRes = *(aiDstk--);
        dCC2(eCode,iArg,iRes);
        m_aPayRsb = (Rsb*)m_pEnt->ayPayload();
        i_Cell_ntv = m_aPayRsb->i_ColToCell_ntv(iArg);
        m_aPayRsb->vVal_ntv(i_Cell_ntv, iRes);
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pushes
      case EO_INTEGER: // push integer
        iArg = * (i8*)aeCode;
        dCC1(eCode,iArg);
        aeCode += 8;
        *(++aiDstk) = iArg;
        break;
        
      case EO_PUSHNULL: // push null
        dCC0(eCode);
        *(++aiDstk) = C_DATUM_NULL;
        break;
        
      case EO_PUSH0: // push integer 0
        dCC0(eCode);
        *(++aiDstk) = 0;
        break;
        
      case EO_PUSH1: // push integer 1
        dCC0(eCode);
        *(++aiDstk) = 1;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ref
      case EO_REF_GETCRW: NEVER_REACH; // not yet implemented
      case EO_REF_SRCRSB: NEVER_REACH; // not yet implemented
      case EO_REF_ALTRSB: NEVER_REACH; // not yet implemented

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb / rowslab
      case EO_RSB_DST_COPYSRC: // copy src rsb to dst rsb
        dCC0(eCode);
        m_pDstRowSlab->vPositionOnCell(0);
        m_aDstRsb = m_pDstRowSlab->aRsb();
        m_pDstRowSlab->vCopyRsb(m_aSrcRsb,bSrcHasPtnHash,bCopyPtnHash);
        break;

      case EO_RSB_DST_PRINT: // print dst rsb
        m_pDstRowSlab->vDumpRow(-1,m_aDstRsb);
        break;

      case EO_RSB_SRC_PRINT: // print src rsb
        m_pSrcRowSlab->vDumpRow(-1,m_aSrcRsb);
        break;

      case EO_RSB_ALT_PRINT: // print alt rsb
        m_pAltRowSlab->vDumpRow(-1,m_aAltRsb);
        break;

      case EO_RSB_DST_NEW: // create new destination rsb
        m_aDstRsb = m_pPot->aNewDstRsb(&m_pDstRowSlab);
        break;

      case EO_RSB_SRC_BEGIN: // begin src rsb traverse
        m_i_SrcRow = -1;
        break;

      case EO_RSB_SRC_NEXT: // move to next src rsb
        if (++m_i_SrcRow < m_nSrcRows) {
          m_aSrcRsb = m_pSrcRowSlab->aRsbAdr(m_i_SrcRow);
          *(++aiDstk) = 1;
        }
        else {
          *(++aiDstk) = 0;
        }
        break;

      case EO_RSB_SRC_PUSHOFS:  NEVER_REACH; // not yet implemented
      case EO_RSB_SRC_SETBYOFS: NEVER_REACH; // not yet implemented
      case EO_RSB_ALT_BEGIN:    NEVER_REACH; // not yet implemented
      case EO_RSB_ALT_NEXT:     NEVER_REACH; // not yet implemented
      case EO_RSB_ALT_PUSHOFS:  NEVER_REACH; // not yet implemented
      case EO_RSB_ALT_SETBYOFS: NEVER_REACH; // not yet implemented

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // relational operators
      case EO_LESSTHAN: // test top < 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA < iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_LESSEQUAL: // test top <= 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA <= iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_EQUAL: // test top == 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA == iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_NOTEQUAL: // test top != 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA != iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_GREATEREQUAL: // test top >= 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA >= iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_GREATERTHAN: // test top > 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA > iTopB;
        dCC3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb operators
      case EO_DEPOSITCOLI: // pop and deposit top into dst column
        iArg = * (i8*)aeCode;
        aeCode += 8;
        iRes = *(aiDstk--);
        dCC2(eCode,iArg,iRes);
        m_pDstRowSlab->vPositionOnCell(iArg);
        m_pDstRowSlab->vDeposit(iRes);
        break;
        
      case EO_PUSHDSTCOLI: // push dst column
        iArg = * (i8*)aeCode;
        aeCode += 8;
        m_aDstRsb = m_pDstRowSlab->aRsbAdr(0);
        i_Cell_ntv = m_aDstRsb->i_ColToCell_ntv(iArg);
        iRes = m_aDstRsb->iVal_ntv(i_Cell_ntv);
        dCC2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_PUSHSRCCOLI: // push src column
        iArg = * (i8*)aeCode;
        aeCode += 8;
        iRes = m_aSrcRsb->iVal_ntv(m_aSrcRsb->i_ColToCell_ntv(iArg));
        dCC2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_PUSHALTCOLI:     NEVER_REACH; // not yet implemented
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // stack operators
      case EO_INC: // increment top, push result
        iTopA = *aiDstk;
        iRes = iTopA + 1;
        dCC2(eCode,iTopA,iRes);
        *aiDstk = iRes;
        break;

      case EO_DEC: // decrement top, push result
        iTopA = *aiDstk;
        iRes = iTopA - 1;
        dCC2(eCode,iTopA,iRes);
        *aiDstk = iRes;
        break;

      case EO_DUP: // duplicate top
        iTopA = *aiDstk;
        dCC1(eCode,iTopA);
        *(++aiDstk) = iTopA;
        break;

      case EO_POP: // pop / discard top
        iTopA = *aiDstk;
        dCC1(eCode,iTopA);
        break;

      case EO_PRINT: // print top
        iTopA = *(aiDstk--);
        dCC1(eCode,iTopA);
        vTrace("%lld\n"_Stz,iTopA);
        break;
    
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // subroutine operators
      case EO_CALL: // call subroutine at location
        iArg = * (i8*)aeCode;
        dCC1(eCode,iArg);
        aeCode += 8;
        * (aaeRstk++) = aeCode;
        aeCode = & m_eaCode[iArg];
        break;

      case EO_RETURN: // return from subroutine and
        dCC0(eCode);
        if (aaeRstk == &m_aaReturnStack[0]) {
          iRes = *(aiDstk--);
          return iRes;
        }
        aeCode = * (--aaeRstk);
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // misc
      case EO_NOOP: // no op
        dCC0(eCode);
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      default:
        vTrace(" unknown opcode in calculator %lld\n"_Stz,(i8)eCode);
        return C_DATUM_NULL;
    }
  }
  
  return m_iaDataStack[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
