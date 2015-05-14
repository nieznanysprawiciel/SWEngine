/**
@file macros_switches.cpp
@brief Plik zawiera definicje sta�ych zadeklarowanych w pliku nag��wkowym ze s�owem extern.*/

#include "macros_switches.h"


const unsigned int FRAMES_PER_SEC_UPDATE = 10;			///<Co tyle sekund aktualizujemy frames_per_sec.

#if defined(INDEX_BUFFER_UINT16)
	const unsigned int VERTICIES_MAX_COUNT = 0xFFFF;
#elif defined(INDEX_BUFFER_UINT32)
	const unsigned int VERTICIES_MAX_COUNT = 0xFFFFFFFF;
#else
	const unsigned int VERTICIES_MAX_COUNT = 0xFFFFFFFF;
#endif

