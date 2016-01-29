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

#include <Remotery.h>

#include <thread>


struct NumberSubset {
	uint64 start;
	uint64 end;

	uint64 total;
};


TASK_ENTRY_POINT(AddNumberSubset) {
	NumberSubset *subset = (NumberSubset *)arg;

	rmt_BeginCPUSample(SUM);

	subset->total = 0;

	while (subset->start != subset->end) {
		subset->total += subset->start;
		++subset->start;
	}

	subset->total += subset->end;

	rmt_EndCPUSample();
}



const uint64 runs = 1024ull * 1024ull;
const uint64 triangleNum = 10ull * 1024ull * 1024ull;
const uint64 numAdditionsPerTask = 100000ull;
const uint64 numTasks = (triangleNum + numAdditionsPerTask - 1ull) / numAdditionsPerTask;


int main(int argc, char *argv) {
	Remotery *rmt;
	rmt_CreateGlobalInstance(&rmt);


	FiberTaskingLib::TaskScheduler *taskScheduler = new FiberTaskingLib::TaskScheduler();
	taskScheduler->Initialize(110);

	for (uint run = 0; run < runs; ++run) {
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

			tasks[i] = {AddNumberSubset, subset};

			nextNumber = subset->end + 1;
		}

		// Schedule the tasks and wait for them to complete
		std::shared_ptr<FiberTaskingLib::AtomicCounter> counter = taskScheduler->AddTasks(numTasks, tasks);
		delete[] tasks;

		taskScheduler->WaitForCounter(counter, 0);

		// Add the results
		uint64 result = 0ull;
		for (uint64 i = 0; i < numTasks; ++i) {
			result += subsets[i].total;
		}

		delete[] subsets;

		// Sleep
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}


	// Cleanup
	taskScheduler->Quit();
	rmt_DestroyGlobalInstance(rmt);

	delete taskScheduler;

	return 0;
}
