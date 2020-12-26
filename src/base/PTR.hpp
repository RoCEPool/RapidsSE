/*============================================================================*/
/* PTR.hpp -- smart pointer                                                   */
/*============================================================================*/
#pragma once

namespace Resources
{
  template <class CLASSNM>
  class PTR
  {
  public:
  
    CLASSNM* m_pObject; // pointer to the object
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    PTR<CLASSNM>(void)
    {
      m_pObject = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                            (nullptr allowed) */
    /*------------------------------------------------------------------------*/
    PTR<CLASSNM>(CLASSNM* pObject)
    {
      if ( pObject && ! AM::bInBufferPool(pObject) )
        THROW("PTR::PTR(pObject) : invalid pObject parameter"_Stz);
    
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
        THROW("PTR::operator =(pObject) : invalid pObject parameter"_Stz);
    
      m_pObject = pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASSNM* operator -> (void)
    {
      if (! m_pObject)
        THROW("PTR::operator ->(void) : m_pObject member is null"_Stz);
   
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
