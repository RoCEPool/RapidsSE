/*============================================================================*/
/* MetadataNode.hpp - An mutable node that holds metadata for the user's query*/
/*============================================================================*/
#pragma once

namespace Parser
{
  class MetadataNode : public MutablePlanNode
  {
  protected:

  private:
    Footer m_xFooter;
    
  public:
    // Overriding these because we inherited from ListItem_t and ListItem_t overrides
    // operator new with a fatal error that it cannot directly be newed.
    void* operator new(size_t);
    void  operator delete(void*);

    MetadataNode(Stz* szName, Stz* szType, Value xValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren);
    virtual ~MetadataNode(void);

    Footer* aFooter(void);

  };
  
  MetadataNode* _pMetadataNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
