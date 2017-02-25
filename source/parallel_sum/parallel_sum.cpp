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

	std::atomic_uint64_t *total;
};

FTL_TASK_ENTRY_POINT(AddNumberSubset) {
	NumberSubset *subset = (NumberSubset *)arg;

	uint64 localTotal = 0;
	for (uint64 i = subset->start; i <= subset->end; ++i) {
		localTotal += i;
	}

	subset->total += localTotal;
}

FTL_TASK_ENTRY_POINT(MainTask) {
	const uint64 triangleNum = 10240ull;
	const uint64 numAdditionsPerTask = 100ull;
	const uint64 numTasks = (triangleNum + numAdditionsPerTask - 1ull) / numAdditionsPerTask;

	// Create the tasks
	FiberTaskingLib::Task tasks[numTasks];
	NumberSubset subsets[numTasks];
	std::atomic_uint64_t total;

	for (uint64 i = 0ull; i < numTasks; ++i) {
		NumberSubset *subset = &subsets[i];

		subset->start = i * numTasks + 1;
		subset->end = i * numTasks + numTasks;
		subset->total = &total;
		tasks[i] = { AddNumberSubset, subset };
	}

	// Schedule the tasks and wait for them to complete
	std::shared_ptr<std::atomic_uint> counter = taskScheduler->AddTasks(numTasks, tasks);
	taskScheduler->WaitForCounter(counter, 0);

	// Verify the result
	assert(triangleNum * (triangleNum + 1ull) / 2ull, result);
}

int main(int argc, char *argv) {
	FiberTaskingLib::TaskScheduler taskScheduler;
	taskScheduler.Run(25, MainTask);

	return 0;
}
