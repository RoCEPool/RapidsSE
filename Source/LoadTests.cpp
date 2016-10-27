/*============================================================================*/
/* LoadTests.cpp - load tests                                                 */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
void LoadTests::vRun(RunTests* pRunTests)
{
  s256 sCommand;
  char* pRowPtr;
  
  // start with /tenants folder
  sCommand = "cf ~";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create tenant ten;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ten";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf catalogs";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf cat";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf schemas";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf sch";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf tables";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table j10 ( "
     "j10_key   integer not null, "
     "j10_name  varchar(32) not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "ld j10 \"../JTEST/j10.csv\"";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel j10";
  vRequire(sCommand, true, EC_NONE, 10);
  
  sCommand = "drop table j10;";
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table j100 ("
     "j100_key   integer not null, "
     "j100_name  varchar(32) not null, "
     "j100_j10   integer not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "ld j100 \"../JTEST/j100.csv\"";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel j100";
  vRequire(sCommand, true, EC_NONE, 100);
  
  sCommand = "drop table j100;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table j1000 ( "
     "j1000_key   integer not null, "
     "j1000_name  varchar(32) not null, "
     "j1000_j100  integer not null, "
     "j1000_j10   integer not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "ld j1000 \"../JTEST/j1000.csv\"";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel j1000";
  vRequire(sCommand, true, EC_NONE, 1000);
  
  sCommand = "drop table j1000;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ~;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;";
  vRequire(sCommand, true, EC_NONE);
  
  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n",
         "Load Tests", m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
