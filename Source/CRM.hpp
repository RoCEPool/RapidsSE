/*============================================================================*/
/* CRM.hpp -- counted reference manager                                       */
/*============================================================================*/
#pragma once

#include "AM.hpp"

namespace Correctness
{
  template <class CLASS>
  class CRM
  {
  public:
  
    CLASS* m_pThis; // pointer to the object
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                        (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CRM<CLASS>(CLASS* pThis)
    {
      if ( ! pThis || ! AM::bInBufferPool(pThis) )
        THROW("CRM::CRM(pThis) : invalid pThis parameter"_Stz);
    
      m_pThis = pThis;
    }
    
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
  
    /*------------------------------------------------------------------------*/
    /* is pointer valid?                                (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    bool bIsValid(void)
    {
      if ( ! m_pThis || (m_pThis && AM::bInBufferPool(m_pThis)) )
        return true;
      else
        return false;
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
