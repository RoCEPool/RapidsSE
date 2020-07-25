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
  { EA_NONE,    0 },  // EO_NONE
  
  // arithmetic operators
  { EA_NONE,    0 },  // EO_ADD
  { EA_NONE,    0 },  // EO_DIVIDE
  { EA_NONE,    0 },  // EO_MODULO
  { EA_NONE,    0 },  // EO_MULTIPLY
  { EA_NONE,    0 },  // EO_NEGATE
  { EA_NONE,    0 },  // EO_SUBTRACT

  // col operators
  { EA_INT,     8 },  // EO_COL_SRC_PUSH
  { EA_INT,     8 },  // EO_COL_SRC_PUSHSTR
  { EA_INT,     8 },  // EO_COL_ALT_PUSH
  
  // crw operators
  { EA_NONE,    0 },  // EO_CRW_SRC_FETCH
  { EA_NONE,    0 },  // EO_CRW_ALT_FETCH
  
  // date operators
  { EA_NONE,    0 },  // EO_DTE_NOW
  { EA_NONE,    0 },  // EO_DTE_PRINT
  { EA_NONE,    0 },  // EO_DTE_YEAR
  
  // entry
  { EA_NONE,    0 },  // EO_ENT_NEXT
  { EA_NONE,    0 },  // EO_ENT_POSPRIOR
  { EA_NONE,    0 },  // EO_ENT_PUSH
  { EA_NONE,    0 },  // EO_ENT_PUSHELMI0
  { EA_NONE,    0 },  // EO_ENT_PUSHOFS
  { EA_NONE,    0 },  // EO_ENT_PUSHREF

  // entry relational operators
  { EA_NONE,    0 },  // EO_ENT_LESS
  { EA_NONE,    0 },  // EO_ENT_LESSEQUAL
  { EA_NONE,    0 },  // EO_ENT_EQUAL
  { EA_NONE,    0 },  // EO_ENT_NOTEQUAL
  { EA_NONE,    0 },  // EO_ENT_GREATEREQUAL
  { EA_NONE,    0 },  // EO_ENT_GREATER

  // entWrk
  { EA_INT,     8 },  // EO_ENTWRK_APPENDELM
  { EA_NONE,    0 },  // EO_ENTWRK_APPENDELMI
  { EA_INT,     8 },  // EO_ENTWRK_CREATEPAY
  { EA_NONE,    0 },  // EO_ENTWRK_POPOFS
  { EA_NONE,    0 },  // EO_ENTWRK_POPREF
  { EA_NONE,    0 },  // EO_ENTWRK_PRINT
  { EA_NONE,    0 },  // EO_ENTWRK_RESET

  // finalise
  { EA_NONE,    0 },  // EO_FIN_TRANSFORM
  
  // initialise
  { EA_NONE,    0 },  // EO_INI_PIX_DEFOUT
  { EA_NONE,    0 },  // EO_INI_PIX_SLABIN
  { EA_NONE,    0 },  // EO_INI_SRC_SLAB
  
  // index tree
  { EA_NONE,    0 },  // EO_IXT_DEL_ENT
  { EA_NONE,    0 },  // EO_IXT_DUMP
  { EA_NONE,    0 },  // EO_IXT_INS_ENT
  { EA_NONE,    0 },  // EO_IXT_TRV_BEGIN
  { EA_NONE,    0 },  // EO_IXT_TRV_NEXT
  { EA_NONE,    0 },  // EO_IXT_TMP_ENTRIES

  { EA_NONE,    0 },  // EO_IXT_SRC_BEGIN
  { EA_NONE,    0 },  // EO_IXT_SRC_NEXT
  { EA_NONE,    0 },  // EO_IXT_SRC_PRINT
  { EA_NONE,    0 },  // EO_IXT_ALT_BEGIN
  { EA_NONE,    0 },  // EO_IXT_ALT_NEXT
  { EA_NONE,    0 },  // EO_IXT_ALT_PRINT
  { EA_NONE,    0 },  // EO_IXT_JOI_MATCH
  { EA_NONE,    0 },  // EO_IXT_CMP_SRCALT
  { EA_NONE,    0 },  // EO_IXT_CMP_WRKALT
  { EA_NONE,    0 },  // EO_IXT_POS_ALTBYSRC
  { EA_NONE,    0 },  // EO_IXT_POS_ALTBYWRK

  // jump operators
  { EA_LABEL,   8 },  // EO_JMP
  { EA_LABEL,   8 },  // EO_JMP_FALSE
  { EA_LABEL,   8 },  // EO_JMP_TRUE

  { EA_LABEL,   8 },  // EO_JMP_LESSTHAN
  { EA_LABEL,   8 },  // EO_JMP_LESSEQUAL
  { EA_LABEL,   8 },  // EO_JMP_EQUAL
  { EA_LABEL,   8 },  // EO_JMP_NOTEQUAL
  { EA_LABEL,   8 },  // EO_JMP_GREATEREQUAL
  { EA_LABEL,   8 },  // EO_JMP_GREATEREQUAL

  // logical operators
  { EA_NONE,    0 },  // EO_LGL_AND
  { EA_NONE,    0 },  // EO_LGL_OR
  { EA_NONE,    0 },  // EO_LGL_NOT

  // null test operators
  { EA_NONE,    0 },  // EO_ISNULL
  { EA_NONE,    0 },  // EO_ISNOTNULL

  // pay load
  { EA_INT,     8 },  // EO_PAY_FETCH_COLI
  { EA_INT,     8 },  // EO_PAY_FETCH_COLS
  { EA_INT,     8 },  // EO_PAY_STORE_COLI
  { EA_INT,     8 },  // EO_PAY_STORE_COLS

  // pix
  { EA_NONE,    0 },  // EO_PIX_STM_PRINT
  
  // partitioning
  { EA_NONE,    0 },  // EO_PTN_DST
  { EA_NONE,    0 },  // EO_PTN_SET

  // pushes
  { EA_DATE,    8 },  // EO_DATE
  { EA_INT,     8 },  // EO_INTEGER
  { EA_NONE,    0 },  // EO_PUSHNULL
  { EA_NONE,    0 },  // EO_PUSH0
  { EA_NONE,    0 },  // EO_PUSH1

  // ref
  { EA_NONE,    0 },  // EO_REF_GETCRW
  { EA_NONE,    0 },  // EO_REF_SRCRSB
  { EA_NONE,    0 },  // EO_REF_ALTRSB

  // rsb / rowslab
  { EA_NONE,    0 },  // EO_RSB_DST_COPYSRC
  { EA_NONE,    0 },  // EO_RSB_DST_PRINT
  { EA_NONE,    0 },  // EO_RSB_DST_RESET
  { EA_NONE,    0 },  // EO_RSB_DST_NEW
  { EA_NONE,    0 },  // EO_RSB_DST_TEST
  { EA_NONE,    0 },  // EO_RSB_DST_ZERO
  { EA_NONE,    0 },  // EO_RSB_SRC_BEGIN
  { EA_NONE,    0 },  // EO_RSB_SRC_NEXT
  { EA_NONE,    0 },  // EO_RSB_SRC_PRINT
  { EA_NONE,    0 },  // EO_RSB_SRC_PUSHOFS
  { EA_NONE,    0 },  // EO_RSB_SRC_SETBYOFS
  { EA_NONE,    0 },  // EO_RSB_ALT_BEGIN
  { EA_NONE,    0 },  // EO_RSB_ALT_NEXT
  { EA_NONE,    0 },  // EO_RSB_ALT_PRINT
  { EA_NONE,    0 },  // EO_RSB_ALT_PUSHOFS
  { EA_NONE,    0 },  // EO_RSB_ALT_SETBYOFS

  // relational operators
  { EA_NONE,    0 },  // EO_LESSTHAN
  { EA_NONE,    0 },  // EO_LESSEQUAL
  { EA_NONE,    0 },  // EO_EQUAL
  { EA_NONE,    0 },  // EO_NOTEQUAL
  { EA_NONE,    0 },  // EO_GREATEREQUAL
  { EA_NONE,    0 },  // EO_GREATERTHAN

  // rsb operators
  { EA_INT,     8 },  // EO_RSB_DST_POPCOLI
  { EA_INT,     8 },  // EO_RSB_DST_POPCOLS
  { EA_INT,     8 },  // EO_RSB_DST_PUSHCOLI
  { EA_INT,     8 },  // EO_RSB_SRC_PUSHCOLI
  { EA_INT,     8 },  // EO_RSB_SRC_PUSHCOLS
  { EA_INT,     8 },  // EO_RSB_ALT_PUSHCOLI

  // stack operators
  { EA_NONE,    0 },  // EO_INC
  { EA_NONE,    0 },  // EO_DEC
  { EA_NONE,    0 },  // EO_DUP
  { EA_NONE,    0 },  // EO_POP
  { EA_NONE,    0 },  // EO_PRINT

  // string operators
  { EA_NONE,    0 },  // EO_STR_COMPARE
  { EA_NONE,    0 },  // EO_STR_PRINTADR
  { EA_NONE,    0 },  // EO_STR_PRINTSTK
  { EA_STRING, -1 },  // EO_STR_LITERAL
  
  // subroutine operators
  { EA_LABEL,   8 },  // EO_CALL
  { EA_NONE,    0 },  // EO_RETURN

  // misc
  { EA_NONE,    0 },  // EO_BREAKPOINT
  { EA_NONE,    0 },  // EO_TRACE
  { EA_NONE,    0 },  // EO_NOOP
  { EA_NONE,    0 },  // EO_ERROR

  // -- end --
  { EA_NONE,    0 },  // EO_ENUMS
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
    case     103: vGenerateQuery103();    break;
 
    case     300: vGenerateQuery300();    break;
    case     301: vGenerateQuery301();    break;

    case     400: vGenerateQuery400();    break;

    case     600: vGenerateQuery600();    break;
    case     601: vGenerateQuery601();    break;
  
    case  100000: vGenerate100000();      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* generate agg group by (Query 1)                                            */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery100(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 100;
  
  // reset
  m_i_Code = 0;
  
  // initialize srcSlab etc
  vAddCode(EO_INI_SRC_SLAB);
  
  // init m_pPot pix from streamDef out
  vAddCode(EO_INI_PIX_DEFOUT);
  
  // -- -- -- -- -- -- -- -- -- -- --
  // insert each rsb -> indexTree
  
  // forearch src rsb
  vAddCode(EO_RSB_SRC_BEGIN);
  
  // NEXT_RSB_SRC:
  vDefineLabel("NEXT_RSB_SRC:"_Stz);
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_RSB_SRC:"_Stz);
  
  // call the filter subroutine
  vAddCodeLabel(EO_CALL, "SUBROUTINE_FILTER:"_Stz);
  vAddCodeLabel(EO_JMP_FALSE, "NEXT_RSB_SRC:"_Stz);
  
  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  // add key L_RETURNFLAG, L_LINESTATUS
  vAddCodeArg(EO_ENTWRK_APPENDELM, (i8)L_RETURNFLAG);
  vAddCodeArg(EO_ENTWRK_APPENDELM, (i8)L_LINESTATUS);
  
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
  vAddCodeArg(EO_ENTWRK_CREATEPAY, Q100_COLUMNS * C_BYTES_PER_WORD);
  vAddCode(EO_IXT_INS_ENT);
  vAddCode(EO_IXT_DUMP);

  // store L_RETURNFLAG -> Q100_RETURNFLAG
  vAddCodeArg(EO_RSB_SRC_PUSHCOLS, (i8)L_RETURNFLAG);
  vAddCodeArg(EO_PAY_STORE_COLS, (i8)Q100_RETURNFLAG);
  
  // store Q1_LINESTATUS -> Q100_LINESTATUS
  vAddCodeArg(EO_RSB_SRC_PUSHCOLS, (i8)L_LINESTATUS);
  vAddCodeArg(EO_PAY_STORE_COLS, (i8)Q100_LINESTATUS);

  // set sum(L_QUANTITY) -> Q100_SUM_QTY
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_QTY);
  
  // set sum(L_QUANTITY) -> Q100_SUM_BASE_PRICE
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_BASE_PRICE);
  
  // set sum(L_QUANTITY) -> Q1_SUM_DISC_PRICE
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_DISC_PRICE);
  
  // set sum(L_QUANTITY) -> Q100_SUM_CHARGE
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_CHARGE);
  vAddCode(EO_IXT_DUMP);
  
  // FOUND:
  vDefineLabel("FOUND"_Stz);

  // sum(L_QUANTITY) -> Q100_SUM_QTY
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_QTY);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_QUANTITY);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_QTY);

  // SUM(L_EXTENDEDPRICE) -> Q100_SUM_BASE_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_BASE_PRICE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_EXTENDEDPRICE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_BASE_PRICE);

  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)) -> Q100_SUM_DISC_PRICE
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_EXTENDEDPRICE);
  vAddCodeArg(EO_INTEGER,100);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_DISCOUNT);
  vAddCode(EO_SUBTRACT);
  vAddCode(EO_MULTIPLY);
  vAddCodeArg(EO_INTEGER,100);
  vAddCode(EO_DIVIDE);

  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_DISC_PRICE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_DISC_PRICE);

  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)*(1+L_TAX)) -> Q100_SUM_CHARGE
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_EXTENDEDPRICE);
  
  vAddCodeArg(EO_INTEGER,100);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_DISCOUNT);
  vAddCode(EO_SUBTRACT);
  
  vAddCode(EO_MULTIPLY);
  
  vAddCodeArg(EO_INTEGER,100);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_TAX);
  vAddCode(EO_ADD);
  
  vAddCode(EO_MULTIPLY);
  
  vAddCodeArg(EO_INTEGER,10000);
  vAddCode(EO_DIVIDE);
  
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_CHARGE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_CHARGE);

  // COUNT(L_QUANTITY) -> Q100_COUNT_QTY
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_QUANTITY);
  vAddCode(EO_ISNULL);
  vAddCodeLabel(EO_JMP_TRUE,"L_QUANTITY_NULL:"_Stz);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_QTY);
  vAddCode(EO_INC);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_COUNT_QTY);
  
  // COUNT(L_EXTENDEDPRICE) -> Q100_COUNT_PRICE
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_EXTENDEDPRICE);
  vAddCode(EO_ISNULL);
  vAddCodeLabel(EO_JMP_TRUE,"L_EXTENTEDPRICE_NULL:"_Stz);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_PRICE);
  vAddCode(EO_INC);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_COUNT_PRICE);
  
  vDefineLabel("L_EXTENTEDPRICE_NULL:"_Stz);
  
  // SUM(L_DISCOUNT) -> Q100_SUM_BASE_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_DISC);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_DISCOUNT);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_DISC);
  
  // COUNT(L_DISCOUNT) -> Q100_COUNT_DISC
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_DISCOUNT);
  vAddCode(EO_ISNULL);
  vAddCodeLabel(EO_JMP_TRUE,"L_DISCOUNT_NULL:"_Stz);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_DISC);
  vAddCode(EO_INC);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_COUNT_DISC);
  
  vDefineLabel("L_DISCOUNT_NULL:"_Stz);
  
  // COUNT(*) -> Q100_COUNT_ORDER
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_ORDER);
  vAddCode(EO_INC);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_COUNT_ORDER);
  
  vAddCode(EO_IXT_DUMP);
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FILTER SUBROUTINE
  vDefineLabel("SUBROUTINE_FILTER:"_Stz);
  
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_SHIPDATE);
  vAddCodeArg(EO_DATE, "1998-09-01"_Stz);
  vAddCode(EO_GREATEREQUAL);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);
  
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_SHIPDATE);
  vAddCodeArg(EO_DATE, "1998-12-01"_Stz);
  vAddCode(EO_LESSEQUAL);
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
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate copy for agg group by (Query 1)                                   */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery101(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 101;
  
  // reset
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
  
  vAddCodeArg(EO_PAY_FETCH_COLS, (i8)Q100_RETURNFLAG);
  vAddCodeArg(EO_RSB_DST_POPCOLS,(i8)Q101_RETURNFLAG);
  
  vAddCodeArg(EO_PAY_FETCH_COLS, (i8)Q100_LINESTATUS);
  vAddCodeArg(EO_RSB_DST_POPCOLS,(i8)Q101_LINESTATUS);
  
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_QTY);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q101_SUM_QTY);
  
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_BASE_PRICE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q101_SUM_BASE_PRICE);

  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_DISC_PRICE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q101_SUM_DISC_PRICE);
  
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_CHARGE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q101_SUM_CHARGE);
  
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_QTY);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q101_COUNT_QTY);
  
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_PRICE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q101_COUNT_PRICE);
  
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_DISC);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q101_SUM_DISC);
  
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_DISC);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q101_COUNT_DISC);
  
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_ORDER);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q101_COUNT_ORDER);
  
  vAddCode(EO_RSB_DST_PRINT);
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"NEXT_IXT_SRC:"_Stz);
  
  vDefineLabel("END_IXT_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate super agg group by (Query 1)                                      */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery102(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 102;
  
  // reset
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
  
  // add key L_RETURNFLAG, L_LINESTATUS
  vAddCodeArg(EO_ENTWRK_APPENDELM, (i8)Q101_RETURNFLAG);
  vAddCodeArg(EO_ENTWRK_APPENDELM, (i8)Q101_LINESTATUS);
  
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
  vAddCodeArg(EO_ENTWRK_CREATEPAY, Q101_COLUMNS * C_BYTES_PER_WORD);
  vAddCode(EO_IXT_INS_ENT);
  //vAddCode(EO_IXT_DUMP);
  
  // store Q101_RETURNFLAG -> Q101_RETURNFLAG
  vAddCodeArg(EO_RSB_SRC_PUSHCOLS, (i8)Q101_RETURNFLAG);
  vAddCodeArg(EO_PAY_STORE_COLS, (i8)Q101_RETURNFLAG);
  
  // store Q101_LINESTATUS -> Q101_LINESTATUS
  vAddCodeArg(EO_RSB_SRC_PUSHCOLS, (i8)Q101_LINESTATUS);
  vAddCodeArg(EO_PAY_STORE_COLS, (i8)Q101_LINESTATUS);
  
  // store Q101_SUM_QTY -> Q101_SUM_QTY
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_QTY);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_QTY);
  
  // store Q101_SUM_BASE_PRICE -> Q101_SUM_BASE_PRICE
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_BASE_PRICE);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_BASE_PRICE);
  
  // store Q101_SUM_DISC_PRICE -> Q101_SUM_DISC_PRICE
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_DISC_PRICE);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_DISC_PRICE);
  
  // store Q101_SUM_CHARGE -> Q101_SUM_CHARGE
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_CHARGE);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_CHARGE);
  
  // store Q101_COUNT_QTY -> Q101_COUNT_QTY
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_COUNT_QTY);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_COUNT_QTY);
  
  // store Q101_COUNT_PRICE -> Q101_COUNT_PRICE
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_COUNT_PRICE);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_COUNT_PRICE);
  
  // store Q101_SUM_DISC -> Q101_SUM_DISC
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_DISC);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_DISC);
  
  // store Q101_COUNT_DISC -> Q101_COUNT_DISC
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_COUNT_DISC);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_COUNT_DISC);
  
  // store Q101_COUNT_ORDER -> Q101_COUNT_ORDER
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_COUNT_ORDER);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_COUNT_ORDER);
  
  //vAddCode(EO_IXT_DUMP);
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  // FOUND:
  vDefineLabel("FOUND"_Stz);
  
  // add Q101_SUM_QTY
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_QTY);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_QTY);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_QTY);
  
  // add Q101_SUM_BASE_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_BASE_PRICE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_BASE_PRICE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_BASE_PRICE);
  
  // add Q101_SUM_DISC_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_DISC_PRICE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_DISC_PRICE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_DISC_PRICE);
  
  // add Q101_SUM_CHARGE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_CHARGE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_CHARGE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_CHARGE);
  
  // add vAddCode(Q101_COUNT_QTY);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_QTY);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_COUNT_QTY);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_COUNT_QTY);
  
  // add vAddCode(Q101_COUNT_PRICE);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_PRICE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_COUNT_PRICE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_COUNT_PRICE);
  
  // add Q101_SUM_DISC
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_DISC);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_SUM_DISC);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_SUM_DISC);
  
  // add vAddCode(Q101_COUNT_DISC);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_DISC);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_COUNT_DISC);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_COUNT_DISC);
  
  // add vAddCode(Q101_COUNT_ORDER);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_ORDER);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q101_COUNT_ORDER);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q101_COUNT_ORDER);
  
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("END_RSB_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate copy for super agg group by (Query 1)                             */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery103(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 103;
  
  // reset
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
  
  // Q101_RETURNFLAG -> Q103_RETURNFLAG
  vAddCodeArg(EO_PAY_FETCH_COLS, (i8)Q101_RETURNFLAG);
  vAddCodeArg(EO_RSB_DST_POPCOLS,(i8)Q103_RETURNFLAG);
  
  // Q101_LINESTATUS -> Q103_LINESTATUS
  vAddCodeArg(EO_PAY_FETCH_COLS, (i8)Q101_LINESTATUS);
  vAddCodeArg(EO_RSB_DST_POPCOLS,(i8)Q103_LINESTATUS);
  
  // Q101_SUM_QTY -> Q103_SUM_QTY
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_QTY);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q103_SUM_QTY);
  
  // Q101_SUM_BASE_PRICE -> Q103_SUM_BASE_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_BASE_PRICE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q103_SUM_BASE_PRICE);
  
  // Q101_SUM_DISC_PRICE -> Q103_SUM_DISC_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_DISC_PRICE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q103_SUM_DISC_PRICE);
  
  // Q101_SUM_CHARGE -> Q103_SUM_CHARGE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_CHARGE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q103_SUM_CHARGE);
  
  // Q101_SUM_QTY/Q101_COUNT_QTY -> Q103_AVG_QTY
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_QTY);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_QTY);
  vAddCode(EO_DIVIDE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q103_AVG_QTY);
  
  // Q101_SUM_BASE_PRICE/Q101_COUNT_PRICE -> Q103_AVG_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_BASE_PRICE);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_PRICE);
  vAddCode(EO_DIVIDE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q103_AVG_PRICE);
  
  // Q101_SUM_DISC/Q101_COUNT_DISC -> Q103_AVG_DISC
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_DISC);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_DISC);
  vAddCode(EO_DIVIDE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q103_AVG_DISC);
  
  // Q101_COUNT_ORDER -> Q103_COUNT_ORDER
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_ORDER);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q103_COUNT_ORDER);
  
  vAddCode(EO_RSB_DST_PRINT);
  
  // NEXT_IXT_SRC:
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"NEXT_IXT_SRC:"_Stz);
  
  vDefineLabel("END_IXT_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 3                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery300(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 300;
  
  m_i_Code = 0;
    
  vAddCodeString(EO_STR_LITERAL,"Hello World!"_Stz);
  vAddCode(EO_STR_PRINTADR);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // iterate all CUSTOMER
  vAddCode(EO_IXT_SRC_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"END:"_Stz);
  vDefineLabel("GOT_S:"_Stz);
  vAddCode(EO_IXT_SRC_PRINT);
  
  vAddCode(EO_CRW_SRC_FETCH);
  vAddCodeArg(EO_COL_SRC_PUSHSTR,C_MKTSEGMENT);
  vAddCode(EO_STR_PRINTSTK);
  
  vAddCodeArg(EO_COL_SRC_PUSHSTR,C_MKTSEGMENT);
  vAddCodeString(EO_STR_LITERAL,"BUILDING"_Stz);
  vAddCode(EO_STR_COMPARE);
  vAddCodeLabel(EO_JMP_NOTEQUAL,"NXT_S:"_Stz);

  
  vAddCodeLabel(EO_CALL,"JOI_A:"_Stz);


  // next CUSTOMER
  vDefineLabel("NXT_S:"_Stz);
  vAddCode(EO_IXT_SRC_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"GOT_S:"_Stz);

  // end of CUSTOMER
  vDefineLabel("END:"_Stz);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // join ORDERS
  vDefineLabel("JOI_A:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"Join ORDERS"_Stz);
  vAddCode(EO_STR_PRINTADR);
  
  vAddCode(EO_IXT_POS_ALTBYSRC);
  vAddCodeLabel(EO_JMP_FALSE,"RET_A:"_Stz);
  vDefineLabel("NXT_A:"_Stz);
  vAddCode(EO_IXT_ALT_NEXT);
  
  vAddCode(EO_IXT_SRC_PRINT);
  vAddCode(EO_IXT_ALT_PRINT);


  vAddCodeLabel(EO_JMP_FALSE,"RET_A:"_Stz);
  vAddCode(EO_IXT_CMP_SRCALT);
  vAddCodeLabel(EO_JMP_NOTEQUAL,"RET_A:"_Stz);

  vAddCode(EO_IXT_ALT_PRINT);
  
  vAddCode(EO_CRW_ALT_FETCH);
  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERDATE);
  vAddCodeArg(EO_DATE, "1995-03-15"_Stz);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_A:"_Stz);

  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERDATE);
  vAddCode(EO_DTE_PRINT);

  vAddCode(EO_RSB_DST_NEW);
  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERKEY);
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_ORDERKEY);
  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERDATE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_ORDERDATE);
  vAddCodeArg(EO_COL_ALT_PUSH,O_SHIPPRIORITY);
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_SHIPPRIORITY);
  vAddCode(EO_RSB_DST_PRINT);

  vAddCodeLabel(EO_JMP,"NXT_A:"_Stz);
  
  vDefineLabel("RET_A:"_Stz);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 3                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery301(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 301;
  
  m_i_Code = 0;
  
  vAddCode(EO_PUSH1);
  vAddCode(EO_TRACE);
  vAddCode(EO_PIX_STM_PRINT);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vAddCode(EO_RSB_DST_NEW);

  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NXT_SRC:"_Stz);
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_SRC:"_Stz);

  vAddCodeLabel(EO_CALL,"MOV_DST:"_Stz);
  vAddCodeLabel(EO_CALL,"AGG_REVENUE:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_SRC:"_Stz);

  vDefineLabel("END_SRC:"_Stz);
  
  vAddCodeLabel(EO_CALL,"TRV_IXT:"_Stz);
  
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("MOV_DST:"_Stz);
  
  vAddCode(EO_RSB_SRC_PRINT);

  vAddCode(EO_RSB_DST_ZERO);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERKEY);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_ORDERKEY);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERDATE);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_ORDERDATE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_SHIPPRIORITY);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_SHIPPRIORITY);

  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("AGG_REVENUE:"_Stz);
  
  vAddCode(EO_ENTWRK_RESET);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERKEY);
  vAddCode(EO_ENTWRK_APPENDELMI);

  vAddCode(EO_IXT_POS_ALTBYWRK);
  vAddCodeLabel(EO_JMP_FALSE,"RET_AGG_f:"_Stz);
  vDefineLabel("NXT_AGG:"_Stz);
  vAddCode(EO_IXT_ALT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"RET_AGG_f:"_Stz);
  vAddCode(EO_IXT_ALT_PRINT);
  vAddCode(EO_IXT_CMP_WRKALT);
  vAddCodeLabel(EO_JMP_NOTEQUAL,"RET_AGG_f:"_Stz);
  
  vAddCode(EO_CRW_ALT_FETCH);

  vAddCodeArg(EO_RSB_DST_PUSHCOLI,Q301_REVENUE);
  vAddCodeArg(EO_COL_ALT_PUSH,L_EXTENDEDPRICE);
  vAddCode(EO_ADD);

  vAddCodeArg(EO_INTEGER,100);
  vAddCodeArg(EO_COL_ALT_PUSH,L_DISCOUNT);
  vAddCode(EO_SUBTRACT);
  
  vAddCode(EO_MULTIPLY);
  vAddCodeArg(EO_INTEGER,100);
  vAddCode(EO_DIVIDE);

  vAddCodeArg(EO_RSB_DST_POPCOLI,Q301_REVENUE);
  vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_REVENUE);
  vAddCode(EO_PRINT);

  vAddCodeLabel(EO_JMP,"NXT_AGG:"_Stz);
  
  vDefineLabel("RET_AGG_f:"_Stz);
  
  // not quite right...
  vAddCodeLabel(EO_CALL,"LIM_SBR:"_Stz);
  
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("LIM_SBR:"_Stz);

  vAddCode(EO_IXT_TMP_ENTRIES);
  vAddCode(EO_PRINT);

  vAddCode(EO_IXT_TMP_ENTRIES);
  vAddCodeArg(EO_INTEGER,10);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_TRUE,"LIM_INS:"_Stz);

  // -- -- -- -- -- -- -- -- --
  vDefineLabel("LIM_TST:"_Stz);
  
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"LIM_RET:"_Stz); // need an assert

  vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_REVENUE);
  vAddCode(EO_ENT_PUSHELMI0);
  vAddCode(EO_LESSEQUAL);
  vAddCodeLabel(EO_JMP_TRUE,"LIM_RET:"_Stz);

  vAddCode(EO_IXT_DEL_ENT);

  
  // -- -- -- -- -- -- -- -- --
  vDefineLabel("LIM_INS:"_Stz);

  vAddCode(EO_ENTWRK_RESET);

  vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_REVENUE);
  vAddCode(EO_DUP);
  vAddCode(EO_PRINT);
  vAddCode(EO_ENTWRK_APPENDELMI);
  
  vAddCodeArg(EO_ENTWRK_CREATEPAY, 3 * C_BYTES_PER_WORD);

  vAddCode(EO_IXT_INS_ENT);
  
  vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_ORDERKEY);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)0);
  vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_ORDERDATE);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)1);
  vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_SHIPPRIORITY);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)2);


  // -- -- -- -- -- -- -- -- --
  vDefineLabel("LIM_RET:"_Stz);
  vAddCode(EO_IXT_DUMP);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  vDefineLabel("TRV_IXT:"_Stz);
