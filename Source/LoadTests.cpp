/*============================================================================*/
/* LoadTests.cpp - load tests                                                 */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
void LoadTests::vRun(RunTests* pRunTests)
{
  s48K sCommand;
  Stz* aRowPtr;
  
  // start with /tenants folder
  sCommand = "cf ~"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create tenant ten;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ten"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf catalogs"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf cat"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf schemas"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf sch"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf tables"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table j10 ( "
     "j10_key   integer not null, "
     "j10_name  varchar(32) not null);"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "ld j10 \"../JTEST/j10.csv\""_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel j10"_Stz;
  vRequire(sCommand, true, EC_NONE, 10);
  
  sCommand = "drop table j10;"_Stz;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table j100 ("
     "j100_key   integer not null, "
     "j100_name  varchar(32) not null, "
     "j100_j10   integer not null);"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "ld j100 \"../JTEST/j100.csv\""_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel j100"_Stz;
  vRequire(sCommand, true, EC_NONE, 100);
  
  sCommand = "drop table j100;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table j1000 ( "
     "j1000_key   integer not null, "
     "j1000_name  varchar(32) not null, "
     "j1000_j100  integer not null, "
     "j1000_j10   integer not null);"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "ld j1000 \"../JTEST/j1000.csv\""_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel j1000"_Stz;
  vRequire(sCommand, true, EC_NONE, 1000);
  
  sCommand = "drop table j1000;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table lineitem ( "
	 "l_orderkey      integer not null, "
	 "l_partkey       integer not null, "
	 "l_suppkey       integer not null, "
	 "l_linenumber    integer not null, "
	 "l_quantity      decimal(15,2) not null, "
	 "l_extendedprice decimal(15,2) not null, "
	 "l_discount      decimal(15,2) not null, "
	 "l_tax           decimal(15,2) not null, "
	 "l_returnflag    char(1) not null, "
	 "l_linestatus    char(1) not null, "
	 "l_shipdate      date not null, "
	 "l_commitdate    date not null, "
	 "l_receiptdate   date not null, "
	 "l_shipinstruct  char(25) not null, "
	 "l_shipmode      char(10) not null, "
	 "l_comment       varchar(44) not null "
   "); "_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "ld lineitem \"/tpch/lineitem.tbl\""_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  //sCommand = "getInUseSpace"_Stz;
  //i8 inUseSpace = 0;
  //Stz* aSpace = aRequire(sCommand, true, EC_NONE);
  //if (aSpace != nullptr)
  //  inUseSpace = aSpace->iToI8();
  //vTrace("InUseSpace: %lld\n"_Stz, inUseSpace);
  
  sCommand = "sel lineitem"_Stz;
  vRequire(sCommand, true, EC_NONE, 6001215);

  sCommand = "drop table lineitem;"_Stz;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "cf ~;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;"_Stz;
  vRequire(sCommand, true, EC_NONE);

  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n"_Stz,
         "Load Tests"_Stz, m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
