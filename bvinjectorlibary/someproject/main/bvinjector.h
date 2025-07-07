#include "pch.h"
#pragma once

// Call this macro in your main() to start the injector
#define bvinjectorstart() bvinjector::start_injector_menu()

namespace bvinjector {
    void start_injector_menu();
} 