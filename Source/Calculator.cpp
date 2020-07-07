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

  m_bCompilingKey = false;
  
  m_i_Code = 0;
  m_i_CodeKey = 0;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_eaCode,    sizeof(m_eaCode),    CN_CALCULATOR);
  MemLib::vZero(m_eaCodeKey, sizeof(m_eaCodeKey), CN_CALCULATOR);
  
  MemLib::vZero(m_iaStack,   sizeof(m_iaStack),   CN_CALCULATOR);
#endif
  
  m_nLabels = 0;
  for (i8 i_Label = 0; i_Label < C_CALC_LABEL; i_Label++) {
    m_xaJumpLabel[i_Label].m_bKnown = false;
    m_xaJumpLabel[i_Label].m_i_Code = C_UNKNOWN;
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
  m_bCompilingKey = false;

  bool b = bCompile(pTknVec,false);
  return b;
  
}

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool Calculator::bCompileKey(TokenVec* pTknVec)
{
  m_bCompilingKey = true;

  bool b = bCompile(pTknVec,true);

  m_bCompilingKey = false;

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
  
  if (m_bCompilingKey)
    m_eaCodeKey[m_i_CodeKey++] = eCode;
  else
    m_eaCode[m_i_Code++] = eCode;
}

/*----------------------------------------------------------------------------*/
/* add code                                                                   */
/*----------------------------------------------------------------------------*/
void Calculator::vAddCode(eEO eCode, Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  if (m_bCompilingKey)
    m_eaCodeKey[m_i_CodeKey++] = eCode;
  else
    m_eaCode[m_i_Code++] = eCode;
}

