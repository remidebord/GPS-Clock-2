#ifndef _MENU_
#define _MENU_

#include <stdint.h>

#define ITEM_INIT 0x01
#define ITEM_MAIN 0x02
#define ITEM_SUB1	0x03
#define ITEM_SUB2	0x04

#define ITEM_INIT_TIME 500 // 500ms

struct Item{
	void (*init)(void);
	void (*main)(void);
	void (*sub1)(void);
	void (*sub2)(void);
	void (*timer)(void);
	uint8_t (*state)(void);
	Item* next;
	Item* previous;
};

class Menu
{
	private:
		Item* m_head;
		Item* m_tail;
		Item* m_current;
		
	public:
		Menu(void);
		void insert(Item* element);
		void next(void);
		void previous(void);
		void current(Item** element);
};

#endif
