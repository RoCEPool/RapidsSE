/*============================================================================*/
/* IndexLevels.hpp - set of index buffers by level for an index partition     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  // allow for maximum 16 levels per index
  static const i8 C_LEVELS_PER_INDEX = 16;
  
  class IndexLevels : public Object
  {
  private:
    Lock         m_zLock;          // lock covers all levels
    IndexBuffer* m_pIndexBuffer[C_LEVELS_PER_INDEX];
  
    // must be last
    Footer m_xFooter;
    
  public:
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
  
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexLevels(void);
    ~IndexLevels(void);
    
    IndexBuffer* pFirstBuffer(void);
    
    void vInsertEntry(EntWsp& xEntWsp);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
