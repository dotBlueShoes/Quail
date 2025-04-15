// Created 2025.04.16 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once

#define TOSTRING(x) #x

#define COMPILERLOGINFO(msg) _Pragma(TOSTRING(message("INFO: " msg)))
#define COMPILERLOGWARN(msg) _Pragma(TOSTRING(message("WARN: " msg)))
#define COMPILERLOGERRO(msg) _Pragma(TOSTRING(message("ERRO: " msg)))
