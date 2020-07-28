/*============================================================================*/
/* LogicUnit.hpp -- Logic Unit                                                */
/*============================================================================*/

#include "Expressions.h"

extern EoArg EoArgs[];

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,LogicUnit,CN_LOGICUNIT)
__aNewFunction(LogicUnit,CN_LOGICUNIT)
__vDeleteFunction(LogicUnit,CN_LOGICUNIT)
__aFooterFunction(LogicUnit)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LogicUnit::LogicUnit(Task* pTask)
: Object(CN_LOGICUNIT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
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
  
  m_iSql        = C_UNKNOWN;
  
  m_bPixInited  = false;
  // m_oStreamPix
  m_aDstRsb     = nullptr;
  m_pDstRowSlab = nullptr;

  m_i_SrcRow    = C_UNKNOWN;
  m_nSrcRows    = C_UNKNOWN;
  m_aSrcRsb     = nullptr;
  m_pSrcRowSlab = nullptr;
  m_aSrcEnt     = nullptr;
  m_pSrcIdxTree = nullptr;
  m_pSrcEntBuffer = nullptr;
  m_aSrcCrw     = nullptr;
  m_pSrcStreamDef = nullptr;

  m_i_AltRow    = C_UNKNOWN;
  m_nAltRows    = C_UNKNOWN;
  m_aAltRsb     = nullptr;
  m_pAltRowSlab = nullptr;
  m_aAltEnt     = nullptr;
  m_pAltIdxTree = nullptr;
  m_pAltEntBuffer = nullptr;
  m_aAltCrw     = nullptr;
  m_pAltStreamDef = nullptr;

  m_pEnt        = nullptr;
  // m_oEntWrk
  m_pEntBuffer  = nullptr;
  m_pIndexTree  = nullptr;
  m_aPayload    = nullptr;
  
  m_i_Code = 0;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_eaCode,sizeof(m_eaCode),CN_LOGICUNIT);
  MemLib::vZero(m_iaDataStack,sizeof(m_iaDataStack),CN_LOGICUNIT);
  MemLib::vZero(m_aaReturnStack,sizeof(m_aaReturnStack),CN_LOGICUNIT);
