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

  m_aSrcRsb     = nullptr;
  m_pSrcRowSlab = nullptr;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_eaCode, sizeof(m_eaCode), CN_CALCULATOR);
  MemLib::vZero(m_iaStack,sizeof(m_iaStack),CN_CALCULATOR);
#endif
  
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
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool Calculator::bCompile(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  //bCompileTest(pTknVec);
  
  i8* piArg;
  i8  iArg = 0;

  i8 nTkns = pTknVec->nTokens();
  
  i8 i_Code = 0;
  for (i8 i_Tkn = 0; i_Tkn < nTkns; i_Tkn++) {
    Token* pTkn = pTknVec->pGet(i_Tkn);
    
    vTrace(" >%s<"_Stz,pTkn->szChars());
    
    // -- -- -- -- -- -- -- -- -- -- --

    if (pTkn->bSame("+"_Stz)) { m_eaCode[i_Code++] = EO_ADD;      continue; }
    if (pTkn->bSame("/"_Stz)) { m_eaCode[i_Code++] = EO_DIVIDE;   continue; }
    if (pTkn->bSame("%"_Stz)) { m_eaCode[i_Code++] = EO_MODULO;   continue; }
    if (pTkn->bSame("*"_Stz)) { m_eaCode[i_Code++] = EO_MULTIPLY; continue; }
    if (pTkn->bSame("-"_Stz)) { m_eaCode[i_Code++] = EO_SUBTRACT; continue; }

    // -- -- -- -- -- -- -- -- -- -- --

    if (pTkn->bSame("<"_Stz))  { m_eaCode[i_Code++] = EO_LESSTHAN;     continue; }
    if (pTkn->bSame("<="_Stz)) { m_eaCode[i_Code++] = EO_LESSEQUAL;    continue; }
    if (pTkn->bSame("=="_Stz)) { m_eaCode[i_Code++] = EO_EQUAL;        continue; }
    if (pTkn->bSame("!="_Stz)) { m_eaCode[i_Code++] = EO_NOTEQUAL;     continue; }
    if (pTkn->bSame(">="_Stz)) { m_eaCode[i_Code++] = EO_GREATEREQUAL; continue; }
    if (pTkn->bSame(">"_Stz))  { m_eaCode[i_Code++] = EO_GREATERTHAN;  continue; }
    
    // -- -- -- -- -- -- -- -- -- -- --
    
    if (pTkn->bSame("and"_Stz)) { m_eaCode[i_Code++] = EO_AND; continue; }
    if (pTkn->bSame("or"_Stz))  { m_eaCode[i_Code++] = EO_OR;  continue; }
    if (pTkn->bSame("not"_Stz)) { m_eaCode[i_Code++] = EO_NOT; continue; }

    // -- -- -- -- -- -- -- -- -- -- --
    
    if (pTkn->bSame("isNull"_Stz))    { m_eaCode[i_Code++] = EO_ISNULL;    continue; }
    if (pTkn->bSame("isNotNull"_Stz)) { m_eaCode[i_Code++] = EO_ISNOTNULL; continue; }
    
    // -- -- -- -- -- -- -- -- -- -- --

    if (pTkn->bSame("dup"_Stz)) { m_eaCode[i_Code++] = EO_DUP;   continue; }
    if (pTkn->bSame("."_Stz))   { m_eaCode[i_Code++] = EO_PRINT; continue; }

    // -- -- -- -- -- -- -- -- -- -- --

//    if (pTkn->bIsName()) {
//      m_eaCode[i_Code++] = EO_PUSHCOLI;
//      i8 iColVec;
//      piArg = (i8*) & m_eaCode[i_Code];
//      * piArg = pStreamDef->iFindColumn(pTkn->azChars(), iColVec);
//      i_Code += sizeof(i8);
//      continue;
//    }
    
    // push src column value
    if (pTkn->bSame("@"_Stz)) {
      m_eaCode[i_Code++] = EO_PUSHCOLI;
      pTkn = pTknVec->pGet(++i_Tkn);
      iArg = pTkn->iToI8();
      piArg = (i8*) & m_eaCode[i_Code];
      * piArg = iArg;
      i_Code += sizeof(i8);
      continue;
    }

    // deposit dst column value
    if (pTkn->bSame("!"_Stz)) {
      m_eaCode[i_Code++] = EO_DEPOSITCOLI;
      pTkn = pTknVec->pGet(++i_Tkn);
      iArg = pTkn->iToI8();
      piArg = (i8*) & m_eaCode[i_Code];
      * piArg = iArg;
      i_Code += sizeof(i8);
      continue;
    }

    // copy src row into dst
    if (pTkn->bSame("="_Stz)) {
      m_eaCode[i_Code++] = EO_COPYRSB;
      continue;
    }
    
    if (pTkn->bIsInteger()) {
      m_eaCode[i_Code++] = EO_INTEGER;
      iArg = pTkn->iToI8();
      piArg = (i8*) & m_eaCode[i_Code];
      * piArg = iArg;
      i_Code += sizeof(i8);
      continue;
    }
    if (pTkn->bSame(C_NULL)) {
      m_eaCode[i_Code++] = EO_INTEGER;
      iArg = C_DATUM_NULL;
      piArg = (i8*) & m_eaCode[i_Code];
      i_Code += sizeof(i8);
      continue;
    }
    if (pTkn->bSame("0"_Stz)) {
      m_eaCode[i_Code++] = EO_INTEGER;
      iArg = 0;
      piArg = (i8*) & m_eaCode[i_Code];
      * piArg = iArg;
      i_Code += sizeof(i8);
      continue;
    }
    if (pTkn->bSame("1"_Stz)) {
      m_eaCode[i_Code++] = EO_INTEGER;
      iArg = 1;
      piArg = (i8*) & m_eaCode[i_Code];
      * piArg = iArg;
      i_Code += sizeof(i8);
      continue;
    }

    // -- -- -- -- -- -- -- -- -- -- --
    
    m_eaCode[i_Code++] = EO_RETURN;
    vTrace("Error: %s\n"_Stz,pTkn->szChars());
    return false;
  }

  m_eaCode[i_Code++] = EO_RETURN;
  vTrace("\n"_Stz);

  return true;
}

