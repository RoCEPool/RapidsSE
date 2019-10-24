/*============================================================================*/
/* SMA.hpp -- pointer which must be an address in shared memory (or nullptr)  */
/*============================================================================*/
#pragma once

#include "AM.hpp"

namespace Correctness
{
  template <class CLASS>
  class SMA
  {
  public:
    CLASS* m_pObject;
  
    /*------------------------------------------------------------------------*/
    /* auto conversion operator                                               */
    /*------------------------------------------------------------------------*/
    // auto conversion
    operator CLASS*(void)
    {
      return (CLASS*)m_pObject;
    }

    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    SMA<CLASS>(void)
    {
      m_pObject = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    SMA<CLASS>(CLASS* pObject)
    {
      if ( pObject && ! AM::bInBufferPool(pObject) )
        THROW("SMA::SMA(pObject) : invalid pObject parameter"_Stz);
    
      m_pObject = pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* assignment                                                             */
    /*------------------------------------------------------------------------*/
    void operator = (CLASS* pObject)
    {
      if ( pObject && ! AM::bInBufferPool((Object*)pObject) )
        THROW("SMA::operator =()  : invalid pObject parameter"_Stz);
    
      m_pObject = pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* member                                                                 */
    /*------------------------------------------------------------------------*/
    CLASS* operator -> (void)
    {
      if (! m_pObject)
        THROW("SMA::operator ->() : m_pObject member is null"_Stz);
   
      return m_pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* dereference                                                            */
    /*------------------------------------------------------------------------*/
    CLASS* operator * (void)
    {
      if (! m_pObject)
        THROW("SMA::operator *() : m_pObject member is null"_Stz);
      
      return m_pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* == operator                                                            */
    /*------------------------------------------------------------------------*/
    bool operator == (CLASS* object)
    {
      return m_pObject == object;
    }
    
    /*------------------------------------------------------------------------*/
    /* != operator                                                            */
    /*------------------------------------------------------------------------*/
    bool operator != (CLASS* object)
    {
      return m_pObject != object;
    }
    
    /*------------------------------------------------------------------------*/
    /* is pointer valid                                                       */
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
