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
  sCommand = "cf ~";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop schema;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create schema sch;";
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "drop schema sch;";
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "create tenant ten;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ten";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;";
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "drop schema sch;";
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "cf catalogs";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;";
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "drop schema sch;";
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "create catalog cat;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf cat";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;";
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);
  
  sCommand = "drop schema sch;";
  vRequire(sCommand, false, EC_NOT_IN_SCHEMAS);

  sCommand = "cf schemas";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;";
  vRequire(sCommand, false, EC_SCHEMA_EXISTS);

  sCommand = "drop schema sch;";
  vRequire(sCommand, false, EC_FOLDER_NOT_EMPTY);
  
  sCommand = "cf sch";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf tables";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop schema sch;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop schema sch;";
  vRequire(sCommand, false, EC_NO_SUCH_WORD);                         //??CHECK
  
  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf schemas";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop catalog cat;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf catalogs";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop tenant ten;";
  vRequire(sCommand, true, EC_NONE);
  
  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n",
         "Schema Tests", m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
