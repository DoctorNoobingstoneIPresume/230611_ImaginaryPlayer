#include "Worker.hpp"
#include "WorkerImpl.hpp"

#include <iostream>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

int main ()
{
	std::cout << "Hello, Imaginary World !\n" << std::flush;
	
	using namespace ImaginaryPlayer;
	
	const auto Logger_spWorker {std::make_shared <Worker> ()};
	// [2023-06-17] TODO: In the following line, if we replace braces with parentheses, the thread is not run ! Why ?
	const auto Logger_jthread (ScopedWorkerThread {Logger_spWorker});
	
	std::cout << "Buh-bye, Imaginary World !\n" << std::flush;
}
