/*============================================================================*/
/* REG_t.hpp -- register for logic unit                                       */
/*============================================================================*/
#pragma once

namespace BASE
{
  template <class CLASSNM>
  class REG
  {
  public:
  
    CLASSNM* m_pObject; // pointer to the object
  
    i8       m_iOtherInfo;
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REG<CLASSNM>(void)
    {
      m_pObject = nullptr;
      m_iOtherInfo = C_UNKNOWN;
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
    /* destructor                                                             */
    /*------------------------------------------------------------------------*/
    ~REG<CLASSNM>(void)
    {
      //RASSERT(! m_pObject);
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
        THROW("REG::operator *() : m_pObject member is null"_Stz);
      
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
