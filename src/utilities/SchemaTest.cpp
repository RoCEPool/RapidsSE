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
SchemaTest::SchemaTest(Stz* sStream)
: Object(CN_SCHEMATEST)
, m_oFile(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEMATEST,_FOOTER_);
  
  m_nTests = 0;
  
  m_sStream = sStream;

#ifdef WITH_SETH_SUPPORT
  m_sFilePath = "../seth/schemaTest"_Stz;
  if (*m_sStream.azChar()) {
    m_sFilePath.vConcat("_"_Stz);
    m_sFilePath.vConcat(m_sStream);
  }
  m_sFilePath.vConcat(".test"_Stz);
#else
  m_sFilePath = "../commands/_schemaTest"_Stz;
  m_sFilePath.vConcat(m_sStream);
#endif

  m_sTableNm  = C_NO_NAME;
  m_sColumnNm = C_NO_NAME;
  m_sColumnTy = C_NO_NAME;
  m_sRow      = C_NO_NAME;

  m_nCcolumns = 0;
  m_nXcolumns = 0;
  m_nZcolumns = 0;

  m_nTotalColumns = 0;

  for (i8 i_Col = 0; i_Col < C_COLUMNS; i_Col++)
    m_eColumns[i_Col] = CT_NONE;
  m_sStatement = C_NO_NAME;

  _checkInitialised_(CN_SCHEMATEST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCHEMATEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SchemaTest::~SchemaTest(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEMATEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* run                                                                        */
/*----------------------------------------------------------------------------*/
void SchemaTest::vRun(Stz* sStream)
{
  SchemaTest* pSchemaTest = new SchemaTest(sStream);
  
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
  
  if (*m_sStream.azChar())
    vTrace("  %lld tests generated (for stream %s)\n"_Stz,m_nTests,m_sStream.azChar());
  else
    vTrace("  %lld tests generated\n"_Stz,m_nTests);
}

/*----------------------------------------------------------------------------*/
/* set of tests to generate                                                   */
/*----------------------------------------------------------------------------*/
void SchemaTest::vTestSet(void)
{
  vForPattern(0,1,0);
  vForPattern(0,2,0);
  vForPattern(0,3,0);

  vForPattern(1,1,0);
  vForPattern(1,2,0);
  vForPattern(1,3,0);

  vForPattern(2,1,0);
  vForPattern(2,2,0);
  vForPattern(2,3,0);

  vForPattern(3,1,0);
  vForPattern(3,2,0);
  vForPattern(3,3,0);

  vForPattern(0,1,1);
  vForPattern(0,2,1);
  vForPattern(0,3,1);

  vForPattern(0,1,2);
  vForPattern(0,2,2);
  vForPattern(0,3,2);

  vForPattern(0,1,3);
  vForPattern(0,2,3);
  vForPattern(0,3,3);

  vForPattern(3,1,3);
  vForPattern(3,2,3);
  vForPattern(3,3,3);

  vForPattern(73,1,0);
  vForPattern(73,2,0);
  vForPattern(73,3,0);

  vForPattern(73,1,1);
  vForPattern(73,2,1);
  vForPattern(73,3,1);

  vForPattern(73,1,2);
  vForPattern(73,2,2);
  vForPattern(73,3,2);

  vForPattern(73,1,3);
  vForPattern(73,2,3);
  vForPattern(73,3,3);

  vForPattern(73,3,-1);
  vForPattern(73,2,-1);
  vForPattern(73,1,-1);

  vForPattern(-1,3,3);
  vForPattern(-1,2,3);
  vForPattern(-1,1,3);

  vForPattern(-1,3,2);
  vForPattern(-1,2,2);
  vForPattern(-1,1,2);

  vForPattern(-1,3,1);
  vForPattern(-1,2,1);
  vForPattern(-1,1,1);

  vForPattern(-1,3,0);
  vForPattern(-1,2,0);
  vForPattern(-1,1,0);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* add C columns                                                              */
/*----------------------------------------------------------------------------*/
void SchemaTest::vAddCcolumns(i8 i_Col)
{
  if (0 == i_Col)
    vAddXcolumns();
  
  for (i8 i_Type = CT_NONE + 1; i_Type < CT_ENUMS; i_Type++) {
    
    if (CT_CHAR == i_Type)
      continue;
    
    m_eColumns[m_nXcolumns + i_Col] = (eCT)i_Type;
    
    if (i_Col + 1 == m_nCcolumns) {
      vAddZcolumns();
      vMakeTest();
    }
    else
      vAddCcolumns(i_Col + 1);
  }
}
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
  
  for (i_Col = m_nXcolumns + m_nCcolumns;
       i_Col < m_nTotalColumns;
       i_Col++)
    m_eColumns[i_Col] = eRandomType();
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void SchemaTest::vCloseFile(void)
{
  m_oFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* file footer                                                                */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFileFooter(void)
{
#ifdef WITH_SETH_SUPPORT
  m_oFile.vPrint("%s\n"_Stz,
  "/*----------------------------------------------------------------------------*/"_Stz);
  m_oFile.vPrint("%s\n"_Stz,
  "/* end                                                                        */"_Stz);
  m_oFile.vPrint("%s\n"_Stz,
  "/*----------------------------------------------------------------------------*/"_Stz);
#else
  m_oFile.vPrint("%s\n"_Stz,
  "/*============================================================================*/"_Stz);
  m_oFile.vPrint("%s\n"_Stz,
  "/* end                                                                        */"_Stz);
  m_oFile.vPrint("%s\n"_Stz,
  "/*============================================================================*/"_Stz);
#endif
}

/*----------------------------------------------------------------------------*/
/* file header                                                                */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFileHeader(void)
{
#ifdef WITH_SETH_SUPPORT
  m_oFile.vPrint("%s\n"_Stz,
  "/*----------------------------------------------------------------------------*/"_Stz);
  m_oFile.vPrint("%s\n"_Stz,
  "/* schemaTest.test -- schema stress test (auto generated)                     */"_Stz);
  m_oFile.vPrint("%s\n"_Stz,
   "/*----------------------------------------------------------------------------*/"_Stz);
#else
  m_oFile.vPrint("%s\n"_Stz,
  "/*============================================================================*/"_Stz);
  m_oFile.vPrint("%s\n"_Stz,
  "/* _schemaTest -- schema stress test (auto generated)                         */"_Stz);
  m_oFile.vPrint("%s\n"_Stz,
  "/*============================================================================*/"_Stz);
#endif
}

/*----------------------------------------------------------------------------*/
/* form column list                                                           */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormColumnList(void)
{
  for (i8 i_Col = 0; i_Col < m_nTotalColumns; i_Col++) {
   
    vFormColumnName(i_Col);
    m_sStatement.vConcat("  "_Stz);
    m_sStatement.vConcat(m_sColumnNm);
    
    vFormColumnType(i_Col);
    m_sStatement.vConcat(" "_Stz);
    m_sStatement.vConcat(m_sColumnTy);

    if (i_Col + 1 < m_nTotalColumns)
      m_sStatement.vConcat(",\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* form column name                                                           */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormColumnName(i8 i_Col)
{
  m_sColumnNm = ""_Stz;
  
  if (i_Col < m_nXcolumns)
    m_sColumnNm.vConcat("x"_Stz);
  else if (i_Col < m_nXcolumns + m_nCcolumns)
    m_sColumnNm.vConcat("c"_Stz);
  else
    m_sColumnNm.vConcat("z"_Stz);
  
  s32 sColNo;
  StrLib::wToString(i_Col,sColNo.azChar());
  m_sColumnNm.vConcat(sColNo);
}

/*----------------------------------------------------------------------------*/
/* form column type                                                           */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormColumnType(i8 i_Col)
{
  m_sColumnTy = ""_Stz;
  
  switch (m_eColumns[i_Col]) {
      
    case CT_BOOLEAN:
      m_sColumnTy.vConcat("boolean"_Stz);
      break;
      
    case CT_DATE:
      m_sColumnTy.vConcat("date"_Stz);
      break;
      
    case CT_DECIMAL:
      m_sColumnTy.vConcat("decimal"_Stz);
      break;
      
    case CT_FLOAT:
      m_sColumnTy.vConcat("float"_Stz);
      break;
      
    case CT_INTEGER:
      m_sColumnTy.vConcat("integer"_Stz);
      break;
      
    case CT_INTERVAL:
      m_sColumnTy.vConcat("interval"_Stz);
      break;
      
    case CT_TIME:
      m_sColumnTy.vConcat("time"_Stz);
      break;
      
    case CT_TIMESTAMP:
      m_sColumnTy.vConcat("timestamp"_Stz);
      break;
      
    case CT_VARCHAR:
      m_sColumnTy.vConcat("varchar(32)"_Stz);
      break;
      
    default:
      NEVER_REACH;
  }
}
/*----------------------------------------------------------------------------*/
/* form create table statement                                                */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormCreateTable(void)
{
  m_sStatement = "create table "_Stz;
  
  m_sStatement.vConcat(m_sTableNm);

  m_sStatement.vConcat(" (\n"_Stz);
  
  vFormColumnList();

  m_sStatement.vConcat("\n) ;"_Stz);
}

/*----------------------------------------------------------------------------*/
/* form drop table statement                                                  */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormDropTable(void)
{
  m_sStatement = "drop table "_Stz;
  
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
  
  if (*m_sStream.azChar()) {
    m_sTableNm.vConcat(m_sStream);
    m_sTableNm.vConcat("_"_Stz);
  }
  
  // no columns
//  s32 sTableNo;
//  StrLib::wToString(m_nCcolumns,sTableNo.azChar());
//  m_sTableNm.vConcat(sTableNo);
//  m_sTableNm.vConcat("_"_Stz);
  
  // x columns prefix
  s32 sX;
  StrLib::wToString(m_nXcolumns,sX.azChar());
  m_sTableNm.vConcat("X"_Stz);
  m_sTableNm.vConcat(sX);
  
  s256 sColShorts;
  sColShorts = ""_Stz;
  
  // the t columns
  for (i_Col = m_nXcolumns;
       i_Col < m_nXcolumns + m_nCcolumns;
       i_Col++)
    sColShorts.vConcat(aCT_short[m_eColumns[i_Col]]);

  m_sTableNm.vConcat("_C"_Stz);
  m_sTableNm.vConcat(sColShorts);
  
  // z columns suffix
  s32 sZ;
  StrLib::wToString(m_nZcolumns,sZ.azChar());
  m_sTableNm.vConcat("_Z"_Stz);
  m_sTableNm.vConcat(sZ);
}

/*----------------------------------------------------------------------------*/
/* form row                                                                   */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormRow(i8 i_Col)
{
  m_sRow = ""_Stz;
  
  m_sRow.vConcat("  ("_Stz);
  
  s32 sColNo;
  StrLib::wToString(i_Col,sColNo.azChar());
  m_sRow.vConcat(sColNo);
  m_sRow.vConcat(","_Stz);

  vFormColumnName(i_Col);
  m_sRow.vConcat(" '"_Stz);
  m_sRow.vConcat(m_sColumnNm);
  m_sRow.vConcat("', "_Stz);
  
  switch (m_eColumns[i_Col]) {
      
    case CT_BOOLEAN:
      if (! i_Col)
        m_sRow.vConcat("'BOOLEAN', 0, 0, true, true"_Stz);
      else
        m_sRow.vConcat("'BOOLEAN', 0, 0, true, false"_Stz);
      break;
      
    case CT_DATE:
      if (! i_Col)
        m_sRow.vConcat("'DATE', 0, 0, true, true"_Stz);
      else
        m_sRow.vConcat("'DATE', 0, 0, true, false"_Stz);
      break;
      
    case CT_DECIMAL:
      if (! i_Col)
        m_sRow.vConcat("'DECIMAL', 19, 0, true, true"_Stz);
      else
        m_sRow.vConcat("'DECIMAL', 19, 0, true, false"_Stz);
      break;
      
    case CT_FLOAT:
      if (! i_Col)
        m_sRow.vConcat("'FLOAT', 19, 0, true, true"_Stz);
      else
        m_sRow.vConcat("'FLOAT', 19, 0, true, false"_Stz);
      break;
      
    case CT_INTEGER:
      if (! i_Col)
        m_sRow.vConcat("'INTEGER', 19, 0, true, true"_Stz);
      else
        m_sRow.vConcat("'INTEGER', 19, 0, true, false"_Stz);
      break;
      
    case CT_INTERVAL:
      if (! i_Col)
        m_sRow.vConcat("'INTERVAL', 0, 0, true, true"_Stz);
      else
        m_sRow.vConcat("'INTERVAL', 0, 0, true, false"_Stz);
      break;
      
    case CT_TIME:
      if (! i_Col)
        m_sRow.vConcat("'TIME', 0, 0, true, true"_Stz);
      else
        m_sRow.vConcat("'TIME', 0, 0, true, false"_Stz);
      break;
      
    case CT_TIMESTAMP:
      if (! i_Col)
        m_sRow.vConcat("'TIMESTAMP', 0, 0, true, true"_Stz);
      else
        m_sRow.vConcat("'TIMESTAMP', 0, 0, true, false"_Stz);
      break;
      
    case CT_VARCHAR:
      if (! i_Col)
        m_sRow.vConcat("'VARCHAR', 32, 0, true, true"_Stz);
      else
        m_sRow.vConcat("'VARCHAR', 32, 0, true, false"_Stz);
      break;
      
    default:
      NEVER_REACH;
  }


  m_sRow.vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* for given pattern                                                          */
/*----------------------------------------------------------------------------*/
void SchemaTest::vForPattern(i8 nX, i8 nC, i8 nZ)
{
  
  if (-1 == nX) {
    m_nCcolumns = nC;
    m_nZcolumns = nZ;
    m_nXcolumns = C_MAX_COLUMNS - m_nCcolumns - m_nZcolumns;
  }
  else
    if (-1 == nZ) {
      m_nCcolumns = nC;
      m_nXcolumns = nX;
      m_nZcolumns = C_MAX_COLUMNS - m_nCcolumns - m_nXcolumns;
    }
    else {
      m_nXcolumns = nX;
      m_nCcolumns = nC;
      m_nZcolumns = nZ;
    }
  
  m_nTotalColumns = m_nXcolumns + m_nCcolumns + m_nZcolumns;
  
  vAddCcolumns(0);
  
  m_oFile.vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void SchemaTest::vOpenFile(void)
{
  bool bOpened = m_oFile.bOpenWrite(m_sFilePath);
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
  
#ifdef WITH_SETH_SUPPORT
  vWriteSuccess();
  vWriteLsCol();
  vWriteRows();
#endif
  
  vFormDropTable();
  vWriteStatement();
  
#ifdef WITH_SETH_SUPPORT
  vWriteSuccess();
#endif
  
  m_nTests++;
}

/*----------------------------------------------------------------------------*/
/* random column type                                                         */
/*----------------------------------------------------------------------------*/
eCT SchemaTest::eRandomType(void)
{
  while (true) {
    eCT eColumn = (eCT)RndLib::iInteger(CT_BOOLEAN,CT_VARCHAR);
    
    if (CT_CHAR != eColumn)
      return eColumn;
  }
}

/*----------------------------------------------------------------------------*/
/* write lscols command to file                                               */
/*----------------------------------------------------------------------------*/
void SchemaTest::vWriteLsCol(void)
{
  m_oFile.vPrint("lscol %s ;\n"_Stz,m_sTableNm.azChar());
}

/*----------------------------------------------------------------------------*/
/* write rows to file                                                         */
/*----------------------------------------------------------------------------*/
void SchemaTest::vWriteRows(void)
{
  m_oFile.vPrint("rows:\n"_Stz);
  for (i8 i_Col = 0; i_Col < m_nTotalColumns; i_Col++) {
    vFormRow(i_Col);
    m_oFile.vPrint("%s\n"_Stz,m_sRow.azChar());
  }
  m_oFile.vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write statement to file                                                    */
/*----------------------------------------------------------------------------*/
void SchemaTest::vWriteStatement(void)
{
  m_oFile.vPrint("%s\n"_Stz,m_sStatement.azChar());
}

/*----------------------------------------------------------------------------*/
/* write success to file                                                      */
/*----------------------------------------------------------------------------*/
void SchemaTest::vWriteSuccess(void)
{
  m_oFile.vPrint("success\n\n"_Stz);
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
