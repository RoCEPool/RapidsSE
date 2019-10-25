/*============================================================================*/
/* REF.hpp -- counted reference pointer                                       */
/*============================================================================*/
#pragma once

#include "AM.hpp"

namespace Correctness
{
  template <class CLASS>
  class REF
  {
  public:
  
    CLASS* m_pObject; // pointer to the object
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REF<CLASS>(void)
    {
      m_pObject = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                            (nullptr allowed) */
    /*------------------------------------------------------------------------*/
    REF<CLASS>(CLASS* pObject)
    {
      if ( pObject && ! AM::bInBufferPool(pObject) )
        THROW("REF::REF(pObject) : invalid pObject parameter"_Stz);
    
      m_pObject = pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
    /*------------------------------------------------------------------------*/
    /* assignment operator =                                (nullptr allowed) */
    /*------------------------------------------------------------------------*/
    void operator = (CLASS* pObject)
    {
      if ( pObject && ! AM::bInBufferPool((Object*)pObject) )
        THROW("REF::operator =(pObject) : invalid pObject parameter"_Stz);
    
      m_pObject = pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASS* operator -> (void)
    {
      if (! m_pObject)
        THROW("REF::operator ->(void) : m_pObject member is null"_Stz);
   
      return m_pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* is pointer valid?                                (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    bool bIsValid(void)
    {
      if ( ! m_pObject || (m_pObject && AM::bInBufferPool(m_pObject)) )
        return true;
      else
        return false;
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
