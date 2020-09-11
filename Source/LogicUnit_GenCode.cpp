/*============================================================================*/
/* LogicUnit_GenCode.hpp -- Logic Unit (generate code functions)              */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/* generate code for given test                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenCode(i8 iTestNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  switch (iTestNo) {
    
    case     100: vGenerateQuery100();    break;
    case     103: vGenerateQuery103();    break;
      
    case     300: vGenerateQuery300();    break;
    case     301: vGenerateQuery301();    break;
    case     302: vGenerateQuery302();    break;

    case     400: vGenerateQuery400();    break;
    case     401: vGenerateQuery401();    break;

    case     600: vGenerateQuery600();    break;
    case     601: vGenerateQuery601();    break;
      
    case    1700: vGenerate1700();        break;
    case    1701: vGenerate1701();        break;
    case    1702: vGenerate1702();        break;

    case  100000: vGenerate100000();      break;
    case  100001: vGenerate100001();      break;

    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAcceptToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ++ m_i_Tkn;
  
  m_pTkn = m_pTknVec->pGet(m_i_Tkn);
  
#ifdef WITH_TRACE_LOGICUNIT
  //vTrace("Next %03lld: %s\n"_Stz,m_i_Tkn,m_pTkn->szChars());
#endif
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vBeginToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_i_Tkn = i_Tkn;
  m_pTkn  = m_pTknVec->pGet(m_i_Tkn);
  
#ifdef WITH_TRACE_LOGICUNIT
  vTrace("Begin %03lld: %s\n"_Stz,i_Tkn,m_pTkn->szChars());
#endif
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vCollectOpcode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if (m_pTkn->bSame("Label"_Stz)) {
    vAcceptToken();
    vDefineLabel(m_pTkn->szChars());
    return;
  }
  
  for (i8 i_OpCode = 0; i_OpCode < EO_ENUMS; i_OpCode++) {
        
    if (m_pTkn->bSame(aEO[i_OpCode])) {
      
      switch((i8)(EoArgs[i_OpCode].m_eArgument)) {
          
        case EA_DATE:
          vAcceptToken();
          vAddCodeArg((eEO)i_OpCode, m_pTkn->szChars());
          return;
          
        case EA_INT:
          vAcceptToken();
          vAddCodeArg((eEO)i_OpCode, m_pTkn->iToI8());
          return;
          
        case EA_LABEL:
          vAcceptToken();
          vAddCodeLabel((eEO)i_OpCode, m_pTkn->szChars());
          return;
          
        case EA_STRING:
          vAcceptToken();
          vAddCodeArg((eEO)i_OpCode, m_pTkn->szChars());
          return;
          
        case EA_SUBSCR:
          vAcceptToken();
          vAddCodeArg((eEO)i_OpCode, m_pTkn->iToI8());

          return;
          
       case EO_NONE:
         vAddCode((eEO)i_OpCode);
         return;
          
        case EO_ENUMS:
          ASSERT(0);
      }
    }
  }
        
}

/*----------------------------------------------------------------------------*/
/* generate asm code for given text                                           */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateAsm(Stz* aText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenize the code text
  m_pTknVec->vTokenise(aText);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the toekns
  i8 nTokens = m_pTknVec->nTokens();
  
//  for (i8 i_Tkn = 0; i_Tkn < nTokens; ) {
//    Token* pTkn = m_pTknVec->pGet(i_Tkn++);
//
//    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,pTkn->szChars());
//  }
//  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code
  m_i_Code = 0;
  
  vBeginToken(0);
  
  ASSERT(m_pTkn->bSame("Routine"_Stz) );
  vAcceptToken();
  
  // add label
  vDefineLabel(m_pTkn->szChars());  
  
  while (m_i_Tkn <  nTokens - 1) {
    vAcceptToken();
    vCollectOpcode();
  }
  
  m_pTknVec->vSetAllTokens();
  //vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate iterating index on l_shipdate & project into RSB (Query 1)        */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery100(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 100;
  
  // reset
  m_i_Code = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BEGIN MAIN: QRY_100
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("QRY_100:"_Stz);
  vAddCode(EO_INIT);
  vAddCode(EO_ENTER);

  vAddCode(EO_CRW_SRC_TEST);
  vAddCodeLabel(EO_JMP_TRUE,"BEGIN_CRW_SRC:"_Stz);
  vAddCodeLabel(EO_CALL,"SUBROUTINE_IXT_TO_RSB:"_Stz);
  vAddCodeLabel(EO_JMP,"QRY_100_DONE:"_Stz);
   
  vDefineLabel("BEGIN_CRW_SRC:"_Stz);
  //vAddCodeString(EO_PRINTF,"**100** Got CRW BUFFER\n"_Stz);
  // forearch src
  vAddCode(EO_CRW_SRC_BEGIN);
  
  // NEXT_CRW_SRC:
  vDefineLabel("NEXT_CRW_SRC:"_Stz);
  vAddCode(EO_CRW_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"QRY_100_DONE:"_Stz);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // call PROCESSROW subroutine to process the row
  vAddCodeLabel(EO_CALL, "SUBROUTINE_PROCESSROW:"_Stz);

  vAddCodeLabel(EO_JMP, "NEXT_CRW_SRC:"_Stz);
  
  vDefineLabel("QRY_100_DONE:"_Stz);
  vAddCode(EO_CRW_SRC_NULL);
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // END MAIN: QRY_100
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BEGIN SUBROUTINE: SUBROUTINE_PROCESSROW
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("SUBROUTINE_PROCESSROW:"_Stz);
  
//  vAddCodeArg(EO_COL_SRC_PUSHONE,L_ORDERKEY);
//  vAddCode(EO_PRINT);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // inflate crw required columns
//  vAddCode(EO_PUSH_MINUS1);
//  vAddCodeArg(EO_INTEGER,L_SHIPDATE);
//  vAddCodeArg(EO_INTEGER,L_LINESTATUS);
//  vAddCodeArg(EO_INTEGER,L_RETURNFLAG);
//  vAddCodeArg(EO_INTEGER,L_TAX);
//  vAddCodeArg(EO_INTEGER,L_DISCOUNT);
//  vAddCodeArg(EO_INTEGER,L_EXTENDEDPRICE);
//  vAddCodeArg(EO_INTEGER,L_QUANTITY);
  
  
//  vAddCodeArg(EO_SET_ITEMS,7);
//  vAddCodeArg(EO_SET_COLUMN,L_SHIPDATE);
//  vAddCodeArg(EO_SET_COLUMN,L_LINESTATUS);
//  vAddCodeArg(EO_SET_COLUMN,L_RETURNFLAG);
//  vAddCodeArg(EO_SET_COLUMN,L_TAX);
//  vAddCodeArg(EO_SET_COLUMN,L_DISCOUNT);
//  vAddCodeArg(EO_SET_COLUMN,L_EXTENDEDPRICE);
//  vAddCodeArg(EO_SET_COLUMN,L_QUANTITY);
//  //vAddCode(EO_COL_SRC_PUSHSET);
  vAddCodeArg(EO_SET_LOCALS_SRC,(i8)0);
  //vAddCode(EO_DUMP);
  
  
  
  // pop inflated columns into the locals register
//  vAddCodeArg(EO_POP_LOCALI,Q1_LOCAL_SHIPDATE);
//  vAddCodeArg(EO_POP_LOCALS,Q1_LOCAL_LINESTATUS);
//  vAddCodeArg(EO_POP_LOCALS,Q1_LOCAL_RETURNFLAG);
//  vAddCodeArg(EO_POP_LOCALI,Q1_LOCAL_TAX);
//  vAddCodeArg(EO_POP_LOCALI,Q1_LOCAL_DISCOUNT);
//  vAddCodeArg(EO_POP_LOCALI,Q1_LOCAL_EXTENDEDPRICE);
//  vAddCodeArg(EO_POP_LOCALI,Q1_LOCAL_QUANTITY);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // call the filter subroutine
  // vDefineLabel("FILTER:"_Stz);
  //vAddCodeLabel(EO_CALL, "SUBROUTINE_FILTER:"_Stz);
  //vAddCodeLabel(EO_JMP_FALSE, "PROCESSROW_DONE:"_Stz);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_SHIPDATE);
  vAddCodeArg(EO_DATE, "1998-09-01"_Stz);
  vAddCode(EO_GREATERTHAN);
  vAddCodeLabel(EO_JMP_TRUE, "PROCESSROW_DONE:"_Stz);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  
  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  // add key L_RETURNFLAG, L_LINESTATUS
  //ray100
//  vAddCodeArg(EO_PUSH_LOCALS,Q1_LOCAL_RETURNFLAG);
//  vAddCode(EO_ENTWRK_APPENDELMS);
//  vAddCodeArg(EO_PUSH_LOCALS,Q1_LOCAL_LINESTATUS);
//  vAddCode(EO_ENTWRK_APPENDELMS);
  
  vAddCodeArg(EO_KEY_LCL2C_TO_I,8);
  vAddCode(EO_ENTWRK_SETINTELM);

  
  vAddCode(EO_ENT_FINDI);
  vAddCodeLabel(EO_JMP_TRUE,"FOUND"_Stz);

  
  
  // find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
//  vAddCode(EO_ENT_PUSH);
//  vAddCode(EO_PUSH_0);
//  vAddCode(EO_NOTEQUAL);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"NOT_FOUND"_Stz);
  
  vAddCode(EO_ENT_EQUAL);
  vAddCodeLabel(EO_JMP_TRUE,"FOUND"_Stz);
  
  // NOT_FOUND:
  vDefineLabel("NOT_FOUND"_Stz);
  vAddCodeArg(EO_ENTWRK_CREATEPAY, Q100_COLUMNS * C_BYTES_PER_WORD);
  vAddCode(EO_IXT_INS_ENT);
  //vAddCode(EO_IXT_DUMP);
  
  vAddCode(EO_PAY_ADDRESS);
  
  // store L_RETURNFLAG -> Q100_RETURNFLAG
  vAddCodeArg(EO_PUSH_LOCALS,Q1_LOCAL_RETURNFLAG);
  vAddCodeArg(EO_PAY_STORE_COLS, (i8)Q100_RETURNFLAG);
  
  // store Q1_LINESTATUS -> Q100_LINESTATUS
  vAddCodeArg(EO_PUSH_LOCALS,Q1_LOCAL_LINESTATUS);
  vAddCodeArg(EO_PAY_STORE_COLS, (i8)Q100_LINESTATUS);
  
  // set sum(L_QUANTITY) -> Q100_SUM_QTY
  vAddCode(EO_PUSH_0);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_QTY);
  
  // set sum(L_QUANTITY) -> Q100_SUM_BASE_PRICE
  vAddCode(EO_PUSH_0);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_BASE_PRICE);
  
  // set sum(L_QUANTITY) -> Q1_SUM_DISC_PRICE
  vAddCode(EO_PUSH_0);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_DISC_PRICE);
  
  // set sum(L_QUANTITY) -> Q100_SUM_CHARGE
  vAddCode(EO_PUSH_0);
  vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_CHARGE);
  //vAddCodeArg(EO_IXT_DUMP,(i8)0); ray commented 30 jul 2020
  //vAddCode(EO_IXT_DUMP);
  
  // FOUND:
  vDefineLabel("FOUND"_Stz);
 
  vAddCode(EO_PAY_ADDRESS);

  vOptimise(EO_SUPER_Q100); // +++++++++++++++++++++++++++++++++++++++++++++++++
  
    vAddCode(EO_BREAKPOINT);

    // sum(L_QUANTITY) -> Q100_SUM_QTY
    vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_QTY);
    vAddCodeArg(EO_PUSH_LOCALI,Q1_LOCAL_QUANTITY);
    vAddCode(EO_ADD);
    vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_QTY);

    // SUM(L_EXTENDEDPRICE) -> Q100_SUM_BASE_PRICE
    vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_BASE_PRICE);
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_EXTENDEDPRICE);
    vAddCode(EO_ADD);
    vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_BASE_PRICE);
  
    // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)) -> Q100_SUM_DISC_PRICE
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_EXTENDEDPRICE);
    vAddCodeArg(EO_INTEGER,100);
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_DISCOUNT);
    vAddCode(EO_SUBTRACT);
    vAddCode(EO_MULTIPLY);
    vAddCodeArg(EO_INTEGER,100);
    vAddCode(EO_DIVIDE);
    vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_DISC_PRICE);
    vAddCode(EO_ADD);
    vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_DISC_PRICE);
  

    // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)*(1+L_TAX)) -> Q100_SUM_CHARGE
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_EXTENDEDPRICE);
    vAddCodeArg(EO_INTEGER,100);
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_DISCOUNT);
    vAddCode(EO_SUBTRACT);
    vAddCode(EO_MULTIPLY);
    vAddCodeArg(EO_INTEGER,100);
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_TAX);
    vAddCode(EO_ADD);
    vAddCode(EO_MULTIPLY);
    vAddCodeArg(EO_INTEGER,10000);
    vAddCode(EO_DIVIDE);
    vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_CHARGE);
    vAddCode(EO_ADD);
    vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_CHARGE);
  

    // COUNT(L_QUANTITY) -> Q100_COUNT_QTY
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_QUANTITY);
    vAddCode(EO_ISNULL);
    vAddCodeLabel(EO_JMP_TRUE,"L_QUANTITY_NULL:"_Stz);
    vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_QTY);
    vAddCode(EO_INC);
    vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_COUNT_QTY);
    
    vDefineLabel("L_QUANTITY_NULL:"_Stz);

    // COUNT(L_EXTENDEDPRICE) -> Q100_COUNT_PRICE
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_EXTENDEDPRICE);
    vAddCode(EO_ISNULL);
    vAddCodeLabel(EO_JMP_TRUE,"L_EXTENTEDPRICE_NULL:"_Stz);
    vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_COUNT_PRICE);
    vAddCode(EO_INC);
    vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_COUNT_PRICE);
    
    vDefineLabel("L_EXTENTEDPRICE_NULL:"_Stz);


    // SUM(L_DISCOUNT) -> Q100_SUM_BASE_PRICE
    vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q100_SUM_DISC);
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_DISCOUNT);
    vAddCode(EO_ADD);
    vAddCodeArg(EO_PAY_STORE_COLI, (i8)Q100_SUM_DISC);

    // COUNT(L_DISCOUNT) -> Q100_COUNT_DISC
    vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_DISCOUNT);
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
  
  vEndOptimise(); // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //vAddCodeArg(EO_IXT_DUMP,(i8)0); ray commented 30 jul 2020
  //vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  //vAddCode(EO_IXT_DUMP);
  
  vDefineLabel("PROCESSROW_DONE:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BEGIN SUBROUTINE: SUBROUTINE_FILTER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("SUBROUTINE_FILTER:"_Stz);
  
  vAddCodeArg(EO_PUSH_LOCALI, (i8)Q1_LOCAL_SHIPDATE);
  vAddCodeArg(EO_DATE, "1998-09-01"_Stz);
  vAddCode(EO_GREATERTHAN);
  vAddCodeLabel(EO_JMP_TRUE, "RETURN_FALSE:"_Stz);
  
  // return true
  vAddCode(EO_PUSH_1);
  vAddCodeLabel(EO_JMP,"RETURN:"_Stz);
  
  // return false
  vDefineLabel("RETURN_FALSE:"_Stz);
  vAddCode(EO_PUSH_0);
  
  // return
  vDefineLabel("RETURN:"_Stz);
  vAddCode(EO_RETURN);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // END SUBROUTINE: SUBROUTINE_FILTER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BEGIN SUBROUTINE: SUBROUTINE_IXT_TO_RSB
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("SUBROUTINE_IXT_TO_RSB:"_Stz);
  //vAddCodeString(EO_PRINTF,"**100** SUBROUTINE_IXT_TO_RSB\n"_Stz);
  // begin index tree traversal
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"END_IXT_SRC:"_Stz);
  
  vDefineLabel("NEXT_IXT_SRC:"_Stz);
  
  // new a dst rsb row
  vAddCode(EO_RSB_DST_NEW);
  
  vAddCode(EO_PAY_ADDRESS);

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
  
  //vAddCode(EO_RSB_DST_PRINT);
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"NEXT_IXT_SRC:"_Stz);
  
  vDefineLabel("END_IXT_SRC:"_Stz);
  vAddCode(EO_RETURN);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // END SUBROUTINE: SUBROUTINE_IXT_TO_RSB
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate super agg group by (Query 1)                                      */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery103(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 103;
  
  // reset
  m_i_Code = 0;
  
  vDefineLabel("QRY_103:"_Stz);
  vAddCode(EO_INIT);
  vAddCode(EO_ENTER);
  
  //vAddCode(EO_RSB_SRC_TEST);
  vAddCodeLabel(EO_JMP_TRUE,"BEGIN_RSB_SRC:"_Stz);
  vAddCodeLabel(EO_CALL,"SUBROUTINE_IXT_TO_RSB:"_Stz);
  vAddCodeLabel(EO_JMP,"QRY_103_DONE:"_Stz);
  
  vDefineLabel("BEGIN_RSB_SRC:"_Stz);
  //vAddCodeString(EO_PRINTF,"**103** Got RSB BUFFER\n"_Stz);
  // -- -- -- -- -- -- -- -- -- -- --
  // insert each rsb
  
  // forearch src rsb
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NEXT_RSB_SRC:"_Stz);
  
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"QRY_103_DONE:"_Stz);
  
  //vAddCode(EO_RSB_SRC_PRINT);
  
  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  //ray100
  // add key L_RETURNFLAG, L_LINESTATUS