#endif
  
  m_nLabels = 0;
  for (i8 i_Label = 0; i_Label < C_LOGIC_LABEL; i_Label++) {
    m_xaJumpLabel[i_Label].m_bDefined = false;
    m_xaJumpLabel[i_Label].m_i_Chain = C_UNKNOWN;
  }

  _checkInitialised_(CN_LOGICUNIT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LogicUnit::~LogicUnit(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

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
void LogicUnit::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("LogicUnit\n"_Stz);
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
/* transform compute                                                          */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iTransform(i8 nArgs, i8 iArg0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iResult = iCompute(nArgs,iArg0);

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* -- private ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add code                                                                   */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCode(eEO eCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(0 == EoArgs[eCode].m_nArgBytes);
  
  m_eaCode[m_i_Code++] = eCode;
}

/*----------------------------------------------------------------------------*/
/* add code with argument                                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeArg(eEO eCode, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(8 == EoArgs[eCode].m_nArgBytes);
  
  m_eaCode[m_i_Code++] = eCode;

  i8* piArg = (i8*) & m_eaCode[m_i_Code];
  * piArg = iArg;
  
  m_i_Code += sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* add code with argument                                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeArg(eEO eCode,Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iArg = pTkn->iToI8();
  
  vAddCodeArg(eCode,iArg);
}

/*----------------------------------------------------------------------------*/
/* add code with date argument                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeArg(eEO eCode, Stz* szLiteral)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(EA_DATE == EoArgs[eCode].m_eArgument);
  
  char* acCrs = szLiteral->azChar();
  
  SN_Dte snDate;
  i8 iDate = snDate.i(&acCrs,AM::pFmt());
  
  vAddCodeArg(eCode,iDate);
}

/*----------------------------------------------------------------------------*/
/* add code with named label                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeLabel(eEO eCode, Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label) {
    i_Label = iAddLabel(szLabelNm,false);
    m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code + 1;
    vAddCodeArg(eCode,C_UNKNOWN);
    //vPrintLabels();
    return;
  }

  if (m_xaJumpLabel[i_Label].m_bDefined) {
    vAddCodeArg(eCode,m_xaJumpLabel[i_Label].m_i_Code);
    return;
  }
  
  i8 i_Chain = m_xaJumpLabel[i_Label].m_i_Chain;
  m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code + 1;
  vAddCodeArg(eCode,i_Chain);
  //vPrintLabels();
}

/*----------------------------------------------------------------------------*/
/* add code with label name in given token                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeLabel(eEO eCode, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  vAddCodeLabel(eCode,pTkn->szChars());
}

/*----------------------------------------------------------------------------*/
/* add code with string literal                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeString(eEO eCode, Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(-1 == EoArgs[eCode].m_nArgBytes);
  
  m_eaCode[m_i_Code++] = eCode;

  for (char* acChr = szString->azChar(); *acChr; acChr++)
    m_eaCode[m_i_Code++] = (eEO)*acChr;
  m_eaCode[m_i_Code++] = (eEO)0;
}

/*----------------------------------------------------------------------------*/
/* add label                                                                  */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iAddLabel(Stz* szLabelNm, bool bKnown)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
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
void LogicUnit::vCorrectLabel(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(! m_xaJumpLabel[i_Label].m_bDefined);
  
  i8 i_Chain = m_xaJumpLabel[i_Label].m_i_Chain;
  while (C_UNKNOWN != i_Chain) {
    
    i8* piArg = (i8*) & m_eaCode[i_Chain];
 
    i_Chain = * piArg;

    * piArg = m_i_Code;
  }

  m_xaJumpLabel[i_Label].m_bDefined = true;
  m_xaJumpLabel[i_Label].m_i_Code   = m_i_Code;
}

/*----------------------------------------------------------------------------*/
/* decode label into subscript                                                */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDecodeLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 i_Label = iFindLabel(szLabelNm);

  if (C_UNKNOWN == i_Label)
    i_Label = iAddLabel(szLabelNm,false);
  
  i8 i_Code = m_xaJumpLabel[i_Label].m_i_Chain;
 
  m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code;
  
  //vPrintLabels();
  
  return i_Code;
}

/*----------------------------------------------------------------------------*/
/* define label                                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDefineLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // has this label been used yet?
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label)
    i_Label = iAddLabel(szLabelNm,true);
  else
    vCorrectLabel(i_Label);
  
  //vPrintLabels();
}

/*----------------------------------------------------------------------------*/
/* find code                                                                  */
/*----------------------------------------------------------------------------*/
Stz* LogicUnit::szFindCode(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (i_Code == m_xaJumpLabel[i_Label].m_i_Code)
      return m_xaJumpLabel[i_Label].m_sLabel;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find label                                                                 */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iFindLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (m_xaJumpLabel[i_Label].m_sLabel.bSame(szLabelNm))
      return i_Label;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* print labels                                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vPrintLabels(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
#ifdef WITH_TRACE_LOGICUNIT
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    vTrace("  %s %lld: %4lld\n"_Stz,
           m_xaJumpLabel[i_Label].m_sLabel.szStz(),
           m_xaJumpLabel[i_Label].m_bDefined,
           m_xaJumpLabel[i_Label].m_i_Chain);
#endif
  
}



// calc L AGAIN 1 ;

// calc 10 L AGAIN dup . dec dup jt aAGAIN ;

// calc 1 L AGAIN dup . inc dup 10 > jf AGAIN;

// calc 1 L AGAIN call PRINT inc dup 10 > jf AGAIN ret L PRINT dup . ret;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump code                                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDumpCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
 
//#ifdef WITH_TRACE_LOGICUNIT
  
  //if (100000 != m_iSql)
  if (302 != m_iSql)
    return;
  
  SP_Dte spDate;

  vTrace("EO code for %lld:\n"_Stz,m_iSql);
  
  for (i8 i_Code = 0; i_Code < m_i_Code; ) {
        
    eEO eCode = m_eaCode[i_Code];
        
    Stz* szLabel = szFindCode(i_Code);
    vTrace("%12s "_Stz,szLabel ? szLabel : ""_Stz);
      
    if (eCode <= EO_NONE || eCode >= EO_ENUMS) {
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eo code sequence detected"_Stz);
    }
    
    if (-1 == EoArgs[eCode].m_nArgBytes) {
      char* azArg = (char*) & m_eaCode[i_Code+1];
      vTrace("%04lld: %20s (%lls)\n"_Stz,i_Code,aEO[eCode],azArg);
      i_Code += 1 + StrLib::nSize(azArg);
    }
    else
    if (EA_DATE == EoArgs[eCode].m_eArgument) {
      i8* piArg = (i8*) & m_eaCode[i_Code+1];
      i8 iArg = * piArg;
      vTrace("%04lld: %20s (%lls)\n"_Stz,i_Code,aEO[eCode],spDate.sz(iArg));
      i_Code += 1 + sizeof(i8);
    }
    else
    if (EA_LABEL == EoArgs[eCode].m_eArgument) {
      i8* piArg = (i8*) & m_eaCode[i_Code+1];
      i8 iArg = * piArg;
      Stz* szLabel = szFindCode(iArg);
      vTrace("%04lld: %20s => %s @%lld\n"_Stz,
             i_Code,
             aEO[eCode],
             szLabel ? szLabel : "<unknown>"_Stz,
             iArg
             );
      i_Code += 1 + sizeof(i8);
    }
    else
    if (EA_SUBSCR == EoArgs[eCode].m_eArgument) {
      i8* piArg = (i8*) & m_eaCode[i_Code+1];
      i8 iArg = * piArg;
      vTrace("%04lld: %20s [%lld]\n"_Stz,i_Code,aEO[eCode],iArg);
      i_Code += 1 + sizeof(i8);
    }
    else
    if (EoArgs[eCode].m_nArgBytes) {
      i8* piArg = (i8*) & m_eaCode[i_Code+1];
      i8 iArg = * piArg;
      vTrace("%04lld: %20s (%lld)\n"_Stz,i_Code,aEO[eCode],iArg);
      i_Code += 1 + sizeof(i8);
    }
    else {
      vTrace("%04lld: %20s\n"_Stz,i_Code,aEO[eCode]);
      i_Code++;
    }
    
    if (EO_RETURN == eCode)
      vTrace("\n"_Stz);
  }
  
  vDebugPoint();
//#endif
  
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


#if 1

  #define dLU3(eCode,iA,iB,iC) \
    if (bTrace) \
      vTrace("%lld : %12s %lld %lld %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA,iB,iC);

  #define dLU2(eCode,iA,iB) \
    if (bTrace) \
      vTrace("%lld : %12s %lld %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA,iB);

  #define dLU1(eCode,iA) \
    if (bTrace) \
      vTrace("%lld : %12s %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA);

  #define dLU0(eCode) \
    if (bTrace) \
      vTrace("%lld : %12s\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode]);

  #define GetArg(iArg) \
    ASSERT(8 == EoArgs[eCode].m_nArgBytes); \
    iArg = * (i8*)aeCode; \
    aeCode += 8;

  #define GetStr(iArg) \
    ASSERT(-1 == EoArgs[eCode].m_nArgBytes); \
    iArg = (i8)aeCode; \
    while (*(char*)aeCode) aeCode++; \
    aeCode++;

  #define NoArg() \
    ASSERT(0 == EoArgs[eCode].m_nArgBytes);
#else

  #define dLU3(eCode,iA,iB,iC)
  #define dLU2(eCode,iA,iB)
  #define dLU1(eCode,iA)
  #define dLU0(eCode)

  #define GetArg(iArg) \
    iArg = * (i8*)aeCode; \
    aeCode += 8;

  #define GetStr(iArg) \
    iArg = (i8)aeCode; \
    while (*(char*)aeCode) aeCode++; \
    aeCode++;

  #define NoArg()

#endif



/*============================================================================*/
/* - - - - - - - - - - - - - RELATIONAL LOGIC UNIT - - - - - - - - - - - - - -*/
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iCompute(i8 nOuterArgs, i8 iOuterArg0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iArg;
  i8 iTopA;
  i8 iTopB;
  i8 iRes;
  i8 iCmp;
  i8 i_Cell_ntv;
  eKC eCmp;
  
  Key* aSrcKey;
  Key* aAltKey;
  Key* aWrkKey;

  SP_Ent spEnt;
  Ent* aEnt;

  bool bTrace = false;

  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;

  eEO  eCode;
  eEO* aeCode = & m_eaCode[0];
  i8*  aiDstk = &m_iaDataStack[0];

  eEO** aaeRstk = &m_aaReturnStack[0];

  IndexDef* pIndexDef = nullptr;
  
  i8 iCtr0 = 0;

  i8 nPtns = AM::nPartitions();
  i8 iHash = PtnLib::iResetRunning();
  i8 i_Ptn = 0;
  i8 i_Slot = AM::pTaskJob()->m_pOp->nPartition();

  Timer oTimer;
  AggTimer oAggTimer;
  
#ifdef WITH_TRACE_LOGICUNIT
  vTrace("\nLogic Unit :: sql %lld\n\n"_Stz,m_iSql);
  vDebugPoint();
#endif

  if (nOuterArgs) {
    *(++aiDstk) = iOuterArg0;
  }
  
  for (;;) {
    
    // check for stack growth
    if (aiDstk >= &m_iaDataStack[C_LOGIC_STACK-4])
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "LogicUnit data stack exhausted\n"_Stz);
    
    i8 nStkUse = nDifference(aiDstk,m_iaDataStack);
    i8 iStkPct = (nStkUse * 100) / C_LOGIC_STACK;
    if (iStkPct > 80)
      vTrace("WARNING: LogicUnit stack use %lld%%\n"_Stz,iStkPct);
    
  
    eCode = *aeCode++;
    
    //vTrace("%s  (StkUse %lld)\n"_Stz,aEO[eCode],nStkUse);
    
    switch (eCode) {
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // arithmetic operators
      case EO_ADD: NoArg(); // add top two int's push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA + iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_DIVIDE: NoArg(); // divide top int by second push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA / iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_MODULO: NoArg(); // modulo top int be second push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA % iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_MULTIPLY: NoArg(); // multiply top two int's push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA * iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_NEGATE: NoArg(); // negate the top value
        iTopA = *(aiDstk--);
        iRes  = - iTopA;
        dLU2(eCode,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_SUBTRACT: NoArg(); // subtract second int from top push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA - iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // col operators
      case EO_COL_SRC_PUSH: GetArg(iArg); // push column value from m_aSrcCrw
      {
        HV_Row hRow(m_pSrcStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_aSrcCrw,iArg);
        iRes = hRow->m_waColVal[iArg].m_iWrd;
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
      }
        break;
        
      case EO_COL_SRC_PUSHSTR: GetArg(iArg); // push column value from m_aSrcCrw
      {
        HV_Row hRow(m_pSrcStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_aSrcCrw,iArg);
        iRes = hRow->m_waColVal[iArg].m_iWrd;
        dLU2(eCode,iArg,iRes);
        i8 nSize = StrLib::nSize((char*)iRes);
        char* azDst = (char*)(++aiDstk);
        StrLib::vCopy(azDst,(char*)iRes);
        aiDstk += nSize / 8;
        *(++aiDstk) = nSize;
      }
        break;
        
      case EO_COL_ALT_PUSH: GetArg(iArg); // push column value from m_aAltCrw
      {
        HV_Row hRow(m_pAltStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_aAltCrw,iArg);
        iRes = hRow->m_waColVal[iArg].m_iWrd;
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
      }
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // crw operators
      case EO_CRW_SRC_FETCH: NoArg(); // fetch m_aSrcCrw using m_aSrcEnt.ref
        m_aSrcCrw = CrwBuffer::pRefdCrw(m_aSrcEnt->rxRef());
        break;
        
      case EO_CRW_ALT_FETCH: NoArg(); // fetch m_aAltCrw using m_aAltEnt.ref
        m_aAltCrw = CrwBuffer::pRefdCrw(m_aAltEnt->rxRef());
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // counter
      case EO_CTR_0_DEC: NoArg(); // dec counter 0
        iCtr0--;
        dLU1(eCode,iCtr0);
        break;

      case EO_CTR_0_INC: NoArg(); // inc counter 0
        iCtr0++;
        dLU1(eCode,iCtr0);
        break;

      case EO_CTR_0_POP: NoArg(); // pop counter 0
        iCtr0 = *(aiDstk--);
        dLU1(eCode,iCtr0);
        break;

      case EO_CTR_0_PUSH: NoArg(); // push counter 0
        dLU1(eCode,iCtr0);
        *(++aiDstk) = iCtr0;
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // date operators
      case EO_DTE_NOW: NoArg(); // push current date
      {
        Now oNow;
        Dtm wDte;

        wDte.m_wDte.m_uDay = oNow.m_wTsp.m_uDay;
        wDte.m_wDte.m_uMonth = oNow.m_wTsp.m_uMonth;
        wDte.m_wDte.m_uYearDelta = oNow.m_wTsp.m_uYearDelta;
        wDte.m_wDte.m_uSpare = 0;

        iRes = wDte.iAsI8();
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
      }
        break;
        
      case EO_DTE_PRINT: NoArg(); // pop top and print as date
#ifdef WITH_TRACE_LOGICUNIT
      {
        SP_Dte spDate;
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        vTrace("[Q%lld.%lld] %s\n"_Stz,m_iSql,i_Slot,spDate.sz(iTopA));
      }
#endif
        break;

      case EO_DTE_YEAR: NoArg(); // pop top, push year of date
      {
        Dtm wDte;
        wDte = *(aiDstk--);
        iRes = wDte.m_wDte.m_uYearDelta + C_YEAR_BASE;
        dLU2(eCode,wDte.iAsI8(),iRes);
        *(++aiDstk) = iRes;
      }
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry
      case EO_ENT_NEXT: NoArg(); // move to next m_pEnt
        dLU0(eCode);
        m_pEnt = EntBuffer::pMoveToNextEntry(m_pEnt, &m_pEntBuffer);
        if (m_pEnt)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;
        
      case EO_ENT_POSPRIOR: NoArg(); // position m_pEnt prior using m_oEntWrk.key
        dLU0(eCode);
        m_pEnt = m_pIndexTree->aPositionPriorEntry(m_oEntWrk.aKey(), &m_pEntBuffer);
        break;
        
      case EO_ENT_PUSH: NoArg(); // push entry on the stack
        dLU0(eCode);
        *(++aiDstk) = (i8)m_pEnt;
        break;
        
      case EO_ENT_PUSHELMI0: NoArg(); //  push entry elm 0 int
        dLU0(eCode);
        iRes = m_pEnt->aKey()->iGetIntElm();
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_PUSHELMS0: NoArg(); //  push entry elm 0 str
        NEVER_REACH;
        break;
        
      case EO_ENT_PUSHOFS: NoArg(); // push entry offset
        dLU0(eCode);
        iRes = m_pEnt->iBuffer();
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_PUSHREF: NEVER_REACH; // not yet implemented
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry relational operators
      case EO_ENT_LESS: NoArg(); // test if m_pEnt.key <  m_oEntWrk.key
        iRes  = KC_BEFORE == Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_LESSEQUAL: NoArg(); // test if m_pEnt.key <= m_oEntWrk.key
        iCmp = Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey());
        iRes  = (KC_SAME == iCmp || KC_BEFORE == iCmp ) ? 1 : 0;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_EQUAL: NoArg();
      {
#ifdef WITH_TRACE_LOGICUNIT
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,
               spEnt.sz(m_oEntWrk.aEnt(),m_pIndexTree->pIndexDef()));
        vTrace("\n\nEnt : %s\n"_Stz,spEnt.sz(m_pEnt,m_pIndexTree->pIndexDef()));
#endif
        iRes  = KC_SAME == Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
      }
        
      case EO_ENT_NOTEQUAL: NoArg();
        iRes  = KC_SAME != Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_GREATEREQUAL: NoArg();
        iCmp = Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey());
        iRes = (KC_SAME == iCmp || KC_AFTER == iCmp ) ? 1 : 0;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ENT_GREATER: NoArg();
        iRes  = KC_AFTER == Key::eCompare(m_pEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entWrk
      case EO_ENTWRK_APPENDELM: GetArg(iArg); // append next element value
      {
        dLU1(eCode,iArg);
        m_oEntWrk.vAppendElement(m_aSrcRsb, &m_pSrcRowSlab->m_oStreamPix, iArg);
        
#ifdef WITH_TRACE_LOGICUNIT
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pIndexTree->pIndexDef()));
#endif
        break;
      }
        
      case EO_ENTWRK_APPENDELMI: NoArg(); // append next element value (int)
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        m_oEntWrk.vAppendElementInt(m_aSrcRsb,iTopA);
        break;
        
      case EO_ENTWRK_APPENDELMS: NoArg(); // append next element value (str)
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        m_oEntWrk.vAppendElementStz(m_aSrcRsb,(Stz*)(aiDstk-iTopA/8));
        aiDstk -= (iTopA/8) + 1;
        break;
        
      case EO_ENTWRK_CREATEPAY: GetArg(iArg); // create a payload in entwrk
        dLU1(eCode,iArg);
        
        m_oEntWrk.vPayload(true);
        m_oEntWrk.vPayloadBytes(iArg);
        break;
                
      case EO_ENTWRK_POPOFS: NoArg(); // pop m_oEntWrk.ref.buffer/offset
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        m_oEntWrk.vBuffer(iTopA);
        break;

      case EO_ENTWRK_POPREF: NEVER_REACH; // not yet implemented

      case EO_ENTWRK_PRINT: NoArg(); // print m_oEntWrk
#ifdef WITH_TRACE_LOGICUNIT
        aEnt = m_oEntWrk.aEnt();
        pIndexDef = m_xpIndexTree->pIndexDef();
        vTrace("EntWrk : %s\n"_Stz,spEnt.sz(aEnt,pIndexDef));
#endif
        break;
        
      case EO_ENTWRK_RESET: NoArg(); // reset m_oEntWrk
        dLU0(eCode);
        m_oEntWrk.vReset();
        break;
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // finalise
      case EO_FIN_TRANSFORM: NoArg(); // standard finalise
        dLU0(eCode);
        //m_pPot        = nullptr;
        m_pDstRowSlab = nullptr;

        m_nSrcRows    = C_UNKNOWN;
        m_aSrcRsb     = nullptr;
        m_pSrcRowSlab = nullptr;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // hashing
      case EO_HSH_ADD_HASHI: NoArg(); // add hash of top int to running hash
        iTopA = *(aiDstk--);
        iRes = HshLib::iHash((byte*)&iTopA,sizeof(w8));
        iHash += iRes;
        dLU3(eCode,iTopA,iRes,iHash);
        break;
        
      case EO_HSH_RESET: NoArg(); // reset hash
        dLU0(eCode);
        iHash = 0;
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // initialise
      case EO_INI_PIX_DEFOUT: NoArg(); // init m_pPot pix from stream def out
        dLU0(eCode);
        if (! m_bPixInited) {
          m_bPixInited = true;
          m_xpStreamDefOut->vToStreamPix(&m_oStreamPix,true);
          m_xpPot->vStreamPix(&m_oStreamPix);
          
          // temp hack by alka
          if ( ! m_xpIndexTree.bIsNull() )
            m_pIndexTree = * m_xpIndexTree;
          // end temp hack
        }
        break;
        
      case EO_INI_PIX_SLABIN: NoArg(); // init m_pPot pix from inp slab
      {
        dLU0(eCode);
        StreamPix* aStreamPix = m_pSrcRowSlab->aStreamPix();
        m_xpPot->vStreamPix(aStreamPix);
      
        // temp hack by ray
        if ( ! m_xpIndexTree.bIsNull() )  // alka added :-)
          m_pIndexTree = * m_xpIndexTree;
        // end temp hack
      }
        break;
        
      case EO_INI_SRC_SLAB: NoArg(); // init m_pSrcRowSlab etc
        dLU0(eCode);
        //m_pPot = * m_xpPot;
        //m_pDstRowSlab = nullptr;

        m_aSrcRsb     = nullptr;
        m_pSrcRowSlab = m_xpOperandA.bIsNull() ?
                        nullptr : _pRsbBuffer(* m_xpOperandA)->pRowSlab();
        m_nSrcRows    = m_pSrcRowSlab ? m_pSrcRowSlab->nRows() : 0;
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // index tree operators
      case EO_IXT_DEL_ENT: NoArg(); // delete current entry from indexTree
        m_pIndexTree->vDelete(m_pEntBuffer,m_pEnt);
        break;

      case EO_IXT_DUMP: NoArg(); // dump index tree
#ifdef WITH_TRACE_LOGICUNIT
        m_pIndexTree->vDump(false);
#endif
        break;

      case EO_IXT_INS_ENT: NoArg(); // insert an entry into the indexTree
      {
        dLU0(eCode);
#ifdef WITH_TRACE_LOGICUNIT
        SP_Ent spEnt;
        vTrace("Inserting entry"_Stz);
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pIndexTree->pIndexDef()));
#endif
        m_pEnt = m_pIndexTree->aInsertEntry(m_oEntWrk.aEnt());
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("\n\nEnt : %s\n"_Stz,spEnt.sz(m_pEnt,m_pIndexTree->pIndexDef()));
#endif
        ASSERT(m_pEnt);
      }
        break;
        
      case EO_IXT_TRV_BEGIN: NoArg(); // begin linear traverse
        dLU0(eCode);
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
        
      case EO_IXT_TRV_NEXT: NoArg(); // move to next entry
        dLU0(eCode);
        m_pEnt = EntBuffer::pMoveToNextEntry(m_pEnt, &m_pEntBuffer);
        if (m_pEnt)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;
        
      case EO_IXT_TMP_ENTRIES: NoArg(); // push number of entries in tmp index
        iRes = m_pIndexTree->iCountH0Entries();
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_IXT_SRC_BEGIN: NoArg(); // begin linear traverse
        dLU0(eCode);
        m_pSrcEntBuffer = m_pSrcIdxTree->pFirstH0Buffer();
        if (m_pSrcEntBuffer) {
          m_aSrcEnt = m_pSrcEntBuffer->aLeadEntry();
          if (m_aSrcEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;

      case EO_IXT_SRC_NEXT: NoArg(); // move to next entry
        dLU0(eCode);
        m_aSrcEnt = EntBuffer::pMoveToNextEntry(m_aSrcEnt, &m_pSrcEntBuffer);
        if (m_aSrcEnt)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;

      case EO_IXT_SRC_PRINT: NoArg(); // print src ent
        pIndexDef = m_pSrcIdxTree->pIndexDef();
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("SrcEnt : %s\n"_Stz,spEnt.sz(m_aSrcEnt,pIndexDef));
#endif
        break;

      case EO_IXT_ALT_BEGIN: NoArg(); // begin linear traverse
        dLU0(eCode);
        m_pAltEntBuffer = m_pAltIdxTree->pFirstH0Buffer();
        if (m_pAltEntBuffer) {
          m_aAltEnt = m_pAltEntBuffer->aLeadEntry();
          if (m_aAltEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;

      case EO_IXT_ALT_NEXT: NoArg();
        dLU0(eCode);
        m_aAltEnt = EntBuffer::pMoveToNextEntry(m_aAltEnt, &m_pAltEntBuffer);
        if (m_aAltEnt)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;

      case EO_IXT_ALT_PRINT: NoArg(); // print alt ent
        pIndexDef = m_pAltIdxTree->pIndexDef();
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("AltEnt : %s\n"_Stz,spEnt.sz(m_aAltEnt,pIndexDef));
#endif
        break;
        
      case EO_IXT_JOI_MATCH: NoArg(); // test for join match (src.key == alt.key)
        aSrcKey = m_aSrcEnt->aKey();
        aAltKey = m_aAltEnt->aKey();
        iRes = KC_SAME == Key::eCompare(aSrcKey,aAltKey) ? 1 : 0;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        NEVER_REACH; // not used??
        break;

      case EO_IXT_CMP_SRCALT: NoArg(); // compare src.key to alt.key
        aSrcKey = m_aSrcEnt->aKey();
        aAltKey = m_aAltEnt->aKey();
        eCmp = Key::eCompare(aSrcKey,aAltKey);
        if (KC_BEFORE == eCmp)
          iRes = -1;
        else
        if (KC_SAME == eCmp)
          iRes = 0;
        else
          iRes = 1;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;

      case EO_IXT_CMP_WRKALT: NoArg(); // compare wrk.key to alt.key
        aWrkKey = m_oEntWrk.aKey();
        aAltKey = m_aAltEnt->aKey();
        eCmp = Key::eCompare(aWrkKey,aAltKey);
        if (KC_BEFORE == eCmp)
          iRes = -1;
        else
        if (KC_SAME == eCmp)
          iRes = 0;
        else
          iRes = 1;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;

      case EO_IXT_POS_ALTBYSRC: NoArg(); // position alt ixt using src.key
        aSrcKey = m_aSrcEnt->aKey();
        m_aAltEnt = m_pAltIdxTree->aPositionPriorEntry(aSrcKey,&m_pAltEntBuffer);
        if (m_pAltEntBuffer) {
          if (m_aAltEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;

      case EO_IXT_POS_ALTBYWRK: NoArg(); // position alt ixt using entwrk.key
        aWrkKey = m_oEntWrk.aKey();
        m_aAltEnt = m_pAltIdxTree->aPositionPriorEntry(aWrkKey,&m_pAltEntBuffer);
        if (m_pAltEntBuffer) {
          if (m_aAltEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // jump operators
      case EO_JMP: GetArg(iArg); // jump to location
        dLU1(eCode,iArg);
        aeCode = & m_eaCode[iArg];
        break;
        
      case EO_JMP_FALSE: GetArg(iArg); // pop top and jump to location if false
        dLU1(eCode,iArg);
        iTopA = *(aiDstk--);
        if (! iTopA)
          aeCode = & m_eaCode[iArg];
        break;
        
      case EO_JMP_TRUE: GetArg(iArg); // pop top and jump to location if true
        dLU1(eCode,iArg);
        iTopA = *(aiDstk--);
        if (iTopA)
          aeCode = & m_eaCode[iArg];
        break;
        
      case EO_JMP_LESS: GetArg(iArg); // pop top and jump to location if <
        dLU1(eCode,iArg);
        iTopA = *(aiDstk--);
        if (iTopA < 0)
          aeCode = & m_eaCode[iArg];
        break;

      case EO_JMP_LESSEQUAL: GetArg(iArg); // pop top and jump to location if <=
        dLU1(eCode,iArg);
        iTopA = *(aiDstk--);
        if (iTopA <=0)
          aeCode = & m_eaCode[iArg];
        break;

      case EO_JMP_EQUAL: GetArg(iArg); // pop top and jump to location if ==
        dLU1(eCode,iArg);
        iTopA = *(aiDstk--);
        if (iTopA == 0)
          aeCode = & m_eaCode[iArg];
        break;

      case EO_JMP_NOTEQUAL: GetArg(iArg); // pop top and jump to location if !=
        dLU1(eCode,iArg);
        iTopA = *(aiDstk--);
        if (iTopA != 0)
          aeCode = & m_eaCode[iArg];
        break;

      case EO_JMP_GREATEREQUAL: GetArg(iArg); // pop top and jump to location if >=
        dLU1(eCode,iArg);
        iTopA = *(aiDstk--);
        if (iTopA >= 0)
          aeCode = & m_eaCode[iArg];
        break;

      case EO_JMP_GREATER: GetArg(iArg); // pop top and jump to location if >
        dLU1(eCode,iArg);
        iTopA = *(aiDstk--);
        if (iTopA > 0)
          aeCode = & m_eaCode[iArg];
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // logical operators
      case EO_LGL_AND: NoArg(); // logical and top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA && iTopB) ? 1 : 0;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_LGL_OR: NoArg(); // logical or top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA || iTopB) ? 1 : 0;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_LGL_NOT: NoArg(); // logical not top int, push result
        iTopA = *(aiDstk--);
        iRes  = (! iTopA) ? 1 : 0;
        dLU2(eCode,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // null test operators
      case EO_ISNULL: NoArg(); // test if top is null, push result
        iTopA = *(aiDstk--);
        iRes  = (C_DATUM_NULL == iTopA) ? 1 : 0;
        dLU2(eCode,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_ISNOTNULL: NoArg(); // test if top is not null, push result
        iTopA = *(aiDstk--);
        iRes  = (C_DATUM_NULL != iTopA) ? 1 : 0;
        dLU2(eCode,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pay load
      case EO_PAY_FETCH_COLI: GetArg(iArg); // fetch a column from the entry payload
        m_aPayload = m_pEnt->aiPayCell(iArg);
        iRes = *m_aPayload;
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_PAY_FETCH_COLS: GetArg(iArg); // fetch a column from the entry payload
      {
        m_aPayload = m_pEnt->aiPayCell(iArg);
        char* azSrc = (char*)m_aPayload;
        i8 nSize = StrLib::nSize(azSrc);
        ASSERT(nSize < 8);
        dLU2(eCode,iArg,(i8)azSrc);
        char* azDst = (char*)(++aiDstk);
        StrLib::vCopy(azDst,azSrc);
        aiDstk++;
        *(++aiDstk) = nSize;
      }
        break;
        
      case EO_PAY_STORE_COLI: GetArg(iArg); // store a column of the entry payload
        iRes = *(aiDstk--);
        dLU2(eCode,iArg,iRes);
        m_aPayload = m_pEnt->aiPayCell(iArg);
        *m_aPayload = iRes;
        break;
      
      case EO_PAY_STORE_COLS: GetArg(iArg); // store a column of the entry payload
      {
        i8 nSize = *(aiDstk--);
        RASSERT(nSize < 8);
        dLU2(eCode,iArg,nSize);
        m_aPayload = m_pEnt->aiPayCell(iArg);
        char* azSrc = (char*)(--aiDstk);
        aiDstk--;
        StrLib::vCopy((char*)m_aPayload,azSrc);
      }
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pix
      case EO_PIX_STM_PRINT: NoArg(); // print stream pix
        dLU0(eCode);
        m_oStreamPix.vPrint();
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // partitioning
      case EO_PTN_CALC: NoArg(); // calculate i_Ptn using running iHash
        dLU0(eCode);
        i_Ptn = PtnLib::iPartition(iHash,nPtns);
        break;

      case EO_PTN_PUSH: NoArg(); // push i_Ptn
        dLU1(eCode,i_Ptn);
        *(++aiDstk) = i_Ptn;
        break;

      case EO_PTN_RESET: NoArg(); // reset running iHash
        dLU0(eCode);
        iHash = PtnLib::iResetRunning();
        break;

      case EO_PTN_RUNNING: NoArg(); // mix top into running iHash
        iTopA = *(aiDstk--);
        dLU0(eCode);
        PtnLib::vRunning(&iHash,iTopA);
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pushes
      case EO_DATE: GetArg(iArg); // push date
        dLU1(eCode,iArg);
        *(++aiDstk) = iArg;
        break;
        
      case EO_INTEGER: GetArg(iArg); // push integer
        dLU1(eCode,iArg);
        *(++aiDstk) = iArg;
        break;
        
      case EO_PUSHNULL: NoArg(); // push null
        dLU0(eCode);
        *(++aiDstk) = C_DATUM_NULL;
        break;
        
      case EO_PUSH0: NoArg(); // push integer 0
        dLU0(eCode);
        *(++aiDstk) = 0;
        break;
        
      case EO_PUSH1: NoArg(); // push integer 1
        dLU0(eCode);
        *(++aiDstk) = 1;
        break;
        
      case EO_RANDOM: GetArg(iArg); // push random number 0..arg-1
        iRes = RndLib::iInteger(iArg);
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ref
      case EO_REF_GETCRW: NEVER_REACH; // not yet implemented
      case EO_REF_SRCRSB: NEVER_REACH; // not yet implemented
      case EO_REF_ALTRSB: NEVER_REACH; // not yet implemented

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb / rowslab
      case EO_RSB_DST_COPYSRC: NoArg(); // copy src rsb to dst rsb
        dLU0(eCode);
        m_pDstRowSlab->vPositionOnCell(0);
        m_aDstRsb = m_pDstRowSlab->aRsb();
        m_pDstRowSlab->vCopyRsb(m_aSrcRsb,bSrcHasPtnHash,bCopyPtnHash);
        break;

      case EO_RSB_DST_PRINT: NoArg(); // print dst rsb
        dLU0(eCode);
        m_pDstRowSlab->vDumpRow(-1,m_aDstRsb);
        break;

      case EO_RSB_SRC_PRINT: NoArg(); // print src rsb
        dLU0(eCode);
        m_pSrcRowSlab->vDumpRow(-1,m_aSrcRsb);
        break;

      case EO_RSB_ALT_PRINT: NoArg(); // print alt rsb
        dLU0(eCode);
        m_pAltRowSlab->vDumpRow(-1,m_aAltRsb);
        break;

      case EO_RSB_DST_NEW: NoArg(); // create new destination rsb
        dLU0(eCode);
        //ASSERT(0 == i_Ptn);
        m_aDstRsb = m_xpPot->aNewDstRsb(i_Ptn,&m_pDstRowSlab);
        break;

      case EO_RSB_DST_RESET: NoArg(); // reset dst rowslab
        m_pDstRowSlab->vReset();
        break;
        
      case EO_RSB_DST_TEST: NoArg(); // test if there is a destination rsb
        dLU0(eCode);
        if (m_aDstRsb)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;
        
      case EO_RSB_DST_ZERO: NoArg(); // zero destination rsb
        dLU0(eCode);
        m_pDstRowSlab->vZeroRsb(m_aDstRsb);
        break;
        
      case EO_RSB_SRC_BEGIN: NoArg(); // begin src rsb traverse
        dLU0(eCode);
        m_i_SrcRow = -1;
        m_aSrcRsb  = nullptr;
        m_nSrcRows = m_pSrcRowSlab ? m_pSrcRowSlab->nRows() : 0;
        break;

      case EO_RSB_SRC_NEXT: NoArg(); // move to next src rsb
        dLU0(eCode);
        if (++m_i_SrcRow < m_nSrcRows) {
          m_aSrcRsb = m_pSrcRowSlab->aRsbAdr(m_i_SrcRow);
          *(++aiDstk) = 1;
        }
        else {
          *(++aiDstk) = 0;
        }
        break;

      case EO_RSB_SRC_PUSHOFS: NoArg();
        dLU0(eCode);
        iRes = m_pSrcRowSlab->iOfsOfRsb(m_aSrcRsb);
        *(++aiDstk) = iRes;
        break;

      case EO_RSB_SRC_SETBYOFS: NoArg();  // set src rsb by using offset on stack
        iTopA = *(aiDstk--);
        m_aSrcRsb = m_pSrcRowSlab->aRsbByOfs(iTopA);
        break;

      case EO_RSB_ALT_BEGIN:    NEVER_REACH; // not yet implemented
      case EO_RSB_ALT_NEXT:     NEVER_REACH; // not yet implemented
      case EO_RSB_ALT_PUSHOFS:  NEVER_REACH; // not yet implemented
      case EO_RSB_ALT_SETBYOFS: NEVER_REACH; // not yet implemented

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // relational operators
      case EO_LESSTHAN: NoArg(); // test top < 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA < iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_LESSEQUAL: NoArg(); // test top <= 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA <= iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_EQUAL: NoArg(); // test top == 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA == iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_NOTEQUAL: NoArg(); // test top != 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA != iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_GREATEREQUAL: NoArg(); // test top >= 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA >= iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_GREATERTHAN: NoArg(); // test top > 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA > iTopB;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb operators
      case EO_RSB_DST_POPCOLI: GetArg(iArg); // pop and deposit top into dst column
        iRes = *(aiDstk--);
        dLU2(eCode,iArg,iRes);
        m_pDstRowSlab->vPositionOnCell(iArg);
        m_pDstRowSlab->vDeposit(iRes);
        break;
        
      case EO_RSB_DST_POPCOLS: GetArg(iArg); // pop and deposit top into dst column
      {
        i8 nSize = *(aiDstk--);
        dLU2(eCode,iArg,nSize);
        ASSERT(nSize < 8);
        char* azSrc = (char*)(--aiDstk);
        m_pDstRowSlab->vPositionOnCell(iArg);
        char* azDst = m_pDstRowSlab->azDepositString_begin();
        StrLib::vCopy(azDst,azSrc);
        m_pDstRowSlab->vDepositString_end(nSize);
        aiDstk--;
      }
        break;
        
      case EO_RSB_DST_PUSHCOLI: GetArg(iArg); // push dst column
        m_aDstRsb = m_pDstRowSlab->aRsbAdr(m_pDstRowSlab->nRows()-1);
        i_Cell_ntv = m_aDstRsb->i_ColToCell_ntv(iArg);
        iRes = m_aDstRsb->iVal_ntv(i_Cell_ntv);
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_RSB_SRC_PUSHCOLI: GetArg(iArg); // push src column
        iRes = m_aSrcRsb->iVal_ntv(m_aSrcRsb->i_ColToCell_ntv(iArg));
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_RSB_SRC_PUSHCOLS: GetArg(iArg); // push src column
      {
        char* azSrc = (char*)m_aSrcRsb->szVal_ntv(m_aSrcRsb->i_ColToCell_ntv(iArg));
        dLU2(eCode,iArg,(i8)azSrc);
        i8 nSize = StrLib::nSize(azSrc);
        char* azDst = (char*)(++aiDstk);
        StrLib::vCopy(azDst,azSrc);
        aiDstk++;
        *(++aiDstk) = nSize;
      }
        break;
        
      case EO_RSB_ALT_PUSHCOLI: GetArg(iArg); // push alt column
        iRes = m_aAltRsb->iVal_ntv(m_aAltRsb->i_ColToCell_ntv(iArg));
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // stack operators
      case EO_INC: NoArg(); // increment top, push result
        iTopA = *aiDstk;
        iRes = iTopA + 1;
        dLU2(eCode,iTopA,iRes);
        *aiDstk = iRes;
        break;

      case EO_DEC: NoArg(); // decrement top, push result
        iTopA = *aiDstk;
        iRes = iTopA - 1;
        dLU2(eCode,iTopA,iRes);
        *aiDstk = iRes;
        break;

      case EO_DUP: NoArg(); // duplicate top
        iTopA = *aiDstk;
        dLU1(eCode,iTopA);
        *(++aiDstk) = iTopA;
        break;

      case EO_POP: NoArg(); // pop / discard top
        iTopA = *aiDstk;
        dLU1(eCode,iTopA);
        break;

      case EO_PRINT: NoArg(); // print top
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        vTrace("[Q%lld.%lld] %lld\n"_Stz,m_iSql,i_Slot,iTopA);
        break;
    
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // string operators
      case EO_STR_COMPARE: NoArg(); // pop top stradr and compare str on stack
      {
        char* azTopA;
        char* azTopB;
        azTopA = (char*) *(aiDstk--);
        iTopB = *(aiDstk--);
        azTopB = (char*) (aiDstk-iTopB/8);
        iRes = StrLib::iCompare(azTopA,azTopB);
        aiDstk -= (iTopB/8) + 1;
        *(++aiDstk) = iRes;
      }
        break;
        
      case EO_STR_PRINTADR: NoArg(); // pop top and print iArg as addr of string
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        vTrace("[Q%lld.%lld] %lls\n"_Stz,m_iSql,i_Slot,iTopA);
#endif
        break;
        
      case EO_STR_PRINTSTK: NoArg(); // pop string from top and print
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        vTrace("[Q%lld,%lld] %lls\n"_Stz,m_iSql,i_Slot,(aiDstk-iTopA/8));
        aiDstk -= (iTopA/8) + 1;
#endif
        break;
        
      case EO_STR_LITERAL: GetStr(iArg); // push address of string literal
        dLU1(eCode,iArg);
        *(++aiDstk) = iArg;
        break;
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // subroutine operators
      case EO_CALL: GetArg(iArg); // call subroutine at location
        dLU1(eCode,iArg);

        * (aaeRstk++) = aeCode;
        aeCode = & m_eaCode[iArg];
        break;

      case EO_RETURN: NoArg(); // return from subroutine and
        dLU0(eCode);
        if (aaeRstk == &m_aaReturnStack[0]) {
          iRes = *(aiDstk--);
#ifdef WITH_TRACE_LOGICUNIT
          vTrace("\nEnd Logic Unit :: sql %lld\n\n"_Stz,m_iSql);
#endif
          return iRes;
        }
        aeCode = * (--aaeRstk);
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // counter
      case EO_TMR_START: NoArg(); // timer start
        dLU0(eCode);
        oTimer.vStart();
        break;
        
      case EO_TMR_NSECS: NoArg(); // push timer nanoseconds
        iRes = oTimer.iNanoseconds();
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_TMR_UPDATE: NoArg(); // timer update
        dLU0(eCode);
        oTimer.vUpdate();
        break;
        
      case EO_TMR_AGG_ADD: NoArg(); // add timer to agg timer
        dLU0(eCode);
        oAggTimer.vAdd(&oTimer);
        break;
        
      case EO_TMR_AGG_NSECS: NoArg(); // push agg nanoseconds
        iRes = oAggTimer.iNanoseconds();
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_TMR_AGG_RESET: NoArg(); // reset agg timer
        dLU0(eCode);
        oTimer.vReset();
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // misc
      case EO_ASSERT: NoArg(); // pop top and assert
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        RASSERT(iTopA);
        break;
              
      case EO_BREAKPOINT: NoArg(); // breakpoint
        dLU0(eCode);
        vDebugPoint();
        break;
        
      case EO_SLEEP: NoArg(); // sleep for top seconds
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        sleep((u4)iTopA);
        break;
              
      case EO_SLOT: NoArg(); // sleep for top seconds
        dLU1(eCode,i_Slot);
        *(++aiDstk) = i_Slot;
        break;
              
      case EO_TRACEOFF: NoArg(); // set trace
        bTrace = 0;
        break;
        
      case EO_TRACEON: NoArg(); // set trace
        bTrace = 1;
        break;
        
      case EO_NOOP: NoArg(); // no op
        dLU0(eCode);
        break;

      case EO_ERROR: NoArg(); // error marker
        dLU0(eCode);
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "Code compiled with error"_Stz);
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      default:
        vTrace("Unknown eo opcode %lld\n"_Stz,(i8)eCode);
        return C_DATUM_NULL;
    }
    
  }
  
  return m_iaDataStack[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
