/*============================================================================*/
/* REG_t.hpp -- register of logic unit                                        */
/*============================================================================*/
#pragma once

namespace BASE
{
  template <class CLASSNM>
  class REG
  {
  public:
  
    LogicUnit* m_pLogicUnit; // which logic unit
    Stz*       m_szRegNm;    // register name

  
    CLASSNM*   m_pObject;    // pointer to the object
   
    struct {
      
      u8 m_bUseAllowed      :  1; // this register will be used
      u8 m_bInitialised     :  1; // this register has been initialised
      u8 m_bFinalised       :  1; // this register has been finalised
      
      u8 m_bNullOnDelete    :  1; // m_pObject must be null on delete
      u8 m_bDeleteOnFailure :  1; // m_pObject to be deleted on failure
      
      u8 m_uSpare           : 59; // spare bits
      //                      64b
    };
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REG<CLASSNM>(LogicUnit* pLogicUnit, Stz* szRegNm)
    {
      m_pLogicUnit = nullptr;
      m_szRegNm    = szRegNm;
    
      m_pObject = nullptr;
      
      m_bUseAllowed      = true;
      m_bInitialised     = true;
      m_bFinalised       = true;
    
      m_bNullOnDelete    = false;
      m_bDeleteOnFailure = false;
    
      m_uSpare = 0;
    }
  
    /*------------------------------------------------------------------------*/
    /* destructor                                                             */
    /*------------------------------------------------------------------------*/
    ~REG<CLASSNM>(void)
    {
      RASSERT(m_bInitialised);
      RASSERT(m_bFinalised);
    
      RASSERT(! m_bNullOnDelete || ! m_pObject);
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
    /* delete object                                                          */
    /*------------------------------------------------------------------------*/
    bool vDeleteObject(void)
    {
      delete m_pObject;
      m_pObject = nullptr;
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
  
  
  
    /*------------------------------------------------------------------------*/
    /* set null on delete                                                     */
    /*------------------------------------------------------------------------*/
    bool vNullOnDelete(bool bNullOnDelete)
    {
      m_bNullOnDelete = bNullOnDelete;
    }
  
    /*------------------------------------------------------------------------*/
    /* call m_pObject to recover from failure                                 */
    /*------------------------------------------------------------------------*/
    bool vRecoverFromFailure(void)
    {
      m_pObject->vRecoverFromFailure();
    
      if (m_bDeleteOnFailure) {
        vDeleteObject();
      }
    }

    /*------------------------------------------------------------------------*/
    /* use is allowed                                                         */
    /*------------------------------------------------------------------------*/
    bool vUseAllowed(void)
    {
      m_bNullOnDelete = true;
    }

    /*------------------------------------------------------------------------*/
    /* initialised                                                            */
    /*------------------------------------------------------------------------*/
    bool vInitialised(void)
    {
      m_bNullOnDelete = true;
    }

    /*------------------------------------------------------------------------*/
    /* finalised                                                              */
    /*------------------------------------------------------------------------*/
    bool vFinalised(void)
    {
      m_bFinalised = true;
    }
  
    /*------------------------------------------------------------------------*/
    /* set register name                                                      */
    /*------------------------------------------------------------------------*/
    void vRegNm(Stz* szRegNm)
    {
      m_szRegNm = C_NO_NAME;
    }

    /*------------------------------------------------------------------------*/
    /* set logic unit                                                         */
    /*------------------------------------------------------------------------*/
    void vLogicUnit(LogicUnit* pLogicUnit)
    {
      m_pLogicUnit = pLogicUnit;
    }

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
