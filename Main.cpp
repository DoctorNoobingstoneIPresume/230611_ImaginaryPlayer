#include "Worker.hpp"
#include "WorkerImpl.hpp"
#include "Player.hpp"
#include "WorkerImpl_Player.hpp"

#include "Lexer.hpp"
#include "Cursor.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
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
		
		const auto spPlayer {std::make_shared <Player> (Logger_spWorker)};
		
		const auto Player_spWorker {std::make_shared <Worker> (std::make_unique <WorkerImpl_Player> (spPlayer))};
		const auto Player_jthread {ScopedWorkerThread {Player_spWorker}};
		
		Logger_spWorker->AddWorkItem (std::make_shared <Worker::WorkItem> ([] () { std::cout << "Surprise !\n"; return Worker::RV_Normal; }));
		
		for (;;)
		{
			Logger_spWorker->AddWorkItem (std::make_shared <Worker::WorkItem> ([] () { std::cout << "> " << std::flush; return Worker::RV_Normal; }));
			std::string sLine;
			{
				if (! std::cin)
					{ std::cout << "cin had already failed !\n"; return 1; }
				
				if (std::cin.eof ())
					{ std::cout << "cin has ended.\n"; break; }
				
				if (! std::getline (std::cin, sLine))
					{ std::cout << "getline has failed !\n"; return 1; }
			}
			
			std::istringstream isLine {sLine};
			Cursor cursor;
			const auto Command_opttoken {ExtractToken (isLine, &cursor)};
			if (! Command_opttoken)
				{ std::cout << "ExtractToken has failed !\n"; continue; }
			const auto &Command_token {*Command_opttoken};
			const std::string Command_sText {Command_token.GetText ()};
			
			if (Command_sText == "exit")
				break;
			else
				{ std::cout << "Unrecognized command: \"" << Command_sText << "\" !\n"; continue; }
		}
	}
	
	std::cout << "Buh-bye, Imaginary World !\n" << std::flush;
}
