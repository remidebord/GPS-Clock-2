#include "Menu.h"

Menu :: Menu(void)
{
	m_head = 0;
	m_tail = 0;
	m_current = 0;
}

void Menu :: insert(Item* item)
{
	item->next = m_head;
	
	if(m_head != 0) m_head->previous = item;
	
	m_head = item;
	m_current = m_head;
	
	if(m_head->next == 0) m_tail = item;
}

void Menu :: next(void)
{
	if((m_current->state() != ITEM_SUB1) && (m_current->state() != ITEM_SUB2))
	{
		m_current = (m_current->next != 0) ? m_current->next : m_head;
	
		m_current->init();
	}
}

void Menu :: previous(void)
{
	if((m_current->state() != ITEM_SUB1) && (m_current->state() != ITEM_SUB2))
	{
		m_current = (m_current->previous != 0) ? m_current->previous : m_tail;
		
		m_current->init();
	}
}

void Menu :: current(Item** item)
{
	*item = m_current;
}
