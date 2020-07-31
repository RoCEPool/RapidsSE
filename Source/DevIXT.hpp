/*============================================================================*/
/* DevIXT.hpp -- IXT Device                                                   */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class DevIXT : public Device
  {
  public:
  
    IndexTree* m_pIdxTree;
  
    Ent*       m_aEnt;
    EntBuffer* m_pEntBuffer;

    Crw*       m_aCrw;
    CrwBuffer* m_pCrwBuffer;

    i8         m_i_Row;
    i8         m_nRows;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    DevIXT(void);
   ~DevIXT(void) noexcept(false);
  

  private:
    Footer* aFooter(void);
  };
  
  DevIXT* _pDevIXT(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
