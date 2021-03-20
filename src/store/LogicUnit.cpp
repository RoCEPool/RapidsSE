/*============================================================================*/
/* LogicUnit.cpp -- Logic Unit                                                */
/*============================================================================*/

#include "BASE.h"

#include "eED.h"
#include "eEO.h"
#include "ePX.h"

#include "RowSingle.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"

#include "IndexDef.hpp"

#include "RowInflator.hpp"

#include "EnumGroups.h"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"

#include "Node.hpp"
#include "Node_I.hpp"
#include "OpSpec.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "LIB_interface.h"
#include "DEV_interface.h"

#include "SP_Dte.hpp"
#include "SP_Ent.hpp"

#include "SN_Dte.hpp"

extern EoArg EoArgs[];
Atomic gixQueryNo;

extern void* C_ADR_REGION;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,LogicUnit,CN_LOGICUNIT)
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

  m_iDebugFlag = 0;

  m_xpNow = new Now;
  
  m_iCounter0 = 0;
  m_iCounter1 = 0;
  m_iCounter2 = 0;
  
  m_sStringA  = ""_Stz;

  m_xRluCommon.m_iCtr0 = 0;

  for (i8 i_Local = 0; i_Local < C_LOGIC_LOCALS; i_Local++)
    m_xRluCommon.m_iaLocals[i_Local] = C_UNKNOWN;

   for (i8 i_Cond = 0; i_Cond < C_LOGIC_COND; i_Cond++)
     m_baCondInd[i_Cond] = false;
  
  m_iNextCond = C_UNKNOWN;
  m_bSuperFrag = false;
  m_aLowerEntry = nullptr;
  m_aUpperEntry = nullptr;
  m_pEntBufferOuter = nullptr;

  m_eOperand = ZK_NONE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_afDynFun = nullptr;
  
  m_pTknVec = new TokenVec;
  m_pTkn    = nullptr;
  m_i_Tkn   = C_UNKNOWN;
  
  m_pRluDiary = nullptr;
  
  MemLib::vZero(&m_xRluCommon,sizeof(m_xRluCommon),CN_LOGICUNIT);
  m_xRluCommon.m_iCmnMagic  = 123456789;
  m_xRluCommon.m_iCmnFooter = 987654321;
  
  m_xRluCommon.m_nAltHeadSize = C_CRW_HEADBYTES;
  m_xRluCommon.m_nSrcHeadSize = C_CRW_HEADBYTES;
  m_xRluCommon.m_i_ColSet = 0;

  m_oSince.vStart();
  
  // m_oEntWrk

  m_i_Code = 0;
  
  m_pDevDstRow = new DevROW;
  m_pDevSrcRow = new DevROW;
  m_pDevAltRow = new DevROW;

  m_pDevSrcIxt = new DevIXT;
  m_pDevAltIxt = new DevIXT;
  m_pDevTmpIxt = new DevIXT;

  m_pLogicTimer = nullptr;

  m_xRluCommon.m_pDevDstRow = m_pDevDstRow;
  
  m_xRluCommon.m_pDevSrcRow = m_pDevSrcRow;
  m_xRluCommon.m_pDevSrcIxt = m_pDevSrcIxt;
  m_xRluCommon.m_pDevAltRow = m_pDevAltRow;
  m_xRluCommon.m_pDevAltIxt = m_pDevAltIxt;
  m_xRluCommon.m_pDevTmpIxt = m_pDevTmpIxt;
  
  m_xRluCommon.m_aWrkEnt = (Ent_int*)m_oEntWrk.aEnt();
  m_xRluCommon.m_pEntWrk = (void*)&m_oEntWrk;

  m_xRluCommon.m_pRowPix = (void*)&m_oRowPix;

  m_xRluCommon.m_i_TablePtn = C_UNKNOWN;
  m_xRluCommon.m_nPtns  = AM::nPtnsInGrid();
  m_xRluCommon.m_iHash  = 0;

  m_xRluCommon.m_pPot = nullptr;
  m_xRluCommon.m_pIndexDef = nullptr;
  m_xRluCommon.m_pIndexTree = nullptr;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_eaCode,sizeof(m_eaCode),CN_LOGICUNIT);
#endif
  
  m_nLabels = 0;
  for (i8 i_Label = 0; i_Label < C_LOGIC_LABEL; i_Label++) {
    m_xaJumpLabel[i_Label].m_bDefined = false;
    m_xaJumpLabel[i_Label].m_bSubroutine = false;
    m_xaJumpLabel[i_Label].m_bDestination = false;
    m_xaJumpLabel[i_Label].m_i_Chain = C_UNKNOWN;
  }

  // m_ixRowsProcessed;
  // m_ixRowsInvalid;
  // m_ixRowsSkipped;
  m_eErrorCode = EC_NONE;
  
  _checkInitialised_(CN_LOGICUNIT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LogicUnit::~LogicUnit(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  
  if (LH_HAVE_READ_LOCK == m_eDdlLocked) {
    
    AM::pDdlLock()->vDropRead();
    m_eDdlLocked = LH_HAVE_NO_LOCK;
  }
  else
  if (LH_HAVE_WRITE_LOCK == m_eDdlLocked) {
    
    AM::pDdlLock()->vDropWrite();
    m_eDdlLocked = LH_HAVE_NO_LOCK;
  }

  RASSERT(LH_HAVE_NO_LOCK == m_eDdlLocked);
  
  if (! m_xpAggTimer.bIsNull())
    delete *m_xpAggTimer;
  
  if (! m_xpNow.bIsNull())
    delete *m_xpNow;
  
  if (! m_xpTimer.bIsNull())
    delete *m_xpTimer;
  
  if (m_pRluDiary) {
    vDiaryReport();
    delete m_pRluDiary;
  }
  
  delete m_pTknVec;
  
  delete m_pDevDstRow;
  delete m_pDevSrcRow;
  delete m_pDevAltRow;

  delete m_pDevTmpIxt;
  delete m_pDevAltIxt;
  delete m_pDevSrcIxt;
  
  delete m_pLogicTimer;
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
/* conditional code                                                           */
/*----------------------------------------------------------------------------*/
void LogicUnit::vCC(i8 i_Cond)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (m_baCondInd[i_Cond])
    m_iNextCond = 1;
  else
    m_iNextCond = 0;
}

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
eEO* LogicUnit::aCodeArg(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  eEO* aArg = &m_eaCode[i_Code];

  return aArg;
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
/* get diary                                                                  */
/*----------------------------------------------------------------------------*/
RluDiary* LogicUnit::pDiary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  return m_pRluDiary;
}

/*----------------------------------------------------------------------------*/
/* set diary name                                                             */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryName(Stz *szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vDiaryNm(szName);
}

/*----------------------------------------------------------------------------*/
/* set diary job note                                                         */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryJobNote(Stz *szJobNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vJobNote(szJobNote);
}

/*----------------------------------------------------------------------------*/
/* set diary dir                                                              */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryDir(Stz *szDir)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vDiaryDir(szDir);
}

/*----------------------------------------------------------------------------*/
/* add diary note                                                             */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryNote(i8 i_EnumGroup, i8 i_Enum, i8 iItem, i8 nNanoSecs,
                           i2 iAction, i2 i_ActEnumGroup, i2 i_ActEnum,
                           i8 nQueueLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vAddNote(m_xRluCommon.m_iSql,m_xRluCommon.m_i_TablePtn,
                        nQueueLength,
                        
                        i_EnumGroup,i_Enum,iItem,nNanoSecs,
                        iAction,i_ActEnumGroup,i_ActEnum);
}

/*----------------------------------------------------------------------------*/
/* set diary title                                                            */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryTitle(Stz *szTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vTitle(szTitle);
}

/*----------------------------------------------------------------------------*/
/* call dynamic function compiled by LogicCompiler                            */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDynFun(i8 nOuterArgs, i8 iOuterArg0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iRes = m_afDynFun(this,nOuterArgs,iOuterArg0);

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* optimise with super fragent                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vEndOptimise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  m_bSuperFrag = false;
}

/*----------------------------------------------------------------------------*/
/* diarise dynamic routine entry                                              */
/*----------------------------------------------------------------------------*/
void LogicUnit::vEnter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if ( AM::bRLUDiary() ) {
    m_oSince.vUpdate();
    i8 nNanoSeconds = m_oSince.iNanoseconds();
  
  //  vTrace("Enter (%lld.%lld) %s %lld %lld\n"_Stz,
  //         m_xRluCommon.m_iSql,
  //         m_xRluCommon.m_i_Ptn,
  //
  //         EnumGroups::szEnumText(EG_ED,ED_ENTER),
  //
  //         m_xRluCommon.m_nEnterCount,
  //         nNanoSeconds);
  
    vDiaryNote(EG_ED,ED_ENTER,
                m_xRluCommon.m_nEnterCount,
                nNanoSeconds,
                0,0,0,-1);
  }
}

