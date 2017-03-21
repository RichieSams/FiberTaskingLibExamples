/* FiberTaskingLibExamples
 *
 * This is a set of examples to accompany the FiberTaskingLib
 * https://github.com/RichieSams/FiberTaskingLib
 *
 * FiberTaskingLibExamples is the legal property of Adrian Astley
 * Copyright Adrian Astley 2016
 */

#include "fiber_tasking_lib/task_scheduler.h"


struct NumberSubset {
	uint64 start;
	uint64 end;

	uint64 total;
};


FTL_TASK_ENTRY_POINT(AddNumberSubset) {
	NumberSubset *subset = reinterpret_cast<NumberSubset *>(arg);

	subset->total = 0;

	while (subset->start != subset->end) {
		subset->total += subset->start;
		++subset->start;
	}

	subset->total += subset->end;
}


/**
 * Calculates the value of a triangle number by dividing the additions up into tasks
 *
 * A triangle number is defined as:
 *         Tn = 1 + 2 + 3 + ... + n
 *
 * The code is checked against the numerical solution which is:
 *         Tn = n * (n + 1) / 2
 */
FTL_TASK_ENTRY_POINT(MainTask) {
	// Define the constants to test
	const uint64 triangleNum = 47593243ull;
	const uint64 numAdditionsPerTask = 10000ull;
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

		tasks[i] = {AddNumberSubset, subset};

		nextNumber = subset->end + 1;
	}

	// Schedule the tasks and wait for them to complete
	std::shared_ptr<std::atomic_uint> counter = taskScheduler->AddTasks(numTasks, tasks);
	delete[] tasks;

	taskScheduler->WaitForCounter(counter, 0);


	// Add the results
	uint64 result = 0ull;
	for (uint64 i = 0; i < numTasks; ++i) {
		result += subsets[i].total;
	}

	// Test
	assert(triangleNum * (triangleNum + 1ull) / 2ull == result);

	// Cleanup
	delete[] subsets;
}

int main(int argc, char *argv) {
	FiberTaskingLib::TaskScheduler taskScheduler;
	taskScheduler.Run(25, MainTask);

	return 0;
}
