/*============================================================================*/
/* Grid.hpp -- Grid of Nodes                                                  */
/*============================================================================*/
#pragma once

#include <list>
#include <iterator>
#include "Parcel.hpp"

#include "Node.hpp"
#include "Node_I.hpp"


namespace STORE
{
  class Grid : public Operand
  {
  friend class STORE::Node_I;
  private:
  
    s32    m_sGridNm;      // name of this grid
    Node*  m_pThisNode;    // this node pointer
    SkipListNL* m_pNodes;  // skip list of jobs (ordered by job name)
 
    Parcel* m_pParcel;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Grid(Stz* szGridNm);
   ~Grid(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vRepair(Object* pObject);

    void vAddDetectedNode(i8 iNodeNo, Stz* szNodeNm,
                          Stz* szIPAddr, i8 iPortNo, Stz* szRegionNm);
    void vAddNode(Node* pNode);
    void vAdjustPtnMap(void);
    void vDsv(void);
    s16KB szDumpInfo(void);
    Node* pFindNode(i8 iNodeNo);
    Node* pFindNode(Stz* pName);
    Node* pGetNthNode(i8 i_Node);
    Stz* szGridNm(void);
    i8   nKnownNodes(void);
    Node* pOtherNode(void);
    void vRemoveNode(Node* pNode);
    void vResetNetwork(void);
    void vSeeNodes(void);
    void vShowGrid(void);
    void vShowNetwork(void);
    i8   iThisNodeId(void);
    Node* pThisNode(void);
    void vThisNode(Node* pNode);

    i8   iPrepareParcel(void);
    Parcel* pParcel(void);
    i8 iReassembleFromParcel(Parcel* pParcel);
  
    Footer* aFooter(void);
  };
    
  Grid* _pGrid(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
