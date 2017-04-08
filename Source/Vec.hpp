/*============================================================================*/
/* Vec.hpp - Vector template                                                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{  
  class Vec : public Object
  {
  static const i8 C_VEC = 1024; // max number of items

  private:
    i8      m_nItems;  // number of actual items
    Object* m_pItem[C_VEC]; // vector of items
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    Vec(void);
    ~Vec(void);
  
    void    vAdd(Object* pObject);
    i8      nItems(void);
    Object* pGet(i8 iItem);
  
    Footer* pFooter(void);
  };
  
  Vec* _pVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
