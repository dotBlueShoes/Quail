#pragma once

// ISSUE. POLISH
// Uwzględnienie elementu SDKDDKVer.h powoduje zdefiniowanie najnowszej dostępnej platformy systemu Windows.
// Jeśli chcesz skompilować aplikację dla poprzedniej platformy systemu Windows, uwzględnij element WinSDKVer.h i
// ustaw macro _WIN32_WINNT na platformę, która ma być obsługiwana, przed uwzględnieniem elementu SDKDDKVer.h.
#include <SDKDDKVer.h>