//  vAddCodeArg(EO_ENTWRK_APPENDELM, (i8)Q101_RETURNFLAG);
//  vAddCodeArg(EO_ENTWRK_APPENDELM, (i8)Q101_LINESTATUS);
  vAddCodeArg(EO_KEY_RSB2C_TO_I,11);
  vAddCode(EO_ENTWRK_SETINTELM);
  
  
  // find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
//  vAddCode(EO_ENT_PUSH);
//  vAddCode(EO_PUSH_0);
//  vAddCode(EO_NOTEQUAL);
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
  
  vAddCode(EO_PAY_ADDRESS);
  
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
  
  vAddCode(EO_PAY_ADDRESS);

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
  //vAddCode(EO_IXT_DUMP);
  
  vAddCodeLabel(EO_JMP, "NEXT_RSB_SRC:"_Stz);
  
  vDefineLabel("QRY_103_DONE:"_Stz);
  vAddCode(EO_RSB_SRC_NULL);
  
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BEGIN SUBROUTINE: SUBROUTINE_IXT_TO_RSB
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("SUBROUTINE_IXT_TO_RSB:"_Stz);
  
  //vAddCodeString(EO_PRINTF,"**103** SUBROUTINE_IXT_TO_RSB\n"_Stz);
  
  // begin index tree traversal
  vAddCode(EO_IXT_TRV_BEGIN);
  //vAddCode(EO_IXT_DUMP);
  vAddCodeLabel(EO_JMP_FALSE,"END_IXT_SRC:"_Stz);
  
  vDefineLabel("NEXT_IXT_SRC:"_Stz);
  
  // new a dst rsb row
  vAddCode(EO_RSB_DST_NEW);
  
  vAddCode(EO_PAY_ADDRESS);
  
  // Q101_RETURNFLAG -> Q104_RETURNFLAG
  vAddCodeArg(EO_PAY_FETCH_COLS, (i8)Q101_RETURNFLAG);
  vAddCodeArg(EO_RSB_DST_POPCOLS,(i8)Q104_RETURNFLAG);
  
  // Q101_LINESTATUS -> Q104_LINESTATUS
  vAddCodeArg(EO_PAY_FETCH_COLS, (i8)Q101_LINESTATUS);
  vAddCodeArg(EO_RSB_DST_POPCOLS,(i8)Q104_LINESTATUS);
  
  //vAddCode(EO_RSB_DST_PRINT);
  
  // Q101_SUM_QTY -> Q104_SUM_QTY
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_QTY);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q104_SUM_QTY);
  
  //vAddCode(EO_RSB_DST_PRINT);
  
  // Q101_SUM_BASE_PRICE -> Q104_SUM_BASE_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_BASE_PRICE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q104_SUM_BASE_PRICE);
  
  // Q101_SUM_DISC_PRICE -> Q104_SUM_DISC_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_DISC_PRICE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q104_SUM_DISC_PRICE);
  
  // Q101_SUM_CHARGE -> Q104_SUM_CHARGE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_CHARGE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q104_SUM_CHARGE);
  
  // Q101_SUM_QTY/Q101_COUNT_QTY -> Q104_AVG_QTY
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_QTY);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_QTY);
  vAddCode(EO_DIVIDE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q104_AVG_QTY);
  
  // Q101_SUM_BASE_PRICE/Q101_COUNT_PRICE -> Q104_AVG_PRICE
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_BASE_PRICE);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_PRICE);
  vAddCode(EO_DIVIDE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q104_AVG_PRICE);
  
  // Q101_SUM_DISC/Q101_COUNT_DISC -> Q104_AVG_DISC
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_SUM_DISC);
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_DISC);
  vAddCode(EO_DIVIDE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q104_AVG_DISC);
  
  // Q101_COUNT_ORDER -> Q104_COUNT_ORDER
  vAddCodeArg(EO_PAY_FETCH_COLI, (i8)Q101_COUNT_ORDER);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q104_COUNT_ORDER);
  
  //vAddCode(EO_RSB_DST_PRINT);
  
  // NEXT_IXT_SRC:
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"NEXT_IXT_SRC:"_Stz);
  
  vDefineLabel("END_IXT_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // END SUBROUTINE: SUBROUTINE_IXT_TO_RSB
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 3                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery300(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 300;
  
  m_i_Code = 0;
      
//  vSetCC(1,true);
//  vSetCC(2,true);
//  vSetCC(7,true);

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  // ROUTINE: QRY_300                                               Q300
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  vDefineLabel("QRY_300:"_Stz);
  
  vAddCode(EO_INIT);
  vAddCode(EO_ENTER);

  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_300**\n"_Stz);
  //vAddCode(EO_PIX_STM_PRINT);

  vAddCode(EO_PUSH_0);
  vAddCode(EO_CTR_0_POP);


  // test if rtn has been called again
  vAddCode(EO_CALLEDAGAIN);
  vAddCodeLabel(EO_JMP_TRUE,"PRC_CUS:"_Stz);

  vAddCode(EO_INTEGRITY);


  // iterate all CUSTOMER
  vAddCode(EO_IXT_SRC_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"END_CUS:"_Stz);

  vDefineLabel("GOT_CUS:"_Stz);
  
  vAddCode(EO_CTR_0_INC);
  vAddCode(EO_CTR_0_PUSH);
  vAddCodeArg(EO_INTEGER,10000);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_TRUE,"PRC_CUS:"_Stz);

  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_300 TO BE CALLED AGAIN**\n"_Stz);
  vAddCode(EO_CALLAGAIN);
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);
  
  vDefineLabel("PRC_CUS:"_Stz);

  vAddCode(EO_CRW_SRC_DEREF);
  
  vCC(1); vAddCodeArg(EO_COL_SRC_PUSHONE,C_CUSTKEY);
  vCC(1); vAddCodeString(EO_PRINTF,"Next C_CustKey %i\n"_Stz);


  vAddCodeArg(EO_COL_SRC_PUSHONE_a,C_MKTSEGMENT);
  vAddCodeString(EO_STR_LITERAL,"BUILDING"_Stz);
  vAddCode(EO_STR_COMPARE_a);
  vAddCodeLabel(EO_JMP_NOTEQUAL,"NXT_CUS:"_Stz);

  vCC(2); vAddCodeArg(EO_COL_SRC_PUSHONE,C_MKTSEGMENT);
  vCC(2); vAddCodeString(EO_PRINTF,"  satisfies C_MktSegment %s\n"_Stz);
  
  
  vAddCodeLabel(EO_CALL,"JOI_ORD:"_Stz);


  // next CUSTOMER
  vDefineLabel("NXT_CUS:"_Stz);
  vAddCode(EO_IXT_SRC_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"GOT_CUS:"_Stz);

  // end of CUSTOMER
  vDefineLabel("END_CUS:"_Stz);
  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_300 COMPLETES**\n"_Stz);
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END ROUTINE: QRY_300                                           Q300
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: JOI_ORD  -- join with orders                       Q300 JOI_ORD
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("JOI_ORD:"_Stz);
  
  vAddCode(EO_IXT_POS_ALTBYSRC);
  vAddCodeLabel(EO_JMP_FALSE,"RET_ORD:"_Stz);
  vDefineLabel("NXT_ORD:"_Stz);
  vAddCode(EO_IXT_ALT_NEXT);


  vAddCodeLabel(EO_JMP_FALSE,"RET_ORD:"_Stz);
  vAddCode(EO_IXT_CMP_SRCALT);
  vAddCodeLabel(EO_JMP_FALSE,"RET_ORD:"_Stz);

  
  vAddCode(EO_CRW_ALT_DEREF);
  
  vCC(2); vAddCodeArg(EO_COL_ALT_PUSHONE,O_ORDERKEY);
  vCC(2); vAddCodeString(EO_PRINTF,"  joined O_OrderKey %i\n"_Stz);

  vAddCodeArg(EO_COL_ALT_PUSHONE,O_ORDERDATE);
  vAddCodeArg(EO_DATE, "1995-03-15"_Stz);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_ORD:"_Stz);

  vAddCodeLabel(EO_CALL,"DST_PUT:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_ORD:"_Stz);
  
  vDefineLabel("RET_ORD:"_Stz);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END SUBROUTINE: JOI_ORD                                        Q300 JOI_ORD
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: DST_PUT                                            Q300 DST_PUT
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("DST_PUT:"_Stz);
  
  
  vCC(2); vAddCodeArg(EO_COL_ALT_PUSHONE,O_ORDERKEY);
  vCC(2); vAddCodeString(EO_PRINTF,"  partitioning O_OrderKey %i\n"_Stz);
  
  // calc i_Ptn
  vAddCode(EO_PTN_RESET);
  vAddCodeArg(EO_COL_ALT_PUSHONE,O_ORDERKEY);
  vAddCode(EO_PTN_RUNNING);
  vAddCode(EO_PTN_CALC);
  
  
