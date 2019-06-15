template <class Element_t>
class autoList
{
private:
	int Count;
	int ListSize;
	Element_t *List;

public:
	autoList()
	{
		this->Count = 0;
		this->ListSize = 0;
		this->List = (Element_t *)memAlloc(0);
	}
	autoList(const autoList &source)
	{
		error();
	}
	~autoList()
	{
		memFree(this->List);
	}

	Element_t *UnbindBuffer()
	{
		Element_t *list_ret = this->List;

		this->Count = 0;
		this->ListSize = 0;
		this->List = (Element_t *)memAlloc(0);

		return list_ret;
	}

	int GetCount()
	{
		return this->Count;
	}

	Element_t GetElement(int index)
	{
		errorCase(index < 0 || this->Count <= index);

		return this->List[index];
	}
	Element_t *ElementAt(int index)
	{
		if(index == 0)
			return this->List;

		errorCase(index < 0 || this->Count <= index);

		return this->List + index;
	}

	void AddElement(Element_t element)
	{
		if(this->ListSize <= this->Count)
		{
			errorCase((INT_MAX / sizeof(Element_t)) / 2 < this->ListSize);

			if(this->ListSize < 16)
				this->ListSize += 2;
			else
				this->ListSize *= 2;

			this->List = (Element_t *)memRealloc(this->List, this->ListSize * sizeof(Element_t));
		}
		this->List[this->Count] = element;
		this->Count++;
	}
	Element_t UnaddElement()
	{
		errorCase(this->Count < 1);

		this->Count--;
		return this->List[this->Count];
	}
	Element_t FastDesertElement(int index)
	{
		errorCase(index < 0 || this->Count <= index);

		Element_t element = this->List[index];

		this->Count--;
		this->List[index] = this->List[this->Count];

		return element;
	}

	void AddElements(Element_t *list, int count)
	{
		for(int index = 0; index < count; index++)
		{
			this->AddElement(list[index]);
		}
	}
	void AddElements(autoList<Element_t> *list)
	{
		this->AddElements(list->ElementAt(0), list->GetCount());
	}
	void AddElements_x(autoList<Element_t> *list)
	{
		this->AddElements(list);
		delete list;
	}

	void Swap(int index1, int index2)
	{
		errorCase(index1 < 0 || this->Count <= index1);
		errorCase(index2 < 0 || this->Count <= index2);

		Element_t tmp = this->List[index1];

		this->List[index1] = this->List[index2];
		this->List[index2] = tmp;
	}
	void Shuffle_7()
	{
		if(7 <= this->Count)
		{
			for(int index = 0; index < 7; index++)
			{
				this->Swap(index, getCryptoRand() % this->Count);
			}
		}
		else
			this->Shuffle();
	}
	void Shuffle()
	{
		for(int range = this->Count; 2 <= range; range--)
		{
			this->Swap(range - 1, getCryptoRand() % range);
		}
	}

	void GnomeSort(int (*compFunc)(Element_t, Element_t), int start, int count)
	{
		for(int index = 1; index < count; index++)
		{
			for(int nearPos = index - 1; 0 <= nearPos; nearPos--)
			{
				if(compFunc(this->List[start + nearPos], this->List[start + nearPos + 1]) <= 0)
					break;

				this->Swap(start + nearPos, start + nearPos + 1);
			}
		}
	}
	void Sort(int (*compFunc)(Element_t, Element_t), int start, int count)
	{
		int span = count;

		for(; ; )
		{
			span *= 10;
			span /= 13;

			if(span < 2)
				break;

			if(span == 9 || span == 10)
				span = 11;

			for(int index = 0; index + span < count; index++)
				if(0 < compFunc(this->List[start + index], this->List[start + index + span]))
					this->Swap(start + index, start + index + span);
		}
		this->GnomeSort(compFunc, start, count);
	}
	void Sort(int (*compFunc)(Element_t, Element_t))
	{
		this->Sort(compFunc, 0, this->Count);

#if 1 // test
		{
			for(int c = 1; c < this->Count; c++)
			{
				errorCase(0 < compFunc(this->List[c - 1], this->List[c])); // ? [c - 1] > [c]
			}
		}
#endif
	}

	int BinSearch(int (*compFunc)(Element_t, Element_t), Element_t e)
	{
		int p = 0;
		int q = this->Count;

		while(p < q)
		{
			int mid = (p + q) / 2;
			int comp = compFunc(this->List[mid], e);

			if(comp < 0)
			{
				p = mid + 1;
			}
			else if(0 < comp)
			{
				q = mid;
			}
			else
			{
				return mid;
			}
		}
		return -1; // not found
	}
};
