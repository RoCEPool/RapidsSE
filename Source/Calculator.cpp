/*============================================================================*/
/* Calculator.cpp -- Calculator                                               */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,Calculator,CN_CALCULATOR)
__aNewFunction(Calculator,CN_CALCULATOR)
__vDeleteFunction(Calculator,CN_CALCULATOR)
__aFooterFunction(Calculator)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Calculator::Calculator(void)
: Object(CN_CALCULATOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_pDstRowSlab = nullptr;

  m_aSrcRsb_left      = nullptr;
  m_pSrcRowSlab_left  = nullptr;
  
  m_aSrcRsb_right     = nullptr;
  m_pSrcRowSlab_right = nullptr;
 
  m_pEnt = nullptr;
  // m_oEntWrk
  m_pIndexTree = nullptr;
  
  m_i_Code = 0;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_eaCode,sizeof(m_eaCode),CN_CALCULATOR);
  MemLib::vZero(m_iaDataStack,sizeof(m_iaDataStack),CN_CALCULATOR);
  MemLib::vZero(m_aaReturnStack,sizeof(m_aaReturnStack),CN_CALCULATOR);
#endif
  
  m_nLabels = 0;
  for (i8 i_Label = 0; i_Label < C_CALC_LABEL; i_Label++) {
    m_xaJumpLabel[i_Label].m_bDefined = false;
    m_xaJumpLabel[i_Label].m_i_Chain = C_UNKNOWN;
  }
  
  _checkInitialised_(CN_CALCULATOR);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Calculator::~Calculator(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* project compute                                                            */
/*----------------------------------------------------------------------------*/
i8 Calculator::iAggregate(byte* ayPayload, Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  //m_ayDstPayload = ayPayload;
  
  m_aSrcRsb_left      = aSrcRsb;
  m_pSrcRowSlab_left  = pSrcRowSlab;
  
  m_aSrcRsb_right     = nullptr;
  m_pSrcRowSlab_right = nullptr;
  
  i8 iResult = iCompute();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool Calculator::bCompile(TokenVec* pTknVec)
{
  bool b = bCompileCode(pTknVec);
  return b;
}

/*----------------------------------------------------------------------------*/
/* calc compute                                                               */
/*----------------------------------------------------------------------------*/
i8 Calculator::iCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_pDstRowSlab = nullptr;

  m_aSrcRsb_left      = nullptr;
  m_pSrcRowSlab_left  = nullptr;

  m_aSrcRsb_right     = nullptr;
  m_pSrcRowSlab_right = nullptr;
  
  i8 iResult = iCompute();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* filter compute                                                             */
/*----------------------------------------------------------------------------*/
i8 Calculator::iFilter(Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_pDstRowSlab = nullptr;

  m_aSrcRsb_left      = aSrcRsb;
  m_pSrcRowSlab_left  = pSrcRowSlab;

  m_aSrcRsb_right     = nullptr;
  m_pSrcRowSlab_right = nullptr;

  i8 iResult = iCompute();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* project compute                                                            */
/*----------------------------------------------------------------------------*/
i8 Calculator::iProject(RowSlab* pDstRowSlab, Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_pDstRowSlab = pDstRowSlab;

  m_aSrcRsb_left      = aSrcRsb;
  m_pSrcRowSlab_left  = pSrcRowSlab;

  m_aSrcRsb_right     = nullptr;
  m_pSrcRowSlab_right = nullptr;

  i8 iResult = iCompute();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* joint compute                                                              */
/*----------------------------------------------------------------------------*/
i8 Calculator::iJoin(RowSlab* pDstRowSlab,
                     Rsb* aSrcRsb_left,  RowSlab* pSrcRowSlab_left,
                     Rsb* aSrcRsb_right, RowSlab* pSrcRowSlab_right)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_pDstRowSlab = pDstRowSlab;

  m_aSrcRsb_left      = aSrcRsb_left;
  m_pSrcRowSlab_left  = pSrcRowSlab_left;

  m_aSrcRsb_right     = aSrcRsb_right;
  m_pSrcRowSlab_right = pSrcRowSlab_right;

  i8 iResult = iCompute();
  
  return iResult;
}


/*----------------------------------------------------------------------------*/
/* -- private ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add code                                                                   */
/*----------------------------------------------------------------------------*/
void Calculator::vAddCode(eEO eCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_eaCode[m_i_Code++] = eCode;
}

/*----------------------------------------------------------------------------*/
/* add code with argument                                                     */
/*----------------------------------------------------------------------------*/
void Calculator::vAddCode(eEO eCode, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_eaCode[m_i_Code++] = eCode;

  i8* piArg = (i8*) & m_eaCode[m_i_Code];
  * piArg = iArg;
  
  m_i_Code += sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* add code                                                                   */
/*----------------------------------------------------------------------------*/
void Calculator::vAddCode(eEO eCode, Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label) {
    i_Label = iAddLabel(szLabelNm,false);
    m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code + 1;
    vAddCode(eCode,C_UNKNOWN);
    vPrintLabels();
    return;
  }

  if (m_xaJumpLabel[i_Label].m_bDefined) {
    vAddCode(eCode,m_xaJumpLabel[i_Label].m_i_Code);
    return;
  }
  
  i8 i_Chain = m_xaJumpLabel[i_Label].m_i_Chain;
  m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code + 1;
  vAddCode(eCode,i_Chain);
  vPrintLabels();
}

/*----------------------------------------------------------------------------*/
//void Calculator::vAddCode(eEO eCode, Stz* szLabelNm)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
//
//  i8 iArg = iDecodeLabel(szLabelNm);
//
//  vAddCode(eCode,iArg);
//}

/*----------------------------------------------------------------------------*/
/* add label                                                                  */
/*----------------------------------------------------------------------------*/
i8 Calculator::iAddLabel(Stz* szLabelNm, bool bKnown)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
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
void Calculator::vCorrectLabel(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
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
/* add label                                                                  */
/*----------------------------------------------------------------------------*/
i8 Calculator::iDecodeLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
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
void Calculator::vDefineLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
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
i8 Calculator::iFindLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (m_xaJumpLabel[i_Label].m_sLabel.bSame(szLabelNm))
      return i_Label;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* print labels                                                               */
/*----------------------------------------------------------------------------*/
void Calculator::vPrintLabels(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    vTrace("  %s %lld: %4lld\n"_Stz,
           m_xaJumpLabel[i_Label].m_sLabel.szStz(),
           m_xaJumpLabel[i_Label].m_bDefined,
           m_xaJumpLabel[i_Label].m_i_Chain);
}

// calc l a 1 ;

// calc 10 l a dup . dec dup jt a ;

// calc 1 l a dup . inc dup 10 > jf a;

// calc 1 l a call p inc dup 10 > jf a ret l p dup . ret;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool Calculator::bCompileCode(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  //bCompileTest(pTknVec);
  
  i8* piArg;
  i8  iArg = 0;

  i8 nTkns = pTknVec->nTokens();
  
  for (i8 i_Tkn = 0; i_Tkn < nTkns; i_Tkn++) {
    Token* pTkn = pTknVec->pGet(i_Tkn);
    
    vTrace(" >%s<\n"_Stz,pTkn->szChars());
    
    // -- -- -- -- -- -- -- -- -- -- --

    if (pTkn->bSame("+"_Stz)) { vAddCode(EO_ADD);      continue; }
    if (pTkn->bSame("/"_Stz)) { vAddCode(EO_DIVIDE);   continue; }
    if (pTkn->bSame("%"_Stz)) { vAddCode(EO_MODULO);   continue; }
    if (pTkn->bSame("*"_Stz)) { vAddCode(EO_MULTIPLY); continue; }
    if (pTkn->bSame("-"_Stz)) { vAddCode(EO_SUBTRACT); continue; }

    // -- -- -- -- -- -- -- -- -- -- --

    if (pTkn->bSame("<"_Stz))  { vAddCode(EO_LESSTHAN);     continue; }
    if (pTkn->bSame("<="_Stz)) { vAddCode(EO_LESSEQUAL);    continue; }
    if (pTkn->bSame("=="_Stz)) { vAddCode(EO_EQUAL);        continue; }
    if (pTkn->bSame("!="_Stz)) { vAddCode(EO_NOTEQUAL);     continue; }
    if (pTkn->bSame(">="_Stz)) { vAddCode(EO_GREATEREQUAL); continue; }
    if (pTkn->bSame(">"_Stz))  { vAddCode(EO_GREATERTHAN);  continue; }
    
    // -- -- -- -- -- -- -- -- -- -- --
    
    if (pTkn->bSame("and"_Stz)) { vAddCode(EO_AND); continue; }
    if (pTkn->bSame("or"_Stz))  { vAddCode(EO_OR);  continue; }
    if (pTkn->bSame("not"_Stz)) { vAddCode(EO_NOT); continue; }

    // -- -- -- -- -- -- -- -- -- -- --
    
    if (pTkn->bSame("isNull"_Stz))    { vAddCode(EO_ISNULL);    continue; }
    if (pTkn->bSame("isNotNull"_Stz)) { vAddCode(EO_ISNOTNULL); continue; }
    
    // -- -- -- -- -- -- -- -- -- -- --

    if (pTkn->bSame("++"_Stz))  { vAddCode(EO_INC);   continue; }
    if (pTkn->bSame("inc"_Stz)) { vAddCode(EO_INC);   continue; }
    if (pTkn->bSame("--"_Stz))  { vAddCode(EO_DEC);   continue; }
    if (pTkn->bSame("dec"_Stz)) { vAddCode(EO_DEC);   continue; }
    if (pTkn->bSame("dup"_Stz)) { vAddCode(EO_DUP);   continue; }
    if (pTkn->bSame("pop"_Stz)) { vAddCode(EO_POP);   continue; }
    if (pTkn->bSame("."_Stz))   { vAddCode(EO_PRINT); continue; }

    // -- -- -- -- -- -- -- -- -- -- --
    
    if (pTkn->bSame("ret"_Stz)) { vAddCode(EO_RETURN); continue; }
    
    // -- -- -- -- -- -- -- -- -- -- --

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
      iArg = pTkn->iToI8();
      vAddCode(EO_PUSHSRCCOLI,iArg);
      continue;
    }

    // push dst column value
    if (pTkn->bSame("#"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      iArg = pTkn->iToI8();
      vAddCode(EO_PUSHDSTCOLI,iArg);
      continue;
    }
    
    // deposit dst column value
    if (pTkn->bSame("!"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      iArg = pTkn->iToI8();
      vAddCode(EO_DEPOSITCOLI,iArg);
      continue;
    }

    // copy src row into dst
    if (pTkn->bSame("="_Stz)) {
      vAddCode(EO_COPYRSB);
      continue;
    }
    
    // call subroutine
    if (pTkn->bSame("call"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCode(EO_CALL,pTkn->szChars());
      continue;
    }

    // label/location
    if (pTkn->bSame("L"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vDefineLabel(pTkn->szChars());
      continue;
    }

    // jump
    if (pTkn->bSame("j"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCode(EO_JMP,pTkn->szChars());
      continue;
    }

    // jump on false
    if (pTkn->bSame("jf"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCode(EO_JMPFALSE,pTkn->szChars());
      continue;
    }
    
    // jump on true
    if (pTkn->bSame("jt"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCode(EO_JMPTRUE,pTkn->szChars());
      continue;
    }

    if (pTkn->bIsInteger()) {
      iArg = pTkn->iToI8();
      vAddCode(EO_INTEGER,iArg);
//      iArg = pTkn->iToI8();
//      piArg = (i8*) & m_eaCode[m_i_Code];
//      * piArg = iArg;
//      m_i_Code += sizeof(i8);
      continue;
    }
    
    if (pTkn->bSame(C_NULL)) {
      vAddCode(EO_INTEGER);
      iArg = C_DATUM_NULL;
      piArg = (i8*) & m_eaCode[m_i_Code];
      m_i_Code += sizeof(i8);
      continue;
    }
    
    if (pTkn->bSame("0"_Stz)) {
      vAddCode(EO_INTEGER);
      iArg = 0;
      piArg = (i8*) & m_eaCode[m_i_Code];
      * piArg = iArg;
      m_i_Code += sizeof(i8);
      continue;
    }
    
    if (pTkn->bSame("1"_Stz)) {
      vAddCode(EO_INTEGER);
      iArg = 1;
      piArg = (i8*) & m_eaCode[m_i_Code];
      * piArg = iArg;
      m_i_Code += sizeof(i8);
      continue;
    }

    // -- -- -- -- -- -- -- -- -- -- --
    
    vAddCode(EO_RETURN);
    vTrace("Error: %s\n"_Stz,pTkn->szChars());
    return false;
  }

  vAddCode(EO_RETURN);
  vTrace("\n"_Stz);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (! m_xaJumpLabel[i_Label].m_bDefined)
      return false;

  vDumpCode();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* dump code                                                                  */
/*----------------------------------------------------------------------------*/
void Calculator::vDumpCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  //bCompileTest(pTknVec);
  
  i8* piArg;
  i8  iArg = 0;
  
  for (i8 i_Code = 0; i_Code < m_i_Code; ) {
    
    eEO eCode = m_eaCode[i_Code];
    
    switch (eCode) {
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_INTEGER:        vDumpCode(EO_INTEGER,i_Code);              break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_COPYRSB:        vDumpCode(i_Code,EO_COPYRSB);              break;
      case EO_DEPOSITCOLI:    vDumpCode(EO_DEPOSITCOLI,i_Code);          break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_PUSHDSTCOLI:    vDumpCode(EO_PUSHDSTCOLI,i_Code);          break;
      case EO_PUSHNULL:       vDumpCode(i_Code,EO_PUSHNULL);             break;
      case EO_PUSHSRCCOLI:    vDumpCode(EO_PUSHSRCCOLI,i_Code);          break;
      case EO_PUSH0:          vDumpCode(i_Code,EO_PUSH0);                break;
      case EO_PUSH1:          vDumpCode(i_Code,EO_PUSH1);                break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_ADD:            vDumpCode(i_Code,EO_ADD);                  break;
      case EO_DIVIDE:         vDumpCode(i_Code,EO_DIVIDE);               break;
      case EO_MODULO:         vDumpCode(i_Code,EO_MODULO);               break;
      case EO_MULTIPLY:       vDumpCode(i_Code,EO_MULTIPLY);             break;
      case EO_NEGATE:         vDumpCode(i_Code,EO_NEGATE);               break;
      case EO_SUBTRACT:       vDumpCode(i_Code,EO_SUBTRACT);             break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_LESSTHAN:       vDumpCode(i_Code,EO_LESSTHAN);             break;
      case EO_LESSEQUAL:      vDumpCode(i_Code,EO_LESSEQUAL);            break;
      case EO_EQUAL:          vDumpCode(i_Code,EO_EQUAL);                break;
      case EO_NOTEQUAL:       vDumpCode(i_Code,EO_NOTEQUAL);             break;
      case EO_GREATEREQUAL:   vDumpCode(i_Code,EO_GREATEREQUAL);         break;
      case EO_GREATERTHAN:    vDumpCode(i_Code,EO_GREATERTHAN);          break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_AND:            vDumpCode(i_Code,EO_AND);                  break;
      case EO_OR:             vDumpCode(i_Code,EO_OR);                   break;
      case EO_NOT:            vDumpCode(i_Code,EO_NOT);                  break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_ISNULL:         vDumpCode(i_Code,EO_ISNULL);               break;
      case EO_ISNOTNULL:      vDumpCode(i_Code,EO_ISNOTNULL);            break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_INC:            vDumpCode(i_Code,EO_INC);                  break;
      case EO_DEC:            vDumpCode(i_Code,EO_DEC);                  break;
      case EO_DUP:            vDumpCode(i_Code,EO_DUP);                  break;
      case EO_POP:            vDumpCode(i_Code,EO_POP);                  break;
      case EO_PRINT:          vDumpCode(i_Code,EO_PRINT);                break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_JMP:            vDumpCode(EO_JMP,i_Code);                  break;
      case EO_JMPFALSE:       vDumpCode(EO_JMPFALSE,i_Code);             break;
      case EO_JMPTRUE:        vDumpCode(EO_JMPTRUE,i_Code);              break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case EO_CALL:           vDumpCode(EO_CALL,i_Code);                 break;
      case EO_RETURN:         vDumpCode(i_Code,EO_RETURN);               break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      default:
        vTrace(" unknown opcode in calculator %lld\n"_Stz,(i8)eCode);
        ASSERT(0);
    }

  }

}

/*----------------------------------------------------------------------------*/
/* dump code                                                                  */
/*----------------------------------------------------------------------------*/
void Calculator::vDumpCode(i8& i_Code, eEO eCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  vTrace("%04lld: %15s\n"_Stz,i_Code,aEO[eCode]);
  
  i_Code++;
}

/*----------------------------------------------------------------------------*/
/* dump code                                                                  */
/*----------------------------------------------------------------------------*/
void Calculator::vDumpCode(eEO eCode, i8& i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  i8* piArg = (i8*) & m_eaCode[i_Code+1];
  i8 iArg = * piArg;

  vTrace("%04lld: %15s (%lld)\n"_Stz,i_Code,aEO[eCode],iArg);

  i_Code += 1 + sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 Calculator::iCompute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  i8 iArg;
  i8 iTopA;
  i8 iTopB;
  i8 iRes;
  i8 i_Cell_ntv;
  Rsb* aRsb;

  
  eEO  eCode;
  eEO* aeCode = & m_eaCode[0];
  i8*  aiDstk = &m_iaDataStack[0];

  eEO** aaeRstk = &m_aaReturnStack[0];

  for (;;) {
  
    eCode = *aeCode++;
    
    switch (eCode) {
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_INTEGER:
        iArg = * (i8*)aeCode;
        aeCode += 8;
        *(++aiDstk) = iArg;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
          
      case EO_COPYRSB:
      {
        bool bSrcHasPtnHash = true;
        bool bCopyPtnHash   = false;
        m_pDstRowSlab->vPositionOnCell(0);
        m_pDstRowSlab->vCopyRsb(m_aSrcRsb_left,bSrcHasPtnHash,bCopyPtnHash);
        break;
      }
        
      case EO_DEPOSITCOLI:
        iArg = * (i8*)aeCode;
        aeCode += 8;
        iRes = *(aiDstk--);
        m_pDstRowSlab->vPositionOnCell(iArg);
        m_pDstRowSlab->vDeposit(iRes);
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
      case EO_PUSHDSTCOLI:
        iArg = * (i8*)aeCode;
        aeCode += 8;
        aRsb = m_pDstRowSlab->aRsbAdr(0);
        i_Cell_ntv = aRsb->i_ColToCell_ntv(iArg);
        iRes = aRsb->iVal_ntv(i_Cell_ntv);
        *(++aiDstk) = iRes;
        break;
        
      case EO_PUSHNULL:
        *(++aiDstk) = C_DATUM_NULL;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
      case EO_PUSHSRCCOLI:
        iArg = * (i8*)aeCode;
        aeCode += 8;
        iRes = m_aSrcRsb_left->iVal_ntv(m_aSrcRsb_left->i_ColToCell_ntv(iArg));
        *(++aiDstk) = iRes;
        break;
        
      case EO_PUSH0:
        *(++aiDstk) = 0;
        break;
        
      case EO_PUSH1:
        *(++aiDstk) = 1;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_ADD:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA + iTopB;
        *(++aiDstk) = iRes;
        break;
        
      case EO_DIVIDE:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA / iTopB;
        *(++aiDstk) = iRes;
        break;
        
      case EO_MODULO:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA % iTopB;
        *(++aiDstk) = iRes;
        break;
        
      case EO_MULTIPLY:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA * iTopB;
        *(++aiDstk) = iRes;
        break;
        
      case EO_NEGATE:
        iTopA = *(aiDstk--);
        iRes  = - iTopA;
        *(++aiDstk) = iRes;
        break;
        
      case EO_SUBTRACT:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA - iTopB;
        *(++aiDstk) = iRes;
        break;

      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_LESSTHAN:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA < iTopB;
        *(++aiDstk) = iRes;
        break;

      case EO_LESSEQUAL:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA <= iTopB;
        *(++aiDstk) = iRes;
        break;
        
      case EO_EQUAL:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA == iTopB;
        *(++aiDstk) = iRes;
        break;
        
      case EO_NOTEQUAL:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA != iTopB;
        *(++aiDstk) = iRes;
        break;
        
      case EO_GREATEREQUAL:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA >= iTopB;
        *(++aiDstk) = iRes;
        break;
        
      case EO_GREATERTHAN:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA > iTopB;
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_AND:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA && iTopB) ? 1 : 0;
        *(++aiDstk) = iRes;
        break;
        
      case EO_OR:
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA || iTopB) ? 1 : 0;
        *(++aiDstk) = iRes;
        break;
        
      case EO_NOT:
        iTopA = *(aiDstk--);
        iRes  = (! iTopA) ? 1 : 0;
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
      case EO_ISNULL:
        iTopA = *(aiDstk--);
        iRes  = (C_DATUM_NULL == iTopA) ? 1 : 0;
        *(++aiDstk) = iRes;
        break;

      case EO_ISNOTNULL:
        iTopA = *(aiDstk--);
        iRes  = (C_DATUM_NULL != iTopA) ? 1 : 0;
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --

      case EO_INC:
        iTopA = *aiDstk;
        *aiDstk = iTopA + 1;
        break;

      case EO_DEC:
        iTopA = *aiDstk;
        *aiDstk = iTopA - 1;
        break;

      case EO_DUP:
        iTopA = *aiDstk;
        *(++aiDstk) = iTopA;
        break;

      case EO_POP:
        iTopA = *aiDstk;
        break;

      case EO_PRINT:
        iTopA = *(aiDstk--);
        vTrace("%lld\n"_Stz,iTopA);
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --

      case EO_JMP:
        iArg = * (i8*)aeCode;
        aeCode += 8;
        
        aeCode = & m_eaCode[iArg];
        break;
        
      case EO_JMPFALSE:
        iArg = * (i8*)aeCode;
        aeCode += 8;
        
        iTopA = *(aiDstk--);
        if (! iTopA)
          aeCode = & m_eaCode[iArg];
        
        break;
      
      case EO_JMPTRUE:
        iArg = * (i8*)aeCode;
        aeCode += 8;
        
        iTopA = *(aiDstk--);
        if (iTopA)
          aeCode = & m_eaCode[iArg];
        
        break;
    
      // -- -- -- -- -- -- -- -- -- -- --

      case EO_CALL:
        iArg = * (i8*)aeCode;
        aeCode += 8;
        * (aaeRstk++) = aeCode;
        aeCode = & m_eaCode[iArg];
        break;

      case EO_RETURN:
        if (aaeRstk == &m_aaReturnStack[0]) {
          iRes = *(aiDstk--);
          return iRes;
        }
        aeCode = * (--aaeRstk);
        break;

      // -- -- -- -- -- -- -- -- -- -- --
        
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
