#pragma once
#include <vector>

namespace CommonUtilities {

	template <class T, class compare = std::greater<T>>
	class CHeap {
	public:
		size_t Size() const; //returnerar antal element i heapen
		void Enqueue(const T& anElement); //lägger till elementet i heapen
		const T& GetTop() const; //returnerar det största elementet i heapen
		T Dequeue(); //tar bort det största elementet ur heapen och returnerar det
		bool InHeap(T& anElement) const;
		void Resort();

	private:
		void MoveUp(std::vector<T>& someData, size_t aChild);
		void MoveDown(std::vector<T>& someData, size_t aFirst, size_t aLast);

		compare myComparisonFunction;
		std::vector<T> myData;
	};

	template <class T, class compare>
	size_t CHeap<T, compare>::Size() const {
		return myData.size();
	}

	template <class T, class compare>
	void CHeap<T, compare>::Enqueue(const T& anElement) {
		myData.emplace_back(anElement);
		MoveUp(myData, myData.size() - 1);
	}

	template <class T, class compare>
	const T& CHeap<T, compare>::GetTop() const {
		return myData[0];
	}

	template <class T, class compare>
	T CHeap<T, compare>::Dequeue() {
		T largest = myData[0];
		myData[0] = myData[myData.size() - 1];
		myData.pop_back();
		MoveDown(myData, 0, myData.size() - 1);
		return largest;
	}

	template <class T, class compare>
	bool CHeap<T, compare>::InHeap(T& anElement) const {
		auto it = std::find(myData.begin(), myData.end(), anElement);
		return it != myData.end();
	}

	template <class T, class compare>
	void CHeap<T, compare>::MoveUp(std::vector<T>& someData, size_t aChild) {
		size_t parent = (aChild - 1) / 2; // Parent of a node has index n-1 / 2
		while (aChild != 0 && myComparisonFunction(someData[aChild], someData[parent])) { // If child value is greater than parent, swap
			std::swap(someData[aChild], someData[parent]);
			aChild = parent;   // Move up
			parent = (aChild - 1) / 2;
		}
	}

	template <class T, class compare>
	void CHeap<T, compare>::MoveDown(std::vector<T>& someData, size_t aFirst, size_t aLast) {
		size_t largestChild = 2 * aFirst + 1; // Children of a node have indeces 2*n + 1 and 2*n + 2
		while (largestChild <= aLast) {
			if (largestChild < aLast && myComparisonFunction(someData[largestChild + 1], someData[largestChild])) {
				largestChild++;           // If left child not last element and value is smaller than right child, choose right child
			}

			if (myComparisonFunction(someData[largestChild], someData[aFirst])) {    // If node value is smaller than largest child value, swap      
				std::swap(someData[aFirst], someData[largestChild]);
				aFirst = largestChild;                  // Move down
				largestChild = 2 * aFirst + 1;
			} else largestChild = aLast + 1;            // Exit loop                  
		}
	}

	template <class T, class compare>
	void CHeap<T, compare>::Resort() {
		for (int i = 0u; i < myData.size(); ++i) {
			MoveUp(myData, i);
		}
	}
}
