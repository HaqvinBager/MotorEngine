#pragma once
#include <cassert>
#include <vector>

namespace CommonUtilities {

#pragma region MAX_and_MIN
	//#ifndef MAX( a , b )								
	#define MAX( a , b ) ((a) > (b) ?  (a) : (b) )

	//#endif // MAX

	//#ifndef MIN( a , b );
	#define MIN( a , b ) ((a) < (b) ?  (a) : (b) )

	//#endif // MIN
#pragma endregion // !MAX_and_MIN

#pragma region SAFE_DELETE
#define SAFE_DELETE(pointer)		\
	delete pointer;					\
	pointer = nullptr;

#define SAFE_DELETE_ARRAY(pointer)	\
	delete[] pointer;				\
	pointer = nullptr;

#pragma endregion // !SAFE_DELETE

#pragma region CYCLIC_ERASE
	//#ifndef CYCLIC_ERASE( vector , index );			
	#if _DEBUG
	#define CYCLIC_ERASE( vector , index)				\
				assert((index) >= 0);                   \
				assert((index) < vector.size());       \
				vector.at((index)) = vector.back();     \
				vector.pop_back();					
	#else										    
	#define CYCLIC_ERASE( vector , index );				\
				vector.at((index)) = vector.back();     \
				vector.pop_back();					
	#endif
	//#endif

	//#ifndef CYCLIC_ERASE_SORTED( vector , index );
	#if _DEBUG
	#define CYCLIC_ERASE_SORTED( vector , index )									\
				assert((index) >= 0);                                               \
				assert((index) < vector.size());                                   \
				for (int i = index; i < vector.size(); ++i) {                       \
					if ((i + 1) != vector.size()) {                                 \
						vector.at(i) = vector.at(i + 1);                            \
					} else {                                                        \
						vector.pop_back();                                          \
						break;                                                      \
					}                                                               \
				}
	#else                                                                       
	#define CYCLIC_ERASE_SORTED( vector , index )									\
				for (int i = index; i < vector.size(); ++i) {                       \
					if ((i + 1) != vector.size()) {                                 \
						vector.at(i) = vector.at(i + 1);                            \
					} else {                                                        \
						vector.pop_back();                                          \
						break;                                                      \
					}                                                               \
				}
	#endif
	//#endif

#pragma endregion // !CYCLIC_ERASE
}