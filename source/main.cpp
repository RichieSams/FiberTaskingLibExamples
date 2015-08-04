/* FiberTaskingLib - A tasking library that uses fibers for efficient task switching
*
* This library was created as a proof of concept of the ideas presented by
* Christian Gyrling in his 2015 GDC Talk 'Parallelizing the Naughty Dog Engine Using Fibers'
*
* http://gdcvault.com/play/1022186/Parallelizing-the-Naughty-Dog-Engine
*
* FiberTaskingLib is the legal property of Adrian Astley
* Copyright Adrian Astley 2015
*/

#include "fiber_tasking_lib/task_scheduler.h"
#include "fiber_tasking_lib/global_args.h"

#include <Remotery.h>

#include <memory>


struct NumberSubset {
	uint64 start;
	uint64 end;

	uint64 total;
};


TASK_ENTRY_POINT(AddNumberSubset) {
	rmt_ScopedCPUSample(ParallelAdd);

	volatile int k = 0;
	for (uint j = 0; j < 100000; ++j) {
		k += 1;
	}

	NumberSubset *subset = static_cast<NumberSubset *>(arg);

	subset->total = 0;

	while (subset->start != subset->end) {
		subset->total += subset->start;
		++subset->start;
	}

	subset->total += subset->end;
}


int main(int argc, char *argv) {
	Remotery *rmt;
	rmt_CreateGlobalInstance(&rmt);


	FiberTaskingLib::GlobalArgs *globalArgs = new FiberTaskingLib::GlobalArgs();
	globalArgs->g_taskScheduler.Initialize(25, globalArgs);
	globalArgs->g_allocator.init(&globalArgs->g_heap, 1234);

	// Define the constants to test
	const uint64 triangleNum = 47593243ull;
	const uint64 numAdditionsPerTask = 100000ull;
	const uint64 numTasks = (triangleNum + numAdditionsPerTask - 1ull) / numAdditionsPerTask;

	// Create the tasks
	FiberTaskingLib::Task *tasks = new FiberTaskingLib::Task[numTasks];
	// We have to declare this on the heap so other threads can access it
	NumberSubset *subsets = new NumberSubset[numTasks];
	uint64 nextNumber = 1ull;

	for (uint64 i = 0ull; i < numTasks; ++i) {
		NumberSubset *subset = &subsets[i];

		subset->start = nextNumber;
		subset->end = nextNumber + numAdditionsPerTask - 1ull;
		if (subset->end > triangleNum) {
			subset->end = triangleNum;
		}

		tasks[i] = { AddNumberSubset, subset };

		nextNumber = subset->end + 1;
	}

	while (true) {
		// Schedule the tasks and wait for them to complete
		std::shared_ptr<FiberTaskingLib::AtomicCounter> counter = globalArgs->g_taskScheduler.AddTasks(numTasks, tasks);
		globalArgs->g_taskScheduler.WaitForCounter(counter, 0);


		rmt_BeginCPUSample(SerialAdd);

		// Add the results
		uint64 result = 0ull;
		for (uint64 i = 0; i < numTasks; ++i) {
			result += subsets[i].total;
		}

		rmt_EndCPUSample(SerialAdd);
	}


	// Cleanup
	globalArgs->g_taskScheduler.Quit();
	globalArgs->g_allocator.destroy();
	delete globalArgs;
	delete[] subsets;
}
