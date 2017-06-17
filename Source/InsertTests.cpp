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
  i8 iInUseStart = 0;
  i8 iInUseEnd = 0;
  Str* aSpace;
  
  sCommand = "getInUseSpace"_Str;
  aSpace = aRequire(sCommand, true, EC_NONE);
  if (aSpace != nullptr)
    iInUseStart = aSpace->iToI8();
  
  sCommand = "docol bpa"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  // start with /tenants folder
  sCommand = "cf ~"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create tenant ten;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ten"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf catalogs"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create catalog cat;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf cat"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf schemas"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf sch"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf tables"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table tbl ( "
  "col0   integer not null, "
  "col1   varchar(32) not null);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "begin insert tbl;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  for (i8 i_Idx = 0; i_Idx < 1000; i_Idx++) {
    sCommand.vSprint("insert row \"%lld|%s\";"_Str, i_Idx, "abcdef"_Str);
    vRequire(sCommand, true, EC_NONE);
  }
  
  sCommand = "end insert;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel tbl"_Str;
  vRequire(sCommand, true, EC_NONE, 1000);
  
  sCommand = "begin insert tbl;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  for (i8 i_Idx = 1000; i_Idx < 2000; i_Idx++) {
    sCommand.vSprint("insert row \"%lld|%s\";"_Str, i_Idx, "abcdef"_Str);
    vRequire(sCommand, true, EC_NONE);
  }
  
  sCommand = "end insert;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel tbl"_Str;
  vRequire(sCommand, true, EC_NONE, 2000);
  
  sCommand = "drop table tbl;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ~;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "getInUseSpace"_Str;
  aSpace = aRequire(sCommand, true, EC_NONE);
  if (aSpace != nullptr)
    iInUseEnd = aSpace->iToI8();

  //ASSERT(iInUseEnd == iInUseStart);
  
  sCommand = "docol bpa"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n"_Str,
         "Insert Tests"_Str, m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
