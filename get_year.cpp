#include <windows.h>

extern "C" _declspec(dllexport) WORD get_year()
{
	SYSTEMTIME st; // структура, которая хранит информацию о текущем времени
    
    GetSystemTime(&st); // функция запрашивает текущую дату
	
	return st.wYear;
}