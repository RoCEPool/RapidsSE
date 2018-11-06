/*============================================================================*/
/* SchemaTest.hpp -- Schema Test                                              */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class SchemaTest : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    SchemaTest(void);
    ~SchemaTest(void);
  
    Footer* aFooter(void);
  };
  
  SchemaTest* _pSchemaTest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