/*----------------------------------------------------------------------------*/
/* calc compute                                                               */
/*----------------------------------------------------------------------------*/
i8 Calculator::iCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_pDstRowSlab = nullptr;

  m_aSrcRsb     = nullptr;
  m_pSrcRowSlab = nullptr;

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

  m_aSrcRsb     = aSrcRsb;
  m_pSrcRowSlab = pSrcRowSlab;

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

  m_aSrcRsb     = aSrcRsb;
  m_pSrcRowSlab = pSrcRowSlab;

  i8 iResult = iCompute();
  
  return iResult;
}


/*----------------------------------------------------------------------------*/
/* -- private ----------------------------------------------------------------*/
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

  eEO  eCode;
  eEO* peCode = & m_eaCode[0];
 
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
        m_pDstRowSlab->vCopyRsb(m_aSrcRsb,bSrcHasPtnHash,bCopyPtnHash);
        break;
      }
        
      case EO_DEPOSITCOLI:
        iArgA = * (i8*)peCode;
        peCode += 8;
        iRes = m_iaStack[++i_TopVal];
        m_pDstRowSlab->vPositionOnCell(iArgA);
        m_pDstRowSlab->vDeposit(iRes);
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
      case EO_PUSHCOLI:
        iArgA = * (i8*)peCode;
        peCode += 8;
        iRes = m_aSrcRsb->iVal_ntv(m_aSrcRsb->i_ColToCell_ntv(iArgA));
        m_iaStack[++i_TopVal] = iRes;
        break;
        
      case EO_PUSHNULL:
        m_iaStack[++i_TopVal] = C_DATUM_NULL;
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
