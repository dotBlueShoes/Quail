// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "types.hpp"

namespace COMPARESEARCH {

	void ArrayPartFirstMatch (
		IN		const void* const	compered,
		IN		const u16&			comperedCount,
		IN		const u8&			comperedSize,
		OUT		u32&				index,
		IN		const u32&			elementsCount,
		IN		const void* const	elements
	) {
		auto& iElement = index;
		u8 collision = 1;

		for (; iElement < (elementsCount * collision); ++iElement) {

			for (u8 iInner = 0; iInner < comperedCount * comperedSize; ++iInner) {
				const auto& array = ((u8**)elements)[iElement];
				const u8& byte =  array[iInner * comperedSize];

				collision += ((u8*)compered)[iInner * comperedSize] == byte;
			}
				
			collision = ((collision - 1) != comperedCount);
		}

    	--iElement;
	}

	void ArrayPartFirstMatchVector (
		IN		const void* const&			compered,
		IN		const u16&					comperedCount,
		IN		const u8&					comperedSize,
		OUT		u32&						index,
		IN		const u32&					elementsCount,
		IN		const void* const* const&	elements
	) {
		auto& iElement = index;
		u8 collision = 1;

		for (; iElement < (elementsCount * collision); ++iElement) {

			for (u8 iInner = 0; iInner < comperedCount * comperedSize; ++iInner) {

				const auto& array = (u8*) (elements[iElement]);
				const u8& byte = array[iInner * comperedSize];

				collision += ((u8*)compered)[iInner * comperedSize] == byte;
			}

			collision = ((collision - 1) != comperedCount);
		}

    	--iElement;
		index += collision;
	}

	void ArrayPartFirstMatchVectorLowCase (
		IN		const void* const&			compered,
		IN		const u16&					comperedCount,
		IN		const u8&					comperedSize,
		OUT		u32&						index,
		IN		const u32&					elementsCount,
		IN		const void* const* const&	elements
	) {
		auto& iElement = index;
		u8 collision = 1;

		for (; iElement < (elementsCount * collision); ++iElement) {

			for (u8 iInner = 0; iInner < comperedCount * comperedSize; ++iInner) {

				const auto& array = (u8*) (elements[iElement]);
				const u8& byte = array[iInner * comperedSize];
				const u8 lowCaseByte = tolower (byte);

				collision += ((u8*)compered)[iInner * comperedSize] == lowCaseByte;
			}

			collision = ((collision - 1) != comperedCount);
		}

    	--iElement;
		index += collision;
	}

}
