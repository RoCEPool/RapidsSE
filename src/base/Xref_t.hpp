/*============================================================================*/
/* Xref_t.hpp -- Cross Ref (pointer into another independent directed graph)  */
/*============================================================================*/
#pragma once

namespace BASE
{  
  template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
  class Xref_t : public Object
  {
  private:
  
    // container of graphs
    CONTAINER* m_pContainer;

    // call-back function which discovers a graph in container by id
    GRAPH*     (*m_fpDiscover)(CONTAINER* pContainer, ID tGraphId);

  
    ID     m_tGraphId; // id of graph
    GRAPH* m_pGraph;   // discovered graph (needs to be lockable)
  
    ITEM*  m_pItem;    // supplied item object in graph
    i8     m_iSecret;  // secret of item
    b8     m_bInUse;   // flag indicating item is in use (graph is locked)
  
    // must be last
    Footer m_xFooter;

  public:
    Xref_t(void);
   ~Xref_t(void) noexcept(false);
  
    void   vContainer(CONTAINER* pContainer, (*fpDiscover)(CONTAINER*,ID));
    GRAPH* pDiscoverGraph(ID tGraphId);
    void   vForgetGraph(void);

    void vItem(ID tID, GRAPH* pGraph, ITEM* pItem, i8 iSecret);
    void vItemBeginUse(void);
    void vItemEndUse(void);

    void vReset(void);
  
  private:

    Footer* aFooter(void);
  };

  template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
    Xref_t<XREF_CN, CONTAINER, ID, GRAPH, ITEM>* _pXref_t(void* aAddr);
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
Xref_t<XREF_CN, CONTAINER, ID, GRAPH, ITEM>
::Xref_t(void)
: Object(XREF_CN)
{
  //_precondition_(_ADDRESS_,_MAGIC_,XREF_CN,_FOOTER_);
  
  aFooter()->vAssertFooter();
    
  m_pContainer = nullptr;
  m_fpDiscover = nullptr;
  
  m_tGraphId.vReset();
  m_pGraph  = nullptr;
  
  m_pItem   = nullptr;
  m_iSecret = C_UNKNOWN;
  m_bInUse  = false;
  
  aFooter()->vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
Xref_t<XREF_CN, CONTAINER, ID, GRAPH, ITEM>
::~Xref_t(void) noexcept(false)
{
  aFooter()->vAssertFooter();
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set container information                                                  */
/*----------------------------------------------------------------------------*/
template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
void Xref_t<XREF_CN, CONTAINER, ID, GRAPH, ITEM>
::vContainer(CONTAINER* pContainer, (*fpDiscover)(CONTAINER*,ID));
{
  aFooter()->vAssertFooter();

  m_pContainer = pContainer;
  m_fpDiscover = fpDiscover;
}

/*----------------------------------------------------------------------------*/
/* discover a graph in container by id of graph                               */
/*----------------------------------------------------------------------------*/
template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
GRAPH* Xref_t<XREF_CN, CONTAINER, ID, GRAPH, ITEM>
::pDiscoverGraph(ID tGraphId);
{
  aFooter()->vAssertFooter();

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* record item which is in graph                                              */
/*----------------------------------------------------------------------------*/
template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
void Xref_t<XREF_CN, CONTAINER, ID, GRAPH, ITEM>
::vItem(ID tID, GRAPH* pGraph, ITEM* pItem, i8 iSecret)
{
  aFooter()->vAssertFooter();

}

/*----------------------------------------------------------------------------*/
/* begin use of item                                                          */
/*----------------------------------------------------------------------------*/
template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
void Xref_t<XREF_CN, CONTAINER, ID, GRAPH, ITEM>
::vItemBeginUse(void)
{
  aFooter()->vAssertFooter();

}

/*----------------------------------------------------------------------------*/
/* end use of item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
void Xref_t<XREF_CN, CONTAINER, ID, GRAPH, ITEM>
::vItemEndUse(void)
{
  aFooter()->vAssertFooter();

}

/*----------------------------------------------------------------------------*/
/* hard reset of this cross ref                                               */
/*----------------------------------------------------------------------------*/
template <eCN XREF_CN, class CONTAINER, class ID, class GRAPH, class ITEM>
void Xref_t<XREF_CN, CONTAINER, ID, GRAPH, ITEM>
::vReset(void)
{
  aFooter()->vAssertFooter();

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
