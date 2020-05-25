/*============================================================================*/
/* Thing.hpp -- Arrangeable Thing                                             */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class Thing : public Object
  {
  private:

    s32     m_sObjNm;   // name of object
    Object* m_pObject;  // object being arranged
    i8      m_iSeqNo;   // optional sequence number
   
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Thing(Stz* szObjNm, Object* pObject);
   ~Thing(void) noexcept(false);
  
    Object* pObject(void);
    Stz*    szObjNm(void);
    i8      iSeqNo(void);
    void    vSeqNo(i8 iSeqNo);

    Footer* aFooter(void);
  };
  
  Thing* _pThing(void* aAddr);
}

bool bThingCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
