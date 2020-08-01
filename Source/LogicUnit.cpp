/*============================================================================*/
/* LogicUnit.hpp -- Logic Unit                                                */
/*============================================================================*/

#include "Expressions.h"

#include "DEV_enum.h"
#include "RLU_itf.h"

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
  m_pEntBufferOuter = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  MemLib::vZero(&m_xRLU,sizeof(m_xRLU),CN_LOGICUNIT);
  MemLib::vZero(&m_aaDev,sizeof(m_aaDev),CN_LOGICUNIT);
  
  m_aeCode  = nullptr;
  m_aaeRstk = nullptr;
  m_aiDstk  = nullptr;
  
  m_iCtr0   = 0;

  m_nPtns   = AM::nPartitions();
  m_iHash   = PtnLib::iResetRunning();
  m_i_Ptn   = 0;
  m_i_Slot  = 0;

  m_bTrace  = false;

  m_iSql    = C_UNKNOWN;
  
  m_bPixInited  = false;
  // m_oStreamPix
  //m_pDevDstRsb->m_aRsb     = nullptr;
  //m_pDevDstRsb->m_pRowSlab = nullptr;

  //m_i_SrcRow    = C_UNKNOWN;
  //m_nSrcRows    = C_UNKNOWN;
  //m_pDevSrcRsb->m_aRsb     = nullptr;
  //m_pDevSrcRsb->m_pRowSlab = nullptr;
  //m_pDevSrcIxt->m_aEnt     = nullptr;
  //m_pDevSrcIxt->m_pIdxTree = nullptr;
  //m_pDevSrcIxt->m_pEntBuffer = nullptr;
  //m_aSrcCrw     = nullptr;
  //m_pSrcCrwBuffer = nullptr;
  //m_pDevSrcRsb->m_pStreamDef = nullptr;

  //m_i_AltRow    = C_UNKNOWN;
  //m_nAltRows    = C_UNKNOWN;
  //m_aAltRsb     = nullptr;
  //m_pAltRowSlab = nullptr;
  //m_aAltEnt     = nullptr;
  //m_pAltIdxTree = nullptr;
  //m_pAltEntBuffer = nullptr;
  //m_aAltCrw     = nullptr;
  //m_pAltStreamDef = nullptr;

  //m_pEnt        = nullptr;
  //m_pEntBuffer  = nullptr;
  //m_pDevTmpIxt->m_pIdxTree  = nullptr;

  // m_oEntWrk

  m_i_Code = 0;
  
  m_pDevRlu    = new DevRLU;
  m_pDevDstRsb = new DevRSB;
  m_pDevSrcRsb = new DevRSB;
  m_pDevAltRsb = new DevRSB;

  m_pDevSrcIxt = new DevIXT;
  m_pDevAltIxt = new DevIXT;
  m_pDevTmpIxt = new DevIXT;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_eaCode,sizeof(m_eaCode),CN_LOGICUNIT);
  MemLib::vZero(m_iaDataStack,sizeof(m_iaDataStack),CN_LOGICUNIT);
  MemLib::vZero(m_aaReturnStack,sizeof(m_aaReturnStack),CN_LOGICUNIT);
#endif
  
  m_nLabels = 0;
  for (i8 i_Label = 0; i_Label < C_LOGIC_LABEL; i_Label++) {
    m_xaJumpLabel[i_Label].m_bDefined = false;
    m_xaJumpLabel[i_Label].m_bSubroutine = false;
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

  delete m_pDevRlu;

  delete m_pDevDstRsb;
  delete m_pDevSrcRsb;
  delete m_pDevAltRsb;

  delete m_pDevTmpIxt;
  delete m_pDevAltIxt;
  delete m_pDevSrcIxt;
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
  }
  else
  if (m_xaJumpLabel[i_Label].m_bDefined) {
    vAddCodeArg(eCode,m_xaJumpLabel[i_Label].m_i_Code);
  }
  else {
    i8 i_Chain = m_xaJumpLabel[i_Label].m_i_Chain;
    m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code + 1;
    vAddCodeArg(eCode,i_Chain);
  }
  
  if (EO_CALL == eCode)
    m_xaJumpLabel[i_Label].m_bSubroutine = true;

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
  
//#ifdef WITH_TRACE_LOGICUNIT
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    vTrace("  %s %lld: %4lld%s\n"_Stz,
           m_xaJumpLabel[i_Label].m_sLabel.szStz(),
           (i8)m_xaJumpLabel[i_Label].m_bDefined,
           m_xaJumpLabel[i_Label].m_i_Chain,
           m_xaJumpLabel[i_Label].m_bSubroutine ? " subroutine" : "");
//#endif
  
}



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
  
  vPrintLabels();
//#endif
  
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* - - - - - - - - - - - - - RELATIONAL LOGIC UNIT - - - - - - - - - - - - - -*/
/*============================================================================*/

#if 0

  #define dLU3(eCode,iA,iB,iC) \
    if (m_bTrace) \
      vTrace("%lld : %12s %lld %lld %lld\n"_Stz,\
             nDifference(m_eaCode,m_aeCode-1),\
             aEO[eCode],iA,iB,iC);

  #define dLU2(eCode,iA,iB) \
    if (m_bTrace) \
      vTrace("%lld : %12s %lld %lld\n"_Stz,\
             nDifference(m_eaCode,m_aeCode-1),\
             aEO[eCode],iA,iB);

  #define dLU1(eCode,iA) \
    if (m_bTrace) \
      vTrace("%lld : %12s %lld\n"_Stz,\
             nDifference(m_eaCode,m_aeCode-1),\
             aEO[eCode],iA);

  #define dLU0(eCode) \
    if (m_bTrace) \
      vTrace("%lld : %12s\n"_Stz,\
             nDifference(m_eaCode,m_aeCode-1),\
             aEO[eCode]);

  #define GetArg(iArg) \
    ASSERT(8 == EoArgs[eCode].m_nArgBytes); \
    iArg = * (i8*)m_aeCode; \
    m_aeCode += 8;

  #define GetStr(iArg) \
    ASSERT(-1 == EoArgs[eCode].m_nArgBytes); \
    iArg = (i8)m_aeCode; \
    while (*(char*)m_aeCode) m_aeCode++; \
    m_aeCode++;

  #define NoArg() \
    ASSERT(0 == EoArgs[eCode].m_nArgBytes);

#else

  #define dLU3(eCode,iA,iB,iC)
  #define dLU2(eCode,iA,iB)
  #define dLU1(eCode,iA)
  #define dLU0(eCode)

  #define GetArg(iArg) \
    iArg = * (i8*)m_aeCode; \
    m_aeCode += 8;

  #define GetStr(iArg) \
    iArg = (i8)m_aeCode; \
    while (*(char*)m_aeCode) m_aeCode++; \
    m_aeCode++;

  #define NoArg()

