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
    case     101: vGenerateQuery101();    break;
    case     102: vGenerateQuery102();    break;
    case     103: vGenerateQuery103();    break;
    case     111: vGenerateQuery111();    break;
 
    case     300: vGenerateQuery300();    break;
    case     301: vGenerateQuery301();    break;
    //case     302: vGenerateQuery302();    break;
    case     302: vGenerateQuery302b();    break;

    case     400: vGenerateQuery400();    break;
    case     401: vGenerateQuery401();    break;

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
  //vAddCodeArg(EO_IXT_DUMP,(i8)0); ray commented 30 jul 2020

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
  //vAddCodeArg(EO_IXT_DUMP,(i8)0); ray commented 30 jul 2020
  
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
  
  //vAddCodeArg(EO_IXT_DUMP,(i8)0); ray commented 30 jul 2020
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
  
  //vAddCode(EO_RSB_DST_PRINT);
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
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);
  
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
  
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);
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
  
  //vAddCode(EO_RSB_DST_PRINT);
  
  // NEXT_IXT_SRC:
  vAddCode(EO_ENT_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"NEXT_IXT_SRC:"_Stz);
  
  vDefineLabel("END_IXT_SRC:"_Stz);
  vAddCode(EO_RETURN);
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate project into RSB (Query 1)                                        */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery111(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 111;
  
  // reset
  m_i_Code = 0;
      
  // RESET COLUMNS NOT NEEDED
  vAddCodeArg(EO_COL_DST_RESET, L_ORDERKEY);
  vAddCodeArg(EO_COL_DST_RESET, L_PARTKEY);
  vAddCodeArg(EO_COL_DST_RESET, L_SUPPKEY);
  vAddCodeArg(EO_COL_DST_RESET, L_LINENUMBER);
  vAddCodeArg(EO_COL_DST_RESET, L_COMMITDATE);
  vAddCodeArg(EO_COL_DST_RESET, L_RECEIPTDATE);
  vAddCodeArg(EO_COL_DST_RESET, L_SHIPINSTRUCT);
  vAddCodeArg(EO_COL_DST_RESET, L_SHIPMODE);
  vAddCodeArg(EO_COL_DST_RESET, L_COMMENT);
  
  // forearch src rsb
  vAddCode(EO_CRW_SRC_BEGIN);
  
  // NEXT_CRW_SRC:
  vDefineLabel("NEXT_CRW_SRC:"_Stz);
  vAddCode(EO_CRW_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_CRW_SRC:"_Stz);
  
  // create a new rsb
  vAddCode(EO_RSB_DST_NEW);
  
  // inflate the src CRW to RSB
  vAddCode(EO_CRW_SRC_TORSB);
  //vAddCode(EO_RSB_DST_PRINT);
  
  vAddCodeLabel(EO_JMP, "NEXT_CRW_SRC:"_Stz);
  
  vDefineLabel("END_CRW_SRC:"_Stz);
  
  vAddCode(EO_RETURN);
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
      
//  vAddCode(EO_SLOT);
//  vAddCodeArg(EO_INTEGER,1);
//  vAddCode(EO_EQUAL);
//  vAddCodeLabel(EO_JMP_FALSE,"QRY_300:"_Stz);
//  vAddCodeString(EO_STR_LITERAL,"SLEEPING"_Stz);
//  vAddCode(EO_STR_PRINTADR);
//  vAddCodeArg(EO_INTEGER,5);
//  vAddCode(EO_SLEEP);




  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q300
  vDefineLabel("QRY_300:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"**QRY_300**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  vAddCode(EO_PIX_STM_PRINT);
  //vAddCode(EO_TRACEON);

  // iterate all CUSTOMER
  vAddCode(EO_IXT_SRC_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"END_CUS:"_Stz);
  vDefineLabel("GOT_CUS:"_Stz);
  //vAddCode(EO_IXT_SRC_PRINT);
  
  vAddCode(EO_CRW_SRC_FETCH);
  //vAddCodeArg(EO_COL_SRC_PUSHSTR,C_MKTSEGMENT);
  //vAddCode(EO_STR_PRINTSTK);
  
  vAddCodeArg(EO_COL_SRC_PUSHSTR,C_MKTSEGMENT);
  vAddCodeString(EO_STR_LITERAL,"BUILDING"_Stz);
  vAddCode(EO_STR_COMPARE);
  vAddCodeLabel(EO_JMP_NOTEQUAL,"NXT_CUS:"_Stz);

  
  vAddCodeLabel(EO_CALL,"JOI_ORD:"_Stz);


  // next CUSTOMER
  vDefineLabel("NXT_CUS:"_Stz);
 
//  vAddCode(EO_CTR_0_PUSH);
//  vAddCodeArg(EO_INTEGER,10);
//  vAddCode(EO_LESSEQUAL);
//  vAddCodeLabel(EO_JMP_FALSE,"END_CUS:"_Stz);


  vAddCode(EO_IXT_SRC_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"GOT_CUS:"_Stz);

  // end of CUSTOMER
  vDefineLabel("END_CUS:"_Stz);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q300 JOI_ORD
  // join ORDERS
  vDefineLabel("JOI_ORD:"_Stz);
  //vAddCodeString(EO_STR_LITERAL,"Join ORDERS"_Stz);
  //vAddCode(EO_STR_PRINTADR);
  
  vAddCode(EO_IXT_POS_ALTBYSRC);
  vAddCodeLabel(EO_JMP_FALSE,"RET_ORD:"_Stz);
  vDefineLabel("NXT_ORD:"_Stz);
  vAddCode(EO_IXT_ALT_NEXT);
  
  //vAddCode(EO_IXT_SRC_PRINT);
  //vAddCode(EO_IXT_ALT_PRINT);


  vAddCodeLabel(EO_JMP_FALSE,"RET_ORD:"_Stz);
  vAddCode(EO_IXT_CMP_SRCALT);
  vAddCodeLabel(EO_JMP_NOTEQUAL,"RET_ORD:"_Stz);

  //vAddCode(EO_IXT_ALT_PRINT);
  
  vAddCode(EO_CRW_ALT_FETCH);
  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERDATE);
  vAddCodeArg(EO_DATE, "1995-03-15"_Stz);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_ORD:"_Stz);

  //vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERDATE);
  //vAddCode(EO_DTE_PRINT);

  vAddCodeLabel(EO_CALL,"DST_PUT:"_Stz);
//  vAddCode(EO_RSB_DST_NEW);
//  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERKEY);
//  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_ORDERKEY);
//  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERDATE);
//  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_ORDERDATE);
//  vAddCodeArg(EO_COL_ALT_PUSH,O_SHIPPRIORITY);
//  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_SHIPPRIORITY);
//  vAddCode(EO_RSB_DST_PRINT);

  vAddCodeLabel(EO_JMP,"NXT_ORD:"_Stz);
  
  vDefineLabel("RET_ORD:"_Stz);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q300 DST_PUT
  vDefineLabel("DST_PUT:"_Stz);
  
  // calc i_Ptn (should be zero for the moment)
  vAddCode(EO_PTN_RESET);
  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERKEY);
  vAddCode(EO_PTN_RUNNING);
  vAddCode(EO_PTN_CALC);
//  vAddCode(EO_PTN_PUSH);
//  vAddCode(EO_LGL_NOT);
//  vAddCode(EO_ASSERT);

  
  vAddCode(EO_RSB_DST_NEW);
  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERKEY);
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_ORDERKEY);
  vAddCodeArg(EO_COL_ALT_PUSH,O_ORDERDATE);
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_ORDERDATE);
  vAddCodeArg(EO_COL_ALT_PUSH,O_SHIPPRIORITY);
  vAddCodeArg(EO_RSB_DST_POPCOLI,Q300_SHIPPRIORITY);
  //vAddCode(EO_RSB_DST_PRINT);
  
  vAddCode(EO_CTR_0_INC);


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
  
  vDefineLabel("QRY_301:"_Stz);
  //vAddCodeArg(EO_RANDOM,100);
  //vAddCode(EO_PRINT);
  //vAddCode(EO_BREAKPOINT);
  vAddCodeString(EO_STR_LITERAL,"**QRY_301**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  vAddCode(EO_PIX_STM_PRINT);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q301

  vAddCode(EO_RSB_DST_NEW);

  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NXT_SRC:"_Stz);
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_SRC:"_Stz);

  vAddCodeLabel(EO_CALL,"MOV_DST:"_Stz);
  vAddCodeLabel(EO_CALL,"AGG_REV:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_SRC:"_Stz);

  vDefineLabel("END_SRC:"_Stz);
  
  vAddCodeLabel(EO_CALL,"TRV_IXT:"_Stz);
  
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q301 MOV_DST
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q301 AGG_REV
  vDefineLabel("AGG_REV:"_Stz);
  
  vAddCode(EO_ENTWRK_RESET);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q300_ORDERKEY);
  vAddCode(EO_ENTWRK_APPENDELMI);

  vAddCode(EO_IXT_POS_ALTBYWRK);
  vAddCodeLabel(EO_JMP_FALSE,"RET_AGG_f:"_Stz);
  vDefineLabel("NXT_AGG:"_Stz);
  vAddCode(EO_IXT_ALT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"RET_AGG_f:"_Stz);
  //vAddCode(EO_IXT_ALT_PRINT);
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
  //vAddCodeArg(EO_RSB_DST_PUSHCOLI, Q301_REVENUE);
  //vAddCode(EO_PRINT);

  vAddCodeLabel(EO_JMP,"NXT_AGG:"_Stz);
  
  vDefineLabel("RET_AGG_f:"_Stz);
  
  // not quite right...
  vAddCodeLabel(EO_CALL,"LIM_SBR:"_Stz);
  
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q301 LIM_SBR
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
  vAddCodeArg(EO_IXT_DUMP,(i8)0);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q301 TRV_IXT
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
/* generate code for query 3                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery302(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 302;
  
  m_i_Code = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q302
  vDefineLabel("QRY_302:"_Stz);

  vAddCodeLabel(EO_JMP_FALSE,"QRY_END:"_Stz);

  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NXT_SRC:"_Stz);
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"END_SRC:"_Stz);

  //vAddCodeLabel(EO_CALL,"CPY_DST:"_Stz);
  vAddCodeLabel(EO_CALL,"LIM_SBR:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_SRC:"_Stz);

  vDefineLabel("END_SRC:"_Stz);
  
  vAddCodeLabel(EO_CALL,"TRV_IXT:"_Stz);
  
  vDefineLabel("QRY_END:"_Stz);
  vAddCode(EO_RETURN);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q302 LIM_SBR
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
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q302 TRV_IXT
  vDefineLabel("TRV_IXT:"_Stz);
    
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);

  vDefineLabel("TRV_ECH:"_Stz);
  
  vAddCode(EO_RSB_DST_NEW);

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
  vAddCode(EO_BREAKPOINT);
  vAddCode(EO_RETURN);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q302 CPY_DST
  vDefineLabel("CPY_DST:"_Stz);
  
  vAddCode(EO_RSB_DST_NEW);
  vAddCode(EO_RSB_DST_ZERO);

  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q301_ORDERKEY);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q302_ORDERKEY);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q301_REVENUE);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q302_REVENUE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q301_ORDERDATE);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q302_ORDERDATE);
  vAddCodeArg(EO_RSB_SRC_PUSHCOLI,Q301_SHIPPRIORITY);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q302_SHIPPRIORITY);

  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 3                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery302b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 302;
  
  m_i_Code = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q302
  vDefineLabel("QRY_302:"_Stz);

  vAddCodeLabel(EO_JMP_FALSE,"QRY_OUT:"_Stz);

  vAddCodeString(EO_STR_LITERAL,"**QRY_302 RSB**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  // --
  vAddCode(EO_RSB_SRC_BEGIN);
  
  vDefineLabel("NXT_SRC:"_Stz);
  vAddCode(EO_RSB_SRC_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"QRY_END:"_Stz);

  //vAddCodeLabel(EO_CALL,"CPY_DST:"_Stz);
  vAddCodeLabel(EO_CALL,"LIM_SBR:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_SRC:"_Stz);

  vDefineLabel("END_SRC:"_Stz);
  
  
  // --
  vDefineLabel("QRY_OUT:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"**QRY_302 EOS**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  vAddCodeLabel(EO_CALL,"TRV_IXT:"_Stz);
  
  
  // --
  vDefineLabel("QRY_END:"_Stz);
  vAddCode(EO_RETURN);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q302 LIM_SBR
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
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q302 TRV_IXT
  vDefineLabel("TRV_IXT:"_Stz);
    
  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);

  vDefineLabel("TRV_ECH:"_Stz);
  
  vAddCode(EO_RSB_DST_NEW);

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
  
//  vAddCodeArg(EO_DATE, "1993-07-01"_Stz);
//  vAddCode(EO_DTE_PRINT);
//  vAddCodeArg(EO_DATE, "1993-10-01"_Stz);
//  vAddCode(EO_DTE_PRINT);
//
//  vAddCodeArg(EO_DATE, "1993-07-01"_Stz);
//  vAddCodeArg(EO_DATE, "1993-10-01"_Stz);
//  vAddCode(EO_LESSTHAN);
//  vAddCode(EO_PRINT);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q400
  vDefineLabel("QRY_400:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"**QRY_400**"_Stz);
  vAddCode(EO_STR_PRINTADR);

  //vAddCode(EO_TRACEON);

  // iterate all ORDERS
  vAddCode(EO_IXT_SRC_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"RET_ORD:"_Stz);
  vDefineLabel("GOT_ORD:"_Stz);

  // test O_ORDERDATE
  vAddCode(EO_CRW_SRC_FETCH);
  vAddCodeArg(EO_COL_SRC_PUSH,O_ORDERDATE);
  //vAddCode(EO_DUP);
  //vAddCode(EO_DTE_PRINT);
  vAddCodeArg(EO_DATE,"1993-07-01"_Stz);
  vAddCode(EO_GREATEREQUAL);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_ORD:"_Stz);
  vAddCodeArg(EO_COL_SRC_PUSH,O_ORDERDATE); // doubled, not efficient
  vAddCodeArg(EO_DATE,"1993-10-01"_Stz);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_ORD:"_Stz);
  
  // test for a matching LINEITEM
  vAddCodeLabel(EO_CALL,"TST_ITM:"_Stz);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_ORD:"_Stz);
    
  vAddCodeLabel(EO_CALL,"GET_AGR:"_Stz);

  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)0);
  vAddCode(EO_INC);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)0);

  // next ORDERS
  vDefineLabel("NXT_ORD:"_Stz);
  vAddCode(EO_IXT_SRC_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"GOT_ORD:"_Stz);

  // end of ORDERS
  vDefineLabel("RET_ORD:"_Stz);
    
  vAddCodeLabel(EO_CALL,"TRV_IXT:"_Stz);
  
  
  vAddCode(EO_TMR_AGG_NSECS);
  vAddCode(EO_PRINT);
  vAddCode(EO_CTR_0_PUSH);
  vAddCode(EO_PRINT);
  vAddCode(EO_TMR_AGG_NSECS);
  vAddCode(EO_CTR_0_PUSH);
  vAddCode(EO_DIVIDE);
  vAddCode(EO_PRINT);


  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q400 TST_ITM
  // test for a matching LINEITEM
  vDefineLabel("TST_ITM:"_Stz);
  
  // position / iterate
  vAddCode(EO_TMR_START);
  vAddCode(EO_IXT_POS_ALTBYSRC);
  vAddCode(EO_TMR_UPDATE);
  vAddCode(EO_TMR_AGG_ADD);
  vAddCode(EO_CTR_0_INC);

  vAddCodeLabel(EO_JMP_FALSE,"RET_ITM_f:"_Stz);
  vDefineLabel("NXT_ITM:"_Stz);
  vAddCode(EO_IXT_ALT_NEXT);
  vAddCodeLabel(EO_JMP_FALSE,"RET_ITM_f:"_Stz);
  //vAddCode(EO_IXT_ALT_PRINT);
  vAddCode(EO_IXT_CMP_SRCALT);
  vAddCodeLabel(EO_JMP_NOTEQUAL,"RET_ITM_f:"_Stz);
  
  // test L_COMMITDATE < L_RECEIPTDATE
  vAddCode(EO_CRW_ALT_FETCH);
  vAddCodeArg(EO_COL_ALT_PUSH,L_COMMITDATE);
  vAddCodeArg(EO_COL_ALT_PUSH,L_RECEIPTDATE);
  vAddCode(EO_LESSTHAN);
  vAddCodeLabel(EO_JMP_FALSE,"NXT_ITM:"_Stz);
  vAddCodeLabel(EO_JMP,"RET_ITM_t:"_Stz);

  vAddCodeLabel(EO_JMP,"NXT_ITM:"_Stz);

  // found one matching lineitem
  vDefineLabel("RET_ITM_t:"_Stz);
  vAddCode(EO_PUSH1);
  vAddCode(EO_RETURN);

  // no matching lineitem
  vDefineLabel("RET_ITM_f:"_Stz);
  vAddCode(EO_PUSH0);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q400 GET_AGR
  // get aggregator for o_orderpriority
  vDefineLabel("GET_AGR:"_Stz);

  // reset entwrk
  vAddCode(EO_ENTWRK_RESET);
  
  // add key O_ORDERPRIORITY
  vAddCodeArg(EO_COL_SRC_PUSHSTR,O_ORDERPRIORITY);
  vAddCode(EO_ENTWRK_APPENDELMS);

  // attempt to find the key in the indexTree
  vAddCode(EO_ENT_POSPRIOR);
  vAddCode(EO_ENT_PUSH);
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
  
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)0);
  
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);

  vDefineLabel("END_AGR:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q400 TRV_IXT
  vDefineLabel("TRV_IXT:"_Stz);

  //vAddCode(EO_TRACEON);

  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);

  vDefineLabel("TRV_ECH:"_Stz);

  vAddCode(EO_RSB_DST_NEW);

  vAddCode(EO_ENT_PUSHELMS0);
  vAddCodeArg(EO_RSB_DST_POPCOLS, Q400_ORDERPRIORITY);

  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)0);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q400_ORDERCOUNT);

  vDefineLabel("TRV_NXT:"_Stz);
  vAddCode(EO_IXT_TRV_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"TRV_ECH:"_Stz);

  vDefineLabel("TRV_RET:"_Stz);
  //vAddCode(EO_TRACEOFF);
  //vAddCode(EO_BREAKPOINT);
  vAddCode(EO_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vDumpCode();
}

/*----------------------------------------------------------------------------*/
/* generate code for query 4                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateQuery401(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // set the query no
  m_iSql = 401;
  
  m_i_Code = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q401
  vDefineLabel("QRY_401:"_Stz);

  //vAddCode(EO_TRACEON);
  
  vAddCodeLabel(EO_JMP_FALSE,"QRY_OUT:"_Stz);

  vAddCodeString(EO_STR_LITERAL,"**QRY_401 RSB**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  vAddCodeLabel(EO_CALL,"ITR_SRC:"_Stz);
  vAddCode(EO_RETURN);

  vDefineLabel("QRY_OUT:"_Stz);
  vAddCodeString(EO_STR_LITERAL,"**QRY_401 EOS**"_Stz);
  vAddCode(EO_STR_PRINTADR);
  vAddCodeLabel(EO_CALL,"TRV_IXT:"_Stz);
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
  vAddCode(EO_ENT_PUSH);
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
  
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_PAY_STORE_COLI,(i8)0);
  
  //vAddCodeArg(EO_IXT_DUMP,(i8)0);

  vDefineLabel("END_AGR:"_Stz);
  vAddCode(EO_RETURN);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                                                                Q401 TRV_IXT
  vDefineLabel("TRV_IXT:"_Stz);

  //vAddCode(EO_TRACEON);

  vAddCode(EO_IXT_TRV_BEGIN);
  vAddCodeLabel(EO_JMP_FALSE,"TRV_RET:"_Stz);

  vDefineLabel("TRV_ECH:"_Stz);

  vAddCode(EO_RSB_DST_NEW);

  vAddCode(EO_ENT_PUSHELMS0);
  vAddCodeArg(EO_RSB_DST_POPCOLS, Q401_ORDERPRIORITY);

  vAddCodeArg(EO_PAY_FETCH_COLI,(i8)0);
  vAddCodeArg(EO_RSB_DST_POPCOLI, Q401_ORDERCOUNT);

  vDefineLabel("TRV_NXT:"_Stz);
  vAddCode(EO_IXT_TRV_NEXT);
  vAddCodeLabel(EO_JMP_TRUE,"TRV_ECH:"_Stz);

  vDefineLabel("TRV_RET:"_Stz);
  vAddCode(EO_TRACEOFF);
  vAddCode(EO_BREAKPOINT);
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
      
  // new a rsb dst row if not there
  vAddCode(EO_RSB_DST_TEST);
  vAddCodeLabel(EO_JMP_TRUE,"RSB_SRC_BEGIN:"_Stz);
  vAddCode(EO_RSB_DST_NEW);
  
  // initialze the row
  vAddCode(EO_PUSH0);
  vAddCodeArg(EO_RSB_DST_POPCOLI,(i8)Q600_REVENUE);
  
  //vAddCode(EO_RSB_DST_PRINT);
  
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
      
  // new a rsb dst row if not there
  vAddCode(EO_RSB_DST_TEST);
  vAddCodeLabel(EO_JMP_TRUE,"RSB_SRC_BEGIN:"_Stz);
  vAddCode(EO_RSB_DST_NEW);
  
  // initialize the row
  vAddCode(EO_PUSH0);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