//  vAddCode(EO_PUSH_MINUS1);
//  vAddCodeArg(EO_INTEGER,O_SHIPPRIORITY);      // 7
//  vAddCodeArg(EO_INTEGER,O_ORDERDATE);         // 4
//  vAddCodeArg(EO_INTEGER,O_ORDERKEY);          // 0
  vAddCodeArg(EO_SET_ITEMS,3);
  vAddCodeArg(EO_SET_COLUMN,O_SHIPPRIORITY);  // 7
  vAddCodeArg(EO_SET_COLUMN,O_ORDERDATE);     // 4
  vAddCodeArg(EO_SET_COLUMN,O_ORDERKEY);      // 0
  vAddCode(EO_COL_ALT_PUSHSET);

  vAddCode(EO_RSB_DST_NEW);
  
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_SHIPPRIORITY);
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_ORDERDATE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_ORDERKEY);


  vAddCode(EO_RETURN);
  
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END SUBROUTINE: DST_PUT                                        Q300 DST_PUT
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 3                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery301(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 301;
  
  m_i_Code = 0;
  
//  vSetCC(1,true);
//  vSetCC(2,true);
//  vSetCC(7,true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROUTINE: QRY_301  --                                           Q301
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("QRY_301:"_Stz);
  vAddCode(EO_INIT);
  vAddCode(EO_ENTER);
  
  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_301**\n"_Stz);
  
  vAddCode(EO_RSB_DST_NEW);

  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NXT_SRC:"_Stz);
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_SRC:"_Stz);

  //vAddCodeLabel(EO_CALL,"MOV_DST:"_Stz);
  vAddCodeLabel(EO_CALL,"AGG_REV:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_SRC:"_Stz);

  vDefineLabel("END_SRC:"_Stz);
  
  vAddCodeLabel(EO_CALL,"OUT_AGG:"_Stz);
  
  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_301 COMPLETES**\n"_Stz);
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END ROUTINE: QRY_301                                           Q301
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: MOV_DST  --                                        Q301 MOV_DST
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("MOV_DST:"_Stz);
  
  //vAddCode(EO_RSB_SRC_PRINT);

  vAddCode(EO_RSB_DST_ZERO);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERKEY);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_ORDERKEY);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERDATE);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_ORDERDATE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_SHIPPRIORITY);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_SHIPPRIORITY);

  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END ROUTINE: MOV_DST                                           Q301 MOV_DST
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: AGG_REV  --                                        Q301 AGG_REV
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("AGG_REV:"_Stz);
  
  vAddCode(EO_RSB_DST_ZERO);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERKEY);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_ORDERKEY);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERDATE);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_ORDERDATE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_SHIPPRIORITY);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q301_SHIPPRIORITY);

  vAddCode(EO_ENTWRK_RESET);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERKEY);
  vAddCode(EO_ENTWRK_SETINTELM);
  vAddCode(EO_IXT_POS_ALTBYWRK);
  vAddCodeLabel(EO_JMP_FALSE,"RET_AGG_f:"_Stz);
  
  vDefineLabel("NXT_AGG:"_Stz);
  vAddCode(EO_IXT_ALT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"RET_AGG_f:"_Stz);
  vAddCode(EO_IXT_CMP_WRKALT);
  vAddCodeLabel(EO_JMP_FALSE,"RET_AGG_f:"_Stz);
  
  vAddCode(EO_CRW_ALT_DEREF);
  
  vAddCodeArg(EO_COL_ALT_PUSHONE,L_SHIPDATE);
  vAddCodeArg(EO_DATE, "1995-03-15"_Stz);
  vAddCode(EO_GREATERTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_AGG:"_Stz);
  
  vAddCodeArg(EO_RSB_DST_PUSHCOLI,Q301_REVENUE);
  vAddCodeArg(EO_COL_ALT_PUSHONE,L_EXTENDEDPRICE);
  vAddCode(EO_ADD);

  vAddCodeArg(EO_INTEGER,100);
  vAddCodeArg(EO_COL_ALT_PUSHONE,L_DISCOUNT);
  vAddCode(EO_SUBTRACT);
  
  vAddCode(EO_MULTIPLY);
  vAddCodeArg(EO_INTEGER,100);
  vAddCode(EO_DIVIDE);

  vCC(2); vAddCode(EO_DUP);
  vCC(2); vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERKEY);
  vCC(2); vAddCodeString(EO_PRINTF,"  totalled L_OrderKey %i revenue %i\n"_Stz);
  
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q301_REVENUE);

  vAddCodeLabel(EO_JMP,"NXT_AGG:"_Stz);
  
  vDefineLabel("RET_AGG_f:"_Stz);
  
  // not quite right...
  vAddCodeLabel(EO_CALL,"LIM_SBR:"_Stz);
  
  vAddCode(EO_RETURN);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: LIM_SBR  --                                        Q301 LIM_SBR
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("LIM_SBR:"_Stz);

  //vAddCode(EO_IXT_TMP_ENTRIES);
  //vAddCode(EO_PRINT);

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
  //vAddCode(EO_DUP);
  //vAddCode(EO_PRINT);
  //vAddCode(EO_ENTWRK_APPENDELMI);
  vAddCode(EO_ENTWRK_SETINTELM);

  vAddCodeArg(EO_ENTWRK_CREATEPAY, 3 * C_BYTES_PER_WORD);

  vAddCode(EO_IXT_INS_ENT);
  
  vAddCode(EO_PAY_ADDRESS);

  vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_ORDERKEY);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)0);
  vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_ORDERDATE);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)1);
  vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_SHIPPRIORITY);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)2);

  vCC(2); vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_SHIPPRIORITY);
  vCC(2); vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_ORDERDATE);
  vCC(2); vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_ORDERKEY);
  vCC(2); vAddCodeString(EO_PRINTF,
            "  topping L_OrderKey %i L_OrderDate %d L_ShipPriority %i\n"_Stz);

  // -- -- -- -- -- -- -- -- --
  vDefineLabel("LIM_RET:"_Stz);
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END ROUTINE: LIM_SBR                                           Q301 LIM_SBR
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: OUT_AGG  --                                        Q301 OUT_AGG
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("OUT_AGG:"_Stz);
  
  vAddCode(EO_RSB_DST_RESET);

  
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);

  vDefineLabel("TRV_ECH:"_Stz);
  
  vAddCode(EO_RSB_DST_NEW);

  vAddCode(EO_PAY_ADDRESS);

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
  //vAddCode(EO_BREAKPOINT);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END ROUTINE: OUT_AGG                                           Q301 OUT_AGG
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 3                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery302(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 302;
  
  m_i_Code = 0;
  
  //vSetCC(7,true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROUTINE: QRY_302                                               Q302
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("QRY_302:"_Stz);
  vAddCode(EO_INIT);
  vAddCode(EO_ENTER);

  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_302**\n"_Stz);

  //vAddCode(EO_TRACEON);
  vAddCodeLabel(EO_JMP_FALSE,"QRY_OUT:"_Stz);

  vAddCodeString(EO_STR_LITERAL,"**QRY_302 RSB**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  // --
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NXT_SRC:"_Stz);
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"QRY_RET:"_Stz);

  
  //vAddCodeLabel(EO_CALL,"CPY_DST:"_Stz);
  vAddCodeLabel(EO_CALL,"LIM_SBR:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_SRC:"_Stz);
    
  // --
  vDefineLabel("QRY_OUT:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"**QRY_302 EOS**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  vAddCodeLabel(EO_CALL,"OUT_AGG:"_Stz);
  
  vAddCodeLabel(EO_JMP,"QRY_END:"_Stz);

  // --
  vDefineLabel("QRY_RET:"_Stz);
  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_302 RETURNS**\n"_Stz);
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);
  
  // --
  vDefineLabel("QRY_END:"_Stz);
  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_302 COMPLETES**\n"_Stz);
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END ROUTINE: QRY_302                                           Q302
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: LIM_SBR  --                                        Q302 LIM_SBR
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("LIM_SBR:"_Stz);

  //vAddCode(EO_IXT_TMP_ENTRIES);
  //vAddCode(EO_PRINT);

  vAddCode(EO_IXT_TMP_ENTRIES);
  vAddCodeArg(EO_INTEGER,10);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_TRUE,"LIM_INS:"_Stz);

  // -- -- -- -- -- -- -- -- --
  vDefineLabel("LIM_TST:"_Stz);
  
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"LIM_RET:"_Stz); // need an assert

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, Q301_REVENUE);
  vAddCode(EO_ENT_PUSHELMI0);
  vAddCode(EO_LESSEQUAL);
  vAddCodeLabel(EO_JMP_TRUE,"LIM_RET:"_Stz);

  vAddCode(EO_IXT_DEL_ENT);

  
  // -- -- -- -- -- -- -- -- --
  vDefineLabel("LIM_INS:"_Stz);

  vAddCode(EO_ENTWRK_RESET);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, Q301_REVENUE);
  //vAddCode(EO_DUP);
  //vAddCode(EO_PRINT);
  vAddCode(EO_ENTWRK_APPENDELMI);
  
  vAddCodeArg(EO_ENTWRK_CREATEPAY, 3 * C_BYTES_PER_WORD);

  vAddCode(EO_IXT_INS_ENT);
  
  vAddCode(EO_PAY_ADDRESS);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, Q301_ORDERKEY);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)0);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, Q301_ORDERDATE);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)1);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, Q301_SHIPPRIORITY);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)2);


  // -- -- -- -- -- -- -- -- --
  vDefineLabel("LIM_RET:"_Stz);
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END SUBROUTINE: QRY_302                                        Q302 LIM_SBR
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: OUT_AGG  --                                        Q302 OUT_AGG
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("OUT_AGG:"_Stz);
    
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);

  vDefineLabel("TRV_ECH:"_Stz);
  
  vAddCode(EO_RSB_DST_NEW);

  vAddCode(EO_PAY_ADDRESS);

  vAddCode(EO_ENT_PUSHELMI0);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q302_REVENUE);

  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)0);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q302_ORDERKEY);
  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)1);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q302_ORDERDATE);
  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)2);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q302_SHIPPRIORITY);


  vDefineLabel("TRV_NXT:"_Stz);
  vAddCode(EO_IXT_TRV_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"TRV_ECH:"_Stz);

  vDefineLabel("TRV_RET:"_Stz);
  //vAddCode(EO_BREAKPOINT);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END SUBROUTINE: QRY_302                                        Q302 OUT_AGG
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 4                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery400(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 400;
  
  m_i_Code = 0;
  
//  vSetCC(0,true);
//  vSetCC(1,true);
//  vSetCC(2,true);
//  vSetCC(7,true);

  //vAddCode(EO_TRACEON);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROUTINE: QRY_400                                               Q400
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("QRY_400:"_Stz);
  vAddCode(EO_INIT);
  vAddCode(EO_ENTER);

  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_400**\n"_Stz);

  // iterate all ORDERS
  vAddCode(EO_IXT_SRC_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"RET_ORD:"_Stz);
  
  vDefineLabel("GOT_ORD:"_Stz);
  vAddCode(EO_CRW_SRC_DEREF);
  
  vCC(1); vAddCodeArg(EO_COL_SRC_PUSHONE,O_ORDERKEY);
  vCC(1); vAddCodeString(EO_PRINTF,"Next O_OrderKey %i\n"_Stz);

  // test O_ORDERDATE
  vAddCodeArg(EO_COL_SRC_PUSHONE,O_ORDERDATE);
  vAddCode(EO_DUP);
  vAddCodeArg(EO_DATE,"1993-07-01"_Stz);
  vAddCode(EO_GREATEREQUAL);
  vAddCodeLabel(EO_JMP_TRUE,"TST_2ND:"_Stz);
  vAddCode(EO_POP);
  vAddCodeLabel(EO_JMP,"NXT_ORD:"_Stz);
  
  vDefineLabel("TST_2ND:"_Stz);
  vAddCodeArg(EO_DATE,"1993-10-01"_Stz);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_ORD:"_Stz);
  
  // test for a matching LINEITEM
  vAddCodeLabel(EO_CALL,"TST_ITM:"_Stz);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_ORD:"_Stz);
    

  // get the aggregator for o_orderpriority
  vAddCodeLabel(EO_CALL,"GET_AGR:"_Stz);
  
  vAddCode(EO_PAY_ADDRESS);
  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)0);
  vAddCode(EO_INC);
  vCC(0); vAddCode(EO_DUP);
  vCC(0); vAddCode(EO_ENT_PUSHELMS0);
  vCC(0); vAddCodeString(EO_PRINTF,"Inc O_OrderPriority %s (%i)\n"_Stz);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)0);

  // next ORDERS
  vDefineLabel("NXT_ORD:"_Stz);
  vAddCode(EO_IXT_SRC_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"GOT_ORD:"_Stz);

  // end of ORDERS
  vDefineLabel("RET_ORD:"_Stz);
    
  vAddCodeLabel(EO_CALL,"OUT_AGG:"_Stz);

  vCC(7); vAddCodeString(EO_PRINTF,"**QRY_400 COMPLETES**\n"_Stz);
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END ROUTINE: QRY_400                                           Q400
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: TST_ITM  -- test for matching lineitem             Q400 TST_ITM
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("TST_ITM:"_Stz);
  
  // position / iterate
  vAddCode(EO_IXT_POS_ALTBYSRC);

  vAddCodeLabel(EO_JMP_FALSE,"RET_ITM_f:"_Stz);
  vDefineLabel("NXT_ITM:"_Stz);
  vAddCode(EO_IXT_ALT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"RET_ITM_f:"_Stz);
  //vAddCode(EO_IXT_ALT_PRINT);
  vAddCode(EO_IXT_CMP_SRCALT);
  vAddCodeLabel(EO_JMP_FALSE,"RET_ITM_f:"_Stz);
  
  // test L_COMMITDATE < L_RECEIPTDATE
  vAddCode(EO_CRW_ALT_DEREF);
  
  vCC(2); vAddCodeArg(EO_COL_ALT_PUSHONE,L_LINENUMBER);
  vCC(2); vAddCodeArg(EO_COL_ALT_PUSHONE,L_ORDERKEY);
  vCC(2); vAddCodeString(EO_PRINTF,
                         "  joined L_OrderKey %i, L_LineNumber %i\n"_Stz);
  
  
