/*============================================================================*/
/* SMA.hpp -- pointer which must be an address in shared memory (or nullptr)  */
/*============================================================================*/
#pragma once

namespace Resources
{
  template <class CLASSNM>
  class SMA
  {
  public:
  
    CLASSNM* m_pObject; // pointer to the object
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    SMA<CLASSNM>(void)
    {
      m_pObject = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                            (nullptr allowed) */
    /*------------------------------------------------------------------------*/
    SMA<CLASSNM>(CLASSNM* pObject)
    {
      if ( pObject && ! AM::bInBufferPool(pObject) )
        THROW("SMA::SMA(pObject) : invalid pObject parameter"_Stz);
    
      m_pObject = pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
    /*------------------------------------------------------------------------*/
    /* assignment operator =                                (nullptr allowed) */
    /*------------------------------------------------------------------------*/
    void operator = (CLASSNM* pObject)
    {
      if ( pObject && ! AM::bInBufferPool((Object*)pObject) )
        THROW("SMA::operator =() : invalid pObject parameter"_Stz);
    
      m_pObject = pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* auto conversion operator                                               */
    /*------------------------------------------------------------------------*/
    operator CLASSNM*(void)
    {
      return (CLASSNM*)m_pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* dereference operator *                           (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASSNM* operator * (void)
    {
      if (! m_pObject)
        THROW("SMA::operator *() : m_pObject member is null"_Stz);
    
      return m_pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASSNM* operator -> (void)
    {
      if (! m_pObject) {
        vDebugPoint();
        THROW("SMA::operator ->() : m_pObject member is null"_Stz);
      }
   
      return m_pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* equal to operator ==                                                   */
    /*------------------------------------------------------------------------*/
    bool operator == (CLASSNM* pObject)
    {
      return m_pObject == pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* not equal to operator !=                                               */
    /*------------------------------------------------------------------------*/
    bool operator != (CLASSNM* pObject)
    {
      return m_pObject != pObject;
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
