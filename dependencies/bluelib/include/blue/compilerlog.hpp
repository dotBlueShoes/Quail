// Created 2025.04.16 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once

#define TOSTRING(x) #x

#define COMPILERINFO(msg)  _Pragma(TOSTRING(message("COMPILER-INFO: " msg)))
#define COMPILERWARN(msg)  _Pragma(TOSTRING(message("COMPILER-WARN: " msg)))
#define COMPILERERROR(msg) _Pragma(TOSTRING(message("COMPILER-ERRO: " msg)))