/*----------------------------------------------------------------------------*/
/* add label                                                                  */
/*----------------------------------------------------------------------------*/
void Calculator::vAddLabel(Stz* szLabelNm, bool bKnown)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_xaJumpLabel[m_nLabels].m_sLabel = szLabelNm;
  m_xaJumpLabel[m_nLabels].m_bKnown = bKnown;
  m_xaJumpLabel[m_nLabels].m_i_Code = m_i_Code;
  m_nLabels++;
}

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool Calculator::bCompile(TokenVec* pTknVec, bool bKeyCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  //bCompileTest(pTknVec);
  
  i8* piArg;
  i8  iArg = 0;

  i8 nTkns = pTknVec->nTokens();
  
  for (i8 i_Tkn = 0; i_Tkn < nTkns; i_Tkn++) {
    Token* pTkn = pTknVec->pGet(i_Tkn);
    
    vTrace(" >%s<"_Stz,pTkn->szChars());
    
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

    if (pTkn->bSame("dup"_Stz)) { vAddCode(EO_DUP);   continue; }
    if (pTkn->bSame("."_Stz))   { vAddCode(EO_PRINT); continue; }

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
      vAddCode(EO_PUSHSRCCOLI);
      pTkn = pTknVec->pGet(++i_Tkn);
      iArg = pTkn->iToI8();
      piArg = (i8*) & m_eaCode[m_i_Code];
      * piArg = iArg;
      m_i_Code += sizeof(i8);
      continue;
    }

    // push dst column value
    if (pTkn->bSame("#"_Stz)) {
      vAddCode(EO_PUSHDSTCOLI);
      pTkn = pTknVec->pGet(++i_Tkn);
      iArg = pTkn->iToI8();
      piArg = (i8*) & m_eaCode[m_i_Code];
      * piArg = iArg;
      m_i_Code += sizeof(i8);
      continue;
    }
    
    // deposit dst column value
    if (pTkn->bSame("!"_Stz)) {
      vAddCode(EO_DEPOSITCOLI);
      pTkn = pTknVec->pGet(++i_Tkn);
      iArg = pTkn->iToI8();
      piArg = (i8*) & m_eaCode[m_i_Code];
      * piArg = iArg;
      m_i_Code += sizeof(i8);
      continue;
    }

    // copy src row into dst
    if (pTkn->bSame("="_Stz)) {
      vAddCode(EO_COPYRSB);
      continue;
    }
    
    // label/location
    if (pTkn->bSame("L"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vDefineLabel(pTkn->szChars());
    }

    // jump
    if (pTkn->bSame("j"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCode(EO_JMP,pTkn->szChars());
    }

    // jump on false
    if (pTkn->bSame("jf"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCode(EO_JMPFALSE,pTkn->szChars());
    }
    
    // jump on true
    if (pTkn->bSame("jt"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCode(EO_JMPTRUE,pTkn->szChars());
    }

    if (pTkn->bIsInteger()) {
      vAddCode(EO_INTEGER);
      iArg = pTkn->iToI8();
      piArg = (i8*) & m_eaCode[m_i_Code];
      * piArg = iArg;
      m_i_Code += sizeof(i8);
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

  return true;
}

/*----------------------------------------------------------------------------*/
/* correct label chain                                                        */
/*----------------------------------------------------------------------------*/
void Calculator::vCorrectLabel(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  ASSERT(! m_xaJumpLabel[i_Label].m_bKnown);
  
  i8 i_Chain = m_xaJumpLabel[i_Label].m_i_Code;
  while (C_UNKNOWN != i_Chain) {
    
    i8* piArg = (i8*) & m_eaCode[i_Chain];
 
    i_Chain = * piArg;

    * piArg = m_i_Code;
  }

  m_xaJumpLabel[i_Label].m_bKnown = true;
}

/*----------------------------------------------------------------------------*/
/* define label                                                               */
/*----------------------------------------------------------------------------*/
void Calculator::vDefineLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label)
    vAddLabel(szLabelNm,true);
  else
    vCorrectLabel(i_Label);
}

/*----------------------------------------------------------------------------*/
/* add label                                                                  */
/*----------------------------------------------------------------------------*/
i8 Calculator::iFindLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < C_CALC_LABEL; i_Label++)
    if (m_xaJumpLabel[i_Label].m_sLabel.bSame(szLabelNm))
      return i_Label;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 Calculator::iCompute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  i8 iArgA;
  i8 iArgB;
  i8 iRes;
  i8 i_Cell_ntv;
  
  eEO  eCode;
  eEO* peCode = & m_eaCode[0];
 
  Rsb* aRsb;
  
  i8 i_TopVal = 0;
  m_iaStack[i_TopVal] = C_DATUM_NULL;

  for (;;) {
  
    eCode = *peCode++;
    
    switch (eCode) {
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_INTEGER:
        iRes = * (i8*)peCode;
        peCode += 8;
        m_iaStack[++i_TopVal] = iRes;
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
        iArgA = * (i8*)peCode;
        peCode += 8;
        iRes = m_iaStack[i_TopVal--];
        m_pDstRowSlab->vPositionOnCell(iArgA);
        m_pDstRowSlab->vDeposit(iRes);
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
      case EO_PUSHDSTCOLI:
        iArgA = * (i8*)peCode;
        peCode += 8;
        aRsb = m_pDstRowSlab->aRsbAdr(0);
        i_Cell_ntv = aRsb->i_ColToCell_ntv(iArgA);
        iRes = aRsb->iVal_ntv(i_Cell_ntv);
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_PUSHNULL:
        m_iaStack[++i_TopVal] = C_DATUM_NULL;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
      case EO_PUSHSRCCOLI:
        iArgA = * (i8*)peCode;
        peCode += 8;
        iRes = m_aSrcRsb_left->iVal_ntv(m_aSrcRsb_left->i_ColToCell_ntv(iArgA));
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_PUSH0:
        m_iaStack[++i_TopVal] = 0;
        break;
        
      case EO_PUSH1:
        m_iaStack[++i_TopVal] = 1;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_ADD:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA + iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_DIVIDE:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA / iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_MODULO:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA % iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_MULTIPLY:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA * iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_NEGATE:
        iArgA = m_iaStack[i_TopVal--];
        iRes  = - iArgA;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_SUBTRACT:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA - iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;

      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_LESSTHAN:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA < iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;

      case EO_LESSEQUAL:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA <= iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_EQUAL:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA == iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_NOTEQUAL:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA != iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_GREATEREQUAL:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA >= iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_GREATERTHAN:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = iArgA > iArgB;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_AND:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = (iArgA && iArgB) ? 1 : 0;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_OR:
        iArgB = m_iaStack[i_TopVal--];
        iArgA = m_iaStack[i_TopVal--];
        iRes  = (iArgA || iArgB) ? 1 : 0;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_NOT:
        iArgA = m_iaStack[i_TopVal--];
        iRes  = (! iArgA) ? 1 : 0;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
      case EO_ISNULL:
        iArgA = m_iaStack[i_TopVal--];
        iRes  = (C_DATUM_NULL == iArgA) ? 1 : 0;
        m_iaStack[++i_TopVal] = iRes;
        break;

      case EO_ISNOTNULL:
        iArgA = m_iaStack[i_TopVal--];
        iRes  = (C_DATUM_NULL != iArgA) ? 1 : 0;
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --

      case EO_DUP:
        iRes = m_iaStack[i_TopVal];
        m_iaStack[++i_TopVal] = iRes;
        break;

      case EO_PRINT:
        iRes = m_iaStack[i_TopVal--];
        vTrace("%lld\n"_Stz,iRes);
        break;
        
      case EO_RETURN:
        iRes = m_iaStack[i_TopVal--];
        return iRes;
        
      // -- -- -- -- -- -- -- -- -- -- --

      case EO_JMP:
        ASSERT(0); // net yet implemented
        break;
        
      case EO_JMPFALSE:
        ASSERT(0); // net yet implemented
        break;
      
      case EO_JMPTRUE:
        ASSERT(0); // net yet implemented
        break;
    
      // -- -- -- -- -- -- -- -- -- -- --

      default:
        vTrace(" unknown opcode in calculator\n"_Stz);
        return C_DATUM_NULL;
    }
  }
  
  return m_iaStack[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
