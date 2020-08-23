/*============================================================================*/
/* InflateSet.h -- Inflate Set                                                */
/*============================================================================*/

// Note: this file is included by tcc

// customer columns
static const i8 C_CUSTKEY       = 0;
static const i8 C_NAME          = 1;
static const i8 C_ADDRESS       = 2;
static const i8 C_NATIONKEY     = 3;
static const i8 C_PHONE         = 4;
static const i8 C_ACCTBAL       = 5;
static const i8 C_MKTSEGMENT    = 6;
static const i8 C_COMMENT       = 7;

// lineitem columns
static const i8 L_ORDERKEY      = 0;
static const i8 L_PARTKEY       = 1;
static const i8 L_SUPPKEY       = 2;
static const i8 L_LINENUMBER    = 3;
static const i8 L_QUANTITY      = 4;
static const i8 L_EXTENDEDPRICE = 5;
static const i8 L_DISCOUNT      = 6;
static const i8 L_TAX           = 7;
static const i8 L_RETURNFLAG    = 8;
static const i8 L_LINESTATUS    = 9;
static const i8 L_SHIPDATE      = 10;
static const i8 L_COMMITDATE    = 11;
static const i8 L_RECEIPTDATE   = 12;
static const i8 L_SHIPINSTRUCT  = 13;
static const i8 L_SHIPMODE      = 14;
static const i8 L_COMMENT       = 15;

// orders columns
static const i8 O_ORDERKEY      = 0;
static const i8 O_CUSTKEY       = 1;
static const i8 O_ORDERSTATUS   = 2;
static const i8 O_TOTALPRICE    = 3;
static const i8 O_ORDERDATE     = 4;
static const i8 O_ORDERPRIORITY = 5;
static const i8 O_CLERK         = 6;
static const i8 O_SHIPPRIORITY  = 7;
static const i8 O_COMMENT       = 8;

// query1 locals
static const i8 Q1_LOCAL_QUANTITY      = 0;
static const i8 Q1_LOCAL_EXTENDEDPRICE = 1;
static const i8 Q1_LOCAL_DISCOUNT      = 2;
static const i8 Q1_LOCAL_TAX           = 3;
static const i8 Q1_LOCAL_RETURNFLAG    = 4;
static const i8 Q1_LOCAL_LINESTATUS    = 5;
static const i8 Q1_LOCAL_SHIPDATE      = 6;
static const i8 Q1_LOCAL_TOTAL         = 7;

// query1 100 output
static const i8 Q100_RETURNFLAG     = 0;  // L_RETURNFLAG
static const i8 Q100_LINESTATUS     = 1;  // L_LINESTATUS
static const i8 Q100_SUM_QTY        = 2;  // SUM(L_QUANTITY)
static const i8 Q100_SUM_BASE_PRICE = 3;  // SUM(L_EXTENDEDPRICE)
static const i8 Q100_SUM_DISC_PRICE = 4;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT))
static const i8 Q100_SUM_CHARGE     = 5;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)*(1+L_TAX))
static const i8 Q100_COUNT_QTY      = 6;  // COUNT(L_QUANTITY)
static const i8 Q100_COUNT_PRICE    = 7;  // COUNT(L_EXTENDEDPRICE)
static const i8 Q100_SUM_DISC       = 8;  // SUM(L_DISCOUNT)
static const i8 Q100_COUNT_DISC     = 9;  // COUNT(L_DISCOUNT)
static const i8 Q100_COUNT_ORDER    = 10; // COUNT(*)
static const i8 Q100_COLUMNS        = 11; // TOTAL COLUMNS

// query1 101 output
static const i8 Q101_RETURNFLAG     = 0;  // L_RETURNFLAG
static const i8 Q101_LINESTATUS     = 1;  // L_LINESTATUS
static const i8 Q101_SUM_QTY        = 2;  // SUM(L_QUANTITY)
static const i8 Q101_SUM_BASE_PRICE = 3;  // SUM(L_EXTENDEDPRICE)
static const i8 Q101_SUM_DISC_PRICE = 4;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT))
static const i8 Q101_SUM_CHARGE     = 5;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)*(1+L_TAX))
static const i8 Q101_COUNT_QTY      = 6;  // COUNT(L_QUANTITY)
static const i8 Q101_COUNT_PRICE    = 7;  // COUNT(L_EXTENDEDPRICE)
static const i8 Q101_SUM_DISC       = 8;  // SUM(L_DISCOUNT)
static const i8 Q101_COUNT_DISC     = 9;  // COUNT(L_DISCOUNT)
static const i8 Q101_COUNT_ORDER    = 10; // COUNT(*)
static const i8 Q101_COLUMNS        = 11; // TOTAL COLUMNS

// query1 104 output
static const i8 Q104_RETURNFLAG     = 0;  // L_RETURNFLAG
static const i8 Q104_LINESTATUS     = 1;  // L_LINESTATUS
static const i8 Q104_SUM_QTY        = 2;  // SUM(L_QUANTITY)
static const i8 Q104_SUM_BASE_PRICE = 3;  // SUM(L_EXTENDEDPRICE)
static const i8 Q104_SUM_DISC_PRICE = 4;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT))
static const i8 Q104_SUM_CHARGE     = 5;  // SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)*(1+L_TAX))
static const i8 Q104_AVG_QTY        = 6;  // AVG(L_QUANTITY)
static const i8 Q104_AVG_PRICE      = 7;  // AVG(L_EXTENDEDPRICE)
static const i8 Q104_AVG_DISC       = 8;  // AVG(L_DISCOUNT)
static const i8 Q104_COUNT_ORDER    = 9;  // COUNT(*)

// q300 output
static const i8 Q300_ORDERKEY     = 0;
static const i8 Q300_ORDERDATE    = 1;
static const i8 Q300_SHIPPRIORITY = 2;

// q301 output
static const i8 Q301_ORDERKEY     = 0;
static const i8 Q301_REVENUE      = 1;
static const i8 Q301_ORDERDATE    = 2;
static const i8 Q301_SHIPPRIORITY = 3;

// q302 output
static const i8 Q302_ORDERKEY     = 0;
static const i8 Q302_REVENUE      = 1;
static const i8 Q302_ORDERDATE    = 2;
static const i8 Q302_SHIPPRIORITY = 3;

// q400 output
static const i8 Q400_ORDERPRIORITY = 0;
static const i8 Q400_ORDERCOUNT    = 1;

// q401 output
static const i8 Q401_ORDERPRIORITY = 0;
static const i8 Q401_ORDERCOUNT    = 1;

// query6 locals
static const i8 Q6_LOCAL_QUANTITY      = 0;
static const i8 Q6_LOCAL_EXTENDEDPRICE = 1;
static const i8 Q6_LOCAL_DISCOUNT      = 2;
static const i8 Q6_LOCAL_TOTAL         = 3;

// query6 600 output
static const i8 Q600_REVENUE    = 0;  // SUM(L_EXTENDEDPRICE*L_DISCOUNT)
static const i8 Q600_COLUMNS    = 1;  // TOTAL COLUMNS
//static const i8 Q600_COUNT      = 1;
//static const i8 Q600_COLUMNS    = 2;  // TOTAL COLUMNS
  
// query6 601 output
static const i8 Q601_REVENUE    = 0;  // SUM(L_EXTENDEDPRICE*L_DISCOUNT)
static const i8 Q601_COLUMNS    = 1;  // TOTAL COLUMNS
//static const i8 Q601_COUNT      = 1;
//static const i8 Q601_COLUMNS    = 2;  // TOTAL COLUMNS

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
