/*============================================================================*/
/* SchemaTest.hpp -- Schema Test                                              */
/*============================================================================*/
#pragma once

namespace Utilities
{
  static const i8 C_COLUMNS = 256;

  class SchemaTest : public Object
  {
  private:
    i8   m_nTests;   // number of tests generated
    File m_xFile;    // file to write to
  
    s32  m_sTableNm; // name of the table
  
    i8   m_nXcolumns; // number of X columns
    i8   m_nTcolumns; // number of T columns
    i8   m_nZcolumns; // number of Z columns
    i8   m_nTotalColumns; // total number of columns

    eCT  m_eColumns[C_COLUMNS]; // column pattern
  
    s48K m_sStatement; // statement
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
  
    SchemaTest(void);
    ~SchemaTest(void);

    static void vRun(void);

    void vGenerate(void);

    void vCloseFile(void);
    void vFileFooter(void);
    void vFileHeader(void);
    void vFormColumnList(void);
    void vFormCreateTable(void);
    void vFormDropTable(void);
    void vFormTableName(void);
    void vOpenFile(void);
    void vMakeTest(void);
    eCT  eRandomType(void);
    void vWriteStatement(void);

    void vAddXcolumns();
    void vAddZcolumns();
    void vAddColumns(i8 i_Col);
    void vForPattern(i8 nX, i8 nT, i8 nZ);
  
    void vTestSet(void);
  
    Footer* aFooter(void);
  };
  
  SchemaTest* _pSchemaTest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
