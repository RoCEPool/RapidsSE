/*============================================================================*/
/* MetaGet.hpp -- MetaGet                                                     */
/*============================================================================*/

#pragma once

namespace STORE {

  class MetaGet : public Operand
  {
  private:
    s256 m_sTableName;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaGet(s256 sTableNm);
    MetaGet(void);
    ~MetaGet(void) noexcept(false);

    static void vRepair(Object* pObject);

    void vTableName(s256 sTblNm) { m_sTableName = sTblNm;}
    s256 sTableName(void) { return m_sTableName;}

    Footer* aFooter(void);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  MetaGet *_pMetaGet(void *aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
