#pragma once
#include <initializer_list>
#include <algorithm>
#include <cassert>

template<typename T>
class Vector {
public:
	Vector(int aCapacity = 1);
	Vector(const std::initializer_list<T>& aInitList);
	Vector(const Vector& aOther);
	Vector(const Vector&& aOther);
	Vector& operator=(Vector aOther);
	~Vector();

	T& At(int aIndex);
	T& operator[](int aIndex);
	const T& At(int aIndex) const;
	const T& operator[](int aIndex) const;

	T& Back();
	T& Front();
	const T& Back() const;
	const T& Front() const;

	T* Data();
	const T* Data() const;

	void Clear();
	void Fill(const T& aValue);
	void Swap(Vector& aOther);
	void Insert(const T& aValue, int aIndex);
	void Push(const T& aValue);
	void Pop();
	void RemoveCyclic(int aIndex);
	void Remove(int aIndex);

	void Reserve(int aCapacity);
	void Resize(int aCapacity);
	void Optimize();

	int Size() const;
	int Capacity() const;
	bool IsEmpty() const;
	bool IsFull() const;

private:
	T* myData;
	int mySize;
	int myCapacity;
};
template<typename T>
inline Vector<T>::Vector(int aCapacity) 
	: myData(nullptr), mySize(0), myCapacity(0) {
	assert(aCapacity > 0 && "Can't have a vector with no capacity!");
	Resize(aCapacity);
}

template<typename T>
inline Vector<T>::Vector(const std::initializer_list<T>& aInitList)
	: myData(nullptr), mySize(0), myCapacity(0) {
	static_assert(aInitList.size() > 0, "Can't have a vector with no capacity!");
	Resize(aInitList.size());
	for (auto& o : aInitList)
	{
		Push(o);
	}
}

template<typename T>
inline Vector<T>::Vector(const Vector& aOther) 
	: myData(nullptr), mySize(0), myCapacity(0) {
	Resize(aOther.myCapacity);
	for (int i = 0; i < aOther.mySize; i++)
	{
		Push(aOther.myData[i]);
	}
}

template<typename T>
inline Vector<T>::Vector(const Vector&& aOther) 
	: myData(nullptr), mySize(0), myCapacity(0) {
	myData = aOther.myData;
	mySize = aOther.mySize;
	myCapacity = aOther.myCapacity;

	aOther.myData = nullptr;
	aOther.mySize = 0;
	aOther.myCapacity = 0;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(Vector aOther) {
	Swap(aOther);
	return *this;
}

template<typename T>
inline Vector<T>::~Vector() {
	delete[] myData;
}

template<typename T>
inline T& Vector<T>::At(int aIndex) {
	assert(0 <= aIndex && aIndex < mySize && ("Accessing element %d when vector has %d elements!", aIndex, mySize));
	return myData[aIndex];
}
template<typename T>
inline T& Vector<T>::operator[](int aIndex) {
	assert(0 <= aIndex && aIndex < mySize && ("Accessing element %d when vector has %d elements!", aIndex, mySize));
	return myData[aIndex];
}
template<typename T>
inline const T& Vector<T>::At(int aIndex) const {
	assert(0 <= aIndex && aIndex < mySize && ("Accessing element %d when vector has %d elements!", aIndex, mySize));
	return myData[aIndex];
}
template<typename T>
inline const T& Vector<T>::operator[](int aIndex) const {
	assert(0 <= aIndex && aIndex < mySize && ("Accessing element %d when vector has %d elements!", aIndex, mySize));
	return myData[aIndex];
}
template<typename T>
inline T& Vector<T>::Back() {
	assert(mySize > 0 && ("Accessing element of empty vector!"));
	return myData[mySize - 1];
}
template<typename T>
inline T& Vector<T>::Front() {
	assert(mySize > 0 && ("Accessing element of empty vector!"));
	return myData[0];
}
template<typename T>
inline const T& Vector<T>::Back() const {
	assert(mySize > 0 && ("Accessing element of empty vector!"));
	return myData[mySize - 1];
}
template<typename T>
inline const T& Vector<T>::Front() const {
	assert(mySize > 0 && ("Accessing element of empty vector!"));
	return myData[0];
}
template<typename T>
inline T* Vector<T>::Data() {
	return myData;
}
template<typename T>
inline const T* Vector<T>::Data() const {
	return myData;
}
template<typename T>
inline void Vector<T>::Clear() {
	mySize = 0;
}
template<typename T>
inline void Vector<T>::Fill(const T& aValue) {	
	for (int i = 0; i < myCapacity; ++i)
	{
		myData[i] = aValue;
	}
}
template<typename T>
inline void Vector<T>::Swap(Vector& aOther) {
	std::swap(myData, aOther.myData);
	std::swap(mySize, aOther.mySize);
	std::swap(myCapacity, aOther.myCapacity);
}
template<typename T>
inline void Vector<T>::Insert(const T& aValue, int aIndex) {
	if (IsFull()) { Reserve(myCapacity * 2); }
	for (int i = 0; i < mySize - aIndex; ++i) {
		myData[aIndex + i + 1] = myData[aIndex + i];
	}
	myData[aIndex] = aValue;
	mySize++;
}
template<typename T>
inline void Vector<T>::Push(const T& aValue) {
	if (IsFull()) { 
		Reserve(myCapacity * 2); 
	}
	myData[mySize] = aValue;
	mySize++;
}
template<typename T>
inline void Vector<T>::Pop() {
	assert(mySize > 0 && ("Can't pop an empty vector!"));
	mySize--;
}
template<typename T>
inline void Vector<T>::RemoveCyclic(int aIndex) {
	assert(0 <= aIndex && aIndex < mySize && ("Removing element %d when vector has %d elements!", aIndex, mySize));
	myData[aIndex] = myData[--mySize];
}
template<typename T>
inline void Vector<T>::Remove(int aIndex) {
	assert(0 <= aIndex && aIndex < mySize && ("Removing element %d when vector has %d elements!", aIndex, mySize));
	for (int i = 0; i < mySize - aIndex - 1; ++i) {
		myData[aIndex + i] = myData[aIndex + i + 1];
	}
	mySize--;
}
template<typename T>
inline void Vector<T>::Reserve(int aCapacity) {
	assert(aCapacity > myCapacity && ("Can't reserve less memory! Use Resize instead!"));

	T* data = new T[aCapacity];
	for (int i = 0; i < mySize; ++i) {
		data[i] = std::move(myData[i]);
	}

	delete[] myData;
	myData = data;
	myCapacity = aCapacity;
}
template<typename T>
inline void Vector<T>::Resize(int aCapacity) {
	assert(aCapacity > 0 && ("Can't have a vector with no capacity!"));
	T* data = new T[aCapacity];
	int size = std::min(aCapacity, mySize);
	for (int i = 0; i < size; ++i) {
		data[i] = std::move(myData[i]);
	}
	delete[] myData;
	myData = data;
	mySize = size;
	myCapacity = aCapacity;
}
template<typename T>
inline void Vector<T>::Optimize() {
	Resize(mySize);
}
template<typename T>
inline int Vector<T>::Size() const {
	return mySize;
}
template<typename T>
inline int Vector<T>::Capacity() const {
	return myCapacity;
}
template<typename T>
inline bool Vector<T>::IsEmpty() const {
	return mySize == 0;
}
template<typename T>
inline bool Vector<T>::IsFull() const {
	return mySize == myCapacity;
}