//  vAddCode(EO_PUSH_MINUS1);
//  vAddCodeArg(EO_INTEGER,L_RECEIPTDATE);
//  vAddCodeArg(EO_INTEGER,L_COMMITDATE);
  vAddCodeArg(EO_SET_ITEMS,2);
  vAddCodeArg(EO_SET_COLUMN,L_RECEIPTDATE);
  vAddCodeArg(EO_SET_COLUMN,L_COMMITDATE);
  vAddCode(EO_COL_ALT_PUSHSET);

  
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_ITM:"_Stz);
  vAddCodeLabel(EO_JMP,"RET_ITM_t:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_ITM:"_Stz);
  
  

  // found one matching lineitem
  vDefineLabel("RET_ITM_t:"_Stz);
  
  vCC(2); vAddCodeArg(EO_COL_ALT_PUSHONE,L_RECEIPTDATE);
  vCC(2); vAddCodeArg(EO_COL_ALT_PUSHONE,L_COMMITDATE);
  vCC(2); vAddCodeString(EO_PRINTF,
                         "  satisfies L_CommitDate %d, L_ReceiptDate %d\n"_Stz);

  vAddCode(EO_PUSH_1);
  vAddCode(EO_RETURN);

  
  // no matching lineitem
  vDefineLabel("RET_ITM_f:"_Stz);
  vAddCode(EO_PUSH_0);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END SUBROUTINE: RET_ITM                                        Q400 RET_ITM
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: GET_AGR                                            Q400 GET_AGR
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get aggregator for o_orderpriority
  vDefineLabel("GET_AGR:"_Stz);

  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  // add key O_ORDERPRIORITY
  
  //vAddCodeArg(EO_COL_SRC_PUSHONE,O_ORDERPRIORITY);
  //vAddCode(EO_STR_PRINTSTK);

  vAddCodeArg(EO_COL_SRC_PUSHONE,O_ORDERPRIORITY);
  vAddCode(EO_ENTWRK_APPENDELMS);

  // attempt to find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
