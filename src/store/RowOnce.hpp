/*============================================================================*/
/* RowOnce.hpp -- Row Once                                                    */
/*============================================================================*/
#pragma once

namespace STORE
{
  class RowOnce : public Object
  {
  private:
  
    Lock      m_okLock;
    SkipListNL* m_pSkipList; // skip list of seen rowids (ordered by rowid)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowOnce(void);
    ~RowOnce(void) noexcept(false);

    static void vRepair(Object* pObject);

    bool bRowSeen(Rrf xRowRef);
  
    Footer* aFooter(void);  
  };
  
  RowOnce* _pRowOnce(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
