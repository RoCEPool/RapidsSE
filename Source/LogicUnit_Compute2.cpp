/*============================================================================*/
/* LogicUnit_Compute2.cpp                                                     */
/*============================================================================*/

#include "Expressions.h"

#include "DEV_enum.h"
#include "RLU_itf.h"

extern EoArg EoArgs[];


/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iCompute2(i8 nOuterArgs, i8 iOuterArg0)
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
    
    switch (eCode) {
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // arithmetic operators
//+ void LogicUnit::v_() {
      case EO_ADD: NoArg(); // add top two int's push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA + iTopB;
        dLU3(EO_ADD,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }
        
//+ void LogicUnit::v_() {
      case EO_DIVIDE: NoArg(); // divide top int by second push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA / iTopB;
        dLU3(EO_DIVIDE,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_MODULO: NoArg(); // modulo top int be second push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA % iTopB;
        dLU3(EO_MODULO,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_MULTIPLY: NoArg(); // multiply top two int's push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA * iTopB;
        dLU3(EO_MULTIPLY,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_NEGATE: NoArg(); // negate the top value
        iTopA = *(m_aiDstk--);
        iRes  = - iTopA;
        dLU2(EO_NEGATE,iTopA,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_SUBTRACT: NoArg(); // subtract second int from top push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA - iTopB;
        dLU3(EO_SUBTRACT,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // counter
//+ void LogicUnit::v_() {
      case EO_CTR_0_DEC: NoArg(); // dec counter 0
        m_iCtr0--;
        dLU1(EO_CTR_0_DEC,m_iCtr0);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_CTR_0_INC: NoArg(); // inc counter 0
        m_iCtr0++;
        dLU1(EO_CTR_0_INC,m_iCtr0);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_CTR_0_POP: NoArg(); // pop counter 0
        m_iCtr0 = *(m_aiDstk--);
        dLU1(EO_CTR_0_POP,m_iCtr0);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_CTR_0_PUSH: NoArg(); // push counter 0
        dLU1(EO_CTR_0_PUSH,m_iCtr0);
        *(++m_aiDstk) = m_iCtr0;
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // date operators
//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_DTE_PRINT: NoArg(); // pop top and print as date
#ifdef WITH_TRACE_LOGICUNIT
      {
        SP_Dte spDate;
        iTopA = *(m_aiDstk--);
        dLU1(EO_DTE_PRINT,iTopA);
        vTrace("[Q%lld.%lld] %s\n"_Stz,m_iSql,m_i_Slot,spDate.sz(iTopA));
      }
#endif
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_DTE_YEAR: NoArg(); // pop top, push year of date
      {
        Dtm wDte;
        wDte = *(m_aiDstk--);
        iRes = wDte.m_wDte.m_uYearDelta + C_YEAR_BASE;
        dLU2(EO_DTE_YEAR,wDte.iAsI8(),iRes);
        *(++m_aiDstk) = iRes;
      }
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // hashing
//+ void LogicUnit::v_() {
      case EO_HSH_ADD_HASHI: NoArg(); // add hash of top int to running hash
        iTopA = *(m_aiDstk--);
        iRes = HshLib::iHash((byte*)&iTopA,sizeof(w8));
        m_iHash += iRes;
        dLU3(EO_HSH_ADD_HASHI,iTopA,iRes,m_iHash);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_HSH_RESET: NoArg(); // reset hash
        dLU0(EO_HSH_RESET);
        m_iHash = 0;
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // jump operators
//+ void LogicUnit::v_() {
      case EO_JMP: GetArg(iArg); // jump to location
        dLU1(EO_JMP,iArg);
        m_aeCode = & m_eaCode[iArg];
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_JMP_FALSE: GetArg(iArg); // pop top and jump to location if false
        dLU1(EO_JMP_FALSE,iArg);
        iTopA = *(m_aiDstk--);
        if (! iTopA)
          m_aeCode = & m_eaCode[iArg];
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_JMP_TRUE: GetArg(iArg); // pop top and jump to location if true
        dLU1(EO_JMP_TRUE,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA)
          m_aeCode = & m_eaCode[iArg];
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_JMP_LESS: GetArg(iArg); // pop top and jump to location if <
        dLU1(EO_JMP_LESS,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA < 0)
          m_aeCode = & m_eaCode[iArg];
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_JMP_LESSEQUAL: GetArg(iArg); // pop top and jump to location if <=
        dLU1(EO_JMP_LESSEQUAL,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA <=0)
          m_aeCode = & m_eaCode[iArg];
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_JMP_EQUAL: GetArg(iArg); // pop top and jump to location if ==
        dLU1(EO_JMP_EQUAL,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA == 0)
          m_aeCode = & m_eaCode[iArg];
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_JMP_NOTEQUAL: GetArg(iArg); // pop top and jump to location if !=
        dLU1(EO_JMP_NOTEQUAL,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA != 0)
          m_aeCode = & m_eaCode[iArg];
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_JMP_GREATEREQUAL: GetArg(iArg); // pop top and jump to location if >=
        dLU1(EO_JMP_GREATEREQUAL,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA >= 0)
          m_aeCode = & m_eaCode[iArg];
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_JMP_GREATER: GetArg(iArg); // pop top and jump to location if >
        dLU1(EO_JMP_GREATER,iArg);
        iTopA = *(m_aiDstk--);
        if (iTopA > 0)
          m_aeCode = & m_eaCode[iArg];
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // logical operators
//+ void LogicUnit::v_() {
      case EO_LGL_AND: NoArg(); // logical and top two int's, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = (iTopA && iTopB) ? 1 : 0;
        dLU3(EO_LGL_AND,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_LGL_OR: NoArg(); // logical or top two int's, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = (iTopA || iTopB) ? 1 : 0;
        dLU3(EO_LGL_OR,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_LGL_NOT: NoArg(); // logical not top int, push result
        iTopA = *(m_aiDstk--);
        iRes  = (! iTopA) ? 1 : 0;
        dLU2(EO_LGL_NOT,iTopA,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // null test operators
//+ void LogicUnit::v_() {
      case EO_ISNULL: NoArg(); // test if top is null, push result
        iTopA = *(m_aiDstk--);
        iRes  = (C_DATUM_NULL == iTopA) ? 1 : 0;
        dLU2(EO_ISNULL,iTopA,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ISNOTNULL: NoArg(); // test if top is not null, push result
        iTopA = *(m_aiDstk--);
        iRes  = (C_DATUM_NULL != iTopA) ? 1 : 0;
        dLU2(EO_ISNOTNULL,iTopA,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pushes
//+ void LogicUnit::v_() {
      case EO_DATE: GetArg(iArg); // push date
        dLU1(EO_DATE,iArg);
        *(++m_aiDstk) = iArg;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_INTEGER: GetArg(iArg); // push integer
        dLU1(EO_INTEGER,iArg);
        *(++m_aiDstk) = iArg;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_PUSHNULL: NoArg(); // push null
        dLU0(EO_PUSHNULL);
        *(++m_aiDstk) = C_DATUM_NULL;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_PUSH0: NoArg(); // push integer 0
        dLU0(EO_PUSH0);
        *(++m_aiDstk) = 0;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_PUSH1: NoArg(); // push integer 1
        dLU0(EO_PUSH1);
        *(++m_aiDstk) = 1;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RANDOM: GetArg(iArg); // push random number 0..arg-1
        iRes = RndLib::iInteger(iArg);
        dLU2(EO_RANDOM,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }


      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // relational operators
//+ void LogicUnit::v_() {
      case EO_LESSTHAN: NoArg(); // test top < 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA < iTopB;
        dLU3(EO_LESSTHAN,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_LESSEQUAL: NoArg(); // test top <= 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA <= iTopB;
        dLU3(EO_LESSEQUAL,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_EQUAL: NoArg(); // test top == 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA == iTopB;
        dLU3(EO_EQUAL,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_NOTEQUAL: NoArg(); // test top != 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA != iTopB;
        dLU3(EO_NOTEQUAL,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_GREATEREQUAL: NoArg(); // test top >= 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA >= iTopB;
        dLU3(EO_GREATEREQUAL,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_GREATERTHAN: NoArg(); // test top > 2nd, push result
        iTopB = *(m_aiDstk--);
        iTopA = *(m_aiDstk--);
        iRes  = iTopA > iTopB;
        dLU3(EO_GREATERTHAN,iTopA,iTopB,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // partitioning
//+ void LogicUnit::v_() {
      case EO_PTN_CALC: NoArg(); // calculate m_i_Ptn using running m_iHash
        dLU0(EO_PTN_CALC);
        m_i_Ptn = PtnLib::iPartition(m_iHash,m_nPtns);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_PTN_PUSH: NoArg(); // push m_i_Ptn
        dLU1(EO_PTN_PUSH,m_i_Ptn);
        *(++m_aiDstk) = m_i_Ptn;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_PTN_RESET: NoArg(); // reset running m_iHash
        dLU0(EO_PTN_RESET);
        m_iHash = PtnLib::iResetRunning();
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_PTN_RUNNING: NoArg(); // mix top into running m_iHash
        iTopA = *(m_aiDstk--);
        dLU0(EO_PTN_RUNNING);
        PtnLib::vRunning(&m_iHash,iTopA);
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // stack operators
//+ void LogicUnit::v_() {
      case EO_INC: NoArg(); // increment top, push result
        iTopA = *m_aiDstk;
        iRes = iTopA + 1;
        dLU2(EO_INC,iTopA,iRes);
        *m_aiDstk = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_DEC: NoArg(); // decrement top, push result
        iTopA = *m_aiDstk;
        iRes = iTopA - 1;
        dLU2(EO_DEC,iTopA,iRes);
        *m_aiDstk = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_DUP: NoArg(); // duplicate top
        iTopA = *m_aiDstk;
        dLU1(EO_DUP,iTopA);
        *(++m_aiDstk) = iTopA;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_POP: NoArg(); // pop / discard top
        iTopA = *m_aiDstk;
        dLU1(EO_POP,iTopA);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_PRINT: NoArg(); // print top
        iTopA = *(m_aiDstk--);
        dLU1(EO_PRINT,iTopA);
        vTrace("[Q%lld.%lld] %lld\n"_Stz,m_iSql,m_i_Slot,iTopA);
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // string operators
//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_STR_PRINTADR: NoArg(); // pop top and print iArg as addr of string
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(m_aiDstk--);
        dLU1(EO_STR_PRINTADR,iTopA);
        vTrace("[Q%lld.%lld] %lls\n"_Stz,m_iSql,m_i_Slot,iTopA);
#endif
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_STR_PRINTSTK: NoArg(); // pop string from top and print
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(m_aiDstk--);
        dLU1(EO_STR_PRINTSTK,iTopA);
        vTrace("[Q%lld,%lld] %lls\n"_Stz,m_iSql,m_i_Slot,(m_aiDstk-iTopA/8));
        m_aiDstk -= (iTopA/8) + 1;
#endif
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_STR_LITERAL: GetStr(iArg); // push address of string literal
        dLU1(EO_STR_LITERAL,iArg);
        *(++m_aiDstk) = iArg;
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // subroutine operators
//+ void LogicUnit::v_() {
      case EO_CALL: GetArg(iArg); // call subroutine at location
        dLU1(EO_CALL,iArg);

        * (m_aaeRstk++) = m_aeCode;
        m_aeCode = & m_eaCode[iArg];
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // timer
//+ void LogicUnit::v_() {
      case EO_TMR_START: NoArg(); // timer start
        dLU0(EO_TMR_START);
        m_oTimer.vStart();
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_TMR_NSECS: NoArg(); // push timer nanoseconds
        iRes = m_oTimer.iNanoseconds();
        dLU1(EO_TMR_NSECS,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_TMR_UPDATE: NoArg(); // timer update
        dLU0(EO_TMR_UPDATE);
        m_oTimer.vUpdate();
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_TMR_AGG_ADD: NoArg(); // add timer to agg timer
        dLU0(EO_TMR_AGG_ADD);
        m_oAggTimer.vAdd(&m_oTimer);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_TMR_AGG_NSECS: NoArg(); // push agg nanoseconds
        iRes = m_oAggTimer.iNanoseconds();
        dLU1(EO_TMR_AGG_NSECS,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_TMR_AGG_RESET: NoArg(); // reset agg timer
        dLU0(EO_TMR_AGG_RESET);
        m_oTimer.vReset();
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // misc
//+ void LogicUnit::v_() {
      case EO_ASSERT: NoArg(); // pop top and assert
        iTopA = *(m_aiDstk--);
        dLU1(EO_ASSERT,iTopA);
        RASSERT(iTopA);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_BREAKPOINT: NoArg(); // breakpoint
        dLU0(EO_BREAKPOINT);
        vDebugPoint();
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_SLEEP: NoArg(); // sleep for top seconds
        iTopA = *(m_aiDstk--);
        dLU1(EO_SLEEP,iTopA);
        sleep((u4)iTopA);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_SLOT: NoArg(); // sleep for top seconds
        dLU1(EO_SLOT,m_i_Slot);
        *(++m_aiDstk) = m_i_Slot;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_TRACEOFF: NoArg(); // set trace
        m_bTrace = 0;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_TRACEON: NoArg(); // set trace
        m_bTrace = 1;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_NOOP: NoArg(); // no op
        dLU0(EO_NOOP);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ERROR: NoArg(); // error marker
        dLU0(EO_ERROR);
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "Code compiled with error"_Stz);
        break;
//+ }

      //------------------------------------------------------------------------
      //------------------------------------------------------------------------

        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // col operators
//+ void LogicUnit::v_() {
      case EO_COL_DST_RESET: GetArg(iArg); // reset column in streamPix of dst
      {
        m_oStreamPix.vNotProject(iArg);
      }
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_COL_SRC_PUSH: GetArg(iArg); // push column value from m_aSrcCrw
      {
        HV_Row hRow(m_pDevSrcRsb->m_pStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_pDevSrcIxt->m_aCrw,iArg);
        iRes = hRow->m_waColVal[iArg].m_iWrd;
        dLU2(EO_COL_SRC_PUSH,iArg,iRes);
        *(++m_aiDstk) = iRes;
      }
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_COL_ALT_PUSH: GetArg(iArg); // push column value from m_aAltCrw
      {
        HV_Row hRow(m_pDevAltRsb->m_pStreamDef,C_ALL);
        RowInflator::vInflateColumn(hRow,m_pDevAltIxt->m_aCrw,iArg);
        iRes = hRow->m_waColVal[iArg].m_iWrd;
        dLU2(EO_COL_ALT_PUSH,iArg,iRes);
        *(++m_aiDstk) = iRes;
      }
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // crw operators
//+ void LogicUnit::v_() {
      case EO_CRW_SRC_BEGIN: NoArg(); // begin src rsb traverse
        dLU0(EO_CRW_SRC_BEGIN);
        m_pDevSrcIxt->m_i_Row = -1;
        m_pDevSrcIxt->m_aCrw = nullptr;
        m_pDevSrcIxt->m_nRows = m_pDevSrcIxt->m_pCrwBuffer ?
                         m_pDevSrcIxt->m_pCrwBuffer->nRowCount() : 0;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_CRW_SRC_FETCH: NoArg(); // fetch m_aSrcCrw using m_pDevSrcIxt->m_aEnt.ref
        dLU0(EO_CRW_SRC_FETCH);
        m_pDevSrcIxt->m_aCrw  =
          CrwBuffer::pRefdCrw(m_pDevSrcIxt->m_aEnt->rxRef());
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_CRW_ALT_FETCH: NoArg(); // fetch m_aAltCrw using m_aAltEnt.ref
        dLU0(EO_CRW_ALT_FETCH);
        m_pDevAltIxt->m_aCrw =
          CrwBuffer::pRefdCrw(m_pDevAltIxt->m_aEnt->rxRef());
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_CRW_SRC_TORSB:
        dLU0(EO_CRW_SRC_TORSB);
        RowInflator::vInflateColumns(m_pDevDstRsb->m_pRowSlab,m_pDevSrcIxt->m_aCrw);
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry
//+ void LogicUnit::v_() {
      case EO_ENT_NEXT: NoArg(); // move to next m_pEnt
        dLU0(EO_ENT_NEXT);
        m_pDevTmpIxt->m_aEnt =
          EntBuffer::pMoveToNextEntry(m_pDevTmpIxt->m_aEnt,
                                      &m_pDevTmpIxt->m_pEntBuffer);
        if (m_pDevTmpIxt->m_aEnt)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENT_POSPRIOR: NoArg(); // position m_pEnt prior using m_oEntWrk.key
        dLU0(EO_ENT_POSPRIOR);
        m_pDevTmpIxt->m_aEnt =
          m_pDevTmpIxt->m_pIdxTree->aPositionPriorEntry(m_oEntWrk.aKey(),
                                                        &m_pDevTmpIxt->m_pEntBuffer);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENT_PUSH: NoArg(); // push entry on the stack
        dLU0(EO_ENT_PUSH);
        *(++m_aiDstk) = (i8)m_pDevTmpIxt->m_aEnt;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENT_PUSHELMI0: NoArg(); // push entry elm 0 int
        dLU0(EO_ENT_PUSHELMI0);
        iRes = m_pDevTmpIxt->m_aEnt->aKey()->iGetIntElm();
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENT_PUSHOFS: NoArg(); // push entry offset
        dLU0(EO_ENT_PUSHOFS);
        iRes = m_pDevTmpIxt->m_aEnt->iBuffer();
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENT_PUSHREF: NEVER_REACH; // not yet implemented
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entry relational operators
//+ void LogicUnit::v_() {
      case EO_ENT_LESS: NoArg(); // test if m_pEnt.key <  m_oEntWrk.key
        iRes  = KC_BEFORE == Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_LESS,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENT_LESSEQUAL: NoArg(); // test if m_pEnt.key <= m_oEntWrk.key
        iCmp = Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey());
        iRes  = (KC_SAME == iCmp || KC_BEFORE == iCmp ) ? 1 : 0;
        dLU1(EO_ENT_LESSEQUAL,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
      }
//+ }

//+ void LogicUnit::v_() {
      case EO_ENT_NOTEQUAL: NoArg();
        iRes  = KC_SAME != Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_NOTEQUAL,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENT_GREATEREQUAL: NoArg();
        iCmp = Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey());
        iRes = (KC_SAME == iCmp || KC_AFTER == iCmp ) ? 1 : 0;
        dLU1(EO_ENT_GREATEREQUAL,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENT_GREATER: NoArg();
        iRes  = KC_AFTER == Key::eCompare(m_pDevTmpIxt->m_aEnt->aKey(),m_oEntWrk.aKey()) ? 1 : 0;
        dLU1(EO_ENT_GREATER,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // entWrk
//+ void LogicUnit::v_() {
      case EO_ENTWRK_APPENDELM: GetArg(iArg); // append next element value
      {
        dLU1(EO_ENTWRK_APPENDELM,iArg);
        m_oEntWrk.vAppendElement(m_pDevSrcRsb->m_aRsb, &m_pDevSrcRsb->m_pRowSlab->m_oStreamPix, iArg);
        
#ifdef WITH_TRACE_LOGICUNIT
        SP_Ent spEnt;
        vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(m_oEntWrk.aEnt(),m_pDevTmpIxt->m_pIdxTree->pIndexDef()));
#endif
        break;
      }
//+ }

//+ void LogicUnit::v_() {
      case EO_ENTWRK_APPENDELMI: NoArg(); // append next element value (int)
        iTopA = *(m_aiDstk--);
        dLU1(EO_ENTWRK_APPENDELMI,iTopA);
        m_oEntWrk.vAppendElementInt(m_pDevSrcRsb->m_aRsb,iTopA);
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENTWRK_CREATEPAY: GetArg(iArg); // create a payload in entwrk
        dLU1(EO_ENTWRK_CREATEPAY,iArg);
        
        m_oEntWrk.vPayload(true);
        m_oEntWrk.vPayloadBytes(iArg);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENTWRK_POPOFS: NoArg(); // pop m_oEntWrk.ref.buffer/offset
        iTopA = *(m_aiDstk--);
        dLU1(EO_ENTWRK_POPOFS,iTopA);
        m_oEntWrk.vBuffer(iTopA);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENTWRK_POPREF: NEVER_REACH; // not yet implemented
//+ }

//+ void LogicUnit::v_() {
      case EO_ENTWRK_PRINT: NoArg(); // print m_oEntWrk
#ifdef WITH_TRACE_LOGICUNIT
        aEnt = m_oEntWrk.aEnt();
        pIndexDef = m_xpIndexTree->pIndexDef();
        vTrace("EntWrk : %s\n"_Stz,spEnt.sz(aEnt,pIndexDef));
#endif
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_ENTWRK_RESET: NoArg(); // reset m_oEntWrk
        dLU0(EO_ENTWRK_RESET);
        m_oEntWrk.vReset();
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // index tree operators
//+ void LogicUnit::v_() {
      case EO_IXT_DEL_ENT: NoArg(); // delete current entry from indexTree
        dLU0(EO_IXT_DEL_ENT);
        m_pDevTmpIxt->m_pIdxTree->vDelete(m_pDevTmpIxt->m_pEntBuffer,m_pDevTmpIxt->m_aEnt);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_IXT_DUMP: GetArg(iArg); // dump index tree
        //m_pDevTmpIxt->m_pIdxTree->vDump(false);
        ::iDEV_MAP(this,m_aaDev[iArg],DEV_MAP_DUMP,nullptr,nullptr,0);
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_IXT_TRV_NEXT: NoArg(); // move to next entry
        dLU0(EO_IXT_TRV_NEXT);
        m_pDevTmpIxt->m_aEnt =
           EntBuffer::pMoveToNextEntry(m_pDevTmpIxt->m_aEnt,
                                       &m_pDevTmpIxt->m_pEntBuffer);
        if (m_pDevTmpIxt->m_aEnt)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_IXT_TMP_ENTRIES: NoArg(); // push number of entries in tmp index
        iRes = m_pDevTmpIxt->m_pIdxTree->iCountH0Entries();
        dLU1(EO_IXT_TMP_ENTRIES,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_IXT_SRC_NEXT: NoArg(); // move to next entry
        dLU0(EO_IXT_SRC_NEXT);
        m_pDevSrcIxt->m_aEnt = EntBuffer::pMoveToNextEntry(m_pDevSrcIxt->m_aEnt, &m_pDevSrcIxt->m_pEntBuffer);
        if (m_pDevSrcIxt->m_aEnt)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_IXT_SRC_PRINT: NoArg(); // print src ent
        pIndexDef = m_pDevSrcIxt->m_pIdxTree->pIndexDef();
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("SrcEnt : %s\n"_Stz,spEnt.sz(m_pDevSrcIxt->m_aEnt,pIndexDef));
#endif
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_IXT_ALT_NEXT: NoArg();
        dLU0(EO_IXT_ALT_NEXT);
        m_pDevAltIxt->m_aEnt =
          EntBuffer::pMoveToNextEntry(m_pDevAltIxt->m_aEnt,
                                      &m_pDevAltIxt->m_pEntBuffer);
        if (m_pDevAltIxt->m_aEnt)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_IXT_ALT_PRINT: NoArg(); // print alt ent
        pIndexDef = m_pDevAltIxt->m_pIdxTree->pIndexDef();
#ifdef WITH_TRACE_LOGICUNIT
        vTrace("AltEnt : %s\n"_Stz,spEnt.sz(m_pDevAltIxt->m_aEnt,pIndexDef));
#endif
        break;
        
//+ void LogicUnit::v_() {
      case EO_IXT_JOI_MATCH: NoArg(); // test for join match (src.key == alt.key)
        aSrcKey = m_pDevSrcIxt->m_aEnt->aKey();
        aAltKey = m_pDevAltIxt->m_aEnt->aKey();
        iRes = KC_SAME == Key::eCompare(aSrcKey,aAltKey) ? 1 : 0;
        dLU1(EO_IXT_JOI_MATCH,iRes);
        *(++m_aiDstk) = iRes;
        NEVER_REACH; // not used??
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pay load
//+ void LogicUnit::v_() {
      case EO_PAY_FETCH_COLI: GetArg(iArg); // fetch a column from the entry payload
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        iRes = *aPayload;
        dLU2(EO_PAY_FETCH_COLI,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_PAY_STORE_COLI: GetArg(iArg); // store a column of the entry payload
        iRes = *(m_aiDstk--);
        dLU2(EO_PAY_STORE_COLI,iArg,iRes);
        aPayload = m_pDevTmpIxt->m_aEnt->aiPayCell(iArg);
        *aPayload = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pix
//+ void LogicUnit::v_() {
      case EO_PIX_STM_PRINT: NoArg(); // print stream pix
        dLU0(EO_PIX_STM_PRINT);
        m_oStreamPix.vPrint();
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb / rowslab
//+ void LogicUnit::v_() {
      case EO_RSB_DST_COPYSRC: NoArg(); // copy src rsb to dst rsb
        dLU0(EO_RSB_DST_COPYSRC);
        m_pDevDstRsb->m_pRowSlab->vPositionOnCell(0);
        m_pDevDstRsb->m_aRsb = m_pDevDstRsb->m_pRowSlab->aRsb();
        m_pDevDstRsb->m_pRowSlab->vCopyRsb(m_pDevSrcRsb->m_aRsb,bSrcHasPtnHash,bCopyPtnHash);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_DST_PRINT: NoArg(); // print dst rsb
        dLU0(EO_RSB_DST_PRINT);
        m_pDevDstRsb->m_pRowSlab->vDumpRow(-1,m_pDevDstRsb->m_aRsb);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_SRC_PRINT: NoArg(); // print src rsb
        dLU0(EO_RSB_SRC_PRINT);
        m_pDevSrcRsb->m_pRowSlab->vDumpRow(-1,m_pDevSrcRsb->m_aRsb);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_ALT_PRINT: NoArg(); // print alt rsb
        dLU0(EO_RSB_ALT_PRINT);
        m_pDevAltRsb->m_pRowSlab->vDumpRow(-1,m_pDevAltRsb->m_aRsb);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_DST_NEW: NoArg(); // create new destination rsb
        dLU0(EO_RSB_DST_NEW);
        //ASSERT(0 == m_i_Ptn);
        m_pDevDstRsb->m_aRsb = m_xpPot->aNewDstRsb(m_i_Ptn,&m_pDevDstRsb->m_pRowSlab);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_DST_RESET: NoArg(); // reset dst rowslab
        dLU0(EO_RSB_DST_RESET);
        m_pDevDstRsb->m_pRowSlab->vReset();
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_DST_TEST: NoArg(); // test if there is a destination rsb
        dLU0(EO_RSB_DST_TEST);
        if (m_pDevDstRsb->m_aRsb)
          *(++m_aiDstk) = 1;
        else
          *(++m_aiDstk) = 0;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_DST_ZERO: NoArg(); // zero destination rsb
        dLU0(EO_RSB_DST_ZERO);
        m_pDevDstRsb->m_pRowSlab->vZeroRsb(m_pDevDstRsb->m_aRsb);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_SRC_BEGIN: NoArg(); // begin src rsb traverse
        dLU0(EO_RSB_SRC_BEGIN);
        m_pDevSrcRsb->m_i_Row = -1;
        m_pDevSrcRsb->m_aRsb  = nullptr;
        m_pDevSrcRsb->m_nRows = m_pDevSrcRsb->m_pRowSlab ? m_pDevSrcRsb->m_pRowSlab->nRows() : 0;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_SRC_NEXT: NoArg(); // move to next src rsb
        dLU0(EO_RSB_SRC_NEXT);
        if (++m_pDevSrcRsb->m_i_Row  < m_pDevSrcRsb->m_nRows) {
          m_pDevSrcRsb->m_aRsb = m_pDevSrcRsb->m_pRowSlab->aRsbAdr(m_pDevSrcRsb->m_i_Row );
          *(++m_aiDstk) = 1;
        }
        else {
          *(++m_aiDstk) = 0;
        }
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_SRC_PUSHOFS: NoArg();
        dLU0(EO_RSB_SRC_PUSHOFS);
        iRes = m_pDevSrcRsb->m_pRowSlab->iOfsOfRsb(m_pDevSrcRsb->m_aRsb);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_SRC_SETBYOFS: NoArg();  // set src rsb by using offset on stack
        dLU0(EO_RSB_SRC_SETBYOFS);
        iTopA = *(m_aiDstk--);
        m_pDevSrcRsb->m_aRsb = m_pDevSrcRsb->m_pRowSlab->aRsbByOfs(iTopA);
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_ALT_BEGIN:    NEVER_REACH; // not yet implemented
//+ }
//+ void LogicUnit::v_() {
      case EO_RSB_ALT_NEXT:     NEVER_REACH; // not yet implemented
//+ }
//+ void LogicUnit::v_() {
      case EO_RSB_ALT_PUSHOFS:  NEVER_REACH; // not yet implemented
//+ }
//+ void LogicUnit::v_() {
      case EO_RSB_ALT_SETBYOFS: NEVER_REACH; // not yet implemented
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rsb operators
//+ void LogicUnit::v_() {
      case EO_RSB_DST_POPCOLI: GetArg(iArg); // pop and deposit top into dst column
        iRes = *(m_aiDstk--);
        dLU2(EO_RSB_DST_POPCOLI,iArg,iRes);
        m_pDevDstRsb->m_pRowSlab->vPositionOnCell(iArg);
        m_pDevDstRsb->m_pRowSlab->vDeposit(iRes);
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_DST_PUSHCOLI: GetArg(iArg); // push dst column
        m_pDevDstRsb->m_aRsb = m_pDevDstRsb->m_pRowSlab->aRsbAdr(m_pDevDstRsb->m_pRowSlab->nRows()-1);
        i_Cell_ntv = m_pDevDstRsb->m_aRsb->i_ColToCell_ntv(iArg);
        iRes = m_pDevDstRsb->m_aRsb->iVal_ntv(i_Cell_ntv);
        dLU2(EO_RSB_DST_PUSHCOLI,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_SRC_PUSHCOLI: GetArg(iArg); // push src column
        iRes =
          m_pDevSrcRsb->m_aRsb->iVal_ntv(m_pDevSrcRsb->m_aRsb->i_ColToCell_ntv(iArg));
        dLU2(EO_RSB_SRC_PUSHCOLI,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

//+ void LogicUnit::v_() {
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
        break;
//+ }

//+ void LogicUnit::v_() {
      case EO_RSB_ALT_PUSHCOLI: GetArg(iArg); // push alt column
        iRes = m_pDevAltRsb->m_aRsb->iVal_ntv(m_pDevAltRsb->m_aRsb->i_ColToCell_ntv(iArg));
        dLU2(EO_RSB_ALT_PUSHCOLI,iArg,iRes);
        *(++m_aiDstk) = iRes;
        break;
//+ }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      default:
        vTrace("Unknown eo opcode %lld\n"_Stz,(i8)eCode);
        return C_DATUM_NULL;
    }
    
  }
  
  return m_iaDataStack[0];
}


/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iCompute3(i8 nOuterArgs, i8 iOuterArg0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return m_iaDataStack[0];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
