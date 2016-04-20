#include <iostream>

namespace dmi
{

	template<class key_type, class Data>
	struct pair
	{

		Data data;
		key_type key;


		pair() : data(0), key(0)
		{}

		pair(Data el1, key_type el2) : data(el1), key(el2)
		{}

		pair(const pair<key_type, Data> & obj) : data(obj.data), key(obj.key)
		{}

		pair operator=(const pair& p1)
		{
			if (this == &p1)
				return *this;
			data = p1.data;
			key = p1.key;
			return *this;
		}

		pair operator=(Data _data)
		{
			data = _data;
			return *this;
		}

		friend bool operator!=(const pair& pi1, const pair& pi2)
		{
			if (pi1.data == pi2.data && pi1.key == pi2.key)
				return false;
			return true;
		}
		
		friend bool operator==(const pair& pi1, const pair& pi2)
		{
			if (pi1.data == pi2.data && pi1.key == pi2.key)
				return true;
			return false;
		}


		void swap(pair& p1, pair* p2)
		{
			pair pr;
			pr = *p1;
			*p1 = *p2;
			*p2 = temp;
		}

		friend std::ostream& operator<< (std::ostream& out, pair<key_type, Data> pair1)
		{
			out << pair1.data << " : " << pair1.key << " ";
			return out;
		}

		
	};


	template<class key_type, class Data>
	class value_type
	{
	public:
		value_type * Right;
		value_type * Left;
		value_type * ptrPrev;
		pair<key_type, Data> MyPair;
		bool flag;
		value_type() : Right(nullptr), Left(nullptr), MyPair(), ptrPrev(nullptr)
		{}

		value_type(pair<key_type, Data> _MyPair, value_type * _Right = nullptr, value_type * _Left = nullptr, value_type * _ptrPrev = nullptr) : Right(_Right), Left(_Left), ptrPrev(_ptrPrev), MyPair(_MyPair), flag(false)
		{}

		value_type(const value_type * obj) : Right(nullptr), Left(nullptr), ptrPrev(nullptr), flag(obj->flag)
		{
			MyPair = obj->MyPair;
		}

		~value_type()
		{}

		friend std::ostream& operator<< (std::ostream& out, value_type<key_type, Data> * el)
		{
			if (!el->flag)
				out << el->MyPair;
			return out;
		}
		friend std::ostream& operator<< (std::ostream& out, value_type<key_type, Data>&  el)
		{
			if (!el.flag)
				out << el.MyPair;
			return out;
		}

		value_type* operator=(const value_type * p1)
		{
			if (this == p1)
				return this;
			Right = p1->Right;
			Left = p1->Left;
			MyPair = p1->MyPair;
			ptrPrev = p1->ptrPrev;
			flag = p1->flag;
			return this;
		}

	};

	template <class key_type, class Data, class Compare = std::less<key_type>>
	class iterator
	{

	public:
		value_type<key_type, Data> * it;
		iterator(const Compare& c = Compare()) : it(nullptr), CompareFunc(c)
		{}
		iterator(const iterator& obj) : it(obj.it), CompareFunc(obj.CompareFunc)
		{}
		iterator(value_type<key_type, Data> * el, const Compare& c = Compare()) : it(el), CompareFunc(c)
		{}

