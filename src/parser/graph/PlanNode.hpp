/*============================================================================*/
/* PlanNode.hpp - A generic node in the plan graph                            */
/*============================================================================*/
#pragma once

namespace Parser
{
  union Value {
    i8         (*m_afFun)(void* aArg);  // function pointer

    b1         m_bBoolean;   // boolean value
    i8         m_iInt;       // signed 8-byte integer value
    u8         m_uInt;       // unsigned 8-byte integer value
    d8         m_dFloat;     // 8-byte floating point value
    Text*      m_pText;      // text
    PlanNode*  m_pXref;      // cross-ref to another PlanNode
  };

  class PlanNode : public Object,
                   public ListItem_t<PlanNode>
  {
  protected:
    s256  m_sName;
    s256  m_sType;
    Value m_xValue;
    PlanNodeList* m_pProperties;
    PlanNodeList* m_pChildren;
    
  private:
    Footer m_xFooter;
    
  public:
    // Overriding these because we inherited from ListItem_t
    void* operator new(size_t);
    void  operator delete(void*);

    PlanNode(Stz* szName, Stz* szType, Value xValue,
             PlanNodeList* pProperties, PlanNodeList* pChildren);
    ~PlanNode(void); // TODO: Shouldn't this be virtual so that we can properly clean up members?

    Footer* aFooter(void);

  protected:
    PlanNode(eCN eClassNo, Stz* szName, Stz* szType, Value xValue,
             PlanNodeList* pProperties, PlanNodeList* pChildren);

  public:
    Stz* getName();
    Stz* getType();

    // Convenience methods for accessing the value object
    Value getValue();
    b1   getBooleanValue();
    i8   getIntValue();
    u8   getUnsignedIntValue();
    d8   getFloatValue();
    Stz* getStringValue();
    PlanNode* getXrefValue();


  };
  
  PlanNode* _pPlanNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
