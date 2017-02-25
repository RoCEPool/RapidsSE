/*============================================================================*/
/* InsertTests.cpp - insert tests                                             */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
void InsertTests::vRun(RunTests* pRunTests)
{
  s256 sCommand;
  char* pRowPtr;
  i8 iInUseStart = 0;
  i8 iInUseEnd = 0;
  char* pSpace;
  
  sCommand = "getInUseSpace";
  pSpace = pRequire(sCommand, true, EC_NONE);
  if (pSpace != nullptr)
    iInUseStart = String::iToI8(pSpace);
  
  sCommand = "docol bpa";
  vRequire(sCommand, true, EC_NONE);
  
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
  
  sCommand = "create table tbl ( "
  "col0   integer not null, "
  "col1   varchar(32) not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "begin insert tbl;";
  vRequire(sCommand, true, EC_NONE);
  
  for (i8 iIdx = 0; iIdx < 1000; iIdx++) {
    sprintf(sCommand.aStr(), "insert row \"%lld|%s\";", iIdx, "abcdef");
    vRequire(sCommand, true, EC_NONE);
  }
  
  sCommand = "end insert;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel tbl";
  vRequire(sCommand, true, EC_NONE, 1000);
  
  sCommand = "begin insert tbl;";
  vRequire(sCommand, true, EC_NONE);
  
  for (i8 iIdx = 1000; iIdx < 2000; iIdx++) {
    sprintf(sCommand.aStr(), "insert row \"%lld|%s\";", iIdx, "abcdef");
    vRequire(sCommand, true, EC_NONE);
  }
  
  sCommand = "end insert;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel tbl";
  vRequire(sCommand, true, EC_NONE, 2000);
  
  sCommand = "drop table tbl;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ~;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "getInUseSpace";
  pSpace = pRequire(sCommand, true, EC_NONE);
  if (pSpace != nullptr)
    iInUseEnd = String::iToI8(pSpace);

  //ASSERT(iInUseEnd == iInUseStart);
  
  sCommand = "docol bpa";
  vRequire(sCommand, true, EC_NONE);
  
  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n",
         "Insert Tests", m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
