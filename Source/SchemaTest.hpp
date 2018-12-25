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
    i8   m_nTests;    // number of tests generated
  
    s256 m_sFileNm;   // file name
    File m_oFile;     // file to write to
  
    s32  m_sStream;   // stream prefix
    s32  m_sTableNm;  // name of the table
    s32  m_sColumnNm; // name of the column
    s32  m_sColumnTy; // definition of the column
    s256 m_sRow;      // result row

    i8   m_nXcolumns; // number of X columns
    i8   m_nCcolumns; // number of C columns
    i8   m_nZcolumns; // number of Z columns
    i8   m_nTotalColumns; // total number of columns

    eCT  m_eColumns[C_COLUMNS]; // column pattern
  
    s48K m_sStatement; // statement
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
  
    SchemaTest(Stz* sStream);
    ~SchemaTest(void) noexcept(false);

    static void vRun(Stz* sStream);

    void vGenerate(void);

    void vAddCcolumns(i8 i_Col);
    void vAddXcolumns();
    void vAddZcolumns();
    void vCloseFile(void);
    void vFileFooter(void);
    void vFileHeader(void);
    void vFormColumnList(void);
    void vFormColumnName(i8 i_Col);
    void vFormColumnType(i8 i_Col);
    void vFormCreateTable(void);
    void vFormDropTable(void);
    void vFormRow(i8 i_Col);
    void vFormTableName(void);
    void vOpenFile(void);
    void vMakeTest(void);
    eCT  eRandomType(void);
    void vWriteLsCol(void);
    void vWriteRows(void);
    void vWriteSuccess(void);
    void vWriteStatement(void);

    void vForPattern(i8 nX, i8 nC, i8 nZ);
  
    void vTestSet(void);
  
    Footer* aFooter(void);
  };
  
  SchemaTest* _pSchemaTest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
