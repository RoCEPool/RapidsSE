/*============================================================================*/
/* REG_t.hpp -- register of logic unit                                        */
/*============================================================================*/
#pragma once

#include "eRF.h"
#include "eLR.h"

namespace STORE
{
  template <class COMPONENT, eCN CLASSNO>
  class REG
  {
  public:

    COMPONENT* m_pComponent; // pointer to the component object
    eCN        m_eClassNo;   // class no of the component

    LogicUnit* m_p_LogicUnit;    // which logic unit
    eLR        m_eLogicUnitReg;  // LU register
    eRF        m_eRepair;        // how to repair
  
    void     (*m_fpRepair)(Object*); // the component repair function


    struct {
      
      u8 m_bUseAllowed      :  1; // this register will be used
      u8 m_bInitialised     :  1; // this register has been initialised
      u8 m_bFinalised       :  1; // this register has been finalised
      
      u8 m_bNullOnDelete    :  1; // m_pComponent must be null on delete
      u8 m_bDeleteOnFailure :  1; // m_pComponent to be deleted on failure
      
      u8 m_uSpare           : 59; // spare bits
      //                      64b
    };
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REG<COMPONENT,CLASSNO>(LogicUnit* pLogicUnit,
                   eRF eRepair,
                   void (*fpRepair)(Object* pObject),
                   eLR eLogicUnitReg)
    {
      m_p_LogicUnit   = pLogicUnit;
      m_eLogicUnitReg = eLogicUnitReg;
      m_eRepair       = eRepair;
      m_fpRepair      = fpRepair;
    
      m_pComponent = nullptr;
      m_eClassNo   = CLASSNO;

      m_bUseAllowed      = true;
      m_bInitialised     = true;
      m_bFinalised       = true;
    
      m_bNullOnDelete    = false;
      m_bDeleteOnFailure = false;
    
      m_uSpare = 0;
    
      AM::LOGICUNIT::vAddReg(pLogicUnit,this);
    }

    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REG<COMPONENT,CLASSNO>(LogicUnit* pLogicUnit,
                   eRF eRepair,
                   void (*fpRepair)(Object* pObject),
                   bool bNullOnDelete,
                   bool bDeleteOnFailure,
                   eLR eLogicUnitReg)
    {
      m_p_LogicUnit   = pLogicUnit;
      m_eLogicUnitReg = eLogicUnitReg;
      m_eRepair    = eRepair;
      m_fpRepair   = fpRepair;

      m_pComponent = nullptr;
      m_eClassNo   = CLASSNO;
    
      m_bUseAllowed      = true;
      m_bInitialised     = true;
      m_bFinalised       = true;

      m_bNullOnDelete    = bNullOnDelete;
      m_bDeleteOnFailure = bDeleteOnFailure;

      m_uSpare = 0;

      AM::LOGICUNIT::vAddReg(pLogicUnit,this);
    }