#endif


#define S_PushStr(__azStr) \
  /*MemLib::vDumpWords(m_iaDataStack,nDifference(m_iaDataStack,m_aiDstk)+8);*/ \
  nSize = StrLib::nSize((char*)__azStr); \
  azDst = (char*)(++m_aiDstk); \
  StrLib::vCopy(azDst,(char*)__azStr); \
  m_aiDstk += (nSize / 8) + 1; \
  *(++m_aiDstk) = nSize; \
  /*MemLib::vDumpWords(m_iaDataStack,nDifference(m_iaDataStack,m_aiDstk)+8); */

#define S_PopStr(__azDst) \
  /*MemLib::vDumpWords(m_iaDataStack,nDifference(m_iaDataStack,m_aiDstk)+8);*/ \
  nSize = *(m_aiDstk--); \
  azSrc = (char*)(m_aiDstk - ((nSize / 8) + 1)); \
  StrLib::vCopy(__azDst,azSrc); \
  m_aiDstk -= (nSize / 8) + 2; \
  /*MemLib::vDumpWords(m_iaDataStack,nDifference(m_iaDataStack,m_aiDstk)+8);*/

#define S_UseStr(__azStz) \
  /*MemLib::vDumpWords(m_iaDataStack,nDifference(m_iaDataStack,m_aiDstk)+8);*/ \
  nSize = *(m_aiDstk--); \
  __azStz = (char*)(m_aiDstk - ((nSize / 8) + 1));

#define S_UseEnd() \
  m_aiDstk -= (nSize / 8) + 2; \
  /*MemLib::vDumpWords(m_iaDataStack,nDifference(m_iaDataStack,m_aiDstk)+8);*/


#define JumpTo(__iArg) \
  m_aeCode = & m_eaCode[__iArg];

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
  i8 nSize;
  char* azDst;
  char* azSrc;
  char* azArg;

  Key* aSrcKey;
  Key* aAltKey;
  Key* aWrkKey;

  SP_Ent spEnt;
  Ent* aEnt;


  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;

  IndexDef* pIndexDef = nullptr;
  i8* aPayload = nullptr;

  

  
  // -- -- -- -- -- -- -- -- -- --
  m_aeCode  = & m_eaCode[0];
  m_aaeRstk = &m_aaReturnStack[0];
  m_aiDstk  = &m_iaDataStack[0];
  *m_aiDstk = 0xFFFFFFFFFFFFFFFF;

  m_i_Slot  = AM::pTaskJob()->m_pOp->nPartition();
  
  // temp
  m_aaDev[0] = m_pDevTmpIxt->m_pIdxTree;
  // end temp
  
#ifdef WITH_TRACE_LOGICUNIT
  vTrace("\nLogic Unit :: sql %lld\n\n"_Stz,m_iSql);
  vDebugPoint();
#endif

  if (nOuterArgs) {
    *(++m_aiDstk) = iOuterArg0;
  }
  
  for (;;) {
    
#ifdef WITH_TRACE_LOGICUNIT
    // check for stack growth
    if (m_aiDstk >= &m_iaDataStack[C_LOGIC_STACK-4])
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "LogicUnit data stack exhausted\n"_Stz);
    
    i8 nStkUse = nDifference(m_aiDstk,m_iaDataStack);
    i8 iStkPct = (nStkUse * 100) / C_LOGIC_STACK;
    if (iStkPct > 80)
      vTrace("WARNING: LogicUnit stack use %lld%%\n"_Stz,iStkPct);