/*----------------------------------------------------------------------------*/
/* diarise dynamic routine exit                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if ( AM::bRLUDiary() ) {
    m_oSince.vUpdate();
    i8 nNanoSeconds = m_oSince.iNanoseconds();
    
  //  vTrace("Exit (%lld.%lld) %s %lld %lld\n"_Stz,
  //         m_xRluCommon.m_iSql,
  //         m_xRluCommon.m_i_Ptn,
  //
  //         EnumGroups::szEnumText(EG_ED,ED_EXIT),
  //
  //         m_xRluCommon.m_nEnterCount,
  //         nNanoSeconds);

    vDiaryNote(EG_ED,ED_EXIT,
               m_xRluCommon.m_nEnterCount,
               nNanoSeconds,
               0,0,0,-1);
  }
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
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogicUnit::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_xRluCommon.m_i_TablePtn = AM::pTaskJob()->m_pOp->tPtnNo();
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
/* is given label a destination?                                              */
/*----------------------------------------------------------------------------*/
bool LogicUnit::bLabelIsDst(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  bool bIsDst = m_xaJumpLabel[i_Label].m_bDestination;
  
  return bIsDst;
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
  
  return m_nLabels;
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
/* optimise with super fragent                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vOptimise(eEO eOpt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (EO_NONE == eOpt) {
    m_bSuperFrag = false;
    return;
  }
  
  vAddCode(eOpt);
  
  m_bSuperFrag = true;
}

//*----------------------------------------------------------------------------*/
/* return query fragment number                                               */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iQueryFragment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return m_xRluCommon.m_iSql;
}

/*----------------------------------------------------------------------------*/
/* set conditional compilation state                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vSetCC(i8 i_Cond, bool bState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if (bState)
    m_baCondInd[i_Cond] = 1;
  else
    m_baCondInd[i_Cond] = 0;
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
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  m_eaCode[m_i_Code++] = eCode;

  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with argument                                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeArg(eEO eCode, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(8 == EoArgs[eCode].m_nArgBytes);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  m_eaCode[m_i_Code++] = eCode;

  i8* piArg = (i8*) & m_eaCode[m_i_Code];
  * piArg = iArg;
  
  m_i_Code += sizeof(i8);
  
  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with argument                                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeArg(eEO eCode,Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  i8 iArg = pTkn->iToI8();
  
  vAddCodeArg(eCode,iArg);

  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with date argument                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeArg(eEO eCode, Stz* szLiteral)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(EA_DATE == EoArgs[eCode].m_eArgument);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  char* acCrs = szLiteral->azChar();
  
  SN_Dte snDate;
  i8 iDate = snDate.i(&acCrs,AM::pFmt());
  
  vAddCodeArg(eCode,iDate);

  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with named label                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeLabel(eEO eCode, Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
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
  else if (eCode >= EO_JMP && eCode <= EO_JMP_GREATER)
    m_xaJumpLabel[i_Label].m_bDestination = true;

  //vPrintLabels();
  
  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with label name in given token                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeLabel(eEO eCode, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  vAddCodeLabel(eCode,pTkn->szChars());
  
  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with string literal                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeString(eEO eCode, Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(-1 == EoArgs[eCode].m_nArgBytes);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  m_eaCode[m_i_Code++] = eCode;

  for (char* acChr = szString->azChar(); *acChr; acChr++)
    m_eaCode[m_i_Code++] = (eEO)*acChr;
  m_eaCode[m_i_Code++] = (eEO)0;
  
  m_iNextCond = C_UNKNOWN;
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
  
  if (m_bSuperFrag)
    return;
  
  // has this label been used yet?
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label)
    i_Label = iAddLabel(szLabelNm,true);
  else
    vCorrectLabel(i_Label);
  
  //vPrintLabels();
}

/*----------------------------------------------------------------------------*/
/* diary report                                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  s256 sDiaryPath;
  
  if (0 != m_xRluCommon.m_iSql)
    sDiaryPath.vSprint("%s/RluDiary_%s_%lld_%lld.csv"_Stz,
                       m_pRluDiary->szDiaryDir(),
                       m_pRluDiary->szDiaryNm(),
                       m_xRluCommon.m_iSql,
                       m_xRluCommon.m_i_TablePtn
                       );
  else
    sDiaryPath.vSprint("%s/RluDiary_%s.csv"_Stz,
                       m_pRluDiary->szDiaryDir(),
                       m_pRluDiary->szDiaryNm()
                       );

  m_pRluDiary->vReportDiary(sDiaryPath);
  
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
    vTrace("  [%02lld] %s %lld: %4lld%s%s\n"_Stz,
           i_Label,
           m_xaJumpLabel[i_Label].m_sLabelNm.szStz(),
           (i8)m_xaJumpLabel[i_Label].m_bDefined,
           m_xaJumpLabel[i_Label].m_i_Chain,
           m_xaJumpLabel[i_Label].m_bSubroutine ? " subroutine" : "",
           m_xaJumpLabel[i_Label].m_bDestination ? " destination" : ""
           );
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

  return;
  
  SP_Dte spDate;

  vTrace("EO code for %lld:\n"_Stz,m_xRluCommon.m_iSql);
  
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

/*----------------------------------------------------------------------------*/
/* initialise and return address of rlu common                                */
/*----------------------------------------------------------------------------*/
struct RluCommon* LogicUnit::aRlu_INIT(void* aLU)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  UNUSED(aLU);
  
  return & m_xRluCommon;
}

/*----------------------------------------------------------------------------*/
/* dump the rlu Dstk                                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vRlu_DUMP(void* aLU, i8* aiDstk)
{
  vDebugPoint();
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  i8* aiDstkBase = pLogicUnit->m_xRluCommon.m_iaDataStack;
  i8  nUsageBytes = nDifference(pLogicUnit->m_xRluCommon.m_iaDataStack,
                                aiDstk)
                     + 8;
  
  vTrace("\nRlu_Dump: base %llx ; aiDstk %llx ; %lld bytes usage\n"_Stz,
      
         aiDstkBase,
         aiDstk,
         nUsageBytes
         );
  
  MemLib::vHexDump(aiDstkBase,nUsageBytes);
  
  vTrace("\n"_Stz);
}

/*============================================================================*/
/* - - - - - - - - - - - - - RELATIONAL LOGIC UNIT - - - - - - - - - - - - - -*/
/*============================================================================*/

