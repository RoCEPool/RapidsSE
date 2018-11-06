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
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
  
    SchemaTest(void);
    ~SchemaTest(void);

    static void vRun(void);
  
    void vGenerate(void);

    Footer* aFooter(void);
  };
  
  SchemaTest* _pSchemaTest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
