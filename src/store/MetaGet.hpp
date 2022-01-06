/*============================================================================*/
/* MetaGet.hpp -- MetaGet                                                     */
/*============================================================================*/

#pragma once

namespace STORE {

  class MetaGet : public Operand
  {
  private:
    s32 m_sTableName;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaGet(s32 sTableNm);
    MetaGet(void);
    ~MetaGet(void) noexcept(false);

    static void vRepair(Object* pObject);

    void vTableName(s32 sTblNm) { m_sTableName = sTblNm;}
    s32 sTableName(void) { return m_sTableName;}

    Footer* aFooter(void);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  MetaGet *_pMetaGet(void *aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
