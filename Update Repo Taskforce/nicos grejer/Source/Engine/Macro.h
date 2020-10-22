#pragma once
#define MIN(a, b)										\
	((a)<(b) ? (a) : (b))

#define MAX(a, b)										\
	((a)>(b) ? (a) : (b))

#define SAFE_DELETE(a)									\
	delete (a); (a)=nullptr

#define SAFE_DELETE_ARRAY(a)							\
	delete[] (a); (a)=nullptr

#define CYCLIC_ERASE(vector, a)							\
assert((a)>= 0;											\
assert((a) < vector.size());							\
if((a) >= 0 && (a) < vector.size()){					\
	vector[(a)] = vector[vector.size() - 1];			\
	vector.pop_back();									\
}