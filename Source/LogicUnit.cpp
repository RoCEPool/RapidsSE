/*============================================================================*/
/* LogicUnit.hpp -- Logic Unit                                                */
/*============================================================================*/

#include "Expressions.h"

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
  m_aPayRsb     = nullptr;
  
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


//
// ..from Calculator
//

/*----------------------------------------------------------------------------*/
/* Description of EO Args                                                     */
/*----------------------------------------------------------------------------*/
EoArg EoArgs[]
{
  { EA_NONE,  0 },  // EO_NONE
  
  // arithmetic operators
  { EA_NONE,  0 },  // EO_ADD
  { EA_NONE,  0 },  // EO_DIVIDE
  { EA_NONE,  0 },  // EO_MODULO
  { EA_NONE,  0 },  // EO_MULTIPLY
  { EA_NONE,  0 },  // EO_NEGATE
  { EA_NONE,  0 },  // EO_SUBTRACT

  // col operators
  { EA_INT,   8 },  // EO_COL_SRC_PUSH
  { EA_INT,   8 },  // EO_COL_ALT_PUSH
  
  // crw operators
  { EA_NONE,  0 },  // EO_CRW_SRC_FETCH
  { EA_NONE,  0 },  // EO_CRW_ALT_FETCH
  
  // date operators
  { EA_NONE,  0 },  // EO_DTE_NOW
  { EA_NONE,  0 },  // EO_DTE_PRINT
  { EA_NONE,  0 },  // EO_DTE_YEAR
  
  // entry
  { EA_NONE,  0 },  // EO_ENT_NEXT
  { EA_NONE,  0 },  // EO_ENT_POSPRIOR
  { EA_NONE,  0 },  // EO_ENT_PUSH
  { EA_NONE,  0 },  // EO_ENT_PUSHELMI0
  { EA_NONE,  0 },  // EO_ENT_PUSHOFS
  { EA_NONE,  0 },  // EO_ENT_PUSHREF

  // entry relational operators
  { EA_NONE,  0 },  // EO_ENT_LESS
  { EA_NONE,  0 },  // EO_ENT_LESSEQUAL
  { EA_NONE,  0 },  // EO_ENT_EQUAL
  { EA_NONE,  0 },  // EO_ENT_NOTEQUAL
  { EA_NONE,  0 },  // EO_ENT_GREATEREQUAL
  { EA_NONE,  0 },  // EO_ENT_GREATER

  // entWrk
  { EA_INT,   8 },  // EO_ENTWRK_APPENDELM
  { EA_NONE,  0 },  // EO_ENTWRK_APPENDELMI
  { EA_INT,   8 },  // EO_ENTWRK_CREATEPAY
  { EA_NONE,  0 },  // EO_ENTWRK_POPOFS
  { EA_NONE,  0 },  // EO_ENTWRK_POPREF
  { EA_NONE,  0 },  // EO_ENTWRK_PRINT
  { EA_NONE,  0 },  // EO_ENTWRK_RESET

  // finalise
  { EA_NONE,  0 },  // EO_FIN_TRANSFORM
  
  // initialise
  { EA_NONE,  0 },  // EO_INI_PIX_DEFOUT
  { EA_NONE,  0 },  // EO_INI_PIX_SLABIN
  { EA_NONE,  0 },  // EO_INI_SRC_SLAB
  
  // index tree
  { EA_NONE,  0 },  // EO_IXT_DUMP
  { EA_NONE,  0 },  // EO_IXT_INS_ENT
  { EA_NONE,  0 },  // EO_IXT_TRV_BEGIN
  { EA_NONE,  0 },  // EO_IXT_TRV_NEXT

  { EA_NONE,  0 },  // EO_IXT_SRC_BEGIN
  { EA_NONE,  0 },  // EO_IXT_SRC_NEXT
  { EA_NONE,  0 },  // EO_IXT_SRC_PRINT
  { EA_NONE,  0 },  // EO_IXT_ALT_BEGIN
  { EA_NONE,  0 },  // EO_IXT_ALT_NEXT
  { EA_NONE,  0 },  // EO_IXT_ALT_PRINT
  { EA_NONE,  0 },  // EO_IXT_JOI_MATCH
  { EA_NONE,  0 },  // EO_IXT_CMP_SRCALT

  // jump operators
  { EA_LABEL, 8 },  // EO_JMP
  { EA_LABEL, 8 },  // EO_JMP_FALSE
  { EA_LABEL, 8 },  // EO_JMP_TRUE

  { EA_LABEL, 8 },  // EO_JMP_LESSTHAN
  { EA_LABEL, 8 },  // EO_JMP_LESSEQUAL
  { EA_LABEL, 8 },  // EO_JMP_EQUAL
  { EA_LABEL, 8 },  // EO_JMP_NOTEQUAL
  { EA_LABEL, 8 },  // EO_JMP_GREATEREQUAL
  { EA_LABEL, 8 },  // EO_JMP_GREATEREQUAL

  // logical operators
  { EA_NONE,  0 },  // EO_AND
  { EA_NONE,  0 },  // EO_OR
  { EA_NONE,  0 },  // EO_NOT

  // null test operators
  { EA_NONE,  0 },  // EO_ISNULL
  { EA_NONE,  0 },  // EO_ISNOTNULL

  // pay load
  { EA_INT,   8 },  // EO_PAY_FETCH_COL
  { EA_INT,   8 },  // EO_PAY_STORE_COL

  // pushes
  { EA_DATE,  8 },  // EO_DATE
  { EA_INT,   8 },  // EO_INTEGER
  { EA_NONE,  0 },  // EO_PUSHNULL
  { EA_NONE,  0 },  // EO_PUSH0
  { EA_NONE,  0 },  // EO_PUSH1

  // ref
  { EA_NONE,  0 },  // EO_REF_GETCRW
  { EA_NONE,  0 },  // EO_REF_SRCRSB
  { EA_NONE,  0 },  // EO_REF_ALTRSB

  // rsb / rowslab
  { EA_NONE,  0 },  // EO_RSB_DST_COPYSRC
  { EA_NONE,  0 },  // EO_RSB_DST_PRINT
  { EA_NONE,  0 },  // EO_RSB_DST_NEW
  { EA_NONE,  0 },  // EO_RSB_DST_TEST
  { EA_NONE,  0 },  // EO_RSB_SRC_BEGIN
  { EA_NONE,  0 },  // EO_RSB_SRC_NEXT
  { EA_NONE,  0 },  // EO_RSB_SRC_PRINT
  { EA_NONE,  0 },  // EO_RSB_SRC_PUSHOFS
  { EA_NONE,  0 },  // EO_RSB_SRC_SETBYOFS
  { EA_NONE,  0 },  // EO_RSB_ALT_BEGIN
  { EA_NONE,  0 },  // EO_RSB_ALT_NEXT
  { EA_NONE,  0 },  // EO_RSB_ALT_PRINT
  { EA_NONE,  0 },  // EO_RSB_ALT_PUSHOFS
  { EA_NONE,  0 },  // EO_RSB_ALT_SETBYOFS

  // relational operators
  { EA_NONE,  0 },  // EO_LESSTHAN
  { EA_NONE,  0 },  // EO_LESSEQUAL
  { EA_NONE,  0 },  // EO_EQUAL
  { EA_NONE,  0 },  // EO_NOTEQUAL
  { EA_NONE,  0 },  // EO_GREATEREQUAL
  { EA_NONE,  0 },  // EO_GREATERTHAN

  // rsb operators
  { EA_INT,   8 },  // EO_DEPOSITCOLI
  { EA_INT,   8 },  // EO_PUSHDSTCOLI
  { EA_INT,   8 },  // EO_PUSHSRCCOLI
  { EA_INT,   8 },  // EO_PUSHALTCOLI

  // stack operators
  { EA_NONE,  0 },  // EO_INC
  { EA_NONE,  0 },  // EO_DEC
  { EA_NONE,  0 },  // EO_DUP
  { EA_NONE,  0 },  // EO_POP
  { EA_NONE,  0 },  // EO_PRINT

  // subroutine operators
  { EA_LABEL, 8 },  // EO_CALL
  { EA_NONE,  0 },  // EO_RETURN

  // misc
  { EA_NONE,  0 },  // EO_BREAKPOINT
  { EA_NONE,  0 },  // EO_TRACE
  { EA_NONE,  0 },  // EO_NOOP
  { EA_NONE,  0 },  // EO_ERROR

  // -- end --
  { EA_NONE,  0 },  // EO_ENUMS
};


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate code for given test                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenCode(i8 iTestNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  switch (iTestNo) {
    case     100: vGenerateQuery100();    break;
    case     101: vGenerateQuery101();    break;
    case     102: vGenerateQuery102();    break;
      
    case     600: vGenerateQuery600();    break;
    case     601: vGenerateQuery601();    break;
      
    case   12000: vGenerateSort12000();   break;
      
    case   13000: vGenerateAgg13000();    break;
      
    case   14000: vGenerateAggGby14000(); break;
    case   14001: vGenerateAggGby14001(); break;
    case   14002: vGenerateAggGby14002(); break;
    default:
      NEVER_REACH;
  }
}

