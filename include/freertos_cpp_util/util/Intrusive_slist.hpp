/**
 * @brief Intrusive singly linked list 
 * @author Jacob Schloss <jacob@schloss.io>
 * @copyright Copyright (c) 2018 Jacob Schloss. All rights reserved.
 * @license Licensed under the 3-Clause BSD license. See LICENSE for details
*/

#pragma once

#include "freertos_cpp_util/util/Non_copyable.hpp"

#include <type_traits>

//A minimal intrusive singly linked list for OS use
//Not recommended for general use, since this does not manage memory or have many features

class Intrusive_slist;

class Intrusive_slist_node
{
public:

	friend class Intrusive_slist;

	Intrusive_slist_node()
	{
		m_next = nullptr;
	}

	~Intrusive_slist_node() = default;

	//copy & assign are default
	Intrusive_slist_node(const Intrusive_slist_node& rhs) = default;
	Intrusive_slist_node& operator=(const Intrusive_slist_node& rhs) = default;

	//permit move
	Intrusive_slist_node(Intrusive_slist_node&& rhs)
	{
		m_next = rhs.m_next;
		rhs.m_next = nullptr;
	}

	Intrusive_slist_node* next()
	{
		return m_next;
	}

	const Intrusive_slist_node* next() const
	{
		return m_next;
	}

protected:
	Intrusive_slist_node* m_next;
};

//in this list, nodes are held on the stack externally
//lifetime of nodes must be managed by the creator
class Intrusive_slist : private Non_copyable
{
public:

	Intrusive_slist()
	{
		m_head = nullptr;
	}

	~Intrusive_slist() = default;

	//copy & assign are banned
	//Since the nodes are owned externally, it is probably a bad idea to clone the list.
	Intrusive_slist(const Intrusive_slist& rhs) = delete;
	Intrusive_slist& operator=(const Intrusive_slist& rhs) = delete;

	//permit move
	Intrusive_slist(Intrusive_slist&& rhs)
	{
		m_head = rhs.m_head;
		rhs.m_head = nullptr;
	}

	template<typename T>
	T* front()
	{
		static_assert(std::is_base_of<Intrusive_slist_node, T>::value);

		return static_cast<T*>(m_head);
	}

	template<typename T>
	const T* front() const
	{
		static_assert(std::is_base_of<Intrusive_slist_node, T>::value);
		
		return static_cast<const T*>(m_head);
	}

	bool empty() const
	{
		return m_head == nullptr;
	}

	void push_front(Intrusive_slist_node* const node)
	{
		if(m_head)
		{
			node->m_next = m_head;
		}
		else
		{
			node->m_next = nullptr;
		}
		
		m_head = node;
	}

	void pop_front()
	{
		if(m_head)
		{
			m_head = m_head->m_next;
		}
	}

	bool erase(Intrusive_slist_node* const node)
	{
		Intrusive_slist_node* prev = nullptr;
		Intrusive_slist_node* curr = m_head;

		while(curr)
		{
			if(curr == node)
			{
				if(prev)
				{
					prev->m_next = curr->m_next;
				}

				node->m_next = nullptr;
				return true;
			}

			prev = curr;
			curr = curr->m_next;
		}

		return false;
	}

protected:
	Intrusive_slist_node* m_head;
};