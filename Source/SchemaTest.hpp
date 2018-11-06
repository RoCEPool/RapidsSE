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
  
    File m_xFile;    // file to write to
  
    s32  m_sTableNm; // name of the table
    i8   m_nColumns; // number of columns
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
    void vFormCreateTable(void);
    void vFormDropTable(void);
    void vFormTable(void);
    void vIterate(void);
    void vOpenFile(void);
    void vMakeTest(void);
    void vWriteStatement(void);

    void v_1col(void);
    void v_2col(void);
    void v_3col(void);
    void v_4col(void);
    void v_5col(void);
    void v_6col(void);

    void v_x1col(void);
    void v_x2col(void);
    void v_x3col(void);

    void v_y1col(void);
    void v_y2col(void);
    void v_y3col(void);
  
    void v_z1col(void);
    void v_z2col(void);
    void v_z3col(void);
  
    Footer* aFooter(void);
  };
  
  SchemaTest* _pSchemaTest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