//! TEMP : TO GO

/*----------------------------------------------------------------------------*/
/* generate agg group by                                                      */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery100(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 100;
  
  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // insert each rsb
  
  // forearch src rsb
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NEXT_RSB_SRC:"_Stz);
  
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_RSB_SRC:"_Stz);
  
  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  // add key
  vAddCodeArg(EO_ENTWRK_APPENDELM,(i8)0);
  
  // find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
  vAddCode(EO_ENT_PUSH);
  vAddCode(EO_PUSH0);
  vAddCode(EO_NOTEQUAL);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_EQUAL);
  vAddCodeLabel(EO_JMP_TRUE,"FOUND"_Stz);
  
  // NOT_FOUND:
  vDefineLabel("NOT_FOUND"_Stz);
  vAddCodeArg(EO_ENTWRK_CREATEPAY, 8);
  vAddCode(EO_IXT_INS_ENT);
  vAddCodeArg(EO_PUSHSRCCOLI,(i8)0);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)0);
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)1);
  
  // FOUND:
  vDefineLabel("FOUND"_Stz);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)1);
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)0);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)1);
  
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate agg group by                                                      */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery101(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 101;
  
  m_i_Code = 0;
  
  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // begin index tree traversal
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"END_IXT_SRC:"_Stz);
  
  vDefineLabel("NEXT_IXT_SRC:"_Stz);
  
  // new a dst rsb row
  vAddCode(EO_RSB_DST_NEW);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)0);
  vAddCodeArg(EO_DEPOSITCOLI,(i8)0);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)1);
  vAddCodeArg(EO_DEPOSITCOLI,(i8)1);
  vAddCode(EO_RSB_DST_PRINT);
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"NEXT_IXT_SRC:"_Stz);
  
  //vAddCode(EO_IXT_TRV_NEXT);
  //vAddCodeLabel(EO_JMP_TRUE,"NEXT_IXT_SRC:"_Stz);
  
  vDefineLabel("END_IXT_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate super agg group by                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery102(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 102;
  
  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // insert each rsb
  
  // forearch src rsb
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NEXT_RSB_SRC:"_Stz);
  
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_RSB_SRC:"_Stz);
  
  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  // add key
  vAddCodeArg(EO_PUSHSRCCOLI,(i8)0);
  vAddCodeArg(EO_ENTWRK_APPENDELM,(i8)0);
  
  // find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
  vAddCode(EO_ENT_PUSH);
  vAddCode(EO_PUSH0);
  vAddCode(EO_NOTEQUAL);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_EQUAL);
  vAddCodeLabel(EO_JMP_TRUE,"FOUND"_Stz);
  
  // NOT_FOUND:
  vDefineLabel("NOT_FOUND"_Stz);
  vAddCodeArg(EO_ENTWRK_CREATEPAY, 8);
  vAddCode(EO_IXT_INS_ENT);
  vAddCodeArg(EO_PUSHSRCCOLI,(i8)0);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)0);
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)1);
  
  // FOUND:
  vDefineLabel("FOUND"_Stz);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)1);
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)1);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)1);
  
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}
/*----------------------------------------------------------------------------*/
/* generate query6                                                            */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery600(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // generate the lower bound date
  s32 sDateLower = "1994-01-01"_Stz;
  SN_Dte snDteLower;
  char* aDateLower = sDateLower.azChar();
  i8 iDateLower = snDteLower.i(&aDateLower, AM::pFmt());
  
  // generate the upper bound date
  s32 sDateUpper = "1995-01-01"_Stz;
  SN_Dte snDteUpper;
  char* aDateUpper = sDateUpper.azChar();
  i8 iDateUpper = snDteUpper.i(&aDateUpper, AM::pFmt());
  
  // set the query no
  m_iSql = 600;
  
  // init the code
  m_i_Code = 0;
  
  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // new a rsb dst row if not there
  vAddCode(EO_RSB_DST_TEST);
  vAddCodeLabel(EO_JMP_TRUE,"RSB_SRC_BEGIN:"_Stz);
  vAddCode(EO_RSB_DST_NEW);
  
  // begin RSB traversal
  vDefineLabel("RSB_SRC_BEGIN:"_Stz);
  vAddCode(EO_RSB_SRC_BEGIN);

  vDefineLabel("NEXT_RSB_SRC:"_Stz);
  
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_RSB_SRC:"_Stz);
  
  // call the filter subroutine
  vAddCodeLabel(EO_CALL, "SUBROUTINE_FILTER:"_Stz);
  vAddCodeLabel(EO_JMP_FALSE, "NEXT_RSB_SRC:"_Stz);
  
  // call the aggregate subroutine
  vAddCodeLabel(EO_CALL, "SUBROUTINE_AGGREGATE:"_Stz);
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FILTER SUBROUTINE
  vDefineLabel("SUBROUTINE_FILTER:"_Stz);

  vAddCodeArg(EO_PUSHSRCCOLI, (i8)10);   // l_shipdate  
  vAddCodeArg(EO_DATE, iDateLower);
  //vAddCodeArg(EO_DATE, "1994-01-01"_Stz);
  vAddCode(EO_GREATEREQUAL);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);

  vAddCodeArg(EO_PUSHSRCCOLI, (i8)10);   // l_shipdate
  vAddCodeArg(EO_DATE, iDateUpper);
  //vAddCodeArg(EO_DATE, "1995-01-01"_Stz);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);

  vAddCodeArg(EO_PUSHSRCCOLI, (i8)6);   // l_discount
  vAddCodeArg(EO_INTEGER, 5);
  vAddCode(EO_GREATEREQUAL);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);
  
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)6);   // l_discount
  vAddCodeArg(EO_INTEGER, 7);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);

  vAddCodeArg(EO_PUSHSRCCOLI, (i8)4);   // l_quantity
  vAddCodeArg(EO_INTEGER, 2400);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);

  // return true
  vAddCode(EO_PUSH1);
  vAddCodeLabel(EO_JMP,"RETURN:"_Stz);
  
  // return false
  vDefineLabel("RETURN_FALSE:"_Stz);
  vAddCode(EO_PUSH0);
  
  // return
  vDefineLabel("RETURN:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AGGREGATE SUBROUTINE
  vDefineLabel("SUBROUTINE_AGGREGATE:"_Stz);
  //vAddCode(EO_RSB_SRC_PRINT);
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)5);   // l_exetendedprice
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)6);   // l_discount
  vAddCode(EO_MULTIPLY);                // *
  vAddCodeArg(EO_INTEGER,100);
  vAddCode(EO_DIVIDE);
  
  vAddCodeArg(EO_PUSHDSTCOLI,(i8)0);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_DEPOSITCOLI,(i8)0);
  
  //vAddCode(EO_RSB_DST_PRINT);
  vAddCode(EO_RETURN);

  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate query601 (super agg for query 6)                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery601(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 601;
  
  // init the code
  m_i_Code = 0;
  
  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // new a rsb dst row if not there
  vAddCode(EO_RSB_DST_TEST);
  vAddCodeLabel(EO_JMP_TRUE,"RSB_SRC_BEGIN:"_Stz);
  vAddCode(EO_RSB_DST_NEW);
  
  // begin RSB traversal
  vDefineLabel("RSB_SRC_BEGIN:"_Stz);
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NEXT_RSB_SRC:"_Stz);
  
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_RSB_SRC:"_Stz);
  
  // call the aggregate subroutine
  vAddCodeLabel(EO_CALL, "SUBROUTINE_AGGREGATE:"_Stz);
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AGGREGATE SUBROUTINE
  vDefineLabel("SUBROUTINE_AGGREGATE:"_Stz);
  vAddCode(EO_RSB_SRC_PRINT);
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)0);
  vAddCodeArg(EO_PUSHDSTCOLI,(i8)0);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_DEPOSITCOLI,(i8)0);
  
  vAddCode(EO_RSB_DST_PRINT);
  vAddCode(EO_RETURN);
  
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate aggregate query 13000                                             */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateAgg13000(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 13000;
  
  // init the code
  m_i_Code = 0;

  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // new a rsb dst row if not there
  vAddCode(EO_RSB_DST_TEST);
  vAddCodeLabel(EO_JMP_TRUE,"RSB_SRC_BEGIN:"_Stz);
  vAddCode(EO_RSB_DST_NEW);
  
  // begin RSB traversal
  vDefineLabel("RSB_SRC_BEGIN:"_Stz);
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NEXT_RSB_SRC:"_Stz);
  
  vAddCode(EO_RSB_SRC_NEXT);
  
  vAddCodeLabel(EO_JMP_FALSE,"END_RSB_SRC:"_Stz);
  
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)0);
  vAddCodeArg(EO_PUSHDSTCOLI,(i8)0);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_DEPOSITCOLI,(i8)0);
  vAddCodeLabel(EO_JMP,"NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);
  vAddCode(EO_RSB_DST_PRINT);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate agg group by                                                      */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateAggGby14000(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 14000;
  
  // init the code
  m_i_Code = 0;
  
  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // insert each rsb
  
  // forearch src rsb
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NEXT_RSB_SRC:"_Stz);
  
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_RSB_SRC:"_Stz);
  
  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  // add key
  vAddCodeArg(EO_ENTWRK_APPENDELM,(i8)0);
  
  // find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
  vAddCode(EO_ENT_PUSH);
  vAddCode(EO_PUSH0);
  vAddCode(EO_NOTEQUAL);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_EQUAL);
  vAddCodeLabel(EO_JMP_TRUE,"FOUND"_Stz);
  
  // NOT_FOUND:
  vDefineLabel("NOT_FOUND"_Stz);
  vAddCodeArg(EO_ENTWRK_CREATEPAY, 8);
  vAddCode(EO_IXT_INS_ENT);
  vAddCodeArg(EO_PUSHSRCCOLI,(i8)0);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)0);
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)1);
  
  // FOUND:
  vDefineLabel("FOUND"_Stz);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)1);
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)0);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)1);
  
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate agg group by                                                      */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateAggGby14001(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 14001;
  
  // init the code
  m_i_Code = 0;
  
  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // begin index tree traversal
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"END_IXT_SRC:"_Stz);
  
  vDefineLabel("NEXT_IXT_SRC:"_Stz);
  
  // new a dst rsb row
  vAddCode(EO_RSB_DST_NEW);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)0);
  vAddCodeArg(EO_DEPOSITCOLI,(i8)0);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)1);
  vAddCodeArg(EO_DEPOSITCOLI,(i8)1);
  vAddCode(EO_RSB_DST_PRINT);
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"NEXT_IXT_SRC:"_Stz);
  
  //vAddCode(EO_IXT_TRV_NEXT);
  //vAddCodeLabel(EO_JMP_TRUE,"NEXT_IXT_SRC:"_Stz);
  
  vDefineLabel("END_IXT_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate super agg group by                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateAggGby14002(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 14002;
  
  // init the code
  m_i_Code = 0;
  
  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // insert each rsb
  
  // forearch src rsb
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NEXT_RSB_SRC:"_Stz);
  
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_RSB_SRC:"_Stz);
  
  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  // add key
  vAddCodeArg(EO_PUSHSRCCOLI,(i8)0);
  vAddCodeArg(EO_ENTWRK_APPENDELM,(i8)0);
  
  // find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
  vAddCode(EO_ENT_PUSH);
  vAddCode(EO_PUSH0);
  vAddCode(EO_NOTEQUAL);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_EQUAL);
  vAddCodeLabel(EO_JMP_TRUE,"FOUND"_Stz);
  
  // NOT_FOUND:
  vDefineLabel("NOT_FOUND"_Stz);
  vAddCodeArg(EO_ENTWRK_CREATEPAY, 8);
  vAddCode(EO_IXT_INS_ENT);
  vAddCodeArg(EO_PUSHSRCCOLI,(i8)0);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)0);
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)1);
  
  // FOUND:
  vDefineLabel("FOUND"_Stz);
  vAddCodeArg(EO_PAY_FETCH_COL, (i8)1);
  vAddCodeArg(EO_PUSHSRCCOLI, (i8)1);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COL, (i8)1);
  
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate sort test code                                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateSort12000(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  // set the query no
  m_iSql = 12000;
  
  // -- -- -- -- -- -- -- -- -- -- --
  // insert each rsb
  
  // forearch src rsb
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NEXT_RSB_SRC:"_Stz);

  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_RSB_SRC:"_Stz);

  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);

  // add key
  vAddCodeArg(EO_PUSHSRCCOLI,(i8)0);
  vAddCodeArg(EO_ENTWRK_APPENDELM,(i8)0);

  vAddCodeArg(EO_PUSHSRCCOLI,(i8)3);
  vAddCodeArg(EO_ENTWRK_APPENDELM,(i8)3);

  // add offset
  vAddCode(EO_RSB_SRC_PUSHOFS);
  vAddCode(EO_ENTWRK_POPREF);

  // insert entry into index tree
  vAddCode(EO_IXT_INS_ENT);

  vAddCodeLabel(EO_JMP,"NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);

  
  // -- -- -- -- -- -- -- -- -- -- --

  // copy src to dst rsb in index order

  // forearch src rsb
  vAddCode(EO_IXT_TRV_BEGIN);

  vDefineLabel("NEXT_IXT_ENT:"_Stz);

  vAddCode(EO_IXT_TRV_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_IXT_ENT:"_Stz);
  

  // ...
  vAddCode(EO_RSB_DST_NEW);
  vAddCode(EO_REF_SRCRSB);
  vAddCode(EO_RSB_DST_COPYSRC);

  vAddCodeLabel(EO_JMP,"NEXT_IXT_ENT:"_Stz);

  
  // -- -- -- -- -- -- -- -- -- -- --

  vDefineLabel("END_IXT_ENT:"_Stz);
  vAddCode(EO_RETURN);

  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* aggregate compute                                                          */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iAggregate(RowSlab* pSrcRowSlab, StreamDef* pStreamDefOut)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_pDstRowSlab = nullptr;
  
  m_aSrcRsb      = nullptr;
  m_pSrcRowSlab  = pSrcRowSlab;
  m_nSrcRows    = m_pSrcRowSlab->nRows();
  
  m_aAltRsb     = nullptr;
  m_pAltRowSlab = nullptr;
  
  pStreamDefOut->vToStreamPix(&m_oStreamPix,true);
  m_xpPot->vStreamPix(&m_oStreamPix);
  
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
i8 LogicUnit::iAggregateGby(Pot* pPot, IndexTree* pIndexTree, RowSlab* pDstRowSlab,
                             Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_xpPot       = pPot;
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
bool LogicUnit::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  TokenVec* pTknVec = TokenVec::pTokenise(m_sSource);
  
  bool b = bCompile(pTknVec);
  
  delete pTknVec;
  
  return b;
}


/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool LogicUnit::bCompile(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  bool b = bCompileCode(pTknVec);
  return b;
}

/*----------------------------------------------------------------------------*/
/* transform compute                                                          */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iTransform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iResult = iCompute();

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
void LogicUnit::vAddCodeLabel(eEO eCode, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  vAddCodeLabel(eCode,pTkn->szChars());
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
  
  vPrintLabels();
  
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
  
  vPrintLabels();
}

/*----------------------------------------------------------------------------*/
/* add label                                                                  */
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
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    vTrace("  %s %lld: %4lld\n"_Stz,
           m_xaJumpLabel[i_Label].m_sLabel.szStz(),
           m_xaJumpLabel[i_Label].m_bDefined,
           m_xaJumpLabel[i_Label].m_i_Chain);
}

/*----------------------------------------------------------------------------*/
/* get source                                                                 */
/*----------------------------------------------------------------------------*/
Stz* LogicUnit::szSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return m_sSource;
}