//
//  vAddCode(EO_IXT_TRV_BEGIN);
//  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);
//
//  vDefineLabel("TRV_ECH:"_Stz);
//
//  vAddCode(EO_ENT_PUSHELMI0);
//  vAddCode(EO_PRINT);
//  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)0);
//  vAddCode(EO_PRINT);
//  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)1);
//  vAddCode(EO_DTE_PRINT);
//  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)2);
//  vAddCode(EO_PRINT);
//
//
//  vDefineLabel("TRV_NXT:"_Stz);
//  vAddCode(EO_IXT_TRV_NEXT);
//  vAddCodeLabel(EO_JMP_TRUE,"TRV_ECH:"_Stz);
//
//  vDefineLabel("TRV_RET:"_Stz);
//  vAddCode(EO_BREAKPOINT);
//  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vDefineLabel("TRV_IXT:"_Stz);
  
  vAddCode(EO_RSB_DST_RESET);

  
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);

  vDefineLabel("TRV_ECH:"_Stz);
  
  vAddCode(EO_RSB_DST_NEW);

  
  vAddCode(EO_ENT_PUSHELMI0);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_REVENUE);

  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)0);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_ORDERKEY);
  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)1);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_ORDERDATE);
  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)2);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_SHIPPRIORITY);


  vDefineLabel("TRV_NXT:"_Stz);
  vAddCode(EO_IXT_TRV_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"TRV_ECH:"_Stz);

  vDefineLabel("TRV_RET:"_Stz);
  vAddCode(EO_BREAKPOINT);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 4                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery400(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 400;
  
  m_i_Code = 0;
  
  vAddCodeArg(EO_DATE, "1993-07-01"_Stz);
  vAddCode(EO_DTE_PRINT);
  vAddCodeArg(EO_DATE, "1993-10-01"_Stz);
  vAddCode(EO_DTE_PRINT);

  vAddCodeArg(EO_DATE, "1993-07-01"_Stz);
  vAddCodeArg(EO_DATE, "1993-10-01"_Stz);
  vAddCode(EO_LESSTHAN);
  vAddCode(EO_PRINT);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // iterate all ORDERS
  vAddCode(EO_IXT_SRC_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"END:"_Stz);
  vDefineLabel("GOT_S:"_Stz);
  vAddCode(EO_IXT_SRC_PRINT);

  // test O_ORDERDATE
  vAddCode(EO_CRW_SRC_FETCH);
  vAddCodeArg(EO_COL_SRC_PUSH,O_ORDERDATE);
  vAddCode(EO_DUP);
  vAddCode(EO_DTE_PRINT);
  vAddCodeArg(EO_DATE,"1993-07-01"_Stz);
  vAddCode(EO_GREATEREQUAL);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_S:"_Stz);
  vAddCodeArg(EO_COL_SRC_PUSH,O_ORDERDATE); // doubled, not efficient
  vAddCodeArg(EO_DATE,"1993-10-01"_Stz);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_S:"_Stz);
  
  
  // some string tests
  vAddCodeArg(EO_COL_SRC_PUSHSTR,O_COMMENT);
  vAddCode(EO_STR_PRINTSTK);

  
  // test for a matching LINEITEM
  vAddCodeLabel(EO_CALL,"TST_A:"_Stz);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_S:"_Stz);
    
  // next ORDERS
  vDefineLabel("NXT_S:"_Stz);
  vAddCode(EO_BREAKPOINT);
  vAddCode(EO_IXT_SRC_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"GOT_S:"_Stz);

  // end of ORDERS
  vDefineLabel("END:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for a matching LINEITEM

  vDefineLabel("TST_A:"_Stz);
  vAddCode(EO_IXT_POS_ALTBYSRC);
  vAddCodeLabel(EO_JMP_FALSE,"RET_A_f:"_Stz);
  vDefineLabel("NXT_A:"_Stz);
  vAddCode(EO_IXT_ALT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"RET_A_f:"_Stz);
  vAddCode(EO_IXT_ALT_PRINT);
  vAddCode(EO_IXT_CMP_SRCALT);
  vAddCodeLabel(EO_JMP_NOTEQUAL,"RET_A_f:"_Stz);
  

  // test L_COMMITDATE < L_RECEIPTDATE
  vAddCode(EO_CRW_ALT_FETCH);
  vAddCodeArg(EO_COL_ALT_PUSH,L_COMMITDATE);
  vAddCodeArg(EO_COL_ALT_PUSH,L_RECEIPTDATE);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_A:"_Stz);
  vAddCodeLabel(EO_JMP,"RET_A_t:"_Stz);

  
  //vAddCode(EO_BREAKPOINT);
  vAddCodeLabel(EO_JMP,"NXT_A:"_Stz);

  vDefineLabel("RET_A_t:"_Stz);
  vAddCode(EO_PUSH1);
  vAddCode(EO_PRINT);
  vAddCode(EO_PUSH1);
  vAddCode(EO_RETURN);

  vDefineLabel("RET_A_f:"_Stz);
  vAddCode(EO_PUSH0);
  vAddCode(EO_PRINT);
  vAddCode(EO_PUSH0);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate agg code (query6)                                                 */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery600(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
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
  
  // initialze the row
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q600_REVENUE);
  
  vAddCode(EO_RSB_DST_PRINT);
  
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

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_SHIPDATE);
  vAddCodeArg(EO_DATE, "1994-01-01"_Stz);
  vAddCode(EO_GREATEREQUAL);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_SHIPDATE);
  vAddCodeArg(EO_DATE, "1995-01-01"_Stz);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_DISCOUNT);
  vAddCodeArg(EO_INTEGER, 5);
  vAddCode(EO_GREATEREQUAL);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);
  
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_DISCOUNT);
  vAddCodeArg(EO_INTEGER, 7);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_QUANTITY);
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

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_EXTENDEDPRICE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)L_DISCOUNT);
  vAddCode(EO_MULTIPLY);
  vAddCodeArg(EO_INTEGER,100);
  vAddCode(EO_DIVIDE);
  
  vAddCodeArg(EO_RSB_DST_PUSHCOLI,(i8)Q600_REVENUE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q600_REVENUE);
  
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate super agg (query 6)                                               */
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
  
  // initialize the row
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q600_REVENUE);
  vAddCode(EO_RSB_DST_PRINT);
  
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
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q600_REVENUE);
  vAddCodeArg(EO_RSB_DST_PUSHCOLI,(i8)Q601_REVENUE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q601_REVENUE);
  
  vAddCode(EO_RETURN);
  
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for dynamic compilation test 100000                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerate100000(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 100000;
  
  // init the code
  m_i_Code = 0;
  
  vAddCodeArg(EO_INTEGER,1);
  vAddCodeArg(EO_INTEGER,2);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_INTEGER,4);
  vAddCode(EO_MULTIPLY);
  vAddCode(EO_PRINT);
  vAddCode(EO_RETURN);
  
  vDumpCode();
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
  