//  vAddCode(EO_ENT_PUSH);
  vAddCodeLabel(EO_JMP_FALSE,"NEW_AGR:"_Stz);

  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"NEW_AGR:"_Stz);
  
  //vAddCode(EO_ENT_PRINTKEYS);
  vAddCode(EO_ENT_EQUAL);
  vAddCodeLabel(EO_JMP_FALSE,"NEW_AGR:"_Stz);
  
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);

  // found the agg
  vAddCodeLabel(EO_JMP,"END_AGR:"_Stz);
  
  // did not find agg, new one
  vDefineLabel("NEW_AGR:"_Stz);
  
  vAddCodeArg(EO_ENTWRK_CREATEPAY, 1 * C_BYTES_PER_WORD);
  
  vAddCode(EO_IXT_INS_ENT);
  
  vAddCode(EO_PAY_ADDRESS);

  vAddCode(EO_PUSH_0);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)0);
  
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);

  vDefineLabel("END_AGR:"_Stz);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END SUBROUTINE: GET_AGR                                        Q400 GET_AGR
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SUBROUTINE: OUT_AGG                                            Q400 OUT_AGG
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("OUT_AGG:"_Stz);

  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);

  vDefineLabel("TRV_ECH:"_Stz);

  vAddCode(EO_RSB_DST_NEW);

  vAddCode(EO_ENT_PUSHELMS0);
  vAddCodeArg(EO_RSB_DST_POPCOLS, Q400_ORDERPRIORITY);

  vAddCode(EO_PAY_ADDRESS);
  
  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)0);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q400_ORDERCOUNT);

  vDefineLabel("TRV_NXT:"_Stz);
  vAddCode(EO_IXT_TRV_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"TRV_ECH:"_Stz);

  vDefineLabel("TRV_RET:"_Stz);
  vAddCode(EO_RETURN);
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // END SUBROUTINE: OUT_AGG                                        Q400 OUT_AGG
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 4                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery401(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 401;
  
  m_i_Code = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q401
  vDefineLabel("QRY_401:"_Stz);
  vAddCode(EO_INIT);
  vAddCode(EO_ENTER);

  //vAddCode(EO_TRACEON);
  
  vAddCodeLabel(EO_JMP_FALSE,"QRY_OUT:"_Stz);

  vAddCodeString(EO_STR_LITERAL,"**QRY_401 RSB**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  vAddCodeLabel(EO_CALL,"ITR_SRC:"_Stz);
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);

  vDefineLabel("QRY_OUT:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"**QRY_401 EOS**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  vAddCodeLabel(EO_CALL,"OUT_AGG:"_Stz);
  vAddCode(EO_EXIT);
  vAddCode(EO_RETURN);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q401 ITR_SRC
  vDefineLabel("ITR_SRC:"_Stz);

  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NXT_SRC:"_Stz);
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"ITR_END:"_Stz);

  //vAddCode(EO_RSB_SRC_PRINT);
  
  vAddCodeLabel(EO_CALL,"GET_AGR:"_Stz);

  vAddCodeLabel(EO_CALL,"SUP_AGR:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_SRC:"_Stz);
  
  // --
  vDefineLabel("ITR_END:"_Stz);
  vAddCode(EO_RETURN);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q401 SUP_AGR
  vDefineLabel("SUP_AGR:"_Stz);

  vAddCode(EO_PAY_ADDRESS);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q400_ORDERCOUNT);
  vAddCodeArg(EO_PAY_FETCH_COLI,  (i8)0);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_PAY_STORE_COLI,  (i8)0);

  vDefineLabel("SUP_END:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q401 GET_AGR
  // get aggregator for o_orderpriority
  vDefineLabel("GET_AGR:"_Stz);

  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  // add key O_ORDERPRIORITY
  vAddCodeArg(EO_RSB_SRC_PUSHCOLS,Q400_ORDERPRIORITY);
  vAddCode(EO_ENTWRK_APPENDELMS);

  // attempt to find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
//  vAddCode(EO_ENT_PUSH);
  vAddCodeLabel(EO_JMP_FALSE,"NEW_AGR:"_Stz);

  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"NEW_AGR:"_Stz);
  
  vAddCode(EO_ENT_EQUAL);
  vAddCodeLabel(EO_JMP_FALSE,"NEW_AGR:"_Stz);
  
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);

  // found the agg
  vAddCodeLabel(EO_JMP,"END_AGR:"_Stz);
  
  // did not find agg, new one
  vDefineLabel("NEW_AGR:"_Stz);
  
  vAddCodeArg(EO_ENTWRK_CREATEPAY, 1 * C_BYTES_PER_WORD);
  
  vAddCode(EO_IXT_INS_ENT);
  
  vAddCode(EO_PAY_ADDRESS);

  vAddCode(EO_PUSH_0);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)0);
  
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);

  vDefineLabel("END_AGR:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q401 OUT_AGG
  vDefineLabel("OUT_AGG:"_Stz);

  //vAddCode(EO_TRACEON);

  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);

  vDefineLabel("TRV_ECH:"_Stz);

  vAddCode(EO_RSB_DST_NEW);

  vAddCode(EO_PAY_ADDRESS);

  vAddCode(EO_ENT_PUSHELMS0);
  vAddCodeArg(EO_RSB_DST_POPCOLS, Q401_ORDERPRIORITY);

  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)0);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q401_ORDERCOUNT);

  vDefineLabel("TRV_NXT:"_Stz);
  vAddCode(EO_IXT_TRV_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"TRV_ECH:"_Stz);

  vDefineLabel("TRV_RET:"_Stz);
  vAddCode(EO_TRACEOFF);
  //vAddCode(EO_BREAKPOINT);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate iterating index on l_shipdate                    (Query 6)        */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery600(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 600;
  
  // reset
  m_i_Code = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BEGIN MAIN: QRY_600
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("QRY_600:"_Stz);
  vAddCode(EO_INIT);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the lower key
  vAddCodeArg(EO_DATE, "1994-01-01"_Stz);
  //vAddCode(EO_ENTWRK_APPENDELMI);
  vAddCode(EO_ENTWRK_SETINTELM);
  vAddCode(EO_IXT_SRC_LBOUND);
  vAddCodeLabel(EO_JMP_TRUE, "UPPERBOUND:"_Stz);
  
  // no lower key, start position at beginning of index
  vAddCode(EO_IXT_SRC_BEGIN);
  
  // no entry, return
  vAddCodeLabel(EO_JMP_FALSE, "QRY_600_DONE:"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the upper key
  vDefineLabel("UPPERBOUND:"_Stz);
  vAddCode(EO_ENTWRK_RESET);
  vAddCodeArg(EO_DATE, "1995-01-01"_Stz);
  //vAddCode(EO_ENTWRK_APPENDELMI);
  vAddCode(EO_ENTWRK_SETINTELM);
  vAddCode(EO_IXT_SRC_UBOUND_LT);
  vAddCodeLabel(EO_JMP_TRUE, "TRV_TILL_UPPER:"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no upper key, traverse till end
  vDefineLabel("TRV_TILL_END:"_Stz);
  vAddCode(EO_IXT_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE, "QRY_600_DONE:"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // call PROCESSROW subroutine to process the row
  vAddCodeLabel(EO_CALL, "SUBROUTINE_PROCESSROW:"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // loop back to getting next row till end
  vAddCodeLabel(EO_JMP, "TRV_TILL_END:"_Stz);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // traverse the index till we reach upper bound
  vDefineLabel("TRV_TILL_UPPER:"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fetch next entry from index
  vAddCode(EO_IXT_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE, "QRY_600_DONE:"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // have we reached the upper entry
  vAddCode(EO_ENT_SRC_PUSH);
  vAddCode(EO_ENT_SRC_PUSHUPPER);
  vAddCode(EO_EQUAL);
  vAddCodeLabel(EO_JMP_TRUE, "QRY_600_DONE:"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // call PROCESSROW subroutine to process the row
  vAddCodeLabel(EO_CALL, "SUBROUTINE_PROCESSROW:"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // loop back to getting next row
  vAddCodeLabel(EO_JMP, "TRV_TILL_UPPER:"_Stz);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // query done
  vDefineLabel("QRY_600_DONE:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // END MAIN: QRY_600
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BEGIN SUBROUTINE: SUBROUTINE_PROCESSROW
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("SUBROUTINE_PROCESSROW:"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fetch the crw
  vAddCode(EO_CRW_SRC_DEREF);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // inflate crw required columns
//  vAddCode(EO_PUSH_MINUS1);
//  vAddCodeArg(EO_INTEGER,L_DISCOUNT);
//  vAddCodeArg(EO_INTEGER,L_EXTENDEDPRICE);
//  vAddCodeArg(EO_INTEGER,L_QUANTITY);
  vAddCodeArg(EO_SET_ITEMS,3);
  vAddCodeArg(EO_SET_COLUMN,L_DISCOUNT);
  vAddCodeArg(EO_SET_COLUMN,L_EXTENDEDPRICE);
  vAddCodeArg(EO_SET_COLUMN,L_QUANTITY);
  vAddCode(EO_COL_SRC_PUSHSET);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pop inflated columns into the locals register
  vAddCodeArg(EO_POP_LOCALI,Q6_LOCAL_DISCOUNT);
  vAddCodeArg(EO_POP_LOCALI,Q6_LOCAL_EXTENDEDPRICE);
  vAddCodeArg(EO_POP_LOCALI,Q6_LOCAL_QUANTITY);

  // new a rsb dst row if not there
  vAddCode(EO_RSB_DST_TESTROW);
  vAddCodeLabel(EO_JMP_TRUE,"FILTER:"_Stz);
  vAddCode(EO_RSB_DST_NEW);
  
  // initialze the row
  vAddCode(EO_PUSH_0);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q600_REVENUE);
  
  //vAddCode(EO_RSB_DST_PRINT);
  
  // call the filter subroutine
  vDefineLabel("FILTER:"_Stz);
  vAddCodeLabel(EO_CALL, "SUBROUTINE_FILTER:"_Stz);
  vAddCodeLabel(EO_JMP_FALSE, "PROCESSROW_DONE:"_Stz);
  
  // call the aggregate subroutine
  vAddCodeLabel(EO_CALL, "SUBROUTINE_AGGREGATE:"_Stz);
  //vAddCode(EO_PUSH_0);
  vAddCodeLabel(EO_JMP, "PROCESSROW_DONE:"_Stz);
  
  //vDefineLabel("FILTER_FALSE:"_Stz);
  //vAddCode(EO_PUSH_0);
  
  vDefineLabel("PROCESSROW_DONE:"_Stz);
  vAddCode(EO_RETURN);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // END SUBROUTINE: SUBROUTINE_PROCESSROW
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BEGIN SUBROUTINE: SUBROUTINE_FILTER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("SUBROUTINE_FILTER:"_Stz);
  
  vAddCodeArg(EO_PUSH_LOCALI,Q6_LOCAL_DISCOUNT);
  vAddCodeArg(EO_INTEGER, 5);
  vAddCode(EO_GREATEREQUAL);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);
  
  vAddCodeArg(EO_PUSH_LOCALI,Q6_LOCAL_DISCOUNT);
  vAddCodeArg(EO_INTEGER, 7);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);
  
  vAddCodeArg(EO_PUSH_LOCALI,Q6_LOCAL_QUANTITY);
  vAddCodeArg(EO_INTEGER, 2400);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE, "RETURN_FALSE:"_Stz);
  
  // return true
  vAddCode(EO_PUSH_1);
  vAddCodeLabel(EO_JMP,"RETURN:"_Stz);
  
  // return false
  vDefineLabel("RETURN_FALSE:"_Stz);
  vAddCode(EO_PUSH_0);
  
  // return
  vDefineLabel("RETURN:"_Stz);
  vAddCode(EO_RETURN);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // END SUBROUTINE: SUBROUTINE_FILTER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BEGIN SUBROUTINE: SUBROUTINE_AGGREGATE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDefineLabel("SUBROUTINE_AGGREGATE:"_Stz);
  
  vAddCodeArg(EO_PUSH_LOCALI,Q6_LOCAL_DISCOUNT);
  vAddCodeArg(EO_PUSH_LOCALI,Q6_LOCAL_EXTENDEDPRICE);
  
  vAddCode(EO_MULTIPLY);
  vAddCodeArg(EO_INTEGER,100);
  vAddCode(EO_DIVIDE);
  
  vAddCodeArg(EO_RSB_DST_PUSHCOLI,(i8)Q600_REVENUE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q600_REVENUE);
  
  // TEMP !!!
  // COUNT(*) -> Q600_COUNT
  //  vAddCodeArg(EO_RSB_DST_PUSHCOLI, (i8)Q600_COUNT);
  //  vAddCode(EO_INC);
  //  vAddCodeArg(EO_RSB_DST_POPCOLI, (i8)Q600_COUNT);
  
  vAddCode(EO_RETURN);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // END SUBROUTINE: SUBROUTINE_AGGREGATE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate super agg (query 6)                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery601(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 601;
  
  // init the code
  m_i_Code = 0;
  
  vDefineLabel("QRY_601:"_Stz);
  vAddCode(EO_INIT);
  
  // new a rsb dst row if not there
  vAddCode(EO_RSB_DST_TESTROW);
  vAddCodeLabel(EO_JMP_TRUE,"RSB_SRC_BEGIN:"_Stz);
  vAddCode(EO_RSB_DST_NEW);
  
  // initialize the row
  vAddCode(EO_PUSH_0);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q600_REVENUE);
  //vAddCode(EO_RSB_DST_PRINT);
  
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
  //vAddCode(EO_RSB_SRC_PRINT);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q600_REVENUE);
  vAddCodeArg(EO_RSB_DST_PUSHCOLI,(i8)Q601_REVENUE);
  vAddCode(EO_ADD);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q601_REVENUE);
  
  // TEMP !!!
  // COUNT(*) -> Q600_COUNT
//  vAddCodeArg(EO_RSB_SRC_PUSHCOLI, (i8)Q600_COUNT);
//  vAddCodeArg(EO_RSB_DST_PUSHCOLI,(i8)Q601_COUNT);
//  vAddCode(EO_ADD);
//  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q601_COUNT);
  
  
  vAddCode(EO_RETURN);
    
  //vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code test 1700                                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerate1700(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_xRluCommon.m_iSql = 1700;
  m_i_Code = 0;

  vDefineLabel("QRY_1700:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"**QRY_1700**"_Stz);
  vAddCode(EO_STR_PRINTADR);

  vAddCode(EO_RETURN);
}

/*----------------------------------------------------------------------------*/
/* generate code test 1701                                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerate1701(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_xRluCommon.m_iSql = 1701;
  m_i_Code = 0;

  vDefineLabel("QRY_1701:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"**QRY_1701**"_Stz);
  vAddCode(EO_STR_PRINTADR);

  vAddCode(EO_RETURN);
}

/*----------------------------------------------------------------------------*/
/* generate code test 1702                                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerate1702(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_xRluCommon.m_iSql = 1702;
  m_i_Code = 0;

  vDefineLabel("QRY_1702:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"**QRY_1702**"_Stz);
  vAddCode(EO_STR_PRINTADR);

  vAddCode(EO_RETURN);
}

/*----------------------------------------------------------------------------*/
/* generate code for dynamic compilation test 100000                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerate100000(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 100000;
  
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
/* generate code for dynamic compilation test 100001                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerate100001(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_xRluCommon.m_iSql = 100001;
  
  // init the code
  m_i_Code = 0;
  
  vDefineLabel("RTN_TST:"_Stz);
  vAddCode(EO_INIT);
  vAddCodeLabel(EO_JMP, "LBL_A:"_Stz);

  vDefineLabel("LBL_A:"_Stz);
  vAddCodeArg(EO_RANDOM,100);
  vAddCode(EO_PRINT);
  vAddCode(EO_DTE_NOW);
  vAddCode(EO_DTE_YEAR);
  vAddCode(EO_PRINT);
  vAddCode(EO_DTE_NOW);
  vAddCode(EO_DTE_PRINT);
  vAddCode(EO_HSH_RESET);
  vAddCodeArg(EO_INTEGER,42);
  vAddCode(EO_HSH_ADD_HASHI);
  vAddCodeArg(EO_INTEGER,3);
  vAddCode(EO_SLEEP);
  
  vAddCodeLabel(EO_CALL,"SBR_ADD:"_Stz);
  vAddCodeLabel(EO_CALL,"SBR_MUL:"_Stz);
  vAddCodeLabel(EO_CALL,"SBR_PRT:"_Stz);
  vAddCode(EO_RETURN);

  vDefineLabel("SBR_ADD:"_Stz);
  vAddCodeArg(EO_INTEGER,1);
  vAddCodeArg(EO_INTEGER,2);
  vAddCode(EO_ADD);
  vAddCode(EO_RETURN);
  
  vDefineLabel("SBR_PRT:"_Stz);
  vAddCode(EO_PRINT);
  vAddCode(EO_RETURN);
  
  vDefineLabel("SBR_MUL:"_Stz);
  vAddCodeArg(EO_INTEGER,4);
  vAddCode(EO_MULTIPLY);
  vAddCode(EO_RETURN);

  vDumpCode();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