/*----------------------------------------------------------------------------*/
/* set source                                                                 */
/*----------------------------------------------------------------------------*/
void LogicUnit::vSource(Stz* szSource)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
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
/* compile short cut source in token vector                                   */
/*----------------------------------------------------------------------------*/
bool LogicUnit::bCompileCode(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8* piArg;
  i8  iArg = 0;

  i8 nTkns = pTknVec->nTokens();
  
  for (i8 i_Tkn = 0; i_Tkn < nTkns; i_Tkn++) {
    
    Token* pTkn = pTknVec->pGet(i_Tkn);
    vTrace(" >%s<\n"_Stz,pTkn->szChars());
    
    if (pTkn->bSame("0"_Stz))   { vAddCode(EO_PUSH0);               continue; }
    if (pTkn->bSame("1"_Stz))   { vAddCode(EO_PUSH1);               continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // arithmetic operators
    if (pTkn->bSame("+"_Stz))   { vAddCode(EO_ADD);                 continue; }
    if (pTkn->bSame("/"_Stz))   { vAddCode(EO_DIVIDE);              continue; }
    if (pTkn->bSame("%"_Stz))   { vAddCode(EO_MODULO);              continue; }
    if (pTkn->bSame("*"_Stz))   { vAddCode(EO_MULTIPLY);            continue; }
    if (pTkn->bSame("-"_Stz))   { vAddCode(EO_SUBTRACT);            continue; }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // date operators
    if (pTkn->bSame("dn"_Stz))  { vAddCode(EO_DTE_NOW);             continue; }
    if (pTkn->bSame("dp"_Stz))  { vAddCode(EO_DTE_PRINT);           continue; }
    if (pTkn->bSame("dy"_Stz))  { vAddCode(EO_DTE_YEAR);            continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // entry relational operators
    if (pTkn->bSame("<"_Stz))   { vAddCode(EO_LESSTHAN);            continue; }
    if (pTkn->bSame("<="_Stz))  { vAddCode(EO_LESSEQUAL);           continue; }
    if (pTkn->bSame("=="_Stz))  { vAddCode(EO_EQUAL);               continue; }
    if (pTkn->bSame("!="_Stz))  { vAddCode(EO_NOTEQUAL);            continue; }
    if (pTkn->bSame(">="_Stz))  { vAddCode(EO_GREATEREQUAL);        continue; }
    if (pTkn->bSame(">"_Stz))   { vAddCode(EO_GREATERTHAN);         continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // logical operators
    if (pTkn->bSame("and"_Stz)) { vAddCode(EO_AND);                 continue; }
    if (pTkn->bSame("or"_Stz))  { vAddCode(EO_OR);                  continue; }
    if (pTkn->bSame("not"_Stz)) { vAddCode(EO_NOT);                 continue; }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // null test operators
    if (pTkn->bSame("isn"_Stz)) { vAddCode(EO_ISNULL);              continue; }
    if (pTkn->bSame("inn"_Stz)) { vAddCode(EO_ISNOTNULL);           continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // stack operators
    if (pTkn->bSame("++"_Stz))  { vAddCode(EO_INC);                 continue; }
    if (pTkn->bSame("inc"_Stz)) { vAddCode(EO_INC);                 continue; }
    if (pTkn->bSame("--"_Stz))  { vAddCode(EO_DEC);                 continue; }
    if (pTkn->bSame("dec"_Stz)) { vAddCode(EO_DEC);                 continue; }
    if (pTkn->bSame("dup"_Stz)) { vAddCode(EO_DUP);                 continue; }
    if (pTkn->bSame("pop"_Stz)) { vAddCode(EO_POP);                 continue; }
    if (pTkn->bSame("."_Stz))   { vAddCode(EO_PRINT);               continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // rsb operators
    if (pTkn->bSame("bs"_Stz))  { vAddCode(EO_RSB_SRC_BEGIN);       continue; }
    if (pTkn->bSame("gs"_Stz))  { vAddCode(EO_RSB_SRC_NEXT);        continue; }
    if (pTkn->bSame("os"_Stz))  { vAddCode(EO_RSB_SRC_PUSHOFS);     continue; }
    if (pTkn->bSame("nd"_Stz))  { vAddCode(EO_RSB_DST_NEW);         continue; }

    if (pTkn->bSame("="_Stz))   { vAddCode(EO_RSB_DST_COPYSRC);     continue; }
    if (pTkn->bSame(".d"_Stz))  { vAddCode(EO_RSB_DST_PRINT);       continue; }
    if (pTkn->bSame(".s"_Stz))  { vAddCode(EO_RSB_SRC_PRINT);       continue; }
    if (pTkn->bSame(".a"_Stz))  { vAddCode(EO_RSB_ALT_PRINT);       continue; }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // finalise
    if (pTkn->bSame("ft"_Stz))  { vAddCode(EO_FIN_TRANSFORM);       continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // initialise
    if (pTkn->bSame("po"_Stz))  { vAddCode(EO_INI_PIX_DEFOUT);      continue; }
    if (pTkn->bSame("pi"_Stz))  { vAddCode(EO_INI_PIX_SLABIN);      continue; }
    if (pTkn->bSame("ss"_Stz))  { vAddCode(EO_INI_SRC_SLAB);        continue; }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // entWrk
    if (pTkn->bSame("wo"_Stz))  { vAddCode(EO_ENTWRK_POPOFS);       continue; }
    if (pTkn->bSame("wr"_Stz))  { vAddCode(EO_ENTWRK_RESET);        continue; }
    if (pTkn->bSame("ae"_Stz))  { vAddCode(EO_ENTWRK_APPENDELMI);   continue; }
    if (pTkn->bSame(".w"_Stz))  { vAddCode(EO_ENTWRK_PRINT);        continue; }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // index trees
    if (pTkn->bSame("xd"_Stz))  { vAddCode(EO_IXT_DUMP);            continue; }
    if (pTkn->bSame("xi"_Stz))  { vAddCode(EO_IXT_INS_ENT);         continue; }
    
    if (pTkn->bSame("xsb"_Stz)) { vAddCode(EO_IXT_SRC_BEGIN);       continue; }
    if (pTkn->bSame("xsn"_Stz)) { vAddCode(EO_IXT_SRC_NEXT);        continue; }
    if (pTkn->bSame("xsp"_Stz)) { vAddCode(EO_IXT_SRC_PRINT);       continue; }
    if (pTkn->bSame("xab"_Stz)) { vAddCode(EO_IXT_ALT_BEGIN);       continue; }
    if (pTkn->bSame("xan"_Stz)) { vAddCode(EO_IXT_ALT_NEXT);        continue; }
    if (pTkn->bSame("xap"_Stz)) { vAddCode(EO_IXT_ALT_PRINT);       continue; }
    if (pTkn->bSame("xcm"_Stz)) { vAddCode(EO_IXT_CMP_SRCALT);      continue; }

    if (pTkn->bSame("rsf"_Stz)) { vAddCode(EO_CRW_SRC_FETCH);       continue; }
    if (pTkn->bSame("raf"_Stz)) { vAddCode(EO_CRW_ALT_FETCH);       continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // subroutine operators
    if (pTkn->bSame("ret"_Stz)) { vAddCode(EO_RETURN);              continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // misc
    if (pTkn->bSame("bp"_Stz))  { vAddCode(EO_BREAKPOINT);          continue; }
    if (pTkn->bSame("tr"_Stz))  { vAddCode(EO_TRACE);               continue; }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
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

    // call subroutine
    if (pTkn->bSame("call"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_CALL,pTkn);
      continue;
    }
    
    // fetch column value from m_aSrcCrw
    if (pTkn->bSame("csf"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_COL_SRC_PUSH,pTkn);
      continue;
    }

    // fetch column value from m_aAltCrw
    if (pTkn->bSame("caf"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_COL_ALT_PUSH,pTkn);
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
      vAddCodeLabel(EO_JMP_FALSE,pTkn);
      continue;
    }
    
    // jump on true
    if (pTkn->bSame("jt"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMP_TRUE,pTkn);
      continue;
    }

    // jump on less than
    if (pTkn->bSame("jlt"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMP_LESS,pTkn);
      continue;
    }

    // jump on less than or equal to
    if (pTkn->bSame("jle"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMP_LESSEQUAL,pTkn);
      continue;
    }

    // jump on equal to
    if (pTkn->bSame("jeq"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMP_EQUAL,pTkn);
      continue;
    }

    // jump on not equal to
    if (pTkn->bSame("jne"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMP_NOTEQUAL,pTkn);
      continue;
    }

    // jump on greater than or equal to
    if (pTkn->bSame("jge"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMP_GREATEREQUAL,pTkn);
      continue;
    }

    // jump on greater than
    if (pTkn->bSame("jgt"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeLabel(EO_JMP_GREATER,pTkn);
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
    
    if (pTkn->bIsInteger()) {
      iArg = pTkn->iToI8();
      vAddCodeArg(EO_INTEGER,iArg);
      continue;
    }
    
    if (pTkn->bSame(C_NULL)) {
      vAddCodeArg(EO_INTEGER,C_DATUM_NULL);
      continue;
    }
    
    //    if (pTkn->bIsName()) {
    //      vAddCode(EO_PUSHCOLI;
    //      i8 iColVec;
    //      piArg = (i8*) & m_eaCode[m_i_Code];
    //      * piArg = pStreamDef->iFindColumn(pTkn->azChars(), iColVec);
    //      m_i_Code += sizeof(i8);
    //      continue;
    //    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    vAddCode(EO_RETURN);
    m_eaCode[0] = EO_ERROR;
    vTrace("Error: %s\n"_Stz,pTkn->szChars());
    return false;
  }

  vAddCode(EO_RETURN);
  vTrace("\n"_Stz);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (! m_xaJumpLabel[i_Label].m_bDefined) {
      m_eaCode[0] = EO_ERROR;
      vTrace("Error: undefined label %s\n"_Stz,
             m_xaJumpLabel[i_Label].m_sLabel.szStz());
      return false;
    }
  
  vDumpCode();

  return true;
}

/*----------------------------------------------------------------------------*/
/* dump code                                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDumpCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
      
  for (i8 i_Code = 0; i_Code < m_i_Code; ) {
    
    eEO eCode = m_eaCode[i_Code];
    
    if (eCode <= EO_NONE || eCode >= EO_ENUMS) {
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eo code sequence detected"_Stz);
    }
    
    if (EoArgs[eCode].m_nArgBytes) {
      //vDumpCode(eCode,i_Code);
      i8* piArg = (i8*) & m_eaCode[i_Code+1];
      i8 iArg = * piArg;
      vTrace("%04lld: %18s (%lld)\n"_Stz,i_Code,aEO[eCode],iArg);
      i_Code += 1 + sizeof(i8);
    }
    else {
      //vDumpCode(i_Code,eCode);
      vTrace("%04lld: %18s\n"_Stz,i_Code,aEO[eCode]);
      i_Code++;
    }
  }
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

  #define NoArg()

#endif


/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iCompute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iArg;
  i8 iTopA;
  i8 iTopB;
  i8 iRes;
  i8 iCmp;
  i8 i_Cell_ntv;
  
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
  
  for (;;) {
    
    // check for stack growth
    i8 nStkUse = nDifference(aiDstk,m_iaDataStack);
    i8 iStkPct = (nStkUse * 100) / C_LOGIC_STACK;
    if (iStkPct > 80)
      vTrace("WARNING: LogicUnit stack use %lld%%\n"_Stz,iStkPct);
    
    
    eCode = *aeCode++;
    
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
      {
        SP_Dte spDate;
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        vTrace("%s\n"_Stz,spDate.sz(iTopA));
      }
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
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,
               spEnt.sz(m_oEntWrk.aEnt(),m_pIndexTree->pIndexDef()));
        vTrace("\n\nEnt : %s\n"_Stz,spEnt.sz(m_pEnt,m_pIndexTree->pIndexDef()));
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
        m_oEntWrk.vAppendElement(m_aSrcRsb, m_pIndexTree->pIndexDef()->pGetColumn(iArg));
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pIndexTree->pIndexDef()));
        break;
      }
        
      case EO_ENTWRK_APPENDELMI: NoArg(); // append next element value (int)
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        m_oEntWrk.vAppendElementInt(m_aSrcRsb,iTopA);
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
        aEnt = m_oEntWrk.aEnt();
        pIndexDef = m_xpIndexTree->pIndexDef();
        vTrace("EntWrk : %s\n"_Stz,spEnt.sz(aEnt,pIndexDef));
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
      case EO_IXT_DUMP: NoArg(); // dump index tree
        m_pIndexTree->vDump(false);
        break;

      case EO_IXT_INS_ENT: NoArg(); // insert an entry into the indexTree
      {
        dLU0(eCode);
        SP_Ent spEnt;
        vTrace("Inserting entry"_Stz);
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pIndexTree->pIndexDef()));
        m_pEnt = m_pIndexTree->aInsertEntry(m_oEntWrk.aEnt());
        vTrace("\n\nEnt : %s\n"_Stz,spEnt.sz(m_pEnt,m_pIndexTree->pIndexDef()));
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
        vTrace("SrcEnt : %s\n"_Stz,spEnt.sz(m_aSrcEnt,pIndexDef));
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
        pIndexDef = m_pSrcIdxTree->pIndexDef();
        vTrace("AltEnt : %s\n"_Stz,spEnt.sz(m_aAltEnt,pIndexDef));
        break;
        
      case EO_IXT_JOI_MATCH: NoArg(); // test for join match (src.key == alt.key)
        iRes  = KC_SAME == Key::eCompare(m_aSrcEnt->aKey(),m_aAltEnt->aKey()) ? 1 : 0;
        dLU1(eCode,iRes);
        *(++aiDstk) = iRes;
        break;

      case EO_IXT_CMP_SRCALT: NoArg(); NEVER_REACH; // not yet implemented
        
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
        
      case EO_JMP_LESS:         GetArg(iArg); NEVER_REACH; // not yet implemented
      case EO_JMP_LESSEQUAL:    GetArg(iArg); NEVER_REACH; // not yet implemented
      case EO_JMP_EQUAL:        GetArg(iArg); NEVER_REACH; // not yet implemented
      case EO_JMP_NOTEQUAL:     GetArg(iArg); NEVER_REACH; // not yet implemented
      case EO_JMP_GREATEREQUAL: GetArg(iArg); NEVER_REACH; // not yet implemented
      case EO_JMP_GREATER:      GetArg(iArg); NEVER_REACH; // not yet implemented

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // logical operators
      case EO_AND: NoArg(); // logical and top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA && iTopB) ? 1 : 0;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_OR: NoArg(); // logical or top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = (iTopA || iTopB) ? 1 : 0;
        dLU3(eCode,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_NOT: NoArg(); // logical not top int, push result
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
      case EO_PAY_FETCH_COL: GetArg(iArg); // fetch a column from the entry payload
        m_aPayRsb = (Rsb*)m_pEnt->ayPayload();
        i_Cell_ntv = m_aPayRsb->i_ColToCell_ntv(iArg);
        iRes = m_aPayRsb->iVal_ntv(i_Cell_ntv);
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_PAY_STORE_COL: GetArg(iArg); // store a column of the entry payload
        iRes = *(aiDstk--);
        dLU2(eCode,iArg,iRes);
        m_aPayRsb = (Rsb*)m_pEnt->ayPayload();
        i_Cell_ntv = m_aPayRsb->i_ColToCell_ntv(iArg);
        m_aPayRsb->vVal_ntv(i_Cell_ntv, iRes);
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pushes
        
      case EO_DATE: GetArg(iArg); // push date
        dLU1(eCode,iArg);
        // convert date string to an m8
        
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
        m_aDstRsb = m_xpPot->aNewDstRsb(&m_pDstRowSlab);
        break;

      case EO_RSB_DST_TEST: NoArg(); // test if there is a destination rsb
        dLU0(eCode);
        if (m_aDstRsb)
          *(++aiDstk) = 1;
        else
          *(++aiDstk) = 0;
        break;
        
      case EO_RSB_SRC_BEGIN: NoArg(); // begin src rsb traverse
        dLU0(eCode);
        m_i_SrcRow = -1;
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
      case EO_DEPOSITCOLI: GetArg(iArg); // pop and deposit top into dst column
        iRes = *(aiDstk--);
        dLU2(eCode,iArg,iRes);
        m_pDstRowSlab->vPositionOnCell(iArg);
        m_pDstRowSlab->vDeposit(iRes);
        break;
        
      case EO_PUSHDSTCOLI: GetArg(iArg); // push dst column
        m_aDstRsb = m_pDstRowSlab->aRsbAdr(0);
        i_Cell_ntv = m_aDstRsb->i_ColToCell_ntv(iArg);
        iRes = m_aDstRsb->iVal_ntv(i_Cell_ntv);
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_PUSHSRCCOLI: GetArg(iArg); // push src column
        iRes = m_aSrcRsb->iVal_ntv(m_aSrcRsb->i_ColToCell_ntv(iArg));
        dLU2(eCode,iArg,iRes);
        *(++aiDstk) = iRes;
        break;
        
      case EO_PUSHALTCOLI: NEVER_REACH; // not yet implemented
      
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
        vTrace("%lld\n"_Stz,iTopA);
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
          return iRes;
        }
        aeCode = * (--aaeRstk);
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // misc
      case EO_BREAKPOINT: NoArg(); // breakpoint
        dLU0(eCode);
        vDebugPoint();
        break;
        
      case EO_TRACE: NoArg(); // set trace
        bTrace = (bool)*(aiDstk--);
        dLU1(eCode,(i8)bTrace);
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
