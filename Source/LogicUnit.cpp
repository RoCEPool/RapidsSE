/*============================================================================*/
/* LogicUnit.hpp -- Logic Unit                                                */
/*============================================================================*/

#include "Expressions.h"

#include "DEV_enum.h"
#include "DEV_interface.h"

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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get eCode at given i_Code                                                  */
/*----------------------------------------------------------------------------*/
eEO LogicUnit::eCode(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  eEO eCode = m_eaCode[i_Code];

  return eCode;
}

/*----------------------------------------------------------------------------*/
/* return number of eo codes                                                  */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::nCodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return m_i_Code;
}

/*----------------------------------------------------------------------------*/
/* get iArg at given i_Code                                                   */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iCodeArg(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iArg = * (i8*)&m_eaCode[i_Code];

  return iArg;
}

/*----------------------------------------------------------------------------*/
/* find label which is at given i_Code                                        */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iFindLabel(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (i_Code == m_xaJumpLabel[i_Label].m_i_Code)
      return i_Label;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find label by name and return i_Label                                      */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iFindLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (m_xaJumpLabel[i_Label].m_sLabelNm.bSame(szLabelNm))
      return i_Label;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get code point of label                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iLabelCode(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 i_Code = m_xaJumpLabel[i_Label].m_i_Code;
  
  return i_Code;
}

/*----------------------------------------------------------------------------*/
/* is given label a subroutine?                                               */
/*----------------------------------------------------------------------------*/
bool LogicUnit::bLabelIsSbr(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  bool bIsSbr = m_xaJumpLabel[i_Label].m_bSubroutine;
  
  return bIsSbr;
}

/*----------------------------------------------------------------------------*/
/* return number of labels                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::nLabels(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return m_i_Code;
}

/*----------------------------------------------------------------------------*/
/* return number of labels                                                    */
/*----------------------------------------------------------------------------*/
Stz* LogicUnit::szLabelNm(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  Stz* szLblNm = m_xaJumpLabel[i_Label].m_sLabelNm;
  
  return szLblNm;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get entwrk                                                                 */
/*----------------------------------------------------------------------------*/
EntWrk* LogicUnit::pEntWrk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return & m_oEntWrk;
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
  
  m_xaJumpLabel[i_Label].m_sLabelNm = szLabelNm;
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
      return m_xaJumpLabel[i_Label].m_sLabelNm;

  return nullptr;
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
           m_xaJumpLabel[i_Label].m_sLabelNm.szStz(),
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
  
  if (100001 != m_iSql)
  //if (302 != m_iSql)
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

  #define ArgGet(iArg) \
    ASSERT(8 == EoArgs[eCode].m_nArgBytes); \
    iArg = * (i8*)m_aeCode; \
    m_aeCode += 8;

  #define GetStr(iArg) \
    ASSERT(-1 == EoArgs[eCode].m_nArgBytes); \
    iArg = (i8)m_aeCode; \
    while (*(char*)m_aeCode) m_aeCode++; \
    m_aeCode++;

  #define ArgNil() \
    ASSERT(0 == EoArgs[eCode].m_nArgBytes);

#else

  #define dLU3(eCode,iA,iB,iC)
  #define dLU2(eCode,iA,iB)
  #define dLU1(eCode,iA)
  #define dLU0(eCode)

  #define ArgGet(iArg) \
    iArg = * (i8*)m_aeCode; \
    m_aeCode += 8;

  #define GetStr(iArg) \
    iArg = (i8)m_aeCode; \
    while (*(char*)m_aeCode) m_aeCode++; \
    m_aeCode++;

  #define ArgNil()

#endif


#define S_PushStr(__azStr) \
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8);*/ \
  nSize = StrLib::nSize((char*)__azStr); \
  azDst = (char*)(++aiDstk); \
  StrLib::vCopy(azDst,(char*)__azStr); \
  aiDstk += (nSize / 8) + 1; \
  *(++aiDstk) = nSize; \
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8); */

#define S_PopStr(__azDst) \
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8);*/ \
  nSize = *(aiDstk--); \
  azSrc = (char*)(aiDstk - ((nSize / 8) + 1)); \
  StrLib::vCopy(__azDst,azSrc); \
  aiDstk -= (nSize / 8) + 2; \
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8);*/

#define S_UseStr(__azStz) \
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8);*/ \
  nSize = *(aiDstk--); \
  __azStz = (char*)(aiDstk - ((nSize / 8) + 1));

#define S_UseEnd() \
  aiDstk -= (nSize / 8) + 2; \
  /*MemLib::vDumpWords(iaDataStack,nDifference(iaDataStack,aiDstk)+8);*/


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

  i8*  aiDstk = nullptr;

  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;

  IndexDef* pIndexDef = nullptr;
  i8* aPayload = nullptr;

  
  i8     iaDataStack[C_LOGIC_STACK];
  eEO*   aaReturnStack[C_LOGIC_STACK];

  
  // -- -- -- -- -- -- -- -- -- --
  m_aeCode  = & m_eaCode[0];
  eEO** aaeRstk = &aaReturnStack[0];
  aiDstk  = &iaDataStack[0];
  *aiDstk = 0xFFFFFFFFFFFFFFFF;

  m_i_Slot  = AM::pTaskJob()->m_pOp->nPartition();
  
  // temp
  m_aaDev[0] = m_pDevTmpIxt->m_pIdxTree;
  // end temp
  
#ifdef WITH_TRACE_LOGICUNIT
  vTrace("\nLogic Unit :: sql %lld\n\n"_Stz,m_iSql);
  vDebugPoint();
#endif

  if (nOuterArgs) {
    *(++aiDstk) = iOuterArg0;
  }
  
  LogicUnit* pRLU = this;
  
  for (;;) {
    
#ifdef WITH_TRACE_LOGICUNIT
    // check for stack growth
    if (aiDstk >= &iaDataStack[C_LOGIC_STACK-4])
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "LogicUnit data stack exhausted\n"_Stz);
    
    i8 nStkUse = nDifference(aiDstk,iaDataStack);
    i8 iStkPct = (nStkUse * 100) / C_LOGIC_STACK;
    if (iStkPct > 80)
      vTrace("WARNING: LogicUnit stack use %lld%%\n"_Stz,iStkPct);
