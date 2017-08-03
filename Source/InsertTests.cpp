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
  Stz* aSpace;
  
  sCommand = "getInUseSpace"_Stz;
  aSpace = aRequire(sCommand, true, EC_NONE);
  if (aSpace != nullptr)
    iInUseStart = aSpace->iToI8();
  
  sCommand = "docol bpa"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
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
  
  sCommand = "create table tbl ( "
  "col0   integer not null, "
  "col1   varchar(32) not null);"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "begin insert tbl;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  for (i8 i_Idx = 0; i_Idx < 1000; i_Idx++) {
    sCommand.vSprint("insert row \"%lld|%s\";"_Stz, i_Idx, "abcdef"_Stz);
    vRequire(sCommand, true, EC_NONE);
  }
  
  sCommand = "end insert;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel tbl"_Stz;
  vRequire(sCommand, true, EC_NONE, 1000);
  
  sCommand = "begin insert tbl;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  for (i8 i_Idx = 1000; i_Idx < 2000; i_Idx++) {
    sCommand.vSprint("insert row \"%lld|%s\";"_Stz, i_Idx, "abcdef"_Stz);
    vRequire(sCommand, true, EC_NONE);
  }
  
  sCommand = "end insert;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "sel tbl"_Stz;
  vRequire(sCommand, true, EC_NONE, 2000);
  
  sCommand = "drop table tbl;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ~;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "getInUseSpace"_Stz;
  aSpace = aRequire(sCommand, true, EC_NONE);
  if (aSpace != nullptr)
    iInUseEnd = aSpace->iToI8();

  //ASSERT(iInUseEnd == iInUseStart);
  
  sCommand = "docol bpa"_Stz;
  vRequire(sCommand, true, EC_NONE);
  
  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n"_Stz,
         "Insert Tests"_Stz, m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