		iterator& operator++()
		{
			if (it->flag)
				return *this;
			if (it == nullptr)
				return *this;
			if (it->ptrPrev == nullptr)
			{
				it = getLeft(it->Right);
				return *this;
			}
			if (CompareFunc(it->MyPair.key, it->ptrPrev->MyPair.key))
			{
				if (it->Right != nullptr)
				{
					if (getLeft(it->Right)->MyPair == it->Right->MyPair)
					{
						it = it->Right;
						return *this;
					}
					it = getLeft(it->Right);
					return *this;
				}
				it = it->ptrPrev;
				return *this;
			}
			if (CompareFunc(it->ptrPrev->MyPair.key, it->MyPair.key))
			{
				if (it->Right == nullptr)
				{
					value_type<key_type, Data> * el = it;
					while (CompareFunc(el->ptrPrev->MyPair.key, it->MyPair.key))
					{
						el = el->ptrPrev;
						if (el->ptrPrev == nullptr)
						{
							it = el->Right;
							break;
						}
					}
					it = el->ptrPrev;
					return *this;
				}
				it = getLeft(it->Right);
				return *this;

			}
		}
		iterator& operator--()
		{
			if (it->flag)
			{
				it = it->ptrPrev;
				it->flag = false;
				return *this;
			}
			if (it->ptrPrev == nullptr)
			{
				it = getRight(it->Left);
				return *this;
			}
			if (CompareFunc(it->MyPair.key, it->ptrPrev->MyPair.key))
			{
				if (it->Left == nullptr)
				{
					value_type<key_type, Data> * el = it;
					while (CompareFunc(it->MyPair.key, el->ptrPrev->MyPair.key))
						el = el->ptrPrev;
					it = el->ptrPrev;
					return *this;
				}
				it = getRight(it->Left);
				return *this;
			}
			if (CompareFunc(it->ptrPrev->MyPair.key, it->MyPair.key))
			{
				if (it->Left != nullptr)
				{
					if (getRight(it->Left)->MyPair == it->Left->MyPair)
					{
						it = it->Left;
						return *this;
					}
					it = getRight(it->Left);
					return *this;
				}
				it = it->ptrPrev;
				return *this;

			}
		}

		bool operator !=(iterator it)
		{
			if (it.it->flag != it->flag)
				return true;
			if (this->it != it.it)
				return true;
			return false;
		}

		value_type<key_type, Data>& operator*()
		{
			if (!it->flag && it != nullptr)
				return *it;
			else
				throw std::invalid_argument("lol");
		}

		value_type<key_type, Data>* operator->()
		{
			return it;
		}

		iterator& operator=(value_type<key_type, Data> * it1)
		{
			it = it1;
			return *this;
		}

		bool operator==(iterator& obj)
		{
			if (this->it->flag == obj.it->flag)
				return true;
			if (this->it == obj.it)
				return true;
			return false;
		}

		iterator& operator=(iterator& el)
		{
			if (*this == el)
				return *this;
			it = el.it;
			return *this;
		}

		friend std::ostream& operator<< (std::ostream& out, iterator<key_type, Data> it1)
		{
			if (!it1.it->flag && it != nullptr)
					out << it1.it;
			return out;
		}
		~iterator()
		{}
	private:
		Compare CompareFunc;
	
		value_type<key_type, Data> * getLeft(value_type<key_type, Data>* _root)
		{
			value_type<key_type, Data> * el = _root;
			while (el->Left != nullptr)
			{
				el = el->Left;
			}
			return el;
		}

		value_type<key_type, Data> * getRight(value_type<key_type, Data>* _root)
		{
			value_type<key_type, Data> * el = _root;
			while (el->Right != nullptr)
			{
				el = el->Right;
			}
			return el;
		}
		
	};


	template <class key_type, class Data, class Compare = std::less<key_type>>
	class map
	{
	public:
		
		
		map(const Compare& c = Compare()) : root(nullptr), CompareFunc(c), it(c)
		{}
		map(map& obj) : CompareFunc(obj.CompareFunc), it(obj.it)
		{
			root = nullptr;
			insertArea(obj.begin(), obj.end());
		}
		
		void swap(map & obj)
		{
			map buf(*this);
			clear();
			for (iterator<key_type, Data> iter = obj.begin(); iter != obj.end(); ++iter)
				insert(it.it->MyPair.key, it.it->MyPair.data);
			obj.clear();
			for (iterator<key_type, Data> iter = buf.begin(); iter != buf.end(); ++iter)
				obj.insert(it.it->MyPair.key, it.it->MyPair.data);
		}

		Data& operator[](key_type Key)
		{
			value_type<key_type, Data> * el = root;
			while (el != nullptr)
			{
				if (CompareFunc( el->MyPair.key, Key))
				{
					el = el->Right;
					if (el == nullptr)
						break;
				}
				if (CompareFunc(Key, el->MyPair.key))
				{
					el = el->Left;
					if (el == nullptr)
						break;
				}
				if (el->MyPair.key == Key)
				{
					return el->MyPair.data;
				}
				
			}
			return insert(NULL, Key).data;
			
		}

