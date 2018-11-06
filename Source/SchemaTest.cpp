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
  m_nColumns = 0;
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
  
  vIterate();
  
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
/* form create table statement                                                */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormCreateTable(void)
{
  m_sStatement = "create table "_Stz;
  
  m_sStatement.vConcat(m_sTableNm);

  m_sStatement.vConcat(" ("_Stz);
  
  m_sStatement.vConcat(" ) ;"_Stz);
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
/* form table                                                                 */
/*----------------------------------------------------------------------------*/
void SchemaTest::vFormTable(void)
{
  m_sTableNm = "St_"_Stz;
  
  s32 sTableNo;
  
  StrLib::wToString(m_nColumns,sTableNo.aChar());
  
  m_sTableNm.vConcat(sTableNo);
  m_sTableNm.vConcat("_"_Stz);
  
  s256 sColShorts;
  sColShorts = ""_Stz;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
    sColShorts.vConcat(aCT_short[m_eColumns[i_Col]]);

  m_sTableNm.vConcat(sColShorts);

}

/*----------------------------------------------------------------------------*/
/* iterate                                                                    */
/*----------------------------------------------------------------------------*/
void SchemaTest::vIterate(void)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  v_1col();  // 1 column variants
  
  // 2 column variants
  
  // 3 column variants
  
  // 4 column variants
  
  // 5 column variants
  
  // 6 column variants

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // x + 1 column variants
  
  // x + 2 column variants
  
  // x + 3 column variants
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // y + 1 column variants
  
  // y + 2 column variants
  
  // y + 3 column variants
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // z + 1 column variants
  
  // z + 2 column variants
  
  // z + 3 column variants
  

  for (i8 i_Type = CT_NONE + 1; i_Type < CT_ENUMS; i_Type++) {
    
  }
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void SchemaTest::vOpenFile(void)
{
  bool bOpened = m_xFile.bOpen("/rapids/schemaTest.txt"_Stz, C_OPEN_WRITE);
  RASSERT(bOpened);
}

/*----------------------------------------------------------------------------*/
/* make a test for given column pattern                                       */
/*----------------------------------------------------------------------------*/
void SchemaTest::vMakeTest(void)
{
  vFormTable();
  
  vFormCreateTable();
  vWriteStatement();
  
  vFormDropTable();
  vWriteStatement();
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
/* 1 column variants                                                          */
/*----------------------------------------------------------------------------*/
void SchemaTest::v_1col(void)
{
  i8 i_Col = 0;
  
  m_nColumns = 1;
  
  for (i8 i_Type = CT_NONE + 1; i_Type < CT_ENUMS; i_Type++) {
    
    if (CT_CHAR == i_Type)
      continue;
  
    m_eColumns[i_Col] = (eCT)i_Type;
    
    vMakeTest();
  }
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
