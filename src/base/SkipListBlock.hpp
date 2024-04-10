/*============================================================================*/
/* SkipListBlock.hpp -- Block of Node and Item                                */
/*============================================================================*/
#pragma once

#include "SkipListNode.hpp"

#define member_size(type, member) sizeof(((type *)0)->member)

namespace BASE
{
  struct conceptual__item{
    i8 iKey;

    //body start here
  };

  // mini-block
  class SkipListBlock
  {
  public:
    SkipListNode  m_xNode;
    i8 m_xItem[1];

  public:
    SkipListBlock(eCN eClassNo, i8 nSizeEnt);
    ~SkipListBlock(void) noexcept(false);

		/*----------------------------------------------------------------------------*/
		/* Get start addr of item                                                     */
		/*----------------------------------------------------------------------------*/
		inline void* aItem(void)
		{
			return (void*) m_xItem;
		}

		/*----------------------------------------------------------------------------*/
		/* Get key as i8                                                              */
		/*----------------------------------------------------------------------------*/
		inline i8 iKey(void)
		{
		//  conceptual__item* pItem = (conceptual__item*) m_xItem;
			return m_xItem[0];
		}

		/*----------------------------------------------------------------------------*/
		/* return next                                                                */
		/*----------------------------------------------------------------------------*/
		inline SkipListBlock* aNext(void)
		{
			return nullptr;
		}

		/*----------------------------------------------------------------------------*/
		/* set key                                                                    */
		/*----------------------------------------------------------------------------*/
		inline void vSetKey(i8 iKey)
		{
			conceptual__item* pItem = (conceptual__item*) m_xItem;
			pItem->iKey = (i8) iKey;
		}

		/*----------------------------------------------------------------------------*/
		/* get Row start addr (assumes i8 key in pds)                                 */
		/*----------------------------------------------------------------------------*/
		inline Row* aRow(void)
		{
			return aAdd<Row*>(m_xItem, sizeof(i8));
		}

	};
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
