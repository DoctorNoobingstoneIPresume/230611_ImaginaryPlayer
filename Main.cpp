#include "Worker.hpp"
#include "WorkerImpl.hpp"
#include "WorkerImpl_Player.hpp"

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
		// [2023-06-17] TODO: In the following line, if we replace the inner braces with parentheses, the thread is not run ! Why ?
		const auto Logger_jthread {ScopedWorkerThread {Logger_spWorker}};
		
		const auto Player_spWorker {std::make_shared <Worker> (std::make_unique <WorkerImpl_Player> (Logger_spWorker))};
		const auto Player_jthread {ScopedWorkerThread {Player_spWorker}};
		
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
