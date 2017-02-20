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
  sCommand = "cf ~";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create t;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);

  sCommand = "create table;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t [x];";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t [x int];";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t x;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop t;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);

  sCommand = "drop table;";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "create table t (col0 int);";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "create tenant ten;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ten";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 int);";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "cf catalogs";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 int);";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "create catalog cat;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf cat";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 int);";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "cf schemas";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create schema sch;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 int);";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);

  sCommand = "cf sch";
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table t (col0 int);";
  vRequire(sCommand, false, EC_NOT_IN_TABLES);
  
  sCommand = "cf tables";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer);";
  vRequire(sCommand, false, EC_TABLE_EXISTS);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table x (col0 integer);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table x;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 decimal, col1 char, col3 integer, col4 varchar, col5 char, col6 boolean, col7 date);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table t (col0 decimal(2,3), col1 char(3), col3 integer, col4 varchar(40), col5 char(5), col6 timestamp, col7 date);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 decimal[1]);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 decimal[1,2]);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 decimal(2));";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 char[1]);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);

  sCommand = "create table t (col0 char[1,2]);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 char(2,3));";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 varchar[1]);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 varchar[1,2]);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);

  sCommand = "create table t (col0 varchar(2,3));";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);

  sCommand = "create table t (col0 int not);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 int null);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 integer primary key);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer unique);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 varchar unique);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 varchar not unique);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);
  
  sCommand = "create table t (col0 integer not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 char not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 varchar not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 decimal not null, col1 date not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 decimal(2,3) not null, col1 varchar(10) not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table t (col0 integer primary key, col0 boolean not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);

  sCommand = "create table t (col0 integer unique, col0 timestamp not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer(10) unique, col0 timestamp not null);";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop table t;";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "create table t (col0 integer(10) unique, col0 timestamp(1) not null);";
  vRequire(sCommand, false, EC_INVALID_COMMAND);
  
  sCommand = "drop table t;";
  vRequire(sCommand, false, EC_NO_SUCH_TABLE);

  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf tables";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "drop schema sch;";
  vRequire(sCommand, true, EC_NONE);                  // FOLDER IN USE
  
  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "rf schemas";
  vRequire(sCommand, true, EC_NONE);
  
  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);
  
  vTrace("1\n");
  
  sCommand = "drop catalog cat;";                     //FOLDER_IN_USE
  vRequire(sCommand, true, EC_NONE);

  vTrace("2\n");
  
  sCommand = "rf catalogs";
  vRequire(sCommand, true, EC_NONE);

  vTrace("3\n");
  
  sCommand = "cf ..";
  vRequire(sCommand, true, EC_NONE);

  sCommand = "drop tenant ten;";
  vRequire(sCommand, true, EC_NONE);
  
  pRunTests->m_iTestsTotal += m_iTestsTotal;
  pRunTests->m_iTestsPassed += m_iTestsPassed;
  pRunTests->m_iTestsFailed += m_iTestsFailed;
  
  vTrace("%-15s - Total: %lld Passed: %lld Failed: %lld\n",
         "Table Tests", m_iTestsTotal, m_iTestsPassed, m_iTestsFailed);}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
