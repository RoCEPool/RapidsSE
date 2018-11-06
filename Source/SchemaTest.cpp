/*============================================================================*/
/* SchemaTest.cpp -- Schema Test                                              */
/*============================================================================*/

#include "Utilities.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SchemaTest* Utilities::_pSchemaTest(void* aAddr)
{
  SchemaTest* pSchemaTest = reinterpret_cast<SchemaTest*>(aAddr);
  
  RASSERT_TYPE(pSchemaTest,CN_SCHEMATEST);
  
  return pSchemaTest;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SchemaTest::operator new(size_t)
{
  //UNUSED(iSize)
  
  void* pObject = HeapMem::pAllocate(CN_SCHEMATEST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SchemaTest::operator delete(void* pAddress)
{
  SchemaTest* pSchemaTest =
    reinterpret_cast<SchemaTest*>(pAddress);
  
  HeapMem::vFree(pSchemaTest);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SchemaTest::SchemaTest(void)
: Object(CN_SCHEMATEST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEMATEST,_FOOTER_);
  
  //m_xFile
  m_sTableNm = "<none>"_Stz;
  
  m_nTcolumns = 0;
  m_nXcolumns = 0;
  m_nZcolumns = 0;

  m_nTotalColumns = 0;

  for (i8 i_Col = 0; i_Col < C_COLUMNS; i_Col++)
    m_eColumns[i_Col] = CT_NONE;
  m_sStatement = "<none>"_Stz;

  _checkInitialised_(CN_SCHEMATEST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCHEMATEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SchemaTest::~SchemaTest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEMATEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* run                                                                        */
/*----------------------------------------------------------------------------*/
void SchemaTest::vRun(void)
{
  SchemaTest* pSchemaTest = new SchemaTest;
  
  pSchemaTest->vGenerate();
  
  delete pSchemaTest;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate                                                                   */
/*----------------------------------------------------------------------------*/
void SchemaTest::vGenerate(void)
{
  vOpenFile();
  vFileHeader();

  vTestSet();
  
  vFileFooter();
  vCloseFile();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void SchemaTest::vCloseFile(void)
{
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* file footer                                                                */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFileFooter(void)
{
  m_xFile.vPrint("%s\n"_Stz,
                 "/*============================================================================*/"_Stz);
  m_xFile.vPrint("%s\n"_Stz,
                 "/* end                                                                        */"_Stz);
  m_xFile.vPrint("%s\n"_Stz,
                 "/*============================================================================*/"_Stz);
}

/*----------------------------------------------------------------------------*/
/* file header                                                                */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFileHeader(void)
{
  m_xFile.vPrint("%s\n"_Stz,
  "/*============================================================================*/"_Stz);
  m_xFile.vPrint("%s\n"_Stz,
  "/* _schemaTest -- schema stress test (auto generated)                         */"_Stz);
  m_xFile.vPrint("%s\n"_Stz,
  "/*============================================================================*/"_Stz);
}

/*----------------------------------------------------------------------------*/
/* form column list                                                           */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormColumnList(void)
{
  m_sStatement.vConcat("  col0 integer "_Stz);
}

/*----------------------------------------------------------------------------*/
/* form create table statement                                                */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormCreateTable(void)
{
  m_sStatement = "time create table "_Stz;
  
  m_sStatement.vConcat(m_sTableNm);

  m_sStatement.vConcat(" (\n"_Stz);
  
  vFormColumnList();

  m_sStatement.vConcat(" ) ;"_Stz);
}

/*----------------------------------------------------------------------------*/
/* form drop table statement                                                  */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormDropTable(void)
{
  m_sStatement = "time drop table "_Stz;
  
  m_sStatement.vConcat(m_sTableNm);
  
  m_sStatement.vConcat(" ;"_Stz);
}

/*----------------------------------------------------------------------------*/
/* form table name                                                            */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormTableName(void)
{
  i8 i_Col;
  
  // table prefix
  m_sTableNm = ""_Stz;
  
  // no columns
//  s32 sTableNo;
//  StrLib::wToString(m_nTcolumns,sTableNo.aChar());
//  m_sTableNm.vConcat(sTableNo);
//  m_sTableNm.vConcat("_"_Stz);
  
  // x columns prefix
  s32 sX;
  StrLib::wToString(m_nXcolumns,sX.aChar());
  m_sTableNm.vConcat("X"_Stz);
  m_sTableNm.vConcat(sX);
  m_sTableNm.vConcat("_"_Stz);
  
  s256 sColShorts;
  sColShorts = ""_Stz;
  
  // the t columns
  for (i_Col = m_nXcolumns;
       i_Col < m_nXcolumns + m_nTcolumns;
       i_Col++)
    sColShorts.vConcat(aCT_short[m_eColumns[i_Col]]);

  m_sTableNm.vConcat(sColShorts);
  
  // z columns suffix
  s32 sZ;
  StrLib::wToString(m_nZcolumns,sZ.aChar());
  m_sTableNm.vConcat("_Z"_Stz);
  m_sTableNm.vConcat(sZ);
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void SchemaTest::vOpenFile(void)
{
  bool bOpened = m_xFile.bOpen("../Commands/_schemaTest"_Stz, C_OPEN_WRITE);
  RASSERT(bOpened);
}

/*----------------------------------------------------------------------------*/
/* make a test for given column pattern                                       */
/*----------------------------------------------------------------------------*/
void SchemaTest::vMakeTest(void)
{
  vFormTableName();
  
  vFormCreateTable();
  vWriteStatement();
  
  vFormDropTable();
  vWriteStatement();
}

/*----------------------------------------------------------------------------*/
/* random column type                                                         */
/*----------------------------------------------------------------------------*/
eCT SchemaTest::eRandomType(void)
{
  return CT_VARCHAR;
}

/*----------------------------------------------------------------------------*/
/* write statement to file                                                    */
/*----------------------------------------------------------------------------*/
void SchemaTest::vWriteStatement(void)
{
  m_xFile.vPrint("%s\n"_Stz,m_sStatement.aChar());
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add the x columns                                                          */
/*----------------------------------------------------------------------------*/
void SchemaTest::vAddXcolumns(void)
{
  i8 i_Col;
  
  for (i_Col = 0;
       i_Col < m_nXcolumns;
       i_Col++)
    m_eColumns[i_Col] = eRandomType();
}

/*----------------------------------------------------------------------------*/
/* add the z column                                                           */
/*----------------------------------------------------------------------------*/
void SchemaTest::vAddZcolumns(void)
{
  i8 i_Col;
  
  for (i_Col = m_nXcolumns + m_nTcolumns;
       i_Col < m_nTotalColumns;
       i_Col++)
    m_eColumns[i_Col] = eRandomType();
}

/*----------------------------------------------------------------------------*/
/* column variants                                                            */
/*----------------------------------------------------------------------------*/
void SchemaTest::vAddColumns(i8 i_Col)
{
  if (0 == i_Col)
    vAddXcolumns();
  
  for (i8 i_Type = CT_NONE + 1; i_Type < CT_ENUMS; i_Type++) {
    
    if (CT_CHAR == i_Type)
      continue;
    
    m_eColumns[m_nXcolumns + i_Col] = (eCT)i_Type;
    
    if (i_Col + 1 == m_nTcolumns) {
      vAddZcolumns();
      vMakeTest();
    }
    else
      vAddColumns(i_Col + 1);
  }
}

/*----------------------------------------------------------------------------*/
/* for nColumns                                                               */
/*----------------------------------------------------------------------------*/
void SchemaTest::vForPattern(i8 nX, i8 nT, i8 nZ)
{
  m_nTcolumns = nT;
  m_nZcolumns = nZ;
  
  if (-1 == nX)
    m_nXcolumns = C_MAX_COLUMNS - m_nTcolumns - m_nZcolumns;
  else
    m_nXcolumns = nX;
  
  m_nTotalColumns = m_nXcolumns + m_nTcolumns + m_nZcolumns;

  vAddColumns(0);
}



/*----------------------------------------------------------------------------*/
/* 1 column variants                                                          */
/*----------------------------------------------------------------------------*/
void SchemaTest::vTestSet(void)
{
  // left
  vForPattern(0,1,0);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(0,2,0);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(0,3,0);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(0,4,0);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(0,5,0);  m_xFile.vPrint("\n"_Stz);
//
//  vForPattern(3,1,0);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(3,2,0);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(3,3,0);  m_xFile.vPrint("\n"_Stz);
//
//  vForPattern(0,1,3);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(0,2,3);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(0,3,3);  m_xFile.vPrint("\n"_Stz);
//
//  vForPattern(3,1,3);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(3,2,3);  m_xFile.vPrint("\n"_Stz);
//  vForPattern(3,3,3);  m_xFile.vPrint("\n"_Stz);
//
//  vForPattern(73,1,0); m_xFile.vPrint("\n"_Stz);
//  vForPattern(73,2,0); m_xFile.vPrint("\n"_Stz);
//  vForPattern(73,3,0); m_xFile.vPrint("\n"_Stz);
//
//  vForPattern(-1,3,3); m_xFile.vPrint("\n"_Stz);
//  vForPattern(-1,2,3); m_xFile.vPrint("\n"_Stz);
//  vForPattern(-1,1,3); m_xFile.vPrint("\n"_Stz);
//
//  vForPattern(-1,3,2); m_xFile.vPrint("\n"_Stz);
//  vForPattern(-1,2,2); m_xFile.vPrint("\n"_Stz);
//  vForPattern(-1,1,2); m_xFile.vPrint("\n"_Stz);
//
//  vForPattern(-1,3,1); m_xFile.vPrint("\n"_Stz);
//  vForPattern(-1,2,1); m_xFile.vPrint("\n"_Stz);
//  vForPattern(-1,1,1); m_xFile.vPrint("\n"_Stz);
//
//  vForPattern(-1,3,0); m_xFile.vPrint("\n"_Stz);
//  vForPattern(-1,2,0); m_xFile.vPrint("\n"_Stz);
//  vForPattern(-1,1,0); m_xFile.vPrint("\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SchemaTest::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEMATEST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
