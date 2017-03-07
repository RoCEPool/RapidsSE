/*============================================================================*/
/* SchemaTests.cpp - schema tests                                             */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
void SchemaTests::vRun(RunTests* pRunTests)
{
  s256 sCommand;
  
  // start with /tenants folder
  sCommand = "cf ~_Str"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop schema;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create schema sch;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "drop schema sch;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "create tenant ten;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ten"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "drop schema sch;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "cf catalogs"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "drop schema sch;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "create catalog cat;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf cat"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "drop schema sch;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);

  sCommand = "cf schemas"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;"_Str;
  vRequire(sCommand, false, EC_SCHEMA_EXISTS);

  sCommand = "drop schema sch;"_Str;
  vRequire(sCommand, false, EC_FOLDER_NOT_EMPTY);
  
  sCommand = "cf sch"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf tables"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop schema sch;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop schema sch;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_WORD);                         //??CHECK
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf schemas"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop catalog cat;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf catalogs"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n"_Str,
         "Schema Tests"_Str, m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
