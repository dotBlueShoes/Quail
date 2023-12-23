#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"
#include "Types.hpp"

namespace IO::Projects::Parse {

	//ptr<uint8> projectsBuffor = ptr<uint8>(memoryBlockA, sizeof(size) /* offset */ );


	block DisplayFiles() {
		const uint8& filesCount = memoryBlockA.data[0];
		uint8& nameCount = memoryBlockA.data[1];
		uint8& contextCount = memoryBlockA.data[2];

		printf("FC: %" PRIu8 "\n", filesCount);
		printf("NC: %" PRIu8 "\n", nameCount);
		printf("CC: %" PRIu8 "\n", contextCount);

		const uint8 SPACE_SIZE_CONTEXT = 1;
		const uint8 SPACE_SIZE_NAME = 1;

		uint8 nextFileIndex = 1;

		// TODO:
		//  For now only for the fisrt command file is being saved in buffor properly.
		//  Inside Stages.cpp make it safe on different indexes so that nameCount, contextCount would work.
		
		//for (uint8 i = 0; i < filesCount; ++i) {
			nextFileIndex += nameCount + contextCount;
			nameCount = memoryBlockA.data[nextFileIndex];
			nextFileIndex += SPACE_SIZE_NAME;
			contextCount = memoryBlockA.data[nextFileIndex];
			nextFileIndex += SPACE_SIZE_CONTEXT;
		//}

		printf("NC: %" PRIu8 "\n", nameCount);
		printf("CC: %" PRIu8 "\n", contextCount);
	}

}