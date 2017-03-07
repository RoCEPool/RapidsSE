/*============================================================================*/
/* TableTests.cpp - schema tests                                             */
/*============================================================================*/

#include "UnitTests.h"

/*----------------------------------------------------------------------------*/
/* run tests                                                                  */
/*----------------------------------------------------------------------------*/
void TableTests::vRun(RunTests* pRunTests)
{
  s256 sCommand;
  
  // start with /tenants folder
  sCommand = "cf ~"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create t;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);

  sCommand = "create table;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t [x];"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t [x int];"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t x;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop t;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);

  sCommand = "drop table;"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t (col0 int);"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "create tenant ten;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ten"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 int);"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "cf catalogs"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 int);"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "create catalog cat;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf cat"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 int);"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "cf schemas"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 int);"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);

  sCommand = "cf sch"_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table t (col0 int);"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "cf tables"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer);"_Str;
  vRequire(sCommand, false, EC_TABLE_EXISTS);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table x (col0 integer);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table x;"_Str;
  vRequire(sCommand, false, EC_NOT_IN_TABLES);

  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table x;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 decimal, col1 char, col3 integer, col4 varchar, col5 char, col6 boolean, col7 date);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table t (col0 decimal(2,3), col1 char(3), col3 integer, col4 varchar(40), col5 char(5), col6 timestamp, col7 date);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 decimal[1]);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  //sCommand = "cf .."_Str;
  //vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 decimal[1,2]);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 decimal(2));"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 char[1]);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);

  sCommand = "create table t (col0 char[1,2]);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 char(2,3));"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 varchar[1]);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 varchar[1,2]);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);

  sCommand = "create table t (col0 varchar(2,3));"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);

  sCommand = "create table t (col0 int not);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 int null);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 integer primary key);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer unique);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 varchar unique);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 varchar not unique);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t (col0 varchar, col0 integer);"_Str;
  vRequire(sCommand, false, EC_DUPLICATE_COLUMN);
  
  sCommand = "create table t (col0 varchar, col1 integer, col1 decimal);"_Str;
  vRequire(sCommand, false, EC_DUPLICATE_COLUMN);
  
  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 integer not null);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 char not null);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 varchar not null);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 decimal not null, col1 date not null);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 decimal(2,3) not null, col1 varchar(10) not null);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table t (col0 integer primary key, col0 boolean not null);"_Str;
  vRequire(sCommand, false, EC_DUPLICATE_COLUMN);
  
  sCommand = "create table t (col0 integer primary key, col1 boolean not null);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table t (col0 integer unique, col1 timestamp not null);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer(10) unique, col1 timestamp not null);"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer(10) unique, col1 timestamp(1) not null);"_Str;
  vRequire(sCommand, false, EC_INVALID_COMMAND);

  sCommand = "drop table t;"_Str;
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);

  for (i8 iIndex=0 ; iIndex<1000; iIndex++) {
    StrLib::vSprint(sCommand.aStr(),
                    "create table t%d (col0 integer unique, col1 timestamp not null);",
                    iIndex);
    vRequire(sCommand, true, EC_NONE);
    
    sCommand = "cf .."_Str;
    vRequire(sCommand, true, EC_NONE);
  }

  for (i8 iIndex=0 ; iIndex<1000; iIndex++) {
    StrLib::vSprint(sCommand.aStr(),
                    "drop table t%d;",
                    iIndex);
    vRequire(sCommand, true, EC_NONE);
  }
 
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf tables"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop schema sch;"_Str;
  vRequire(sCommand, true, EC_NONE);                  // FOLDER IN USE
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf schemas"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf .."_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop catalog cat;"_Str;
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf catalogs"_Str;
  vRequire(sCommand, false, EC_FOLDER_IN_USE);

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
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n",
         "Table Tests", m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