		pair<key_type, Data>& insert(key_type key, Data data)
		{
			++Num;
			pair<key_type, Data> MyPair(data, key);
			if (root == nullptr)
			{
				root = new value_type<key_type, Data>(MyPair);
				return root->MyPair;
			}
			value_type<key_type, Data> * el = root;
			while (1)
			{
				if (CompareFunc( el->MyPair.key, key))
				{
					if (el->Right == nullptr)
					{
						value_type<key_type, Data> * k = new value_type<key_type, Data>(MyPair);
						el->Right = k;
						k->ptrPrev = el;
						return k->MyPair;
					}
					el = el->Right;
				}

				if (CompareFunc(key, el->MyPair.key))
				{
					if (el->Left == nullptr)
					{
						value_type<key_type, Data> * k = new value_type<key_type, Data>(MyPair);
						el->Left = k;
						k->ptrPrev = el;
						return k->MyPair;
					}
					el = el->Left;
				}
				if (key == el->MyPair.key)
				{
					--Num;
					return MyPair;
				}
			}
		}

		map& operator=(map& obj)
		{
			if (obj == *this)
				return *this;
			if (root)
				Delete(root);
			insertArea(obj.begin(), obj.end());
			return *this;
		}
	
		template <class Iterator>
		void insertArea(Iterator begin, Iterator end)
		{
			for (Iterator it = begin; it != end; ++it)
				insert(it.it->MyPair.key, it.it->MyPair.data);
			return;

		}

		bool empty()
		{
			if (root == nullptr)
				return false;
			return true;
		}

		size_t size()
		{
			return Num;
		}

		iterator<key_type, Data, Compare>& find(key_type Key)
		{
			value_type<key_type, Data> * el = root;
			while (el != nullptr)
			{		
				if (el->MyPair.key == Key)
				{
					it = el;
					return it;
				}
				if (CompareFunc(el->MyPair.key, Key))
				{
					if (el->Right == nullptr)
						return end();
					el = el->Right;
					
				}
				if (CompareFunc(Key, el->MyPair.key))
				{
					if (el->Left == nullptr)
						return end();
					el = el->Left;
					
				}		
				
			}
		}

		pair<key_type, Data> erase(key_type key)
		{
			--Num;
			if (root == nullptr)
				throw std::invalid_argument("map is empty");
			if (root->MyPair.key == key)
			{
				pair<key_type, Data> k(root->MyPair);
				if (root->Right != nullptr && root->Left != nullptr)
				{
					root->Left->ptrPrev = getLeft(root->Right);
					getLeft(root->Right)->Left = root->Left;
					root = root->Right;
					return k;
				}
				if (root->Right == nullptr && root->Left == nullptr)
				{
					root = nullptr;
					return k;
				}
				if (root->Right != nullptr)
				{
					root = root->Right;
					return k;
				}
				if (root->Left != nullptr)
				{
					root = root->Left;
					return k;
				}

				return k;
			}
			value_type<key_type, Data> * el = root;
			while (1)
			{
				if (key == el->MyPair.key)
				{
					pair<key_type, Data> k(el->MyPair);
					if (el->Right == nullptr && el->Left == nullptr)
					{
						if (CompareFunc(el->ptrPrev->MyPair.key, el->MyPair.key))
							el->ptrPrev->Right = nullptr;
						if (CompareFunc(el->MyPair.key, el->ptrPrev->MyPair.key))
							el->ptrPrev->Left = nullptr;
						delete el;
						return k;
					}
					if (el->Right != nullptr && el->Left != nullptr)
					{
						if (CompareFunc(el->ptrPrev->MyPair.key, el->MyPair.key))
							el->ptrPrev->Right = el->Right;
						if (CompareFunc(el->MyPair.key, el->ptrPrev->MyPair.key))
							el->ptrPrev->Left = el->Right;

						el->Left->ptrPrev = getLeft(el->Right);
						getLeft(el->Right)->Left = el->Left;
						return k;
						el->Right->ptrPrev = el->ptrPrev;
					}
					if (el->Right != nullptr)
					{
						if (CompareFunc(el->ptrPrev->MyPair.key, el->MyPair.key))
							el->ptrPrev->Right = el->Right;
						if (CompareFunc(el->MyPair.key, el->ptrPrev->MyPair.key))
							el->ptrPrev->Left = el->Right;
						el->Right->ptrPrev = el->ptrPrev;
						delete el;
						return k;
					}
					if (el->Left != nullptr)
					{
						if (CompareFunc(el->ptrPrev->MyPair.key, el->MyPair.key))
							el->ptrPrev->Right = el->Left;
						if (CompareFunc(el->MyPair.key, el->ptrPrev->MyPair.key))
							el->ptrPrev->Left = el->Left;
						el->Left->ptrPrev = el->ptrPrev;
						delete el;
						return k;
					}

				}
				if (CompareFunc(el->MyPair.key, key))
				{
					if (el->Right == nullptr)
						throw std::invalid_argument("NO SUCH A KEY");
					el = el->Right;
				}
				if (CompareFunc(key, el->MyPair.key))
				{
					if (el->Left == nullptr)
						throw std::invalid_argument("NO SUCH A KEY");
					el = el->Left;
				}
			}
		}

