#include "NullDisplay.h"

NullDisplay::NullDisplay() : Display(nullptr){}

void NullDisplay::update(int reason, const char* msg){}