#ifdef WITH_TRACE_LOGICUNIT
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    vTrace("  %s %lld: %4lld\n"_Stz,
           m_xaJumpLabel[i_Label].m_sLabel.szStz(),
           m_xaJumpLabel[i_Label].m_bDefined,
           m_xaJumpLabel[i_Label].m_i_Chain);
#endif
  
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
    if (pTkn->bSame("and"_Stz)) { vAddCode(EO_LGL_AND);             continue; }
    if (pTkn->bSame("or"_Stz))  { vAddCode(EO_LGL_OR);              continue; }
    if (pTkn->bSame("not"_Stz)) { vAddCode(EO_LGL_NOT);             continue; }

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
    if (pTkn->bSame(".xs"_Stz)) { vAddCode(EO_IXT_SRC_PRINT);       continue; }
    if (pTkn->bSame("xab"_Stz)) { vAddCode(EO_IXT_ALT_BEGIN);       continue; }
    if (pTkn->bSame("xan"_Stz)) { vAddCode(EO_IXT_ALT_NEXT);        continue; }
    if (pTkn->bSame(".xa"_Stz)) { vAddCode(EO_IXT_ALT_PRINT);       continue; }
    if (pTkn->bSame("xcm"_Stz)) { vAddCode(EO_IXT_CMP_SRCALT);      continue; }
    if (pTkn->bSame("xpa"_Stz)) { vAddCode(EO_IXT_POS_ALTBYSRC);    continue; }

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
      vAddCodeArg(EO_RSB_SRC_PUSHCOLI,pTkn);
      continue;
    }

    // push dst column value
    if (pTkn->bSame("#"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_RSB_DST_PUSHCOLI,pTkn);
      continue;
    }
    
    // deposit dst column value
    if (pTkn->bSame("!"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_RSB_DST_POPCOLI,pTkn);
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
      vAddCodeArg(EO_PAY_FETCH_COLI,pTkn);
      continue;
    }
    
    // store pay col
    if (pTkn->bSame("ps"_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn);
      vAddCodeArg(EO_PAY_STORE_COLI,pTkn);
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
 
//#ifdef WITH_TRACE_LOGICUNIT
  
  if (100000 != m_iSql)
    return;
  
  SP_Dte spDate;

  vTrace("EO code for %lld:\n"_Stz,m_iSql);
  
  for (i8 i_Code = 0; i_Code < m_i_Code; ) {
    
    eEO eCode = m_eaCode[i_Code];
    
    if (eCode <= EO_NONE || eCode >= EO_ENUMS) {
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eo code sequence detected"_Stz);
    }
    
    if (-1 == EoArgs[eCode].m_nArgBytes) {
      char* azArg = (char*) & m_eaCode[i_Code+1];
      vTrace("%04lld: %18s (%lls)\n"_Stz,i_Code,aEO[eCode],azArg);
      i_Code += 1 + StrLib::nSize(azArg);
    }
    else
    if (EA_DATE == EoArgs[eCode].m_eArgument) {
      i8* piArg = (i8*) & m_eaCode[i_Code+1];
      i8 iArg = * piArg;
      vTrace("%04lld: %18s (%lls)\n"_Stz,i_Code,aEO[eCode],spDate.sz(iArg));
      i_Code += 1 + sizeof(i8);
    }
    else
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
  
  i8 i_Ptn = 0;
  
#ifdef WITH_TRACE_LOGICUNIT
  vTrace("\nLogic Unit :: sql %lld\n\n"_Stz,m_iSql);
  vDebugPoint();
#endif

  for (;;) {
    
    // check for stack growth
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
        vTrace("%s\n"_Stz,spDate.sz(iTopA));
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
      case EO_PTN_DST: NoArg(); // calc ptn using dst stream def
        dLU0(eCode);
        NEVER_REACH;
        break;

      case EO_PTN_SET: NoArg(); // set i_Ptn register
        dLU0(eCode);
        i_Ptn = *(aiDstk--);
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
        ASSERT(0 == i_Ptn);
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
        vTrace("%lld\n"_Stz,iTopA);
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
        vTrace("%lls\n"_Stz,iTopA);
#endif
        break;
        
      case EO_STR_PRINTSTK: NoArg(); // pop string from top and print
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(aiDstk--);
        dLU1(eCode,iTopA);
        vTrace("%lls\n"_Stz,(aiDstk-iTopA/8));
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