		void print()
		{
			print1(root);
		}

		void clear()
		{
			Delete(root);
			root = nullptr;
		}

		size_t count(const key_type Key)
		{
			value_type<key_type, Data> * el = root;
			while (el != nullptr)
			{
				if (el->MyPair.key == Key)
				{
					return 1;
				}
				if (CompareFunc(el->MyPair.key, Key))
				{
					if (el->Right == nullptr)
						return 0;
					el = el->Right;

				}
				if (CompareFunc(Key, el->MyPair.key))
				{
					if (el->Left == nullptr)
						return 0;
					el = el->Left;

				}
			}
		}

		value_type<key_type, Data>* getroot()
		{
			return root;
		}

		iterator<key_type, Data, Compare> begin()
		{
			it = getLeft(root);
			return it;
		}

		iterator<key_type, Data, Compare> end()
		{	
			value_type<key_type, Data> * el = getRight(root);
			it.it = el;
			if (el->flag == true)
				return it;
			el->Right = new value_type<key_type, Data>();
			el->Right->flag = true;
			el->Right->ptrPrev = el;
			it.it = el->Right;
			return it;
		}

		iterator<key_type, Data, Compare> lower_bound(key_type Key)
		{
			iterator<key_type, Data, Compare> it = find(Key);
			if (it == end())
			{
				insert(NULL, Key);
				it = find(Key);
				++it;
				erase(Key);
				return it;
			}
			return it;
		}

		iterator<key_type, Data, Compare> upper_bound(key_type Key)
		{
			iterator<key_type, Data, Compare> it = find(Key);
			if (it == end())
			{
				insert(NULL, Key);
				it = find(Key);
				++it;
				erase(Key);
				return it;
			}
			++it;
			return it;
		}

		

		~map()
		{
			clear();
		}
	private:
		iterator<key_type, Data, Compare> it;
		void Delete(value_type<key_type, Data> * _root)
		{
			if (_root)
			{
				Delete(_root->Left);
				Delete(_root->Right);
				delete _root;
			}
		}
		size_t Num;
		Compare CompareFunc;
		value_type<key_type, Data> * root;
		value_type<key_type, Data> * getLeft(value_type<key_type, Data>* _root)
		{
			value_type<key_type, Data> * el = _root;
			while (el->Left != nullptr)
			{
				el = el->Left;
			}
			return el;
		}
		value_type<key_type, Data> * getRight(value_type<key_type, Data>* _root)
		{
			value_type<key_type, Data> * el = _root;
			while (el->Right != nullptr)
			{
				el = el->Right;
			}
			return el;
		}
		void print1(value_type<key_type, Data> * _root)
		{
			if (_root)
			{
				print1(_root->Left);
				std::cout << _root->MyPair << std::endl;
				print1(_root->Right);
			}
		}
	};

	template<class key_type, class Data>
	bool operator==(map<key_type, Data>& obj1, map<key_type, Data>& obj2)
	{
		if (obj1.size() != obj2.size())
			return false;
		iterator<key_type, Data> it1 = obj2.begin();
		for (iterator<key_type, Data> it = obj1.begin(); it != obj1.end(); ++it)
		{
			if (it.it->MyPair != it1.it->MyPair)
				return false;
			++it1;
		}
		return true;
	}

	template<class key_type, class Data>
	bool operator!=(map<key_type, Data>& obj1, map<key_type, Data>& obj2)
	{
		
		return(!(obj1 == obj2));
	}
	

}






























































































































//////////////////////////////////////////////////