#endif
  
    eEO eCode = *m_aeCode++;
    
    //vTrace("%s  (StkUse %lld)\n"_Stz,aEO[eCode],nStkUse);
    
    switch (eCode) { //+ExtractBegin
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // arithmetic operators
      case EO_ADD: NoArg(); // add top two int's push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA + iTopB;
        dLU3(EO_ADD,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_DIVIDE: NoArg(); // divide top int by second push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA / iTopB;
        dLU3(EO_DIVIDE,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_MODULO: NoArg(); // modulo top int be second push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA % iTopB;
        dLU3(EO_MODULO,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_MULTIPLY: NoArg(); // multiply top two int's push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA * iTopB;
        dLU3(EO_MULTIPLY,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_NEGATE: NoArg(); // negate the top value
        iTopA = *(m_aiDstk--);
        iRes  = - iTopA;
        dLU2(EO_NEGATE,iTopA,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_SUBTRACT: NoArg(); // subtract second int from top push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA - iTopB;
        dLU3(EO_SUBTRACT,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // counter
      case EO_CTR_0_DEC: NoArg(); // dec counter 0
        m_iCtr0--;
        dLU1(EO_CTR_0_DEC,m_iCtr0);
        break;//+endCase

      case EO_CTR_0_INC: NoArg(); // inc counter 0
        m_iCtr0++;
        dLU1(EO_CTR_0_INC,m_iCtr0);
        break;//+endCase

      case EO_CTR_0_POP: NoArg(); // pop counter 0
        m_iCtr0 = *(m_aiDstk--);
        dLU1(EO_CTR_0_POP,m_iCtr0);
        break;//+endCase

      case EO_CTR_0_PUSH: NoArg(); // push counter 0
        dLU1(EO_CTR_0_PUSH,m_iCtr0);
        *(++m_aiDstk) = m_iCtr0;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // jump operators
      case EO_JMP: GetArg(iArg); // jump to location
        dLU1(EO_JMP,iArg);
        JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase
        
      case EO_JMP_FALSE: GetArg(iArg); // pop top and jump to location if false
        dLU1(EO_JMP_FALSE,iArg);
        iTopA = *(m_aiDstk--);
        if (! iTopA)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase
        
      case EO_JMP_TRUE: GetArg(iArg); // pop top and jump to location if true
        dLU1(EO_JMP_TRUE,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase
        
      case EO_JMP_LESS: GetArg(iArg); // pop top and jump to location if <
        dLU1(EO_JMP_LESS,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA < 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_LESSEQUAL: GetArg(iArg); // pop top and jump to location if <=
        dLU1(EO_JMP_LESSEQUAL,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA <=0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_EQUAL: GetArg(iArg); // pop top and jump to location if ==
        dLU1(EO_JMP_EQUAL,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA == 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_NOTEQUAL: GetArg(iArg); // pop top and jump to location if !=
        dLU1(EO_JMP_NOTEQUAL,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA != 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_GREATEREQUAL: GetArg(iArg); // pop top and jump to location if >=
        dLU1(EO_JMP_GREATEREQUAL,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA >= 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_GREATER: GetArg(iArg); // pop top and jump to location if >
        dLU1(EO_JMP_GREATER,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA > 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // logical operators
      case EO_LGL_AND: NoArg(); // logical and top two int's, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = (iTopA && iTopB) ? 1 : 0;
        dLU3(EO_LGL_AND,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_LGL_OR: NoArg(); // logical or top two int's, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = (iTopA || iTopB) ? 1 : 0;
        dLU3(EO_LGL_OR,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_LGL_NOT: NoArg(); // logical not top int, push result
        iTopA = *(m_aiDstk--);
        iRes  = (! iTopA) ? 1 : 0;
        dLU2(EO_LGL_NOT,iTopA,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // null test operators
      case EO_ISNULL: NoArg(); // test if top is null, push result
        iTopA = *(m_aiDstk--);
        iRes  = (C_DATUM_NULL == iTopA) ? 1 : 0;
        dLU2(EO_ISNULL,iTopA,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_ISNOTNULL: NoArg(); // test if top is not null, push result
        iTopA = *(m_aiDstk--);
        iRes  = (C_DATUM_NULL != iTopA) ? 1 : 0;
        dLU2(EO_ISNOTNULL,iTopA,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pushes
      case EO_DATE: GetArg(iArg); // push date
        dLU1(EO_DATE,iArg);
        *(++m_aiDstk) = iArg;
        break;//+endCase
        
      case EO_INTEGER: GetArg(iArg); // push integer
        dLU1(EO_INTEGER,iArg);
        *(++m_aiDstk) = iArg;
        break;//+endCase
        
      case EO_PUSHNULL: NoArg(); // push null
        dLU0(EO_PUSHNULL);
        *(++m_aiDstk) = C_DATUM_NULL;
        break;//+endCase
        
      case EO_PUSH0: NoArg(); // push integer 0
        dLU0(EO_PUSH0);
        *(++m_aiDstk) = 0;
        break;//+endCase
        
      case EO_PUSH1: NoArg(); // push integer 1
        dLU0(EO_PUSH1);
        *(++m_aiDstk) = 1;
        break;//+endCase
        
      case EO_RANDOM: GetArg(iArg); // push random number 0..arg-1
        iRes = RndLib::iInteger(iArg);
        dLU2(EO_RANDOM,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
      

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // relational operators
      case EO_LESSTHAN: NoArg(); // test top < 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA < iTopB;
        dLU3(EO_LESSTHAN,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_LESSEQUAL: NoArg(); // test top <= 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA <= iTopB;
        dLU3(EO_LESSEQUAL,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_EQUAL: NoArg(); // test top == 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA == iTopB;
        dLU3(EO_EQUAL,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_NOTEQUAL: NoArg(); // test top != 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA != iTopB;
        dLU3(EO_NOTEQUAL,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_GREATEREQUAL: NoArg(); // test top >= 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA >= iTopB;
        dLU3(EO_GREATEREQUAL,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_GREATERTHAN: NoArg(); // test top > 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA > iTopB;
        dLU3(EO_GREATERTHAN,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // partitioning
      case EO_PTN_CALC: NoArg(); // calculate m_i_Ptn using running m_iHash
        dLU0(EO_PTN_CALC);
        m_i_Ptn = PtnLib::iPartition(m_iHash,m_nPtns);
        break;//+endCase

      case EO_PTN_PUSH: NoArg(); // push m_i_Ptn
        dLU1(EO_PTN_PUSH,m_i_Ptn);
        *(++m_aiDstk) = m_i_Ptn;
        break;//+endCase

      case EO_PTN_RESET: NoArg(); // reset running m_iHash
        dLU0(EO_PTN_RESET);
        m_iHash = PtnLib::iResetRunning();
        break;//+endCase

      case EO_PTN_RUNNING: NoArg(); // mix top into running m_iHash
        iTopA = *(m_aiDstk--);
        dLU0(EO_PTN_RUNNING);
        PtnLib::vRunning(&m_iHash,iTopA);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // stack operators
      case EO_INC: NoArg(); // increment top, push result
        iTopA = *m_aiDstk;
        iRes = iTopA + 1;
        dLU2(EO_INC,iTopA,iRes);
        *m_aiDstk = iRes;
        break;//+endCase

      case EO_DEC: NoArg(); // decrement top, push result
        iTopA = *m_aiDstk;
        iRes = iTopA - 1;
        dLU2(EO_DEC,iTopA,iRes);
        *m_aiDstk = iRes;
        break;//+endCase

      case EO_DUP: NoArg(); // duplicate top
        iTopA = *m_aiDstk;
        dLU1(EO_DUP,iTopA);
        *(++m_aiDstk) = iTopA;
        break;//+endCase

      case EO_POP: NoArg(); // pop / discard top
        iTopA = *m_aiDstk;
        dLU1(EO_POP,iTopA);
        break;//+endCase

      case EO_PRINT: NoArg(); // print top
        iTopA = *(m_aiDstk--);
        dLU1(EO_PRINT,iTopA);
        vTrace("[Q%lld.%lld] %lld\n"_Stz,m_iSql,m_i_Slot,iTopA);
        break;//+endCase
    
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // string operators
      case EO_STR_COMPARE: NoArg(); // pop top stradr and compare str on stack
      {
        dLU0(EO_STR_COMPARE);
        char* azTopA;
        char* azTopB;
        azTopA = (char*) *(m_aiDstk--);
      
        iTopB = *(m_aiDstk--);
        azTopB = (char*) (m_aiDstk-(iTopB/8 + 1));
        iRes = StrLib::iCompare(azTopA,azTopB);
        m_aiDstk -= (iTopB/8) + 2;
        *(++m_aiDstk) = iRes;
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_STR_PRINTADR: NoArg(); // pop top and print iArg as addr of string
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(m_aiDstk--);
        dLU1(EO_STR_PRINTADR,iTopA);
        vTrace("[Q%lld.%lld] %lls\n"_Stz,m_iSql,m_i_Slot,iTopA);
#endif
        break;//+endCase
        
      case EO_STR_PRINTSTK: NoArg(); // pop string from top and print
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(m_aiDstk--);
        dLU1(EO_STR_PRINTSTK,iTopA);
        vTrace("[Q%lld,%lld] %lls\n"_Stz,m_iSql,m_i_Slot,(m_aiDstk-iTopA/8));
        m_aiDstk -= (iTopA/8) + 1;
#endif
        break;//+endCase
        
      case EO_STR_LITERAL: GetStr(iArg); // push address of string literal
        dLU1(EO_STR_LITERAL,iArg);
        *(++m_aiDstk) = iArg;
        break;//+endCase
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // subroutine operators
      case EO_CALL: GetArg(iArg); // call subroutine at location
        dLU1(EO_CALL,iArg);

        * (m_aaeRstk++) = m_aeCode;
        m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_RETURN: NoArg(); // return from subroutine and
        dLU0(EO_RETURN);
        if (m_aaeRstk == &m_aaReturnStack[0]) {
          iRes = *(m_aiDstk--);
#ifdef WITH_TRACE_LOGICUNIT
          vTrace("\nEnd Logic Unit :: sql %lld\n\n"_Stz,m_iSql);
#endif
          return iRes;
        }
        m_aeCode = * (--m_aaeRstk);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // misc
      case EO_ASSERT: NoArg(); // pop top and assert
        iTopA = *(m_aiDstk--);
        dLU1(EO_ASSERT,iTopA);
        RASSERT(iTopA);
        break;//+endCase
              
      case EO_BREAKPOINT: NoArg(); // breakpoint
        dLU0(EO_BREAKPOINT);
        vDebugPoint();
        break;//+endCase
        
      case EO_SLEEP: NoArg(); // sleep for top seconds
        iTopA = *(m_aiDstk--);
        dLU1(EO_SLEEP,iTopA);
        sleep((u4)iTopA);
        break;//+endCase
              
      case EO_SLOT: NoArg(); // sleep for top seconds
        dLU1(EO_SLOT,m_i_Slot);
        *(++m_aiDstk) = m_i_Slot;
        break;//+endCase
              
      case EO_TRACEOFF: NoArg(); // set trace
        m_bTrace = 0;
        break;//+endCase
        
      case EO_TRACEON: NoArg(); // set trace
        m_bTrace = 1;
        break;//+endCase
        
      case EO_NOOP: NoArg(); // no op
        dLU0(EO_NOOP);
        break;//+endCase

      case EO_ERROR: NoArg(); // error marker
        dLU0(EO_ERROR);
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "Code compiled with error"_Stz);
        break;//+endCase
        

      //------------------------------------------------------------------------
      //------------------------------------------------------------------------

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
        dLU1(EO_DTE_NOW,iRes);
        *(++m_aiDstk) = iRes;
      }
        break;//+endCase
        
      case EO_DTE_PRINT: NoArg(); // pop top and print as date
#ifdef WITH_TRACE_LOGICUNIT
      {
        SP_Dte spDate;
        iTopA = *(m_aiDstk--);
        dLU1(EO_DTE_PRINT,iTopA);
        vTrace("[Q%lld.%lld] %s\n"_Stz,m_iSql,m_i_Slot,spDate.sz(iTopA));
      }
#endif
        break;//+endCase

      case EO_DTE_YEAR: NoArg(); // pop top, push year of date
      {
        Dtm wDte;
        wDte = *(m_aiDstk--);
        iRes = wDte.m_wDte.m_uYearDelta + C_YEAR_BASE;
        dLU2(EO_DTE_YEAR,wDte.iAsI8(),iRes);
        *(++m_aiDstk) = iRes;
      }
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // hashing
      case EO_HSH_ADD_HASHI: NoArg(); // add hash of top int to running hash
        iTopA = *(m_aiDstk--);
        iRes = HshLib::iHash((byte*)&iTopA,sizeof(w8));
        m_iHash += iRes;
        dLU3(EO_HSH_ADD_HASHI,iTopA,iRes,m_iHash);
        break;//+endCase
        
      case EO_HSH_RESET: NoArg(); // reset hash
        dLU0(EO_HSH_RESET);
        m_iHash = 0;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // timer
      case EO_TMR_START: NoArg(); // timer start
        dLU0(EO_TMR_START);
        m_oTimer.vStart();
        break;//+endCase
        
      case EO_TMR_NSECS: NoArg(); // push timer nanoseconds
        iRes = m_oTimer.iNanoseconds();
        dLU1(EO_TMR_NSECS,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_TMR_UPDATE: NoArg(); // timer update
        dLU0(EO_TMR_UPDATE);
        m_oTimer.vUpdate();
        break;//+endCase
        
      case EO_TMR_AGG_ADD: NoArg(); // add timer to agg timer
        dLU0(EO_TMR_AGG_ADD);
        m_oAggTimer.vAdd(&m_oTimer);
        break;//+endCase
        
      case EO_TMR_AGG_NSECS: NoArg(); // push agg nanoseconds
        iRes = m_oAggTimer.iNanoseconds();
        dLU1(EO_TMR_AGG_NSECS,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_TMR_AGG_RESET: NoArg(); // reset agg timer
        dLU0(EO_TMR_AGG_RESET);
        m_oTimer.vReset();
        break;//+endCase
          

      //------------------------------------------------------------------------
      //------------------------------------------------------------------------

        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // col operators
      case EO_COL_DST_RESET: GetArg(iArg); // reset column in streamPix of dst
      {
        m_oStreamPix.vNotProject(iArg);
      }
        break;//+endCase
        
      case EO_COL_SRC_PUSH: GetArg(iArg); // push column value from m_aSrcCrw
      {
        HV_Row hRow(m_pDevSrcRsb->m_pStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_pDevSrcIxt->m_aCrw,iArg);
        iRes = hRow->m_waColVal[iArg].m_iWrd;
        dLU2(EO_COL_SRC_PUSH,iArg,iRes);
        *(++m_aiDstk) = iRes;
      }
        break;//+endCase
        
      case EO_COL_SRC_PUSHSTR: GetArg(iArg); // push column value from m_aSrcCrw
      {
        HV_Row hRow(m_pDevSrcRsb->m_pStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_pDevSrcIxt->m_aCrw,iArg);
        azArg = (char*)hRow->m_waColVal[iArg].m_iWrd;
        dLU1(EO_COL_SRC_PUSHSTR,iArg);
//        i8 nSize = StrLib::nSize((char*)iRes);
//        char* azDst = (char*)(++m_aiDstk);
//        StrLib::vCopy(azDst,(char*)iRes);
//        m_aiDstk += nSize / 8;
//        *(++m_aiDstk) = nSize;
        S_PushStr(azArg);
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_COL_ALT_PUSH: GetArg(iArg); // push column value from m_aAltCrw
      {
        HV_Row hRow(m_pDevAltRsb->m_pStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_pDevAltIxt->m_aCrw,iArg);
        iRes = hRow->m_waColVal[iArg].m_iWrd;
        dLU2(EO_COL_ALT_PUSH,iArg,iRes);
        *(++m_aiDstk) = iRes;
      }
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // crw operators
      case EO_CRW_SRC_BEGIN: NoArg(); // begin src rsb traverse
        dLU0(EO_CRW_SRC_BEGIN);
        m_pDevSrcIxt->m_i_Row = -1;
        m_pDevSrcIxt->m_aCrw = nullptr;
        m_pDevSrcIxt->m_nRows = m_pDevSrcIxt->m_pCrwBuffer ?
                         m_pDevSrcIxt->m_pCrwBuffer->nRowCount() : 0;
        break;//+endCase
        
      case EO_CRW_SRC_FETCH: NoArg(); // fetch m_aSrcCrw using m_pDevSrcIxt->m_aEnt.ref
        dLU0(EO_CRW_SRC_FETCH);
        m_pDevSrcIxt->m_aCrw  =
          CrwBuffer::pRefdCrw(m_pDevSrcIxt->m_aEnt->rxRef());
        break;//+endCase
        
      case EO_CRW_ALT_FETCH: NoArg(); // fetch m_aAltCrw using m_aAltEnt.ref
        dLU0(EO_CRW_ALT_FETCH);
        m_pDevAltIxt->m_aCrw =
          CrwBuffer::pRefdCrw(m_pDevAltIxt->m_aEnt->rxRef());
        break;//+endCase
       
      case EO_CRW_SRC_NEXT: NoArg(); // move to next src rsb
        dLU0(EO_CRW_SRC_NEXT);
        if (++m_pDevSrcIxt->m_i_Row < m_pDevSrcIxt->m_nRows) {
          m_pDevSrcIxt->m_aCrw =
            m_pDevSrcIxt->m_pCrwBuffer->aCrw(m_pDevSrcIxt->m_i_Row);
          *(++m_aiDstk) = 1;
        }
        else {
          *(++m_aiDstk) = 0;
        }
        break;//+endCase
      
      case EO_CRW_SRC_TORSB:
        dLU0(EO_CRW_SRC_TORSB);
        RowInflator::vInflateColumns(m_pDevDstRsb->m_pRowSlab,m_pDevSrcIxt->m_aCrw);
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry
      case EO_ENT_NEXT: NoArg(); // move to next m_pEnt
        dLU0(EO_ENT_NEXT);
        m_pDevTmpIxt->m_aEnt =
          EntBuffer::pMoveToNextEntry(m_pDevTmpIxt->m_aEnt,
                                      &m_pDevTmpIxt->m_pEntBuffer);
        if (m_pDevTmpIxt->m_aEnt)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;//+endCase
        
      case EO_ENT_POSPRIOR: NoArg(); // position m_pEnt prior using m_oEntWrk.key
        dLU0(EO_ENT_POSPRIOR);
        m_pDevTmpIxt->m_aEnt =
          m_pDevTmpIxt->m_pIdxTree->aPositionPriorEntry(m_oEntWrk.aKey(),
                                                        &m_pDevTmpIxt->m_pEntBuffer);
        break;//+endCase
        
      case EO_ENT_PUSH: NoArg(); // push entry on the stack
        dLU0(EO_ENT_PUSH);
        *(++m_aiDstk) = (i8)m_pDevTmpIxt->m_aEnt;
        break;//+endCase
        
      case EO_ENT_PUSHELMI0: NoArg(); // push entry elm 0 int
        dLU0(EO_ENT_PUSHELMI0);
        iRes = m_pDevTmpIxt->m_aEnt->aKey()->iGetIntElm();
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_ENT_PUSHELMS0: NoArg(); // push entry elm 0 str
      {
        char acFlipped[128];
        dLU0(EO_ENT_PUSHELMS0);
        char* azSrc = (char*)m_pDevTmpIxt->m_aEnt->aKey()->szGetStrElm();
      
        StrLib::iWordFlipBack(acFlipped,azSrc);
      
//        i8 nSize = StrLib::nSize(acFlipped);
//        char* azDst = (char*)(++m_aiDstk);
//        StrLib::vCopy(azDst,acFlipped);
//        m_aiDstk++;
//        *(++m_aiDstk) = nSize;
        S_PushStr((char*)acFlipped);
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_ENT_PUSHOFS: NoArg(); // push entry offset
        dLU0(EO_ENT_PUSHOFS);
        iRes = m_pDevTmpIxt->m_aEnt->iBuffer();
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_ENT_PUSHREF: NoArg(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry relational operators
      case EO_ENT_LESS: NoArg(); // test if m_pEnt.key <  m_oEntWrk.key
        iRes  = KC_BEFORE == Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_LESS,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_ENT_LESSEQUAL: NoArg(); // test if m_pEnt.key <= m_oEntWrk.key
        iCmp = Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey());
        iRes  = (KC_SAME == iCmp || KC_BEFORE == iCmp ) ? 1 : 0;
        dLU1(EO_ENT_LESSEQUAL,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_ENT_EQUAL: NoArg();
      {
#ifdef WITH_TRACE_LOGICUNIT
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,
               spEnt.sz(m_oEntWrk.aEnt(),m_pDevTmpIxt->m_pIdxTree->pIndexDef()));
        vTrace("\n\nEnt : %s\n"_Stz,spEnt.sz(m_pDevTmpIxt->m_aEnt,m_pDevTmpIxt->m_pIdxTree->pIndexDef()));
#endif
        iRes  = KC_SAME == Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_EQUAL,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
      }
        
      case EO_ENT_NOTEQUAL: NoArg();
        iRes  = KC_SAME != Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_NOTEQUAL,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_ENT_GREATEREQUAL: NoArg();
        iCmp = Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey());
        iRes = (KC_SAME == iCmp || KC_AFTER == iCmp ) ? 1 : 0;
        dLU1(EO_ENT_GREATEREQUAL,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_ENT_GREATER: NoArg();
        iRes  = KC_AFTER == Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_GREATER,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entWrk
      case EO_ENTWRK_APPENDELM: GetArg(iArg); // append next element value
      {
        dLU1(EO_ENTWRK_APPENDELM,iArg);
        m_oEntWrk.vAppendElement(m_pDevSrcRsb->m_aRsb, &m_pDevSrcRsb->m_pRowSlab->m_oStreamPix, iArg);
        
#ifdef WITH_TRACE_LOGICUNIT
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pDevTmpIxt->m_pIdxTree->pIndexDef()));
#endif
        break;//+endCase
      }
        
      case EO_ENTWRK_APPENDELMI: NoArg(); // append next element value (int)
        iTopA = *(m_aiDstk--);
        dLU1(EO_ENTWRK_APPENDELMI,iTopA);
        m_oEntWrk.vAppendElementInt(m_pDevSrcRsb->m_aRsb,iTopA);
        break;//+endCase
        
      case EO_ENTWRK_APPENDELMS: NoArg(); // append next element value (str)
      {
        dLU0(EO_ENTWRK_APPENDELMS);
//        iTopA = *(m_aiDstk--);
//        dLU1(eCode,iTopA);
//        Stz* szStr = (Stz*)(m_aiDstk-iTopA/8);
//        m_oEntWrk.vAppendElementStz(m_pDevSrcRsb->m_aRsb,szStr);
//        m_aiDstk -= (iTopA/8) + 1;
        S_UseStr(azSrc);
        m_oEntWrk.vAppendElementStz(m_pDevSrcRsb->m_aRsb,(Stz*)azSrc);
        S_UseEnd();
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_ENTWRK_CREATEPAY: GetArg(iArg); // create a payload in entwrk
        dLU1(EO_ENTWRK_CREATEPAY,iArg);
        
        m_oEntWrk.vPayload(true);
        m_oEntWrk.vPayloadBytes(iArg);
        break;//+endCase
                
      case EO_ENTWRK_POPOFS: NoArg(); // pop m_oEntWrk.ref.buffer/offset
        iTopA = *(m_aiDstk--);
        dLU1(EO_ENTWRK_POPOFS,iTopA);
        m_oEntWrk.vBuffer(iTopA);
        break;//+endCase

      case EO_ENTWRK_POPREF: NoArg(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      case EO_ENTWRK_PRINT: NoArg(); // print m_oEntWrk
#ifdef WITH_TRACE_LOGICUNIT
        aEnt = m_oEntWrk.aEnt();
        pIndexDef = m_xpIndexTree->pIndexDef();
        vTrace("EntWrk : %s\n"_Stz,spEnt.sz(aEnt,pIndexDef));
#endif
        break;//+endCase
        
      case EO_ENTWRK_RESET: NoArg(); // reset m_oEntWrk
        dLU0(EO_ENTWRK_RESET);
        m_oEntWrk.vReset();
        break;//+endCase
                              
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // index tree operators
      case EO_IXT_DEL_ENT: NoArg(); // delete current entry from indexTree
        dLU0(EO_IXT_DEL_ENT);
        m_pDevTmpIxt->m_pIdxTree->vDelete(m_pDevTmpIxt->m_pEntBuffer,m_pDevTmpIxt->m_aEnt);
        break;//+endCase

      case EO_IXT_DUMP: GetArg(iArg); // dump index tree
        //m_pDevTmpIxt->m_pIdxTree->vDump(false);
        ::iDEV_MAP(this,m_aaDev[iArg],DEV_MAP_DUMP,nullptr,nullptr,0);
        break;//+endCase

      case EO_IXT_INS_ENT: NoArg(); // insert an entry into the indexTree
      {
        dLU0(EO_IXT_INS_ENT);
#ifdef WITH_TRACE_LOGICUNIT
        SP_Ent spEnt;
        vTrace("Inserting entry"_Stz);
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pDevTmpIxt->m_pIdxTree->pIndexDef()));
#endif
        m_pDevTmpIxt->m_aEnt = m_pDevTmpIxt->m_pIdxTree->aInsertEntry(m_oEntWrk.aEnt());
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("\n\nEnt : %s\n"_Stz,spEnt.sz(m_pDevTmpIxt->m_aEnt,m_pDevTmpIxt->m_pIdxTree->pIndexDef()));
#endif
        ASSERT(m_pDevTmpIxt->m_aEnt);
      }
        break;//+endCase
        
      case EO_IXT_TRV_BEGIN: NoArg(); // begin linear traverse
        dLU0(EO_IXT_TRV_BEGIN);
        m_pDevTmpIxt->m_pEntBuffer = m_pDevTmpIxt->m_pIdxTree->pFirstH0Buffer();
        if (m_pDevTmpIxt->m_pEntBuffer) {
          m_pDevTmpIxt->m_aEnt = m_pDevTmpIxt->m_pEntBuffer->aLeadEntry();
          if (m_pDevTmpIxt->m_aEnt)
            *(++m_aiDstk) = 1;
          else
            *(++m_aiDstk) = 0;
        }
        else
          *(++m_aiDstk) = 0;
        break;//+endCase
        
      case EO_IXT_TRV_NEXT: NoArg(); // move to next entry
        dLU0(EO_IXT_TRV_NEXT);
        m_pDevTmpIxt->m_aEnt =
           EntBuffer::pMoveToNextEntry(m_pDevTmpIxt->m_aEnt,
                                       &m_pDevTmpIxt->m_pEntBuffer);
        if (m_pDevTmpIxt->m_aEnt)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;//+endCase
        
      case EO_IXT_TMP_ENTRIES: NoArg(); // push number of entries in tmp index
        iRes = m_pDevTmpIxt->m_pIdxTree->iCountH0Entries();
        dLU1(EO_IXT_TMP_ENTRIES,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_IXT_SRC_BEGIN: NoArg(); // begin linear traverse
        dLU0(EO_IXT_SRC_BEGIN);
        m_pDevSrcIxt->m_pEntBuffer = m_pDevSrcIxt->m_pIdxTree->pFirstH0Buffer();
        if (m_pDevSrcIxt->m_pEntBuffer) {
          m_pDevSrcIxt->m_aEnt = m_pDevSrcIxt->m_pEntBuffer->aLeadEntry();
          if (m_pDevSrcIxt->m_aEnt)
            *(++m_aiDstk) = 1;
          else
            *(++m_aiDstk) = 0;
        }
        else
          *(++m_aiDstk) = 0;
        break;//+endCase

      case EO_IXT_SRC_NEXT: NoArg(); // move to next entry
        dLU0(EO_IXT_SRC_NEXT);
        m_pDevSrcIxt->m_aEnt = EntBuffer::pMoveToNextEntry(m_pDevSrcIxt->m_aEnt, &m_pDevSrcIxt->m_pEntBuffer);
        if (m_pDevSrcIxt->m_aEnt)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;//+endCase

      case EO_IXT_SRC_PRINT: NoArg(); // print src ent
        pIndexDef = m_pDevSrcIxt->m_pIdxTree->pIndexDef();
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("SrcEnt : %s\n"_Stz,spEnt.sz(m_pDevSrcIxt->m_aEnt,pIndexDef));
#endif
        break;//+endCase

      case EO_IXT_ALT_BEGIN: NoArg(); // begin linear traverse
        dLU0(EO_IXT_ALT_BEGIN);
        m_pDevAltIxt->m_pEntBuffer = m_pDevAltIxt->m_pIdxTree->pFirstH0Buffer();
        if (m_pDevAltIxt->m_pEntBuffer) {
          m_pDevAltIxt->m_aEnt = m_pDevAltIxt->m_pEntBuffer->aLeadEntry();
          if (m_pDevAltIxt->m_aEnt)
            *(++m_aiDstk) = 1;
          else
            *(++m_aiDstk) = 0;
        }
        else
          *(++m_aiDstk) = 0;
        break;//+endCase

      case EO_IXT_ALT_NEXT: NoArg();
        dLU0(EO_IXT_ALT_NEXT);
        m_pDevAltIxt->m_aEnt =
          EntBuffer::pMoveToNextEntry(m_pDevAltIxt->m_aEnt,
                                      &m_pDevAltIxt->m_pEntBuffer);
        if (m_pDevAltIxt->m_aEnt)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;//+endCase

      case EO_IXT_ALT_PRINT: NoArg(); // print alt ent
        pIndexDef = m_pDevAltIxt->m_pIdxTree->pIndexDef();
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("AltEnt : %s\n"_Stz,spEnt.sz(m_pDevAltIxt->m_aEnt,pIndexDef));
#endif
        break;//+endCase
        
      case EO_IXT_JOI_MATCH: NoArg(); // test for join match (src.key == alt.key)
        aSrcKey = m_pDevSrcIxt->m_aEnt->aKey();
        aAltKey = m_pDevAltIxt->m_aEnt->aKey();
        iRes = KC_SAME == Key::eCompare(aSrcKey,aAltKey) ? 1 : 0;
        dLU1(EO_IXT_JOI_MATCH,iRes);
        *(++m_aiDstk) = iRes;
        NEVER_REACH; // not used??
        break;//+endCase

      case EO_IXT_CMP_SRCALT: NoArg(); // compare src.key to alt.key
        aSrcKey = m_pDevSrcIxt->m_aEnt->aKey();
        aAltKey = m_pDevAltIxt->m_aEnt->aKey();
        eCmp = Key::eCompare(aSrcKey,aAltKey);
        if (KC_BEFORE == eCmp)
          iRes = -1;
        else
        if (KC_SAME == eCmp)
          iRes = 0;
        else
          iRes = 1;
        dLU1(EO_IXT_CMP_SRCALT,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase

      case EO_IXT_CMP_WRKALT: NoArg(); // compare wrk.key to alt.key
        aWrkKey = m_oEntWrk.aKey();
        aAltKey = m_pDevAltIxt->m_aEnt->aKey();
        eCmp = Key::eCompare(aWrkKey,aAltKey);
        if (KC_BEFORE == eCmp)
          iRes = -1;
        else
        if (KC_SAME == eCmp)
          iRes = 0;
        else
          iRes = 1;
        dLU1(EO_IXT_CMP_WRKALT,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase

      case EO_IXT_POS_ALTBYSRC: NoArg(); // position alt ixt using src.key
        aSrcKey = m_pDevSrcIxt->m_aEnt->aKey();
        m_pDevAltIxt->m_aEnt =
          m_pDevAltIxt->m_pIdxTree->aPositionPriorEntry(aSrcKey,&m_pDevAltIxt->m_pEntBuffer);
        if (m_pDevAltIxt->m_pEntBuffer) {
          if (m_pDevAltIxt->m_aEnt)
            *(++m_aiDstk) = 1;
          else
            *(++m_aiDstk) = 0;
        }
        else
          *(++m_aiDstk) = 0;
        dLU1(EO_IXT_POS_ALTBYSRC,iRes);
        break;//+endCase

      case EO_IXT_POS_ALTBYWRK: NoArg(); // position alt ixt using entwrk.key
        aWrkKey = m_oEntWrk.aKey();
        m_pDevAltIxt->m_aEnt =
          m_pDevAltIxt->m_pIdxTree->aPositionPriorEntry(aWrkKey,&m_pDevAltIxt->m_pEntBuffer);
        if (m_pDevAltIxt->m_pEntBuffer) {
          if (m_pDevAltIxt->m_aEnt)
            *(++m_aiDstk) = 1;
          else
            *(++m_aiDstk) = 0;
        }
        else
          *(++m_aiDstk) = 0;
        dLU1(EO_IXT_POS_ALTBYWRK,iRes);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pay load
      case EO_PAY_FETCH_COLI: GetArg(iArg); // fetch a column from the entry payload
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        iRes = *aPayload;
        dLU2(EO_PAY_FETCH_COLI,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_PAY_FETCH_COLS: GetArg(iArg); // fetch a column from the entry payload
      {
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        char* azSrc = (char*)aPayload;
        i8 nSize = StrLib::nSize(azSrc);
        ASSERT(nSize < 8);
        dLU2(EO_PAY_FETCH_COLS,iArg,(i8)azSrc);
//        char* azDst = (char*)(++m_aiDstk);
//        StrLib::vCopy(azDst,azSrc);
//        m_aiDstk++;
//        *(++m_aiDstk) = nSize;
        S_PushStr(azSrc);
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_PAY_STORE_COLI: GetArg(iArg); // store a column of the entry payload
        iRes = *(m_aiDstk--);
        dLU2(EO_PAY_STORE_COLI,iArg,iRes);
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        *aPayload = iRes;
        break;//+endCase
      
      case EO_PAY_STORE_COLS: GetArg(iArg); // store a column of the entry payload
      {
        dLU1(EO_PAY_STORE_COLI,iArg);
//        i8 nSize = *(m_aiDstk--);
//        RASSERT(nSize < 8);
//        dLU2(eCode,iArg,nSize);
//        aPayload = m_pEnt->aiPayCell(iArg);
//        char* azSrc = (char*)(--m_aiDstk);
//        m_aiDstk--;
//        StrLib::vCopy((char*)aPayload,azSrc);
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        S_PopStr((char*)aPayload);
        vDebugPoint();
      }
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pix
      case EO_PIX_STM_PRINT: NoArg(); // print stream pix
        dLU0(EO_PIX_STM_PRINT);
        m_oStreamPix.vPrint();
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb / rowslab
      case EO_RSB_DST_COPYSRC: NoArg(); // copy src rsb to dst rsb
        dLU0(EO_RSB_DST_COPYSRC);
        m_pDevDstRsb->m_pRowSlab->vPositionOnCell(0);
        m_pDevDstRsb->m_aRsb = m_pDevDstRsb->m_pRowSlab->aRsb();
        m_pDevDstRsb->m_pRowSlab->vCopyRsb(m_pDevSrcRsb->m_aRsb,bSrcHasPtnHash,bCopyPtnHash);
        break;//+endCase

      case EO_RSB_DST_PRINT: NoArg(); // print dst rsb
        dLU0(EO_RSB_DST_PRINT);
        m_pDevDstRsb->m_pRowSlab->vDumpRow(-1,m_pDevDstRsb->m_aRsb);
        break;//+endCase

      case EO_RSB_SRC_PRINT: NoArg(); // print src rsb
        dLU0(EO_RSB_SRC_PRINT);
        m_pDevSrcRsb->m_pRowSlab->vDumpRow(-1,m_pDevSrcRsb->m_aRsb);
        break;//+endCase

      case EO_RSB_ALT_PRINT: NoArg(); // print alt rsb
        dLU0(EO_RSB_ALT_PRINT);
        m_pDevAltRsb->m_pRowSlab->vDumpRow(-1,m_pDevAltRsb->m_aRsb);
        break;//+endCase

      case EO_RSB_DST_NEW: NoArg(); // create new destination rsb
        dLU0(EO_RSB_DST_NEW);
        //ASSERT(0 == m_i_Ptn);
        m_pDevDstRsb->m_aRsb = m_xpPot->aNewDstRsb(m_i_Ptn,&m_pDevDstRsb->m_pRowSlab);
        break;//+endCase

      case EO_RSB_DST_RESET: NoArg(); // reset dst rowslab
        dLU0(EO_RSB_DST_RESET);
        m_pDevDstRsb->m_pRowSlab->vReset();
        break;//+endCase
        
      case EO_RSB_DST_TEST: NoArg(); // test if there is a destination rsb
        dLU0(EO_RSB_DST_TEST);
        if (m_pDevDstRsb->m_aRsb)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;//+endCase
        
      case EO_RSB_DST_ZERO: NoArg(); // zero destination rsb
        dLU0(EO_RSB_DST_ZERO);
        m_pDevDstRsb->m_pRowSlab->vZeroRsb(m_pDevDstRsb->m_aRsb);
        break;//+endCase
        
      case EO_RSB_SRC_BEGIN: NoArg(); // begin src rsb traverse
        dLU0(EO_RSB_SRC_BEGIN);
        m_pDevSrcRsb->m_i_Row = -1;
        m_pDevSrcRsb->m_aRsb  = nullptr;
        m_pDevSrcRsb->m_nRows = m_pDevSrcRsb->m_pRowSlab ? m_pDevSrcRsb->m_pRowSlab->nRows() : 0;
        break;//+endCase

      case EO_RSB_SRC_NEXT: NoArg(); // move to next src rsb
        dLU0(EO_RSB_SRC_NEXT);
        if (++m_pDevSrcRsb->m_i_Row  < m_pDevSrcRsb->m_nRows) {
          m_pDevSrcRsb->m_aRsb = m_pDevSrcRsb->m_pRowSlab->aRsbAdr(m_pDevSrcRsb->m_i_Row );
          *(++m_aiDstk) = 1;
        }
        else {
          *(++m_aiDstk) = 0;
        }
        break;//+endCase

      case EO_RSB_SRC_PUSHOFS: NoArg();
        dLU0(EO_RSB_SRC_PUSHOFS);
        iRes = m_pDevSrcRsb->m_pRowSlab->iOfsOfRsb(m_pDevSrcRsb->m_aRsb);
        *(++m_aiDstk) = iRes;
        break;//+endCase

      case EO_RSB_SRC_SETBYOFS: NoArg();  // set src rsb by using offset on stack
        dLU0(EO_RSB_SRC_SETBYOFS);
        iTopA = *(m_aiDstk--);
        m_pDevSrcRsb->m_aRsb = m_pDevSrcRsb->m_pRowSlab->aRsbByOfs(iTopA);
        break;//+endCase

      case EO_RSB_ALT_BEGIN: NoArg(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      case EO_RSB_ALT_NEXT: NoArg(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      case EO_RSB_ALT_PUSHOFS: NoArg(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      case EO_RSB_ALT_SETBYOFS: NoArg(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb operators
      case EO_RSB_DST_POPCOLI: GetArg(iArg); // pop and deposit top into dst column
        iRes = *(m_aiDstk--);
        dLU2(EO_RSB_DST_POPCOLI,iArg,iRes);
        m_pDevDstRsb->m_pRowSlab->vPositionOnCell(iArg);
        m_pDevDstRsb->m_pRowSlab->vDeposit(iRes);
        break;//+endCase
        
      case EO_RSB_DST_POPCOLS: GetArg(iArg); // pop and deposit top into dst column
      {
        dLU1(EO_RSB_DST_POPCOLS,iArg);
//        i8 nSize = *(m_aiDstk--);
//        dLU2(eCode,iArg,nSize);
//        char* azSrc = (char*)(--m_aiDstk);
        m_pDevDstRsb->m_pRowSlab->vPositionOnCell(iArg);
        char* azDst = m_pDevDstRsb->m_pRowSlab->azDepositString_begin();
//        StrLib::vCopy(azDst,azSrc);
        S_PopStr(azDst);
        m_pDevDstRsb->m_pRowSlab->vDepositString_end(nSize);
//        m_aiDstk--;
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_RSB_DST_PUSHCOLI: GetArg(iArg); // push dst column
        m_pDevDstRsb->m_aRsb = m_pDevDstRsb->m_pRowSlab->aRsbAdr(m_pDevDstRsb->m_pRowSlab->nRows()-1);
        i_Cell_ntv = m_pDevDstRsb->m_aRsb->i_ColToCell_ntv(iArg);
        iRes = m_pDevDstRsb->m_aRsb->iVal_ntv(i_Cell_ntv);
        dLU2(EO_RSB_DST_PUSHCOLI,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_RSB_SRC_PUSHCOLI: GetArg(iArg); // push src column
        iRes =
          m_pDevSrcRsb->m_aRsb->iVal_ntv(m_pDevSrcRsb->m_aRsb->i_ColToCell_ntv(iArg));
        dLU2(EO_RSB_SRC_PUSHCOLI,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase
        
      case EO_RSB_SRC_PUSHCOLS: GetArg(iArg); // push src column
      {
        char* azSrc = (char*)m_pDevSrcRsb->m_aRsb->szVal_ntv(m_pDevSrcRsb->m_aRsb->i_ColToCell_ntv(iArg));
        dLU2(EO_RSB_SRC_PUSHCOLS,iArg,(i8)azSrc);
//        i8 nSize = StrLib::nSize(azSrc);
//        char* azDst = (char*)(++m_aiDstk);
//        StrLib::vCopy(azDst,azSrc);
//        m_aiDstk++;
//        *(++m_aiDstk) = nSize;
        S_PushStr(azSrc);
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_RSB_ALT_PUSHCOLI: GetArg(iArg); // push alt column
        iRes = m_pDevAltRsb->m_aRsb->iVal_ntv(m_pDevAltRsb->m_aRsb->i_ColToCell_ntv(iArg));
        dLU2(EO_RSB_ALT_PUSHCOLI,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      default:
        vTrace("Unknown eo opcode %lld\n"_Stz,(i8)eCode);
        return C_DATUM_NULL;
    } //+ExtractEnd
    
  }
  
  return m_iaDataStack[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