#endif
  
    eEO eCode = *m_aeCode++;
    
    //vTrace("%s  (StkUse %lld)\n"_Stz,aEO[eCode],nStkUse);
    
    switch (eCode) { //+ExtractBegin
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // arithmetic operators
      case EO_ADD: ArgNil(); // add top two int's push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA + iTopB;
        dLU3(EO_ADD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_DIVIDE: ArgNil(); // divide top int by second push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA / iTopB;
        dLU3(EO_DIVIDE,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_MODULO: ArgNil(); // modulo top int be second push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA % iTopB;
        dLU3(EO_MODULO,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_MULTIPLY: ArgNil(); // multiply top two int's push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA * iTopB;
        dLU3(EO_MULTIPLY,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_NEGATE: ArgNil(); // negate the top value
        iTopA = *(aiDstk--);
        iRes  = - iTopA;
        dLU2(EO_NEGATE,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_SUBTRACT: ArgNil(); // subtract second int from top push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA - iTopB;
        dLU3(EO_SUBTRACT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // counter
      case EO_CTR_0_DEC: ArgNil(); // dec counter 0
        m_iCtr0--;
        dLU1(EO_CTR_0_DEC,m_iCtr0);
        break;//+endCase

      case EO_CTR_0_INC: ArgNil(); // inc counter 0
        m_iCtr0++;
        dLU1(EO_CTR_0_INC,m_iCtr0);
        break;//+endCase

      case EO_CTR_0_POP: ArgNil(); // pop counter 0
        m_iCtr0 = *(aiDstk--);
        dLU1(EO_CTR_0_POP,m_iCtr0);
        break;//+endCase

      case EO_CTR_0_PUSH: ArgNil(); // push counter 0
        dLU1(EO_CTR_0_PUSH,m_iCtr0);
        *(++aiDstk) = m_iCtr0;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // jump operators
      case EO_JMP: ArgGet(iArg); // jump to location
        dLU1(EO_JMP,iArg);
        JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase
        
      case EO_JMP_FALSE: ArgGet(iArg); // pop top and jump to location if false
        dLU1(EO_JMP_FALSE,iArg);
        iTopA = *(aiDstk--);
        if (! iTopA)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase
        
      case EO_JMP_TRUE: ArgGet(iArg); // pop top and jump to location if true
        dLU1(EO_JMP_TRUE,iArg);
        iTopA = *(aiDstk--);
        if (iTopA)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase
        
      case EO_JMP_LESS: ArgGet(iArg); // pop top and jump to location if <
        dLU1(EO_JMP_LESS,iArg);
        iTopA = *(aiDstk--);
        if (iTopA < 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_LESSEQUAL: ArgGet(iArg); // pop top and jump to location if <=
        dLU1(EO_JMP_LESSEQUAL,iArg);
        iTopA = *(aiDstk--);
        if (iTopA <=0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_EQUAL: ArgGet(iArg); // pop top and jump to location if ==
        dLU1(EO_JMP_EQUAL,iArg);
        iTopA = *(aiDstk--);
        if (iTopA == 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_NOTEQUAL: ArgGet(iArg); // pop top and jump to location if !=
        dLU1(EO_JMP_NOTEQUAL,iArg);
        iTopA = *(aiDstk--);
        if (iTopA != 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_GREATEREQUAL: ArgGet(iArg); // pop top and jump to location if >=
        dLU1(EO_JMP_GREATEREQUAL,iArg);
        iTopA = *(aiDstk--);
        if (iTopA >= 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_GREATER: ArgGet(iArg); // pop top and jump to location if >
        dLU1(EO_JMP_GREATER,iArg);
        iTopA = *(aiDstk--);
        if (iTopA > 0)
          JumpTo(iArg); // m_aeCode = & m_eaCode[iArg];
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // logical operators
      case EO_LGL_AND: ArgNil(); // logical and top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA && iTopB) ? 1 : 0;
        dLU3(EO_LGL_AND,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_LGL_OR: ArgNil(); // logical or top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA || iTopB) ? 1 : 0;
        dLU3(EO_LGL_OR,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_LGL_NOT: ArgNil(); // logical not top int, push result
        iTopA = *(aiDstk--);
        iRes  = (! iTopA) ? 1 : 0;
        dLU2(EO_LGL_NOT,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // null test operators
      case EO_ISNULL: ArgNil(); // test if top is null, push result
        iTopA = *(aiDstk--);
        iRes  = (C_DATUM_NULL == iTopA) ? 1 : 0;
        dLU2(EO_ISNULL,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_ISNOTNULL: ArgNil(); // test if top is not null, push result
        iTopA = *(aiDstk--);
        iRes  = (C_DATUM_NULL != iTopA) ? 1 : 0;
        dLU2(EO_ISNOTNULL,iTopA,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pushes
      case EO_DATE: ArgGet(iArg); // push date
        dLU1(EO_DATE,iArg);
        *(++aiDstk) = iArg;
        break;//+endCase
        
      case EO_INTEGER: ArgGet(iArg); // push integer
        dLU1(EO_INTEGER,iArg);
        *(++aiDstk) = iArg;
        break;//+endCase
        
      case EO_PUSHNULL: ArgNil(); // push null
        dLU0(EO_PUSHNULL);
        *(++aiDstk) = C_DATUM_NULL;
        break;//+endCase
        
      case EO_PUSH0: ArgNil(); // push integer 0
        dLU0(EO_PUSH0);
        *(++aiDstk) = 0;
        break;//+endCase
        
      case EO_PUSH1: ArgNil(); // push integer 1
        dLU0(EO_PUSH1);
        *(++aiDstk) = 1;
        break;//+endCase
        
      case EO_RANDOM: ArgGet(iArg); // push random number 0..arg-1
        iRes = RndLib::iInteger(iArg);
        dLU2(EO_RANDOM,iArg,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
      

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // relational operators
      case EO_LESSTHAN: ArgNil(); // test top < 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA < iTopB;
        dLU3(EO_LESSTHAN,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_LESSEQUAL: ArgNil(); // test top <= 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA <= iTopB;
        dLU3(EO_LESSEQUAL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_EQUAL: ArgNil(); // test top == 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA == iTopB;
        dLU3(EO_EQUAL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_NOTEQUAL: ArgNil(); // test top != 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA != iTopB;
        dLU3(EO_NOTEQUAL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_GREATEREQUAL: ArgNil(); // test top >= 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA >= iTopB;
        dLU3(EO_GREATEREQUAL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_GREATERTHAN: ArgNil(); // test top > 2nd, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iTopA > iTopB;
        dLU3(EO_GREATERTHAN,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // stack operators
      case EO_INC: ArgNil(); // increment top, push result
        iTopA = *aiDstk;
        iRes = iTopA + 1;
        dLU2(EO_INC,iTopA,iRes);
        *aiDstk = iRes;
        break;//+endCase

      case EO_DEC: ArgNil(); // decrement top, push result
        iTopA = *aiDstk;
        iRes = iTopA - 1;
        dLU2(EO_DEC,iTopA,iRes);
        *aiDstk = iRes;
        break;//+endCase

      case EO_DUP: ArgNil(); // duplicate top
        iTopA = *aiDstk;
        dLU1(EO_DUP,iTopA);
        *(++aiDstk) = iTopA;
        break;//+endCase

      case EO_POP: ArgNil(); // pop / discard top
        iTopA = *aiDstk;
        dLU1(EO_POP,iTopA);
        break;//+endCase

      case EO_PRINT: ArgNil(); // print top
        iTopA = *(aiDstk--);
        dLU1(EO_PRINT,iTopA);
        //vTrace("[Q%lld.%lld] %lld\n"_Stz,m_iSql,m_i_Slot,iTopA);
        printf("[Q%lld.%lld] %lld\n",(i8)0,(i8)0,iTopA);
        break;//+endCase
    
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // string operators
      case EO_STR_COMPARE: ArgNil(); // pop top stradr and compare str on stack
      {
        dLU0(EO_STR_COMPARE);
        char* azTopA;
        char* azTopB;
        azTopA = (char*) *(aiDstk--);
      
        iTopB = *(aiDstk--);
        azTopB = (char*) (aiDstk-(iTopB/8 + 1));
        iRes = StrLib::iCompare(azTopA,azTopB);
        aiDstk -= (iTopB/8) + 2;
        *(++aiDstk) = iRes;
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_STR_PRINTADR: ArgNil(); // pop top and print iArg as addr of string
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(aiDstk--);
        dLU1(EO_STR_PRINTADR,iTopA);
        vTrace("[Q%lld.%lld] %lls\n"_Stz,m_iSql,m_i_Slot,iTopA);
#endif
        break;//+endCase
        
      case EO_STR_PRINTSTK: ArgNil(); // pop string from top and print
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(aiDstk--);
        dLU1(EO_STR_PRINTSTK,iTopA);
        vTrace("[Q%lld,%lld] %lls\n"_Stz,m_iSql,m_i_Slot,(aiDstk-iTopA/8));
        aiDstk -= (iTopA/8) + 1;
#endif
        break;//+endCase
        
      case EO_STR_LITERAL: GetStr(iArg); // push address of string literal
        dLU1(EO_STR_LITERAL,iArg);
        *(++aiDstk) = iArg;
        break;//+endCase
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // subroutine operators
      case EO_CALL: ArgGet(iArg); // call subroutine at location
        dLU1(EO_CALL,iArg);
#ifdef __TINYC__
#else
        * (aaeRstk++) = m_aeCode;
        m_aeCode = & m_eaCode[iArg];
#endif
        break;//+endCase

      case EO_RETURN: ArgNil(); // return from subroutine and
        dLU0(EO_RETURN);
#ifdef __TINYC__
        return aiDstk;
#else
        if (aaeRstk == &aaReturnStack[0]) {
          iRes = *(aiDstk--);
#ifdef WITH_TRACE_LOGICUNIT
          vTrace("\nEnd Logic Unit :: sql %lld\n\n"_Stz,m_iSql);
#endif
          return iRes;
        }
        m_aeCode = * (--aaeRstk);
#endif
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // misc
      case EO_ASSERT: ArgNil(); // pop top and assert
        iTopA = *(aiDstk--);
        dLU1(EO_ASSERT,iTopA);
        RASSERT(iTopA);
        break;//+endCase
              
      case EO_BREAKPOINT: ArgNil(); // breakpoint
        dLU0(EO_BREAKPOINT);
        vDebugPoint();
        break;//+endCase
        
      case EO_SLEEP: ArgNil(); // sleep for top seconds
        iTopA = *(aiDstk--);
        dLU1(EO_SLEEP,iTopA);
        sleep((u4)iTopA);
        break;//+endCase
              
      case EO_SLOT: ArgNil(); // sleep for top seconds
        dLU1(EO_SLOT,m_i_Slot);
        *(++aiDstk) = m_i_Slot;
        break;//+endCase
              
      case EO_TRACEOFF: ArgNil(); // set trace
        m_bTrace = 0;
        break;//+endCase
        
      case EO_TRACEON: ArgNil(); // set trace
        m_bTrace = 1;
        break;//+endCase
        
      case EO_NOOP: ArgNil(); // no op
        dLU0(EO_NOOP);
        break;//+endCase

      case EO_ERROR: ArgNil(); // error marker
        dLU0(EO_ERROR);
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "Code compiled with error"_Stz);
        break;//+endCase
        

      //------------------------------------------------------------------------
      //------------------------------------------------------------------------

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // date operators
      case EO_DTE_NOW: ArgNil(); // push current date
      {
        Now oNow;
        Dtm wDte;

        wDte.m_wDte.m_uDay = oNow.m_wTsp.m_uDay;
        wDte.m_wDte.m_uMonth = oNow.m_wTsp.m_uMonth;
        wDte.m_wDte.m_uYearDelta = oNow.m_wTsp.m_uYearDelta;
        wDte.m_wDte.m_uSpare = 0;

        iRes = wDte.iAsI8();
        dLU1(EO_DTE_NOW,iRes);
        *(++aiDstk) = iRes;
      }
        break;//+endCase
        
      case EO_DTE_PRINT: ArgNil(); // pop top and print as date
#ifdef WITH_TRACE_LOGICUNIT
      {
        SP_Dte spDate;
        iTopA = *(aiDstk--);
        dLU1(EO_DTE_PRINT,iTopA);
        vTrace("[Q%lld.%lld] %s\n"_Stz,m_iSql,m_i_Slot,spDate.sz(iTopA));
      }
#endif
        break;//+endCase

      case EO_DTE_YEAR: ArgNil(); // pop top, push year of date
      {
        Dtm wDte;
        wDte = *(aiDstk--);
        iRes = wDte.m_wDte.m_uYearDelta + C_YEAR_BASE;
        dLU2(EO_DTE_YEAR,wDte.iAsI8(),iRes);
        *(++aiDstk) = iRes;
      }
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // hashing
      case EO_HSH_ADD_HASHI: ArgNil(); // add hash of top int to running hash
        iTopA = *(aiDstk--);
        iRes = HshLib::iHash((byte*)&iTopA,sizeof(w8));
        m_iHash += iRes;
        dLU3(EO_HSH_ADD_HASHI,iTopA,iRes,m_iHash);
        break;//+endCase
        
      case EO_HSH_RESET: ArgNil(); // reset hash
        dLU0(EO_HSH_RESET);
        m_iHash = 0;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // partitioning
      case EO_PTN_CALC: ArgNil(); // calculate m_i_Ptn using running m_iHash
        dLU0(EO_PTN_CALC);
        m_i_Ptn = PtnLib::iPartition(m_iHash,m_nPtns);
        break;//+endCase

      case EO_PTN_PUSH: ArgNil(); // push m_i_Ptn
        dLU1(EO_PTN_PUSH,m_i_Ptn);
        *(++aiDstk) = m_i_Ptn;
        break;//+endCase

      case EO_PTN_RESET: ArgNil(); // reset running m_iHash
        dLU0(EO_PTN_RESET);
        m_iHash = PtnLib::iResetRunning();
        break;//+endCase

      case EO_PTN_RUNNING: ArgNil(); // mix top into running m_iHash
        iTopA = *(aiDstk--);
        dLU0(EO_PTN_RUNNING);
        PtnLib::vRunning(&m_iHash,iTopA);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // timer
      case EO_TMR_START: ArgNil(); // timer start
        dLU0(EO_TMR_START);
        m_oTimer.vStart();
        break;//+endCase
        
      case EO_TMR_NSECS: ArgNil(); // push timer nanoseconds
        iRes = m_oTimer.iNanoseconds();
        dLU1(EO_TMR_NSECS,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_TMR_UPDATE: ArgNil(); // timer update
        dLU0(EO_TMR_UPDATE);
        m_oTimer.vUpdate();
        break;//+endCase
        
      case EO_TMR_AGG_ADD: ArgNil(); // add timer to agg timer
        dLU0(EO_TMR_AGG_ADD);
        m_oAggTimer.vAdd(&m_oTimer);
        break;//+endCase
        
      case EO_TMR_AGG_NSECS: ArgNil(); // push agg nanoseconds
        iRes = m_oAggTimer.iNanoseconds();
        dLU1(EO_TMR_AGG_NSECS,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_TMR_AGG_RESET: ArgNil(); // reset agg timer
        dLU0(EO_TMR_AGG_RESET);
        m_oTimer.vReset();
        break;//+endCase
          

      //------------------------------------------------------------------------
      //------------------------------------------------------------------------

        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // col operators
      case EO_COL_DST_RESET: ArgGet(iArg); // reset column in streamPix of dst
      {
        m_oStreamPix.vNotProject(iArg);
      }
        break;//+endCase
        
      case EO_COL_SRC_PUSH: ArgGet(iArg); // push column value from m_aSrcCrw
      {
        HV_Row hRow(m_pDevSrcRsb->m_pStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_pDevSrcIxt->m_aCrw,iArg);
        iRes = hRow->m_waColVal[iArg].m_iWrd;
        dLU2(EO_COL_SRC_PUSH,iArg,iRes);
        *(++aiDstk) = iRes;
      }
        break;//+endCase
        
      case EO_COL_SRC_PUSHSTR: ArgGet(iArg); // push column value from m_aSrcCrw
      {
        HV_Row hRow(m_pDevSrcRsb->m_pStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_pDevSrcIxt->m_aCrw,iArg);
        azArg = (char*)hRow->m_waColVal[iArg].m_iWrd;
        dLU1(EO_COL_SRC_PUSHSTR,iArg);
//        i8 nSize = StrLib::nSize((char*)iRes);
//        char* azDst = (char*)(++aiDstk);
//        StrLib::vCopy(azDst,(char*)iRes);
//        aiDstk += nSize / 8;
//        *(++aiDstk) = nSize;
        S_PushStr(azArg);
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_COL_ALT_PUSH: ArgGet(iArg); // push column value from m_aAltCrw
      {
        HV_Row hRow(m_pDevAltRsb->m_pStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_pDevAltIxt->m_aCrw,iArg);
        iRes = hRow->m_waColVal[iArg].m_iWrd;
        dLU2(EO_COL_ALT_PUSH,iArg,iRes);
        *(++aiDstk) = iRes;
      }
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // crw operators
      case EO_CRW_SRC_BEGIN: ArgNil(); // begin src rsb traverse
        dLU0(EO_CRW_SRC_BEGIN);
        m_pDevSrcIxt->m_i_Row = -1;
        m_pDevSrcIxt->m_aCrw = nullptr;
        m_pDevSrcIxt->m_nRows = m_pDevSrcIxt->m_pCrwBuffer ?
                         m_pDevSrcIxt->m_pCrwBuffer->nRowCount() : 0;
        break;//+endCase
        
      case EO_CRW_SRC_FETCH: ArgNil(); // fetch m_aSrcCrw using m_pDevSrcIxt->m_aEnt.ref
        dLU0(EO_CRW_SRC_FETCH);
        m_pDevSrcIxt->m_aCrw  =
          CrwBuffer::pRefdCrw(m_pDevSrcIxt->m_aEnt->rxRef());
        break;//+endCase
        
      case EO_CRW_ALT_FETCH: ArgNil(); // fetch m_aAltCrw using m_aAltEnt.ref
        dLU0(EO_CRW_ALT_FETCH);
        m_pDevAltIxt->m_aCrw =
          CrwBuffer::pRefdCrw(m_pDevAltIxt->m_aEnt->rxRef());
        break;//+endCase
       
      case EO_CRW_SRC_NEXT: ArgNil(); // move to next src rsb
        dLU0(EO_CRW_SRC_NEXT);
        if (++m_pDevSrcIxt->m_i_Row < m_pDevSrcIxt->m_nRows) {
          m_pDevSrcIxt->m_aCrw =
            m_pDevSrcIxt->m_pCrwBuffer->aCrw(m_pDevSrcIxt->m_i_Row);
          *(++aiDstk) = 1;
        }
        else {
          *(++aiDstk) = 0;
        }
        break;//+endCase
      
      case EO_CRW_SRC_TORSB:
        dLU0(EO_CRW_SRC_TORSB);
        RowInflator::vInflateColumns(m_pDevDstRsb->m_pRowSlab,m_pDevSrcIxt->m_aCrw);
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry
/**/  case EO_ENT_NEXT: ArgNil(); // move to next m_pEnt
        //iDev_ENT_NEXT(pRLU,m_pDevTmpIxt);
        dLU0(EO_ENT_NEXT);
        m_pDevTmpIxt->m_aEnt =
          EntBuffer::pMoveToNextEntry(m_pDevTmpIxt->m_aEnt,
                                      &m_pDevTmpIxt->m_pEntBuffer);
        if (m_pDevTmpIxt->m_aEnt)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;//+endCase
        
/**/  case EO_ENT_POSPRIOR: ArgNil(); // position m_pEnt prior using m_oEntWrk.key
        //vDev_ENT_POSPRIOR(pRLU,m_pDevTmpIxt);
        dLU0(EO_ENT_POSPRIOR);
        m_pDevTmpIxt->m_aEnt =
          m_pDevTmpIxt->m_pIdxTree->aPositionPriorEntry(m_oEntWrk.aKey(),
                                                        &m_pDevTmpIxt->m_pEntBuffer);
        break;//+endCase
        
/**/  case EO_ENT_PUSH: ArgNil(); // push entry on the stack
        //aDev_ENT_PUSH(pRLU,m_pDevTmpIxt);
        dLU0(EO_ENT_PUSH);
        *(++aiDstk) = (i8)m_pDevTmpIxt->m_aEnt;
        break;//+endCase
        
/**/  case EO_ENT_PUSHELMI0: ArgNil(); // push entry elm 0 int
        //iDev_ENT_PUSHELMI0(pRLU,m_pDevTmpIxt);
        dLU0(EO_ENT_PUSHELMI0);
        iRes = m_pDevTmpIxt->m_aEnt->aKey()->iGetIntElm();
        *(++aiDstk) = iRes;
        break;//+endCase
        
/**/  case EO_ENT_PUSHELMS0: ArgNil(); // push entry elm 0 str
      {
        //szDev_ENT_PUSHELMS0(pRLU,m_pDevTmpIxt);
        char acFlipped[128];
        dLU0(EO_ENT_PUSHELMS0);
        char* azSrc = (char*)m_pDevTmpIxt->m_aEnt->aKey()->szGetStrElm();
      
        StrLib::iWordFlipBack(acFlipped,azSrc);
      
//        i8 nSize = StrLib::nSize(acFlipped);
//        char* azDst = (char*)(++aiDstk);
//        StrLib::vCopy(azDst,acFlipped);
//        aiDstk++;
//        *(++aiDstk) = nSize;
        S_PushStr((char*)acFlipped);
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
/**/  case EO_ENT_PUSHOFS: ArgNil(); // push entry offset
        //iDev_ENT_PUSHOFS(pRLU,m_pDevTmpIxt);
        dLU0(EO_ENT_PUSHOFS);
        iRes = m_pDevTmpIxt->m_aEnt->iBuffer();
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_ENT_PUSHREF: ArgNil(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

/**/  case EO_ENT_SRC_PUSH: ArgNil(); // push entry on the stack
        //aDev_ENT_PUSH(pRLU,m_pDevSrcIxt);
        dLU0(EO_ENT_SRC_PUSH);
        *(++aiDstk) = (i8)m_pDevSrcIxt->m_aEnt;
        break;//+endCase
        
/**/  case EO_ENT_SRC_PUSHUPPER: ArgNil(); // push entry on the stack
        //aDev_ENT_PUSH(pRLU,m_pDevTmpIxt);
        dLU0(EO_ENT_SRC_PUSHUPPER);
        *(++aiDstk) = (i8)m_pDevSrcIxt->m_aUpperEnt;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry relational operators
/**/  case EO_ENT_LESS: ArgNil(); // test if m_pEnt.key <  m_oEntWrk.key
        //iDev_ENT_COMPARE(pRLU,m_pDevTmpIxt);
        iRes  = KC_BEFORE == Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_LESS,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
/**/  case EO_ENT_LESSEQUAL: ArgNil(); // test if m_pEnt.key <= m_oEntWrk.key
        //iDev_ENT_COMPARE(pRLU,m_pDevTmpIxt);
        iCmp = Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey());
        iRes  = (KC_SAME == iCmp || KC_BEFORE == iCmp ) ? 1 : 0;
        dLU1(EO_ENT_LESSEQUAL,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
/**/  case EO_ENT_EQUAL: ArgNil();
      {
        //iDev_ENT_COMPARE(pRLU,m_pDevTmpIxt);
#ifdef WITH_TRACE_LOGICUNIT
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,
               spEnt.sz(m_oEntWrk.aEnt(),m_pDevTmpIxt->m_pIdxTree->pIndexDef()));
        vTrace("\n\nEnt : %s\n"_Stz,spEnt.sz(m_pDevTmpIxt->m_aEnt,m_pDevTmpIxt->m_pIdxTree->pIndexDef()));
#endif
//        iRes  =
//          KC_SAME == Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),
//                                  m_oEntWrk.aKey())
//                       ? 1 : 0;
        iRes = KC_SAME == ::iDev_ENT_COMPARE(pRLU,m_pDevTmpIxt) ? 1 : 0;
        dLU1(EO_ENT_EQUAL,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
      }
        
/**/  case EO_ENT_NOTEQUAL: ArgNil();
        //iDev_ENT_COMPARE(pRLU,m_pDevTmpIxt);
        iRes  = KC_SAME != Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_NOTEQUAL,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
/**/  case EO_ENT_GREATEREQUAL: ArgNil();
        //iDev_ENT_COMPARE(pRLU,m_pDevTmpIxt);
        iCmp = Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey());
        iRes = (KC_SAME == iCmp || KC_AFTER == iCmp ) ? 1 : 0;
        dLU1(EO_ENT_GREATEREQUAL,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
/**/  case EO_ENT_GREATER: ArgNil();
        //iDev_ENT_COMPARE(pRLU,m_pDevTmpIxt);
        iRes  = KC_AFTER == Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_GREATER,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entWrk
/**/  case EO_ENTWRK_APPENDELM: ArgGet(iArg); // append next element value
      {
        //vDev_ENTWRK_APPENDELM(pRLU,m_pDevTmpIxt);
        dLU1(EO_ENTWRK_APPENDELM,iArg);
        m_oEntWrk.vAppendElement(m_pDevSrcRsb->m_aRsb, &m_pDevSrcRsb->m_pRowSlab->m_oStreamPix, iArg);
        
#ifdef WITH_TRACE_LOGICUNIT
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pDevTmpIxt->m_pIdxTree->pIndexDef()));
#endif
        break;//+endCase
      }
        
/**/  case EO_ENTWRK_APPENDELMI: ArgNil(); // append next element value (int)
        iTopA = *(aiDstk--);
        //vDev_ENTWRK_APPENDELMI(pRLU,m_pDevTmpIxt,iTopA);
        dLU1(EO_ENTWRK_APPENDELMI,iTopA);
        m_oEntWrk.vAppendElementInt(m_pDevSrcRsb->m_aRsb,iTopA);
        break;//+endCase
        
/**/  case EO_ENTWRK_APPENDELMS: ArgNil(); // append next element value (str)
      {
        dLU0(EO_ENTWRK_APPENDELMS);
//        iTopA = *(aiDstk--);
//        dLU1(eCode,iTopA);
//        Stz* szStr = (Stz*)(aiDstk-iTopA/8);
//        m_oEntWrk.vAppendElementStz(m_pDevSrcRsb->m_aRsb,szStr);
//        aiDstk -= (iTopA/8) + 1;
        S_UseStr(azSrc);
        m_oEntWrk.vAppendElementStz(m_pDevSrcRsb->m_aRsb,(Stz*)azSrc);
        S_UseEnd();
        //vDev_ENTWRK_APPENDELMS(pRLU,m_pDevTmpIxt,azSrc);
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_ENTWRK_CREATEPAY: ArgGet(iArg); // create a payload in entwrk
        dLU1(EO_ENTWRK_CREATEPAY,iArg);
        
        m_oEntWrk.vPayload(true);
        m_oEntWrk.vPayloadBytes(iArg);
        break;//+endCase
                
      case EO_ENTWRK_POPOFS: ArgNil(); // pop m_oEntWrk.ref.buffer/offset
        iTopA = *(aiDstk--);
        dLU1(EO_ENTWRK_POPOFS,iTopA);
        m_oEntWrk.vBuffer(iTopA);
        break;//+endCase

      case EO_ENTWRK_POPREF: ArgNil(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      case EO_ENTWRK_PRINT: ArgNil(); // print m_oEntWrk
#ifdef WITH_TRACE_LOGICUNIT
        aEnt = m_oEntWrk.aEnt();
        pIndexDef = m_xpIndexTree->pIndexDef();
        vTrace("EntWrk : %s\n"_Stz,spEnt.sz(aEnt,pIndexDef));
#endif
        break;//+endCase
        
      case EO_ENTWRK_RESET: ArgNil(); // reset m_oEntWrk
        dLU0(EO_ENTWRK_RESET);
        m_oEntWrk.vReset();
        break;//+endCase
                              
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // index tree operators
/**/  case EO_IXT_DEL_ENT: ArgNil(); // delete current m_aEnt from indexTree
        //vDev_IXT_DEL_ENT(pRLU,m_pDevTmpIxt);
        dLU0(EO_IXT_DEL_ENT);
        m_pDevTmpIxt->m_pIdxTree->vDelete(m_pDevTmpIxt->m_pEntBuffer,m_pDevTmpIxt->m_aEnt);
        break;//+endCase

/**/  case EO_IXT_DUMP: ArgGet(iArg); // dump index tree
        //vDev_IXT_DUMP(pRLU,m_pDevTmpIxt);
        //m_pDevTmpIxt->m_pIdxTree->vDump(false);
        ::iDEV_MAP(this,m_aaDev[iArg],DEV_MAP_DUMP,nullptr,nullptr,0);
        break;//+endCase

/**/  case EO_IXT_INS_ENT: ArgNil(); // insert an entry into the indexTree
      {
        //vDev_IXT_INS_ENT(pRLU,m_pDevTmpIxt);
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
        
/**/  case EO_IXT_TRV_BEGIN: ArgNil(); // begin linear traverse
        dLU0(EO_IXT_TRV_BEGIN);
        //iDev_IXT_TRV_BEGIN(pRLU,m_pDevTmpIxt);
        m_pDevTmpIxt->m_pEntBuffer = m_pDevTmpIxt->m_pIdxTree->pFirstH0Buffer();
        if (m_pDevTmpIxt->m_pEntBuffer) {
          m_pDevTmpIxt->m_aEnt = m_pDevTmpIxt->m_pEntBuffer->aLeadEntry();
          if (m_pDevTmpIxt->m_aEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;//+endCase
        
/**/ case EO_IXT_TRV_NEXT: ArgNil(); // traverse to next entry
        //iDev_IXT_TRV_NEXT(pRLU,m_pDevTmpIxt);
        dLU0(EO_IXT_TRV_NEXT);
        m_pDevTmpIxt->m_aEnt =
           EntBuffer::pMoveToNextEntry(m_pDevTmpIxt->m_aEnt,
                                       &m_pDevTmpIxt->m_pEntBuffer);
        if (m_pDevTmpIxt->m_aEnt)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;//+endCase
        
/**/ case EO_IXT_TMP_ENTRIES: ArgNil(); // push number of entries in tmp index
        //iDev_IXT_TMP_ENTRIES(pRLU,m_pDevTmpIxt);
        iRes = m_pDevTmpIxt->m_pIdxTree->iCountH0Entries();
        dLU1(EO_IXT_TMP_ENTRIES,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_IXT_SRC_BEGIN: ArgNil(); // begin linear traverse
        dLU0(EO_IXT_SRC_BEGIN);
        m_pDevSrcIxt->m_pEntBuffer = m_pDevSrcIxt->m_pIdxTree->pFirstH0Buffer();
        if (m_pDevSrcIxt->m_pEntBuffer) {
          m_pDevSrcIxt->m_aEnt = m_pDevSrcIxt->m_pEntBuffer->aLeadEntry();
          if (m_pDevSrcIxt->m_aEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;//+endCase
        
      case EO_IXT_SRC_LBOUND: ArgNil(); // position in buffer to entry before LBOUND
        dLU0(EO_IXT_TRV_LBOUND);
        //iDev_EO_IXT_TRV_LBOUND(pRLU,m_pDevTmpIxt);
        m_pDevSrcIxt->m_pEntBuffer = m_pDevSrcIxt->m_pIdxTree->pPositionH0Buffer(m_oEntWrk.aKey());
        if (m_pDevTmpIxt->m_pEntBuffer) {
          m_pDevTmpIxt->m_aEnt = m_pDevSrcIxt->m_pEntBuffer->aPositionPriorEntry(m_oEntWrk.aKey());
          if (m_pDevSrcIxt->m_aEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;//+endCase
        
      case EO_IXT_SRC_NEXT: ArgNil(); // move to next entry
        dLU0(EO_IXT_SRC_NEXT);
        m_pDevSrcIxt->m_aEnt = EntBuffer::pMoveToNextEntry(m_pDevSrcIxt->m_aEnt, &m_pDevSrcIxt->m_pEntBuffer);
        if (m_pDevSrcIxt->m_aEnt)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;//+endCase

      case EO_IXT_SRC_PRINT: ArgNil(); // print src ent
        pIndexDef = m_pDevSrcIxt->m_pIdxTree->pIndexDef();
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("SrcEnt : %s\n"_Stz,spEnt.sz(m_pDevSrcIxt->m_aEnt,pIndexDef));
#endif
        break;//+endCase

      case EO_IXT_SRC_UBOUND: ArgNil(); // position in buffer to entry for UBOUND
      {
        dLU0(EO_IXT_TRV_UBOUND);
        //iDev_EO_IXT_TRV_LBOUND(pRLU,m_pDevTmpIxt);
        EntBuffer* pUpperEntBuffer = m_pDevSrcIxt->m_pIdxTree->pPositionH0Buffer(m_oEntWrk.aKey());
        if (pUpperEntBuffer) {
          m_pDevSrcIxt->m_aUpperEnt = pUpperEntBuffer->aPositionPriorEntry(m_oEntWrk.aKey());
          if (m_pDevSrcIxt->m_aUpperEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
      }
        break;//+endCase
        
      case EO_IXT_ALT_BEGIN: ArgNil(); // begin linear traverse
        dLU0(EO_IXT_ALT_BEGIN);
        m_pDevAltIxt->m_pEntBuffer = m_pDevAltIxt->m_pIdxTree->pFirstH0Buffer();
        if (m_pDevAltIxt->m_pEntBuffer) {
          m_pDevAltIxt->m_aEnt = m_pDevAltIxt->m_pEntBuffer->aLeadEntry();
          if (m_pDevAltIxt->m_aEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        break;//+endCase

      case EO_IXT_ALT_NEXT: ArgNil();
        dLU0(EO_IXT_ALT_NEXT);
        m_pDevAltIxt->m_aEnt =
          EntBuffer::pMoveToNextEntry(m_pDevAltIxt->m_aEnt,
                                      &m_pDevAltIxt->m_pEntBuffer);
        if (m_pDevAltIxt->m_aEnt)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;//+endCase

      case EO_IXT_ALT_PRINT: ArgNil(); // print alt ent
        pIndexDef = m_pDevAltIxt->m_pIdxTree->pIndexDef();
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("AltEnt : %s\n"_Stz,spEnt.sz(m_pDevAltIxt->m_aEnt,pIndexDef));
#endif
        break;//+endCase
        
      case EO_IXT_JOI_MATCH: ArgNil(); // test for join match (src.key == alt.key)
        aSrcKey = m_pDevSrcIxt->m_aEnt->aKey();
        aAltKey = m_pDevAltIxt->m_aEnt->aKey();
        iRes = KC_SAME == Key::eCompare(aSrcKey,aAltKey) ? 1 : 0;
        dLU1(EO_IXT_JOI_MATCH,iRes);
        *(++aiDstk) = iRes;
        NEVER_REACH; // not used??
        break;//+endCase

      case EO_IXT_CMP_SRCALT: ArgNil(); // compare src.key to alt.key
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
        *(++aiDstk) = iRes;
        break;//+endCase

      case EO_IXT_CMP_WRKALT: ArgNil(); // compare wrk.key to alt.key
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
        *(++aiDstk) = iRes;
        break;//+endCase

      case EO_IXT_POS_ALTBYSRC: ArgNil(); // position alt ixt using src.key
        aSrcKey = m_pDevSrcIxt->m_aEnt->aKey();
        m_pDevAltIxt->m_aEnt =
          m_pDevAltIxt->m_pIdxTree->aPositionPriorEntry(aSrcKey,&m_pDevAltIxt->m_pEntBuffer);
        if (m_pDevAltIxt->m_pEntBuffer) {
          if (m_pDevAltIxt->m_aEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        dLU1(EO_IXT_POS_ALTBYSRC,iRes);
        break;//+endCase

      case EO_IXT_POS_ALTBYWRK: ArgNil(); // position alt ixt using entwrk.key
        aWrkKey = m_oEntWrk.aKey();
        m_pDevAltIxt->m_aEnt =
          m_pDevAltIxt->m_pIdxTree->aPositionPriorEntry(aWrkKey,&m_pDevAltIxt->m_pEntBuffer);
        if (m_pDevAltIxt->m_pEntBuffer) {
          if (m_pDevAltIxt->m_aEnt)
            *(++aiDstk) = 1;
          else
            *(++aiDstk) = 0;
        }
        else
          *(++aiDstk) = 0;
        dLU1(EO_IXT_POS_ALTBYWRK,iRes);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pay load
/**/  case EO_PAY_FETCH_COLI: ArgGet(iArg); // fetch a column from entry payload
        //iDev_PAY_FETCH_COLI(pRLU,m_pDevTmpIxti8, iArg);
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        iRes = *aPayload;
        dLU2(EO_PAY_FETCH_COLI,iArg,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
/**/  case EO_PAY_FETCH_COLS: ArgGet(iArg); // fetch a column from entry payload
      {
        //szDev_PAY_FETCH_COLS(pRLU,m_pDevTmpIxti8, iArg);
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        char* azSrc = (char*)aPayload;
        i8 nSize = StrLib::nSize(azSrc);
        ASSERT(nSize < 8);
        dLU2(EO_PAY_FETCH_COLS,iArg,(i8)azSrc);
//        char* azDst = (char*)(++aiDstk);
//        StrLib::vCopy(azDst,azSrc);
//        aiDstk++;
//        *(++aiDstk) = nSize;
        S_PushStr(azSrc);
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
/**/  case EO_PAY_STORE_COLI: ArgGet(iArg); // store a column into entry payload
        iTopA = *(aiDstk--);
        //vDev_PAY_STORE_COLI(pRLU,m_pDevTmpIxt,iArg,iRes);
        dLU2(EO_PAY_STORE_COLI,iArg,iRes);
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        *aPayload = iTopA;
        break;//+endCase
      
/**/  case EO_PAY_STORE_COLS: ArgGet(iArg); // store a column into entry payload
      {
        dLU1(EO_PAY_STORE_COLI,iArg);
//        i8 nSize = *(aiDstk--);
//        RASSERT(nSize < 8);
//        dLU2(eCode,iArg,nSize);
//        aPayload = m_pEnt->aiPayCell(iArg);
//        char* azSrc = (char*)(--aiDstk);
//        aiDstk--;
//        StrLib::vCopy((char*)aPayload,azSrc);
        //vDev_PAY_STORE_COLS(pRLU,m_pDevTmpIxt,iArg,iRes);
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        S_PopStr((char*)aPayload);
        vDebugPoint();
      }
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pix
      case EO_PIX_STM_PRINT: ArgNil(); // print stream pix
        dLU0(EO_PIX_STM_PRINT);
        m_oStreamPix.vPrint();
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb / rowslab
      case EO_RSB_DST_COPYSRC: ArgNil(); // copy src rsb to dst rsb
        dLU0(EO_RSB_DST_COPYSRC);
        m_pDevDstRsb->m_pRowSlab->vPositionOnCell(0);
        m_pDevDstRsb->m_aRsb = m_pDevDstRsb->m_pRowSlab->aRsb();
        m_pDevDstRsb->m_pRowSlab->vCopyRsb(m_pDevSrcRsb->m_aRsb,bSrcHasPtnHash,bCopyPtnHash);
        break;//+endCase

      case EO_RSB_DST_PRINT: ArgNil(); // print dst rsb
        dLU0(EO_RSB_DST_PRINT);
        m_pDevDstRsb->m_pRowSlab->vDumpRow(-1,m_pDevDstRsb->m_aRsb);
        break;//+endCase

      case EO_RSB_SRC_PRINT: ArgNil(); // print src rsb
        dLU0(EO_RSB_SRC_PRINT);
        m_pDevSrcRsb->m_pRowSlab->vDumpRow(-1,m_pDevSrcRsb->m_aRsb);
        break;//+endCase

      case EO_RSB_ALT_PRINT: ArgNil(); // print alt rsb
        dLU0(EO_RSB_ALT_PRINT);
        m_pDevAltRsb->m_pRowSlab->vDumpRow(-1,m_pDevAltRsb->m_aRsb);
        break;//+endCase

      case EO_RSB_DST_NEW: ArgNil(); // create new destination rsb
        dLU0(EO_RSB_DST_NEW);
        //ASSERT(0 == m_i_Ptn);
        m_pDevDstRsb->m_aRsb = m_xpPot->aNewDstRsb(m_i_Ptn,&m_pDevDstRsb->m_pRowSlab);
        break;//+endCase

      case EO_RSB_DST_RESET: ArgNil(); // reset dst rowslab
        dLU0(EO_RSB_DST_RESET);
        m_pDevDstRsb->m_pRowSlab->vReset();
        break;//+endCase
        
      case EO_RSB_DST_TEST: ArgNil(); // test if there is a destination rsb
        dLU0(EO_RSB_DST_TEST);
        if (m_pDevDstRsb->m_aRsb)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;//+endCase
        
      case EO_RSB_DST_ZERO: ArgNil(); // zero destination rsb
        dLU0(EO_RSB_DST_ZERO);
        m_pDevDstRsb->m_pRowSlab->vZeroRsb(m_pDevDstRsb->m_aRsb);
        break;//+endCase
        
      case EO_RSB_SRC_BEGIN: ArgNil(); // begin src rsb traverse
        dLU0(EO_RSB_SRC_BEGIN);
        m_pDevSrcRsb->m_i_Row = -1;
        m_pDevSrcRsb->m_aRsb  = nullptr;
        m_pDevSrcRsb->m_nRows = m_pDevSrcRsb->m_pRowSlab ? m_pDevSrcRsb->m_pRowSlab->nRows() : 0;
        break;//+endCase

      case EO_RSB_SRC_NEXT: ArgNil(); // move to next src rsb
        dLU0(EO_RSB_SRC_NEXT);
        if (++m_pDevSrcRsb->m_i_Row  < m_pDevSrcRsb->m_nRows) {
          m_pDevSrcRsb->m_aRsb = m_pDevSrcRsb->m_pRowSlab->aRsbAdr(m_pDevSrcRsb->m_i_Row );
          *(++aiDstk) = 1;
        }
        else {
          *(++aiDstk) = 0;
        }
        break;//+endCase

      case EO_RSB_SRC_PUSHOFS: ArgNil();
        dLU0(EO_RSB_SRC_PUSHOFS);
        iRes = m_pDevSrcRsb->m_pRowSlab->iOfsOfRsb(m_pDevSrcRsb->m_aRsb);
        *(++aiDstk) = iRes;
        break;//+endCase

      case EO_RSB_SRC_SETBYOFS: ArgNil();  // set src rsb by using offset on stack
        dLU0(EO_RSB_SRC_SETBYOFS);
        iTopA = *(aiDstk--);
        m_pDevSrcRsb->m_aRsb = m_pDevSrcRsb->m_pRowSlab->aRsbByOfs(iTopA);
        break;//+endCase

      case EO_RSB_ALT_BEGIN: ArgNil(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      case EO_RSB_ALT_NEXT: ArgNil(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      case EO_RSB_ALT_PUSHOFS: ArgNil(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      case EO_RSB_ALT_SETBYOFS: ArgNil(); // not yet implemented
        NEVER_REACH;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb operators
      case EO_RSB_DST_POPCOLI: ArgGet(iArg); // pop and deposit top into dst column
        iRes = *(aiDstk--);
        dLU2(EO_RSB_DST_POPCOLI,iArg,iRes);
        m_pDevDstRsb->m_pRowSlab->vPositionOnCell(iArg);
        m_pDevDstRsb->m_pRowSlab->vDeposit(iRes);
        break;//+endCase
        
      case EO_RSB_DST_POPCOLS: ArgGet(iArg); // pop and deposit top into dst column
      {
        dLU1(EO_RSB_DST_POPCOLS,iArg);
//        i8 nSize = *(aiDstk--);
//        dLU2(eCode,iArg,nSize);
//        char* azSrc = (char*)(--aiDstk);
        m_pDevDstRsb->m_pRowSlab->vPositionOnCell(iArg);
        char* azDst = m_pDevDstRsb->m_pRowSlab->azDepositString_begin();
//        StrLib::vCopy(azDst,azSrc);
        S_PopStr(azDst);
        m_pDevDstRsb->m_pRowSlab->vDepositString_end(nSize);
//        aiDstk--;
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_RSB_DST_PUSHCOLI: ArgGet(iArg); // push dst column
        m_pDevDstRsb->m_aRsb = m_pDevDstRsb->m_pRowSlab->aRsbAdr(m_pDevDstRsb->m_pRowSlab->nRows()-1);
        i_Cell_ntv = m_pDevDstRsb->m_aRsb->i_ColToCell_ntv(iArg);
        iRes = m_pDevDstRsb->m_aRsb->iVal_ntv(i_Cell_ntv);
        dLU2(EO_RSB_DST_PUSHCOLI,iArg,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_RSB_SRC_PUSHCOLI: ArgGet(iArg); // push src column
        iRes =
          m_pDevSrcRsb->m_aRsb->iVal_ntv(m_pDevSrcRsb->m_aRsb->i_ColToCell_ntv(iArg));
        dLU2(EO_RSB_SRC_PUSHCOLI,iArg,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_RSB_SRC_PUSHCOLS: ArgGet(iArg); // push src column
      {
        char* azSrc = (char*)m_pDevSrcRsb->m_aRsb->szVal_ntv(m_pDevSrcRsb->m_aRsb->i_ColToCell_ntv(iArg));
        dLU2(EO_RSB_SRC_PUSHCOLS,iArg,(i8)azSrc);
//        i8 nSize = StrLib::nSize(azSrc);
//        char* azDst = (char*)(++aiDstk);
//        StrLib::vCopy(azDst,azSrc);
//        aiDstk++;
//        *(++aiDstk) = nSize;
        S_PushStr(azSrc);
        if (m_bTrace)
          vDebugPoint();
      }
        break;//+endCase
        
      case EO_RSB_ALT_PUSHCOLI: ArgGet(iArg); // push alt column
        iRes = m_pDevAltRsb->m_aRsb->iVal_ntv(m_pDevAltRsb->m_aRsb->i_ColToCell_ntv(iArg));
        dLU2(EO_RSB_ALT_PUSHCOLI,iArg,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      default:
        vTrace("Unknown eo opcode %lld\n"_Stz,(i8)eCode);
        return C_DATUM_NULL;
    } //+ExtractEnd
    
  }
  
  return iaDataStack[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
