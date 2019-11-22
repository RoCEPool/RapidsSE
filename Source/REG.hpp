/*============================================================================*/
/* REG.hpp -- hidden shared memory pointer (or nullptr)                       */
/*============================================================================*/
#pragma once

#include "AM.hpp"

namespace Correctness
{
  template <class CLASS>
  class REG
  {
  public:
  
    CLASS* m_pObject; // pointer to the object
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REG<CLASS>(void)
    {
      m_pObject = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                            (nullptr allowed) */
    /*------------------------------------------------------------------------*/
    REG<CLASS>(CLASS* pObject)
    {
      if ( pObject && ! AM::bInBufferPool(pObject) )
        THROW("REG::REG(pObject) : invalid pObject parameter"_Stz);
    
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
        THROW("REG::operator =(pObject) : invalid pObject parameter"_Stz);
    
      m_pObject = pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASS* operator -> (void)
    {
      if (! m_pObject)
        THROW("REG::operator ->(void) : m_pObject member is null"_Stz);
   
      return m_pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator *                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASS* operator * (void)
    {
      if (! m_pObject)
        THROW("SMA::operator *() : m_pObject member is null"_Stz);
      
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