    /*------------------------------------------------------------------------*/
    /* destructor                                                             */
    /*------------------------------------------------------------------------*/
    ~REG<COMPONENT,CLASSNO>(void)
    {
      RASSERT(m_bInitialised);
      RASSERT(m_bFinalised);
    
      RASSERT(! m_bNullOnDelete || ! m_pComponent);
    }


    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
    /*------------------------------------------------------------------------*/
    /* assignment operator =                                (nullptr allowed) */
    /*------------------------------------------------------------------------*/
    void operator = (COMPONENT* pComponent)
    {
      if (pComponent && ! AM::bInBufferPool(pComponent)) {
        vTrace("%20s: %p \n"_Stz,aLR[m_eLogicUnitReg],m_pComponent);
        THROW("REG::operator =(pComponent) : invalid pComponent address"_Stz);
      }
    
      if (pComponent && CN_NONE != m_eClassNo) {
        
        eCN eClassNo = pComponent->eGetClassNo();
        
        if (eClassNo != m_eClassNo)
          vTrace("Attempting to assign %p %lld:%s into REG<%lld:%s>::%s\n"_Stz
                 ,pComponent
                 ,eClassNo
                 ,aCN[eClassNo]
                 ,CLASSNO
                 ,aCN[CLASSNO]
                 ,aLR[m_eLogicUnitReg]
                 );
        RASSERT(eClassNo == m_eClassNo);
      }
    
      m_pComponent = pComponent;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    COMPONENT* operator -> (void)
    {
      if (! m_pComponent) {
        vTrace("%20s: %p \n"_Stz,aLR[m_eLogicUnitReg],m_pComponent);
        THROW("REG::operator ->(void) : m_pComponent member is null"_Stz);
      }
    
      return m_pComponent;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator *                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    COMPONENT* operator * (void)
    {
      if (! m_pComponent) {
        vTrace("%20s: %p \n"_Stz,aLR[m_eLogicUnitReg],m_pComponent);
        THROW("REG::operator *() : m_pComponent member is null"_Stz);
      }
    
      return m_pComponent;
    }
    
    /*------------------------------------------------------------------------*/
    /* delete object                                                          */
    /*------------------------------------------------------------------------*/
    void vDeleteComponent(void)
    {
      delete m_pComponent;
      m_pComponent = nullptr;
    }

    /*------------------------------------------------------------------------*/
    /* dump the register                                                      */
    /*------------------------------------------------------------------------*/
    void vDump(void)
    {
      if (m_pComponent) {
        vTrace("%20s : %p ; \n"_Stz
               ,aLR[m_eLogicUnitReg]
               ,m_pComponent
        );
      }
    }

    /*------------------------------------------------------------------------*/
    /* gurded delete object                                                   */
    /*------------------------------------------------------------------------*/
    void vGuardedDeleteComponent(eCN eClassNo,
                                 eCN eDeleterClassNo, Object* pDeleter, Stz* szFunc)
    {
      RASSERT(CLASSNO == eClassNo);
      m_pComponent->vGuardedDelete(CLASSNO,&m_pComponent,
                                   eDeleterClassNo,pDeleter,szFunc);
    }

    /*------------------------------------------------------------------------*/
    /* is there a pointer?                                                    */
    /*------------------------------------------------------------------------*/
    bool bIsNotNull(void)
    {
      if (m_pComponent)
        return true;
      else
        return false;
    }
    
    /*------------------------------------------------------------------------*/
    /* is null pointer?                                                       */
    /*------------------------------------------------------------------------*/
    bool bIsNull(void)
    {
      if ( ! m_pComponent )
        return true;
      else
        return false;
    }
    
    /*------------------------------------------------------------------------*/
    /* is pointer valid?                                (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    bool bIsValid(void)
    {
      if ( ! m_pComponent || (m_pComponent && AM::bInBufferPool(m_pComponent)) )
        return true;
      else
        return false;
    }
  
  
  
    /*------------------------------------------------------------------------*/
    /* set null on delete                                                     */
    /*------------------------------------------------------------------------*/
    void vNullOnDelete(bool bNullOnDelete)
    {
      m_bNullOnDelete = bNullOnDelete;
    }
  
    /*------------------------------------------------------------------------*/
    /* call m_pComponent repair function to repair itself                     */
    /*------------------------------------------------------------------------*/
    void vRepair(void)
    {

      // m_pComponent already NULL ?
      if ( !m_pComponent ) {
        // nothing to do
        return;
      }

      // RF_REPAIR_NEVER ?
      // Should never reach here for a RF_REPAIR_NEVER ?
      RASSERT(m_eRepair != RF_REPAIR_NEVER);

      // RF_REPAIR_NONE ?
      if (m_eRepair == RF_REPAIR_NONE) {
        // nothing to do
        return;
      }

      // RF_SET_NULL ?
      if (m_eRepair == RF_SET_NULL) {
        // null the component
        m_pComponent = nullptr;
        RASSERT(!m_fpRepair);
        return;
      }

      // Following code to be uncommented once all repairs in place
      /*
      RASSERT(m_fpRepair);
      m_fpRepair((Object *) m_pComponent;

      // null the component
      m_pComponent = nullptr;
      */

      // execute the repair function ?
      if (m_fpRepair) {
        m_fpRepair((Object *) m_pComponent);

        // null the component
        m_pComponent = nullptr;
      }

    }

    /*------------------------------------------------------------------------*/
    /* use is allowed                                                         */
    /*------------------------------------------------------------------------*/
    void vUseAllowed(void)
    {
      m_bNullOnDelete = true;
    }

    /*------------------------------------------------------------------------*/
    /* initialised                                                            */
    /*------------------------------------------------------------------------*/
    void vInitialised(void)
    {
      m_bNullOnDelete = true;
    }

    /*------------------------------------------------------------------------*/
    /* finalised                                                              */
    /*------------------------------------------------------------------------*/
    void vFinalised(void)
    {
      m_bFinalised = true;
    }
  
    /*------------------------------------------------------------------------*/
    /* set register name                                                      */
    /*------------------------------------------------------------------------*/
    void vRegNm(eLR eLogicUnitReg)
    {
      m_eLogicUnitReg = eLogicUnitReg;
    }

    /*------------------------------------------------------------------------*/
    /* set logic unit                                                         */
    /*------------------------------------------------------------------------*/
    void vLogicUnit(LogicUnit* pLogicUnit)
    {
      m_p_LogicUnit = pLogicUnit;
    }

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
