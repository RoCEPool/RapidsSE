/*============================================================================*/
/* REG_t.hpp -- hidden shared memory pointer (or nullptr)                     */
/*============================================================================*/
#pragma once



namespace Resources
{
  template <class CLASSNM>
  class REG
  {
  public:
  
    CLASSNM* m_pObject; // pointer to the object
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REG<CLASSNM>(void)
    {
      m_pObject = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                            (nullptr allowed) */
    /*------------------------------------------------------------------------*/
    REG<CLASSNM>(CLASSNM* pObject)
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
    void operator = (CLASSNM* pObject)
    {
      if ( pObject && ! AM::bInBufferPool((Object*)pObject) )
        THROW("REG::operator =(pObject) : invalid pObject parameter"_Stz);
    
      m_pObject = pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASSNM* operator -> (void)
    {
      if (! m_pObject)
        THROW("REG::operator ->(void) : m_pObject member is null"_Stz);
   
      return m_pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator *                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASSNM* operator * (void)
    {
      if (! m_pObject)
        THROW("SMA::operator *() : m_pObject member is null"_Stz);
      
      return m_pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* is there a pointer?                                                    */
    /*------------------------------------------------------------------------*/
    bool bIsNotNull(void)
    {
      if (m_pObject)
        return true;
      else
        return false;
    }
    
    /*------------------------------------------------------------------------*/
    /* is null pointer?                                                       */
    /*------------------------------------------------------------------------*/
    bool bIsNull(void)
    {
      if ( ! m_pObject )
        return true;
      else
        return false;
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