#if 1

  #define dLU3(eCode,iA,iB,iC) \
    if (aRluCommon->m_bTrace) \
      vTrace("%lld : %12s %lld %lld %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA,iB,iC);

  #define dLU2(eCode,iA,iB) \
    if (aRluCommon->m_bTrace) \
      vTrace("%lld : %12s %lld %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA,iB);

  #define dLU1(eCode,iA) \
    if (aRluCommon->m_bTrace) \
      vTrace("%lld : %12s %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA);

  #define dLU0(eCode) \
    if (aRluCommon->m_bTrace) \
      vTrace("%lld : %12s\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode]);

  #define ArgGet(iArg) \
    ASSERT(8 == EoArgs[eCode].m_nArgBytes); \
    iArg = * (i8*)aeCode; \
    aeCode += 8;

  #define GetStr(iArg) \
    ASSERT(-1 == EoArgs[eCode].m_nArgBytes); \
    iArg = (i8)aeCode; \
    while (*(char*)aeCode) aeCode++; \
    aeCode++;

  #define ArgNil() \
    ASSERT(0 == EoArgs[eCode].m_nArgBytes);

#else

  #define dLU3(eCode,iA,iB,iC)
  #define dLU2(eCode,iA,iB)
  #define dLU1(eCode,iA)
  #define dLU0(eCode)

  #define ArgGet(iArg) \
    iArg = * (i8*)aeCode; \
    aeCode += 8;

  #define GetStr(iArg) \
    iArg = (i8)aeCode; \
    while (*(char*)aeCode) aeCode++; \
    aeCode++;

  #define ArgNil()

#endif

i8 giaVectorSet[] =
{
/* 00 */ L_QUANTITY,
/* 01 */ L_EXTENDEDPRICE,
/* 02 */ L_DISCOUNT,
/* 03 */ L_TAX,
/* 04 */ L_RETURNFLAG,
/* 05 */ L_LINESTATUS,
/* 06 */ L_SHIPDATE,
/* 07 */ C_UNKNOWN,
  
/* 08 */ Q1_LOCAL_RETURNFLAG,
/* 09 */ Q1_LOCAL_LINESTATUS,
/* 10 */ C_UNKNOWN,
  
/* 11 */ Q101_RETURNFLAG,
/* 12 */ Q101_LINESTATUS,
/* 13 */ C_UNKNOWN,
};

/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iCompute(i8 nOuterArgs, i8 iOuterArg0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  #ifdef WITH_TRACE_LOGICUNIT
    vTrace("\nLogic Unit :: sql %lld\n\n"_Stz,m_xRluCommon.m_iSql);
    vDebugPoint();
  #endif

  i8 iArg;
  i8 iTopA;
  i8 iTopB;
  i8 iTopC;

  char* azTopA;
  char* azTopB;
  Stz* szTopA;
  Stz* szTopB;

  i8 iRes;
  Stz* szRes;

  i8 iCmp;

  i8 nSize;
  char* azDst;
  char* azSrc;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // this code equates to aiDynFun() preliminary
  
  LogicUnit* pLU = this;
  
  struct RluCommon* aRluCommon = aRlu_INIT(pLU); // temp... should be nullptr
  ASSERT(123456789 == aRluCommon->m_iCmnMagic);
  ASSERT(987654321 == aRluCommon->m_iCmnFooter);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // temp / incorrect place
  
  i8* aiDstk = aRluCommon->m_iaDataStack;
  *aiDstk = 0xFFFFFFFFFFFFFFFF;

  i8* aiLocals = aRluCommon->m_iaLocals;
  i8* aiPayload;

  if (nOuterArgs) {
    *(++aiDstk) = iOuterArg0;
  }
  
  aRluCommon->m_i_TablePtn = AM::pTaskJob()->m_pOp->tPtnNo();
  aRluCommon->m_nPtns  = AM::nPtnsInGrid();
  aRluCommon->m_iHash  = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // required for interpretative inner loop
  
  eEO*  eaReturnStack[C_LOGIC_STACK];
  eEO** aeaRstk = &eaReturnStack[0];
  eEO*  aeCode  = & m_eaCode[0];

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
  //                    interpretative inner loop proper                      //
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//

  for (;;) {
    
#ifdef WITH_TRACE_LOGICUNIT
    // check for stack growth
    if (aiDstk >= &aRluCommon->m_iaDataStack[C_LOGIC_STACK-4])
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "LogicUnit data stack exhausted\n"_Stz);

    i8 nStkUse = nDifference(aiDstk,aRluCommon->m_iaDataStack);
    i8 iStkPct = (nStkUse * 100) / C_LOGIC_STACK;
    if (iStkPct > 80)
      vTrace("WARNING: LogicUnit stack use %lld%%\n"_Stz,iStkPct);
#endif
    
    eEO eCode = *aeCode++;
#ifdef WITH_TRACE_LOGICUNIT
    vTrace("%s %lld\n"_Stz, aEO[eCode], nStkUse);
#endif
    
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
        aRluCommon->m_iCtr0--;
        dLU1(EO_CTR_0_DEC,aRluCommon->m_iCtr0);
        break;//+endCase

      case EO_CTR_0_INC: ArgNil(); // inc counter 0
        aRluCommon->m_iCtr0++;
        dLU1(EO_CTR_0_INC,aRluCommon->m_iCtr0);
        break;//+endCase

      case EO_CTR_0_POP: ArgNil(); // pop counter 0
        aRluCommon->m_iCtr0 = *(aiDstk--);
        dLU1(EO_CTR_0_POP,aRluCommon->m_iCtr0);
        break;//+endCase

      case EO_CTR_0_PUSH: ArgNil(); // push counter 0
        dLU1(EO_CTR_0_PUSH,aRluCommon->m_iCtr0);
        *(++aiDstk) = aRluCommon->m_iCtr0;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // jump operators
      case EO_JMP: ArgGet(iArg); // jump to location
        dLU1(EO_JMP,iArg);
        JumpTo(iArg,EO_JMP); // aeCode = & m_eaCode[iArg];
        break;//+endCase
        
      case EO_JMP_FALSE: ArgGet(iArg); // pop top and jump to location if false
        dLU1(EO_JMP_FALSE,iArg);
        if (! *(aiDstk--))
          JumpTo(iArg,EO_JMP_FALSE); // aeCode = & m_eaCode[iArg];
        break;//+endCase
        
      case EO_JMP_TRUE: ArgGet(iArg); // pop top and jump to location if true
        dLU1(EO_JMP_TRUE,iArg);
        iRes = *(aiDstk);
        if (*(aiDstk--))
          JumpTo(iArg,EO_JMP_TRUE); // aeCode = & m_eaCode[iArg];
        break;//+endCase
        
      case EO_JMP_LESS: ArgGet(iArg); // pop top and jump to location if <
        dLU1(EO_JMP_LESS,iArg);
        if (*(aiDstk--) < 0)
          JumpTo(iArg,EO_JMP_LESS); // aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_LESSEQUAL: ArgGet(iArg); // pop top and jump to location if <=
        dLU1(EO_JMP_LESSEQUAL,iArg);
        if (*(aiDstk--) <= 0)
          JumpTo(iArg,EO_JMP_LESSEQUAL); // aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_EQUAL: ArgGet(iArg); // pop top and jump to location if ==
        dLU1(EO_JMP_EQUAL,iArg);
        if (*(aiDstk--) == 0)
          JumpTo(iArg,EO_JMP_EQUAL); // aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_NOTEQUAL: ArgGet(iArg); // pop top and jump to location if !=
        dLU1(EO_JMP_NOTEQUAL,iArg);
        if (*(aiDstk--) != 0)
          JumpTo(iArg,EO_JMP_NOTEQUAL); // aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_GREATEREQUAL: ArgGet(iArg); // pop top and jump to location if >=
        dLU1(EO_JMP_GREATEREQUAL,iArg);
        if (*(aiDstk--) >= 0)
          JumpTo(iArg,EO_JMP_GREATEREQUAL); // aeCode = & m_eaCode[iArg];
        break;//+endCase

      case EO_JMP_GREATER: ArgGet(iArg); // pop top and jump to location if >
        dLU1(EO_JMP_GREATER,iArg);
        if (*(aiDstk--) > 0)
          JumpTo(iArg,EO_JMP_GREATER); // aeCode = & m_eaCode[iArg];
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

      case EO_LGL_XOR: ArgNil(); // logical xor top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  =  (!iTopA && iTopB) || (iTopA && !iTopB) ? 1 : 0;
        dLU3(EO_LGL_XOR,iTopA,iTopB,iRes);
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
      // pot
      case EO_POT_USE_STREAMDEFOUT: ArgGet(iArg); // use streamDefOut in the pot locals
        dLU0(EO_POT_USE_STREAMDEFOUT);
        iTopA = *(aiDstk--);
        vDev_POT_USE_STREAMDEFOUT(pLU,iArg);
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // prints
      case EO_PRINT: ArgNil(); // print top
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(aiDstk--);
        dLU1(EO_PRINT,iTopA);

        printf("[Q%lld.%lld] %lld\n",(i8)0,(i8)0,iTopA);
#endif
        break;//+endCase
            
      case EO_PRINTF: GetStr(iArg); // formatted print
#ifdef WITH_TRACE_LOGICUNIT
        dLU0(EO_PRINTF);
        vLib_PRINTF(pLU,(Stz*)iArg,&aiDstk);
#endif
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
        
      case EO_PUSH_MINUS1: ArgNil(); // push integer -1
        dLU0(EO_PUSH_MINUS1);
        *(++aiDstk) = -1;
        break;//+endCase
        
      case EO_PUSH_0: ArgNil(); // push integer 0
        dLU0(EO_PUSH_0);
        *(++aiDstk) = 0;
        break;//+endCase
        
      case EO_PUSH_1: ArgNil(); // push integer 1
        dLU0(EO_PUSH_1);
        *(++aiDstk) = 1;
        break;//+endCase
        
      case EO_RANDOM: ArgGet(iArg); // push random number 0..arg-1
        iRes = iLib_RANDOM(pLU,iArg);
        dLU2(EO_RANDOM,iArg,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase

      case EO_STRING: GetStr(iArg); // push string
        dLU1(EO_STRING,iArg);
        S_PushStr(iArg);
        vRlu_DUMP(pLU,aiDstk);
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
        *aiDstk = *aiDstk > iTopB;
        break;//+endCase

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        // column pushes
      case EO_PUSH_COLUMN_LENGTH: ArgGet(iArg); // push columnn length
        dLU1(EO_PUSH_COLUMN_LENGTH,iArg);
        *(++aiDstk) = iArg;
        break;//+endCase

      case EO_PUSH_COLUMN_SCALE:  ArgGet(iArg); // push columnn scale
        dLU1(EO_PUSH_COLUMN_SCALE,iArg);
        *(++aiDstk) = iArg;
        break;//+endCase

      case EO_PUSH_COLUMN_TYPE: ArgGet(iArg); // push columnn type
        dLU1(EO_PUSH_COLUMN_TYPE,iArg);
        *(++aiDstk) = iArg;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device pushes
      case EO_PUSH_DEVIXT_ALT: ArgNil(); // push alt ixt device
        dLU0(EO_PUSH_DEVIXT_ALT);
        P_PushPtr(aRluCommon->m_pDevAltIxt);
        break;//+endCase

      case EO_PUSH_DEVIXT_ENT: ArgNil(); // push entry from ixt device
        dLU0(EO_PUSH_DEVIXT_ENT);
        vDev_IXT_PUSH_ENT(&aiDstk);
        break;//+endCase

      case EO_PUSH_DEVIXT_SRC: ArgNil(); // push src ixt devive
        dLU0(EO_PUSH_DEVIXT_SRC);
        P_PushPtr(aRluCommon->m_pDevSrcIxt);
        break;//+endCase

      case EO_PUSH_DEVIXT_TMP: ArgNil(); // push tmp ixt device
        dLU0(EO_PUSH_DEVIXT_TMP);
        P_PushPtr(aRluCommon->m_pDevTmpIxt);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device pushes
      case EO_PUSH_DEVROW_ALT: ArgNil(); // push alt row device
        dLU0(EO_PUSH_DEVROW_ALT);
        P_PushPtr(aRluCommon->m_pDevAltRow);
        break;//+endCase

      case EO_PUSH_DEVROW_DST: ArgNil(); // push dst row device
        dLU0(EO_PUSH_DEVROW_DST);
        P_PushPtr(aRluCommon->m_pDevDstRow);
        break;//+endCase

      case EO_PUSH_DEVROW_SRC: ArgNil(); // push src row device
        dLU0(EO_PUSH_DEVROW_SRC);
        P_PushPtr(aRluCommon->m_pDevSrcRow);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu pushes
      case EO_PUSH_RLU_COLSET: ArgNil(); // push rlu colset
        dLU0(EO_PUSH_RLU_COLSET);
        P_PushPtr(aRluCommon->m_iaColSet);
        break;//+endCase

      case EO_PUSH_RLU_ENT: ArgNil(); // push rlu ent
        dLU0(EO_PUSH_RLU_ENT);
        P_PushPtr(aRluCommon->m_aWrkEnt);
        break;//+endCase

      case EO_PUSH_RLU_ENTWRK: ArgNil(); // push rlu entwrk
        dLU0(EO_PUSH_RLU_ENTWRK);
        P_PushPtr(aRluCommon->m_pEntWrk);
        break;//+endCase

      case EO_PUSH_RLU_INDEXDEF: ArgNil(); // push rlu indexDef
        dLU0(EO_PUSH_RLU_INDEXDEF);
        P_PushPtr(aRluCommon->m_pIndexDef);
        break;//+endCase

      case EO_PUSH_RLU_POT: ArgNil(); // push rlu pot
        dLU0(EO_PUSH_RLU_POT);
        P_PushPtr(aRluCommon->m_pPot);
        break;//+endCase

      case EO_PUSH_RLU_PTN: ArgNil(); // push rlu partition
        dLU0(EO_PUSH_RLU_PTN);
        P_PushPtr(aRluCommon->m_i_Ptn);
        break;//+endCase

      case EO_PUSH_RLU_ROWPIX: ArgNil(); // push rlu row pix
        dLU0(EO_PUSH_RLU_ROWPIX);
        P_PushPtr(aRluCommon->m_pRowPix);
        break;//+endCase

      // locals pushes
      case EO_PUSH_RLU_LOCALS: ArgNil(); // push rlu locals
        dLU0(EO_PUSH_RLU_LOCALS);
        aiLocals = aRluCommon->m_iaLocals - 1;
        P_PushPtr(&aiLocals);
        break;//+endCase

      // register pushes
      case EO_PUSH_RLU_INDEXTREE: ArgNil(); // push rlu indexTree
        dLU0(EO_PUSH_RLU_INDEXTREE);
        P_PushPtr(aRluCommon->m_pIndexTree);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // table ptn
      case EO_TABLEPTN_DROPREAD: ArgNil(); // drop the tableptn read lock
        dLU0(EO_TABLEPTN_DROPREAD);
        vDev_TABLEPTN_DROPREAD(pLU);
        break;//+endCase

      case EO_TABLEPTN_GRABREAD: ArgNil(); // grab the tableptn read lock
        dLU0(EO_TABLEPTN_GRABREAD);
        vDev_TABLEPTN_GRABREAD(pLU);
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
        aiDstk--;
        dLU0(EO_POP);
        break;//+endCase
        
      case EO_POP_LOCALI: ArgGet(iArg); // pop into a local reg
        dLU1(EO_POP_LOCALI, iArg);
        //ASSERT(iArg < C_LOGIC_LOCALS);
        iTopA = *(aiDstk--);
        vLib_SET_LOCAL(pLU, iArg, iTopA);
        break;//+endCase
        
      case EO_POP_LOCALS: ArgGet(iArg); // pop into a local reg
        dLU1(EO_POP_LOCALS, iArg);
        //if (iArg >= C_LOGIC_LOCALS)
        //  vLib_ASSERT(pLU,0);
        S_PopStr(sLib_GET_LOCALS(pLU, iArg));
        break;//+endCase
        
      case EO_PUSH_LOCALI: ArgGet(iArg); // push a local reg
        dLU1(EO_PUSH_LOCALI, iArg);
        //ASSERT(iArg < C_LOGIC_LOCALS);
        *(++aiDstk) = aRluCommon->m_iaLocals[iArg]; //iLib_GET_LOCAL(pLU, iArg);
        break;//+endCase
        
      case EO_PUSH_LOCALS: ArgGet(iArg); // push a local reg
        dLU1(EO_PUSH_LOCALS, iArg);
        //ASSERT(iArg < C_LOGIC_LOCALS);
        S_PushStr(sLib_GET_LOCALS(pLU, iArg));
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // string operators
      case EO_STR_COMPARE: ArgNil(); // pop top stradr and compare str on stack
        dLU0(EO_STR_COMPARE);
        szTopB = (Stz*) *(aiDstk--);
      
        iTopA = *(aiDstk--);
        szTopA = (Stz*) (aiDstk-(nWordRoundedStrBytes(iTopA)/8 - 1));
        iRes = iLib_STR_COMPARE(pLU,szTopA,szTopB);
        aiDstk -= nWordRoundedStrBytes(nSize)/8;
        *(++aiDstk) = iRes;
        break;//+endCase
        
       // string operators
      case EO_STR_COMPARE_a: ArgNil(); // pop top two azStr and compare
        dLU0(EO_STR_COMPARE_a);
        azTopB = (char*) *(aiDstk--);
        azTopA = (char*) *(aiDstk--);
        iRes = strcmp(azTopA,azTopB);
        *(++aiDstk) = iRes;
        break;//+endCase

      case EO_STR_PRINTADR: ArgNil(); // pop top and print iArg as addr of string
#ifdef WITH_TRACE_LOGICUNIT
        azTopA = (char*) *(aiDstk--);
        dLU1(EO_STR_PRINTADR,azTopA);
        printf("[Q%lld.%lld] %s\n",
               aRluCommon->m_iSql,aRluCommon->m_i_TablePtn,azTopA);
#endif
        break;//+endCase
        
      case EO_STR_PRINTSTK: ArgNil(); // pop string from top and print
        iTopA = *(aiDstk--);
        szTopA = (Stz*) (aiDstk-(iTopA/8 + 1));
        printf("EO_STR_PRINTSTK : %lld %p\n",iTopA,(char*)szTopA);
        dLU1(EO_STR_PRINTSTK,iTopA);
        printf("[Q%lld,%lld] %s\n",
               aRluCommon->m_iSql,aRluCommon->m_i_TablePtn,(char*)szTopA);
        aiDstk -= (iTopA/8) + 2;
        break;//+endCase
        
      case EO_STR_LITERAL: GetStr(iArg); // push address of string literal
        dLU1(EO_STR_LITERAL,iArg);
        *(++aiDstk) = (i8)iArg;
        break;//+endCase
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // subroutine operators
      case EO_CALL: ArgGet(iArg); // call subroutine at location
        dLU1(EO_CALL,iArg);
#ifdef __TINYC__
#else
        * (aeaRstk++) = aeCode;
        aeCode = & m_eaCode[iArg];
#endif
        break;//+endCase

      case EO_RETURN: ArgNil(); // return from subroutine and
        dLU0(EO_RETURN);
#ifdef __TINYC__
        return aiDstk;
#else
        if (aeaRstk == &eaReturnStack[0]) {
          iRes = *(aiDstk--);
#ifdef WITH_TRACE_LOGICUNIT
          vTrace("\nEnd Logic Unit :: sql %lld\n\n"_Stz,aRluCommon->m_iSql);
#endif
          return iRes;
        }
        aeCode = * (--aeaRstk);
#endif
        break;//+endCase

      case EO_RETURN_FALSE: ArgNil(); // return from subroutine on false
        dLU0(EO_RETURN);
#ifdef __TINYC__
        if (! *(aiDstk--))
          return aiDstk;
#else
        if (! *(aiDstk--)) {
          if (aeaRstk == &eaReturnStack[0]) {
            iRes = *(aiDstk--);
#ifdef WITH_TRACE_LOGICUNIT
            vTrace("\nEnd Logic Unit :: sql %lld\n\n"_Stz,aRluCommon->m_iSql);
#endif
            return iRes;
          }
          aeCode = * (--aeaRstk);
        }

#endif
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // misc
      case EO_ASSERT: ArgNil(); // pop top and assert
        iTopA = *(aiDstk--);
        dLU1(EO_ASSERT,iTopA);
        vLib_ASSERT(pLU,(void*)iTopA);
        break;//+endCase
              
      case EO_BREAKPOINT: ArgNil(); // breakpoint
        dLU0(EO_BREAKPOINT);
        vLib_BREAKPOINT(pLU);
        break;//+endCase
        
      case EO_CALLAGAIN: ArgNil(); // set call again
        dLU0(EO_CALLAGAIN);
        aRluCommon->m_bCallAgain = 1;
        break;//+endCase
        
      case EO_CALLEDAGAIN: ArgNil(); // push called again
        dLU0(EO_CALLEDAGAIN);
        *(++aiDstk) = aRluCommon->m_bCalledAgain;
        break;//+endCase
        
      case EO_DUMP: ArgNil(); // stack dump
        dLU0(EO_DUMP);
        vRlu_DUMP(pLU,aiDstk);
        break;//+endCase
              
      case EO_ENTER: ArgNil(); //  diarise routine enter
        dLU0(EO_ENTER);
        aRluCommon->m_nEnterCount++;
        vRlu_ENTER(pLU);
        break;//+endCase
        
      case EO_EXIT: ArgNil(); // diarise routine exit
        dLU0(EO_EXIT);
        vRlu_EXIT(pLU);
        break;//+endCase

      case EO_HANDLER: GetStr(iArg); // set logicUnit handler
        dLU0(EO_EXIT);
        vRlu_HANDLER(pLU, (Stz*)iArg);
        break;//+endCase

      case EO_INIT: ArgNil(); // initialise the rlu common
        dLU0(EO_INIT);
        aRluCommon = aRlu_INIT(pLU);
#ifdef __TINYC__
        aiDstk = & aRluCommon->m_iaDataStack[0];
        *aiDstk = 0xFFFFFFFFFFFFFFFF;
        if (nOuterArgs) {
          *(++aiDstk) = iOuterArg0;
        }
#endif
        //printf("[Q%lld.%lld] INIT: %lld %lld\n",(i8)0,(i8)0,
        //       aRluCommon->m_iCmnMagic,aRluCommon->m_iCmnFooter);
        break;//+endCase
        
      case EO_INTEGRITY: ArgNil(); // integrity  check
        dLU0(EO_INTEGRITY);
        if (aRluCommon->m_iaDataStack != aiDstk) {
          printf("Dstk growth\n");
          vLib_ASSERT(pLU,(void*)0);
        }
        if (0xFFFFFFFFFFFFFFFF != (u8)aRluCommon->m_iaDataStack[0]) {
          printf("Dstk underflow\n");
          vLib_ASSERT(pLU,(void*)0);
        }
        break;//+endCase
        
      case EO_NEVERREACH: ArgNil(); // always assert
        dLU0(EO_NEVERREACH);
        vLib_ASSERT(pLU,0);
        break;//+endCase
        
      case EO_SANITY: ArgNil(); // sanity check
        dLU0(EO_SANITY);
        break;//+endCase
        
      case EO_SLEEP: ArgNil(); // sleep for top seconds
        iTopA = *(aiDstk--);
        dLU1(EO_SLEEP,iTopA);
        vLib_SLEEP(pLU,iTopA);
        break;//+endCase
              
      case EO_SQL: ArgGet(iArg); //  set sql no
        dLU0(EO_SQL);
        aRluCommon->m_iSql = iArg;
        vRlu_QUERYFRAG(pLU,iArg);
        break;//+endCase
        
      case EO_TABLEPTN: ArgNil(); // push i_TablePtn
        dLU1(EO_TABLEPTN,aRluCommon->m_i_TablePtn);
        *(++aiDstk) = aRluCommon->m_i_TablePtn;
        break;//+endCase
        
      case EO_TRACEOFF: ArgNil(); // set trace off
        aRluCommon->m_bTrace = 0;
        break;//+endCase
        
      case EO_TRACEON: ArgNil(); // set trace on
        aRluCommon->m_bTrace = 1;
        break;//+endCase
        
      case EO_NOOP: ArgNil(); // no op
        dLU0(EO_NOOP);
        break;//+endCase

      case EO_ERROR: ArgNil(); // error marker
        dLU0(EO_ERROR);
        vLib_ERROR(pLU,"Code compiled with error"_Stz);
        break;//+endCase
        

      //------------------------------------------------------------------------
      //------------------------------------------------------------------------

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // date operators
      case EO_DTE_NOW: ArgNil(); // push current date
        iRes = iLib_DTE_NOW(pLU);
        dLU1(EO_DTE_NOW,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_DTE_PRINT: ArgNil(); // pop top and print as date
        iTopA = *(aiDstk--);
        szRes = szLib_DTE_PRINT(pLU,iTopA);
        printf("[Q%lld.%lld] %s\n",
               aRluCommon->m_iSql,aRluCommon->m_i_TablePtn,(char*)szRes);
        break;//+endCase

      case EO_DTE_YEAR: ArgNil(); // pop top, push year of date
        iTopA = *(aiDstk--);
        iRes = iLib_DTE_YEAR(pLU,iTopA);
        dLU1(EO_DTE_NOW,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // hashing
      case EO_HSH_ADD_HASHI: ArgNil(); // add hash of top int to running hash
        iTopA = *(aiDstk--);
        dLU1(EO_HSH_ADD_HASHI,iTopA);
        vLib_HSH_ADD_HASHI(pLU,iTopA);
        break;//+endCase
        
      case EO_HSH_RESET: ArgNil(); // reset hash
        dLU0(EO_HSH_RESET);
        aRluCommon->m_iHash = 0;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // partitioning
      case EO_PTN_CALC: ArgNil(); // calculate m_i_Ptn using running m_iHash
        dLU0(EO_PTN_CALC);
        vLib_PTN_CALC(pLU);
        break;//+endCase

      case EO_PTN_PUSH: ArgNil(); // push m_i_Ptn
        dLU1(EO_PTN_PUSH,aRluCommon->m_i_Ptn);
        *(++aiDstk) = aRluCommon->m_i_Ptn;
        break;//+endCase

      case EO_PTN_RESET: ArgNil(); // reset running m_iHash
        dLU0(EO_PTN_RESET);
        aRluCommon->m_iHash = 0;
        break;//+endCase

      case EO_PTN_RUNNING: ArgNil(); // mix top into running m_iHash
        iTopA = *(aiDstk--);
        dLU0(EO_PTN_RUNNING);
        vLib_PTN_RUNNING(pLU,iTopA);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // timer
      case EO_TMR_SINCE: ArgNil(); // get nsecs since ::constructor
        iRes = iRlu_TMR_SINCE(pLU);
        dLU1(EO_TMR_SINCE,iRes);
        *(++aiDstk) = iRes;        break;//+endCase
        
      case EO_TMR_START: ArgNil(); // timer start
        dLU0(EO_TMR_START);
        vRlu_TMR_START(pLU);
        break;//+endCase
        
      case EO_TMR_NSECS: ArgNil(); // push timer nanoseconds
        iRes = iRlu_TMR_NSECS(pLU);
        dLU1(EO_TMR_NSECS,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_TMR_UPDATE: ArgNil(); // timer update
        dLU0(EO_TMR_UPDATE);
        vRlu_TMR_UPDATE(pLU);
        break;//+endCase
        
      case EO_TMR_AGG_ADD: ArgNil(); // add timer to agg timer
        dLU0(EO_TMR_AGG_ADD);
        vRlu_TMR_AGG_ADD(pLU);
        break;//+endCase
        
      case EO_TMR_AGG_NSECS: ArgNil(); // push agg nanoseconds
        iRes = iRlu_TMR_AGG_NSECS(pLU);
        dLU1(EO_TMR_AGG_NSECS,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_TMR_AGG_RESET: ArgNil(); // reset agg timer
        dLU0(EO_TMR_AGG_RESET);
        vRlu_TMR_AGG_RESET(pLU);
        break;//+endCase
          

      //------------------------------------------------------------------------

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu column set
      case EO_RLU_SET_COLUMN: ArgGet(iArg); // add column to set
        dLU1(EO_RLU_SET_COLUMN,iArg);
        aRluCommon->m_iaColSet[--aRluCommon->m_i_ColSet] = iArg;
        //*(++aiDstk) = iArg;
        break;//+endCase

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        // columns set
      case EO_RLU_SET_COLUMNS: { ArgGet(iArg); // add columns to set
        dLU1(EO_RLU_SET_COLUMNS, iArg);
        i8 iCol = 0;

        for (i8 iBit = 0; iBit < (i8)C_BITS_PER_WORD; iBit++) {
          u8 uBitMap = iArg;
          i8 iToLeft  = 63 - iBit;
          i8 iToRight = iBit;

          // strip off upper bits
          uBitMap <<= iToLeft;
          uBitMap >>= iToLeft;

          // strip off lower bits
          uBitMap >>= iToRight;

          // test if given bit is on
          if (uBitMap) {
            aRluCommon->m_iaColSet[iCol++] = iBit;
          }
        }
        aRluCommon->m_iaColSet[iCol] = C_UNKNOWN;
      }
        break;//+endCase

      case EO_RLU_SET_ITEMS: ArgGet(iArg); // set number of column items
        dLU1(EO_RLU_SET_ITEMS,iArg);
        aRluCommon->m_i_ColSet = iArg;
        aRluCommon->m_iaColSet[aRluCommon->m_i_ColSet] = C_UNKNOWN;
        //*(++aiDstk) = -1;
        break;//+endCase

       // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu operators
      case EO_RLU_INDEXDEF_ADDCOLUMN: ArgNil(); // add a column to indexDef in rlu
        dLU0(EO_RLU_INDEXDEF_ADDCOLUMN);
        vDev_RLU_INDEXDEF_ADDCOLUMN(&aiDstk);  // dstk: column name / column type / indexdef
        break;//+endCase

      case EO_RLU_INDEXDEF_DELETE: ArgNil();   // delete the indexDef from rlu
        dLU0(EO_RLU_INDEXDEF_DELETE);
        vDev_RLU_INDEXDEF_DELETE(&aiDstk);     // dstk: indexdef
        aRluCommon->m_pIndexDef = nullptr;
        break;//+endCase

      case EO_RLU_INDEXDEF_NEW: ArgNil();      // new an indexDef in the rlu
        dLU0(EO_RLU_INDEXDEF_NEW);
        vDev_RLU_INDEXDEF_NEW(&aiDstk);        // dstk: name
        aRluCommon->m_pIndexDef = P_PopPtr();
        break;//+endCase

      case EO_RLU_INDEXTREE_NEW: ArgNil();     // new an indexTree in the rlu
        dLU0(EO_RLU_INDEXTREE_NEW);
        vDev_RLU_INDEXTREE_NEW(&aiDstk);       // dstk: indexdef
        aRluCommon->m_pIndexTree = P_PopPtr();
        break;//+endCase

      case EO_RLU_ROWPIX_PRINT: ArgNil();      // print row pix from rlu
        dLU0(EO_RLU_ROWPIX_PRINT);
        vDev_RLU_ROWPIX_PRINT(&aiDstk);        // dstk: rowPix
        break;//+endCase

      case EO_RLU_POT_COMPLETED: ArgNil();     // do we have a full pot buffer ?
        dLU0(EO_RLU_POT_COMPLETED);
        *(++aiDstk) = aRluCommon->m_bPotCompleted;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device col operators
      case EO_DEVIXT_COLS_TO_LOCALS: ArgNil(); // inflate dev columns direct into locals
        dLU0(EO_DEVIXT_COLS_TO_LOCALS);
        vDev_IXT_COLS_TO_LOCALS(&aiDstk);      // locals / colset / devRow / devIxt
        break;//+endCase

      case EO_DEVIXT_COLS_TO_ROW: ArgNil();    // inflate dev columns to dev row
        dLU0(EO_DEVIXT_COLS_TO_ROW);
        vDev_IXT_COLS_TO_ROW(&aiDstk);         // devRow / devIxt
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device col operators
      case EO_DEVROW_COLS_TO_LOCALS: ArgNil(); // row buffer columns of row dev into locals
        dLU0(EO_DEVROW_COLS_TO_LOCALS);
        vDev_ROW_COLS_TO_LOCALS(&aiDstk);      // locals / colset / devRow
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device crwbuffer operators
      case EO_DEVIXT_CRWBUF_BEGIN: ArgNil(); // begin row traverse
        dLU0(EO_DEVIXT_CRWBUF_BEGIN);
        vDev_IXT_CRWBUF_BEGIN(&aiDstk);      // devixt
        break;//+endCase

      case EO_DEVIXT_CRWBUF_NEXT: ArgNil(); // move to next row
        dLU0(EO_DEVIXT_CRWBUF_NEXT);
        vDev_IXT_CRWBUF_NEXT(&aiDstk);      // devixt
        break;//+endCase

      case EO_DEVIXT_CRWBUF_NULL: ArgNil(); // null the crw
        dLU0(EO_DEVIXT_CRWBUF_NULL);
        vDev_IXT_CRWBUF_NULL(&aiDstk);      // devixt
        break;//+endCase

      case EO_DEVIXT_CRWBUF_TEST: ArgNil(); // test for crw
        dLU0(EO_DEVIXT_CRWBUF_TEST);
        vDev_IXT_CRWBUF_TEST(&aiDstk);      // devixt
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device entry operators
      case EO_DEVIXT_ENT_DEREF: ArgNil(); // deref Crw using m_aEnt.ref
        dLU0(EO_DEVIXT_ENT_DEREF);
        vDev_IXT_ENT_DEREF(&aiDstk);      // devixt
        break;//+endCase

      case EO_DEVIXT_ENT_FINDI: ArgNil(); // find entry with int key
        dLU0(EO_DEVIXT_ENT_FINDI);
        P_PushPtr(aRluCommon->m_aWrkEnt);
        vDev_IXT_ENT_FIND(&aiDstk);       // devixt / entry
        break;//+endCase
        
      case EO_DEVIXT_ENT_NEXT: ArgNil(); // move to next m_pEnt
        dLU0(EO_DEVIXT_ENT_NEXT);
        vDev_IXT_ENT_NEXT(&aiDstk);      // devixt
        break;//+endCase
        
      case EO_DEVIXT_ENT_POSPRIOR: ArgNil(); // position m_pEnt prior using m_oEntWrk.key
        dLU0(EO_DEVIXT_ENT_POSPRIOR);
        P_PushPtr(aRluCommon->m_aWrkEnt);
        vDev_IXT_ENT_POSPRIOR(&aiDstk);      // devixt / entry
        break;//+endCase
        
      case EO_DEVIXT_ENT_PUSH: ArgNil(); // push entry on the stack
        dLU0(EO_DEVIXT_ENT_PUSH);
        vDev_IXT_ENT_PUSH(&aiDstk);         // devixt
        break;//+endCase
        
      case EO_DEVIXT_ENT_PUSHELMI0: ArgNil(); // push entry elm 0 int
        dLU0(EO_DEVIXT_ENT_PUSHELMI0);
        vDev_IXT_ENT_PUSHELMI0(&aiDstk);      // devixt
        break;//+endCase
        
      case EO_DEVIXT_ENT_PUSHELMS0: ArgNil(); // push entry elm 0 str
        dLU0(EO_DEVIXT_ENT_PUSHELMS0);
        vDev_IXT_ENT_PUSHELMS0(&aiDstk);      // devixt
        break;//+endCase
        
      case EO_DEVIXT_ENT_PUSHUPPER: ArgNil(); // push entry on the stack
        dLU0(EO_DEVIXT_ENT_PUSHUPPER);
        vDev_IXT_ENT_PUSHUPPER(&aiDstk);      // devixt
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device entry relational operators
      case EO_DEVIXT_ENTKEY_LESS: ArgNil(); // test if dev m_pEnt.key <  ent.key
        vDev_IXT_ENT_COMPARE(&aiDstk);      // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_BEFORE == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_LESS, iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_LESSEQUAL: ArgNil(); // test if dev m_pEnt.key <= m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);           // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_BEFORE == iCmp || KC_SAME == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_LESSEQUAL, iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_EQUAL: ArgNil(); // test if dev m_pEnt.key == m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);       // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_SAME == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_EQUAL, iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_NOTEQUAL: ArgNil(); // test if dev m_pEnt.key != m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);          // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_SAME != iCmp);
        dLU1(EO_DEVIXT_ENTKEY_NOTEQUAL, iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_GREATEREQUAL: ArgNil(); // test if dev m_pEnt.key >= m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);              // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_AFTER == iCmp || KC_SAME == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_GREATEREQUAL, iRes);
        *(++aiDstk) = iRes;
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_GREATER: ArgNil(); // test if dev m_pEnt.key >  m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);         // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_AFTER == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_GREATER, iRes);
        *(++aiDstk) = iRes;
        break;//+endCase

      case EO_DEVIXT_ENTKEY_PRINTKEYS: ArgNil(); // print dev m_pEnt.key and other working key
        vDev_IXT_ENT_PRINTKEYS(&aiDstk);         // devixt / entry
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device entwrk operators
      case EO_ENTWRK_PRINT: ArgNil(); // print m_oEntWrk
        dLU0(EO_ENTWRK_PRINT);
        vDev_ENTWRK_PRINT(&aiDstk);   // entry
        break;//+endCase

      case EO_ENTWRK_RESET: ArgNil(); // reset m_oEntWrk
        dLU0(EO_ENTWRK_RESET);
        vDev_ENTWRK_RESET(&aiDstk);   // entry
        break;//+endCase

      case EO_ENTWRK_SETINTELM: ArgNil(); // append next element value (int)
        dLU0(EO_ENTWRK_SETINTELM);
        vDev_ENTWRK_SETINTELM(&aiDstk);   // int /entry
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device index operators
      case EO_DEVIXT_IDX_DEL: ArgNil(); // delete current m_aEnt from indexTree
        dLU0(EO_DEVIXT_IDX_DEL);
        vDev_IXT_IDX_DEL(&aiDstk);      // devixt
        break;//+endCase

      case EO_DEVIXT_IDX_DUMP: ArgNil(); // dump index tree
        dLU0(EO_DEVIXT_IDX_DUMP);
        vDev_IXT_IDX_DUMP(&aiDstk);      // devixt
        break;//+endCase

      case EO_DEVIXT_IDX_ENTRIES: ArgNil(); // push number of entries in dev index
        dLU0(EO_DEVIXT_IDX_ENTRIES);
        vDev_IXT_IDX_ENTRIES(&aiDstk);      // devixt
        break;//+endCase

      case EO_DEVIXT_IDX_INS: ArgNil(); // insert an entry into the indexTree
        dLU0(EO_DEVIXT_IDX_INS);
        vDev_IXT_IDX_INS(&aiDstk);      // devixt/ entry
        break;//+endCase

      case EO_DEVIXT_IDX_BEGIN: ArgNil(); // begin linear traverse
        dLU0(EO_DEVIXT_IDX_BEGIN);
        vDev_IXT_IDX_BEGIN(&aiDstk);      // devixt
        break;//+endCase

      case EO_DEVIXT_IDX_NEXT: ArgNil(); // traverse to next entry
        dLU0(EO_DEVIXT_IDX_NEXT);
        vDev_IXT_IDX_NEXT(&aiDstk);      // devixt
        break;//+endCase
        
      case EO_DEVIXT_LBOUND: ArgNil(); // pos in buffer to entry before LBOUND
        dLU0(EO_DEVIXT_LBOUND);
        vDev_IXT_POS_LBOUND(&aiDstk);  // entry / devixt
      break;//+endCase

      case EO_DEVIXT_UBOUND: ArgNil(); // pos in buffer to entry for UBOUND
        dLU0(EO_DEVIXT_UBOUND);
        vDev_IXT_POS_UBOUND(&aiDstk);      // entry / devixt
        break;//+endCase
        
      case EO_DEVIXT_UBOUND_LT: ArgNil(); // pos in buffer to entry for UBOUND
        dLU0(EO_DEVIXT_UBOUND_LT);
        vDev_IXT_POS_UBOUND_LT(&aiDstk);  // entry / devixt
        break;//+endCase
        
      case EO_DEVIXT_CMP_KEYS: ArgNil(); // compare keys of two devices
        dLU0(EO_DEVIXT_CMP_KEYS);
        vDev_IXT_CMP_KEYS(&aiDstk);     // entry / entry
        break;//+endCase

      case EO_DEVIXT_POS_BYKEY: ArgNil(); // position alt ixt using src.key
        dLU0(EO_DEVIXT_POS_BYKEY);
        vDev_IXT_POS_BYKEY(&aiDstk);      // devixt / entry
        break;//+endCase

      case EO_DEVIXT_ENT_PRINT: ArgNil(); // print ent
        dLU0(EO_DEVIXT_ENT_PRINT);
        vDev_IXT_ENT_PRINT(&aiDstk);      // devixt
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device index set operators
      case EO_DEVIXT_INDEXNM_SET: GetStr(iArg); // ALKA: set the index name
        dLU0(EO_DEVIXT_INDEXNM_SET);
        S_PushStr(iArg);
        vDev_IXT_INDEXNM_SET(&aiDstk);
        break;//+endCase

      case EO_DEVIXT_INDEXTREE_SET: ArgNil(); // set indexTree from indexTree[]
        dLU0(EO_DEVIXT_INDEXTREE_SET);
        vDev_IXT_INDEXTREE_SET(&aiDstk);      // indextree / devixt
        break;//+endCase

      case EO_DEVIXT_INDEXTREE_DELETE: ArgNil(); // delete indexTree
        dLU0(EO_DEVIXT_INDEXTREE_DELETE);
        vDev_IXT_INDEXTREE_DELETE(&aiDstk);   // devixt
        aRluCommon->m_pIndexTree = nullptr;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device entWrk operators
      case EO_DEVROW_ENTWRK_APPENDELMI: ArgNil(); // append next element value (int)
        dLU0(EO_DEVROW_ENTWRK_APPENDELMI);
        vDev_ROW_ENTWRK_APPENDELMI(&aiDstk);      // entry / devrow
        break;//+endCase

      case EO_DEVROW_ENTWRK_APPENDELMS: ArgNil(); // append next element value (str)
        dLU0(EO_DEVROW_ENTWRK_APPENDELMS);
        vDev_ROW_ENTWRK_APPENDELMS(&aiDstk);      // entry / devrow
        break;//+endCase

      case EO_DEVROW_ENTWRK_CREATEPAY: ArgGet(iArg); // ALKA: create a payload in entwrk
        dLU1(EO_DEVROW_ENTWRK_CREATEPAY,iArg);
        *(++aiDstk) = iArg;
        vDev_ROW_ENTWRK_CREATEPAY(&aiDstk);          // entry / devrow
        break;//+endCase

      case EO_DEVROW_TABLENM_SET: GetStr(iArg); // ALKA: set tablenm in dev row
        dLU0(EO_DEVROW_TABLENM_SET);
        S_PushStr(iArg);
        vDev_ROW_TABLENM_SET(&aiDstk);
        break;//+endCase

      case EO_DEVROW_STREAMDEF_DELETE: ArgNil(); // delete streamDef
        dLU0(EO_DEVROW_STREAMDEF_DELETE);
        vDev_ROW_STREAMDEF_DELETE(&aiDstk);   // devixt
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device row operators
      case EO_DEVROW_ROW_NEW: ArgNil(); // create new row
        dLU0(EO_DEVROW_ROW_NEW);
        vDev_ROW_ROW_NEW(&aiDstk);     // devrow / pot /ptn
        aRluCommon->m_bPotCompleted = *(aiDstk--);
        break;//+endCase

      case EO_DEVROW_ROW_PRINT: ArgNil(); // print row
        dLU0(EO_DEVROW_ROW_PRINT);
        vDev_ROW_ROW_PRINT(&aiDstk);      // devrow
        break;//+endCase

      case EO_DEVROW_ROW_SET: ArgNil(); // set dst row
        dLU0(EO_DEVROW_ROW_SET);
        vDev_ROW_ROW_SET(&aiDstk);      // rowNo / devrow
        break;//+endCase

      case EO_DEVROW_ROW_TEST: ArgNil(); // test for row
        dLU0(EO_DEVROW_ROW_TEST);
        vDev_ROW_ROW_TEST(&aiDstk);      // devrow
        break;//+endCase

      case EO_DEVROW_ROW_ZERO: ArgNil(); // zero device row
        dLU0(EO_DEVROW_ROW_ZERO);
        vDev_ROW_ROW_ZERO(&aiDstk);      // devrow
        break;//+endCase

       // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device row buffer operators
      case EO_DEVROW_ROWBUF_BEGIN: ArgNil(); // begin row traverse
        dLU0(EO_DEVROW_ROWBUF_BEGIN);
        vDev_ROW_ROWBUF_BEGIN(&aiDstk);      // devrow
        break;//+endCase

      case EO_DEVROW_ROWBUF_NEXT: ArgNil(); // move to next row
        dLU0(EO_DEVROW_ROWBUF_NEXT);
        vDev_ROW_ROWBUF_NEXT(&aiDstk);      // devrow
        break;//+endCase

      case EO_DEVROW_ROWBUF_NULL: ArgNil(); // null the buffer
        dLU0(EO_DEVROW_ROWBUF_NULL);
        vDev_ROW_ROWBUF_NULL(&aiDstk);      // devrow
        break;//+endCase

      case EO_DEVROW_ROWBUF_RESET: ArgNil(); // reset rowslab
        dLU0(EO_DEVROW_ROWBUF_RESET);
        vDev_ROW_ROWBUF_RESET(&aiDstk);      // devrow
        break;//+endCase

      case EO_DEVROW_ROWBUF_TEST: ArgNil(); // test for buffer
        dLU0(EO_DEVROW_ROWBUF_TEST);
        vDev_ROW_ROWBUF_TEST(&aiDstk);      // devrow
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      //  row device column operators
      case EO_DEVROW_POPCOLI: ArgGet(iArg); // ALKA: pop and deposit top into dev column
        *(++aiDstk) = iArg;
        dLU2(EO_DEVROW_POPCOLI,iArg,iTopA);
        vDev_ROW_POPCOLI(&aiDstk);          // columnNo / devrow
        break;//+endCase

      case EO_DEVROW_POPCOLS: ArgGet(iArg); // ALKA : pop and deposit top into dev column
        *(++aiDstk) = iArg;
        dLU1(EO_DEVROW_POPCOLS,iArg);
        vDev_ROW_POPCOLS(&aiDstk);         // columnNo / devrow
        break;//+endCase

      case EO_DEVROW_PUSHCOLI: ArgGet(iArg); // ALKA: push column
        dLU1(EO_DEVROW_PUSHCOLI,iArg);
        *(++aiDstk) = iArg;
        vDev_ROW_PUSHCOLI(&aiDstk);         // columnNo / devrow
        break;//+endCase

      case EO_DEVROW_PUSHCOLS: ArgGet(iArg); // ALKA: push column
        dLU1(EO_DEVROW_PUSHCOLS,iArg);
        *(++aiDstk) = iArg;
        vDev_ROW_PUSHCOLS(&aiDstk);          // columnNo / devrow
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pay load operators
      case EO_PAY_ADDRESS: ArgNil(); // get payload address
        dLU0(EO_PAY_ADDRESS);
        vDev_PAY_ADDRESS(&aiDstk);
        aiPayload = (i8*)*(aiDstk--);
        break;//+endCase

      case EO_PAY_FETCH_COLI: ArgGet(iArg); // fetch a column from entry payload
        iRes = aiPayload[iArg];
        dLU2(EO_PAY_FETCH_COLI,iArg,iRes);
        *(++aiDstk) = iRes;
        break;//+endCase

      case EO_PAY_FETCH_COLS: ArgGet(iArg); // fetch a column from entry payload
        azSrc = (char*)&aiPayload[iArg];
        nSize = strlen(azSrc) + 1;
        //ASSERT(nSize < 8);
        dLU2(EO_PAY_FETCH_COLS,iArg,(i8)azSrc);
        S_PushStr(azSrc);
        break;//+endCase

      case EO_PAY_STORE_COLI: ArgGet(iArg); // store a column into entry payload
        iTopA = *(aiDstk--);
        dLU2(EO_PAY_STORE_COLI,iArg,iTopA);
        aiPayload[iArg] = iTopA;
        break;//+endCase

      case EO_PAY_STORE_COLS: ArgGet(iArg); // store a column into entry payload
        dLU1(EO_PAY_STORE_COLS,iArg);
        azDst = (char*)&aiPayload[iArg];
        S_PopStr(azDst);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // -- TO GO --
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device col operators
      case EO_DEVIXT_COL_PUSHSET_ADR: ArgNil(); // push column value from dev crw
        vDev_IXT_COL_PUSHSET_ADR(&aiDstk);
        dLU0(EO_DEVIXT_COL_PUSHSET_ADR);
        break;//+endCase

      case EO_DEVIXT_COL_PUSHSET_PST: ArgNil(); // push column value from dev
        vDev_IXT_COL_PUSHSET_PST(&aiDstk); // colset / devrow / devixt
        dLU0(EO_DEVIXT_COL_PUSHSET_PST);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // key
      case EO_KEY_LCL2C_TO_I: ArgGet(iArg); // convert two lcl varchar(1) to int
        dLU1(EO_KEY_LCL2C_TO_I,iArg);
        aiLocals = aRluCommon->m_iaLocals;
//        azTopA = (char*)aiLocals[giaVectorSet[iArg]];
//        azTopB = (char*)aiLocals[giaVectorSet[iArg+1]];
//        iRes = *azTopA * 256 + *azTopB;
//        *(++aiDstk) = iRes;

        *(++aiDstk) = *(char*)aiLocals[giaVectorSet[iArg]] * 256
                      + *(char*)aiLocals[giaVectorSet[iArg+1]];
        break;//+endCase

      case EO_KEY_ROW2C_TO_I: ArgGet(iArg); // convert two row varchar(1) to int
        dLU1(EO_KEY_ROW2C_TO_I,iArg);
        azTopA = (char*)szDev_ROW_PUSHCOLS(pLU,aRluCommon->m_pDevSrcRow,
                                           giaVectorSet[iArg]);
        azTopB = (char*)szDev_ROW_PUSHCOLS(pLU,aRluCommon->m_pDevSrcRow,
                                           giaVectorSet[iArg+1]);
        iRes = *azTopA * 256 + *azTopB;
        *(++aiDstk) = iRes;
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // register operators
      case EO_REG_EOSMARKER_NEW: ArgNil(); // new an EosMarker in OperandA[]
        dLU0(EO_REG_EOSMARKER_NEW);
        vDev_REG_EOSMARKER_NEW(pLU);
        break;//+endCase

      case EO_REG_INDEXDEF_ADDCOLUMN: GetStr(iArg); // add a column to indexDef[]
//        dLU0(EO_REG_INDEXDEF_ADDCOLUMN);
//        iTopA = *(aiDstk--);
//        vDev_REG_INDEXDEF_ADDCOLUMN(pLU, (Stz*)iArg, iTopA);
        RASSERT(0);
        break;//+endCase

      case EO_REG_PTNSTREAM_CLOSE: ArgNil(); // close ptnStream[]
        dLU0(EO_REG_PTNSTREAM_CLOSE);
        vDev_REG_PTNSTREAM_CLOSE(pLU);
        break;//+endCase

      case EO_REG_PTNSTREAM_DELETE: ArgNil(); // delete ptnStream[]
        dLU0(EO_REG_PTNSTREAM_DELETE);
        vDev_REG_PTNSTREAM_DELETE(pLU);
        break;//+endCase

      case EO_REG_PTNSTREAM_NEW: ArgNil(); // new an ptnStream in ptnStream[]
        dLU0(EO_REG_PTNSTREAM_NEW);
        vDev_REG_PTNSTREAM_NEW(pLU);
        break;//+endCase

      case EO_REG_PTNSTREAM_OPEN: ArgNil(); // new an ptnStream in ptnStream[]
        dLU0(EO_REG_PTNSTREAM_OPEN);
        vDev_REG_PTNSTREAM_OPEN(pLU);
        break;//+endCase

      case EO_REG_PTNSTREAM_READ: ArgNil(); // read from ptnStream[]
        dLU0(EO_REG_PTNSTREAM_READ);
        vDev_REG_PTNSTREAM_READ(pLU);
        break;//+endCase

      case EO_REG_STREAMDEF_STREAMDEFOUT: ArgNil(); // new a streamDefOut[] from streamDef[]
        dLU0(EO_REG_STREAMDEF_STREAMDEFOUT);
        vDev_REG_STREAMDEF_STREAMDEFOUT(pLU);
        break;//+endCase

      case EO_REG_STREAMDEFOUT_NEW: ArgNil(); // new a streamDef in streamDefOut[]
        dLU0(EO_REG_STREAMDEFOUT_NEW);
        vDev_REG_STREAMDEFOUT_NEW(pLU);
        break;//+endCase

      case EO_REG_STREAMDEFOUT_ADDCOLUMN: GetStr(iArg); // add a column to streamDefOut[]
        dLU0(EO_REG_STREAMDEFOUT_ADDCOLUMN);
        iTopA = *(aiDstk--); // column type
        iTopB = *(aiDstk--); // column length
        iTopC = *(aiDstk--); // column scale
        vDev_REG_STREAMDEFOUT_ADDCOLUMN(pLU, (Stz*)iArg, iTopA, iTopB, iTopC);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // super fragments
      case EO_SUPER_Q100: { ArgNil(); // super frag for Q100
        i8 iWrkA, iWrkB, iWrkC;
        aiLocals = aRluCommon->m_iaLocals;

        iWrkA = aiLocals[Q1_LOCAL_EXTENDEDPRICE];
        iWrkB = aiLocals[Q1_LOCAL_DISCOUNT];
        iWrkC = iWrkA * (100 - iWrkB);

        aiPayload[Q100_SUM_QTY] += aiLocals[Q1_LOCAL_QUANTITY];

        aiPayload[Q100_SUM_BASE_PRICE] += iWrkA;

        aiPayload[Q100_SUM_DISC_PRICE] += ((iWrkC) / 100);

        aiPayload[Q100_SUM_CHARGE] +=
            ((iWrkC) * (100 + aiLocals[Q1_LOCAL_TAX])) / 10000;

        aiPayload[Q100_COUNT_QTY]++;

        aiPayload[Q100_COUNT_PRICE]++;

        aiPayload[Q100_SUM_DISC] += iWrkB;

        aiPayload[Q100_COUNT_DISC]++;

        aiPayload[Q100_COUNT_ORDER]++;
      }
        break;//+endCase

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      default:
        vTrace("Unknown eo opcode %lld\n"_Stz,(i8)eCode);
        RASSERT(0);
        return C_DATUM_NULL;
    } //+ExtractEnd
    
  }
  
  return -1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
