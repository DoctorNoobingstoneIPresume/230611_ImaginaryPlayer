#include "Worker.hpp"
#include "WorkerImpl.hpp"

#include <iostream>
#include <string>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

int main ()
{
	std::cout << "Hello, Imaginary World !\n" << std::flush;
	
	using namespace ImaginaryPlayer;
	
	{
		std::ostream *const posPreviouslyTied {std::cin.tie (nullptr)};
		const auto guardPreviouslyTied {MakeScopeGuard ([=] () { std::cin.tie (posPreviouslyTied); })};
		
		const auto Logger_spWorker {std::make_shared <Worker> ()};
		// [2023-06-17] TODO: In the following line, if we replace braces with parentheses, the thread is not run ! Why ?
		const auto Logger_jthread (ScopedWorkerThread {Logger_spWorker});
		
		Logger_spWorker->AddWorkItem (std::make_shared <Worker::WorkItem> ([] () { std::cout << "Surprise !\n"; return Worker::WorkItemRV (0); }));
		
		for (;;)
		{
			Logger_spWorker->AddWorkItem (std::make_shared <Worker::WorkItem> ([] () { std::cout << "> " << std::flush; return Worker::WorkItemRV (0); }));
			std::string sLine;
			std::getline (std::cin, sLine);
			
			if (sLine == "exit")
				break;
		}
	}
	
	std::cout << "Buh-bye, Imaginary World !\n" << std::flush;
}
