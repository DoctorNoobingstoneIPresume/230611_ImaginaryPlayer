#include "Worker.hpp"
#include "WorkerImpl.hpp"
#include "Player.hpp"
#include "WorkerImpl_Player.hpp"
#include "Logging.hpp"

#include "Lexer.hpp"
#include "Cursor.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>

#include <algorithm>

#include <string>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

int main ()
{
	std::cout << "Hello, Imaginary World !\n" << std::flush;
	const std::locale loc {std::cout.getloc ()};
	const std::ctype <char> &ctype {std::use_facet <std::ctype <char>> (loc)};
	
	using namespace ImaginaryPlayer;
	
	{
		std::ostream *const posPreviouslyTied {std::cin.tie (nullptr)};
		const auto guardPreviouslyTied {MakeScopeGuard ([=] () { std::cin.tie (posPreviouslyTied); })};
		
		const auto Logger_spWorker {std::make_shared <Worker> ()};
		// [2023-06-17] TODO: In the following line, if we replace the inner braces with parentheses, the thread is not run ! Why ?
		const auto Logger_jthread {ScopedWorkerThread {Logger_spWorker}};
		
		const auto t0 {Now ()};
		const auto logcontext     {LogContext {t0}.SetSPWorker (Logger_spWorker).SetThreadName ("Main")};
		const auto logcontextPlay {LogContext {t0}.SetSPWorker (Logger_spWorker).SetThreadName ("Play")};
		
		const auto spPlayer {std::make_shared <Player> (logcontextPlay)};
		const auto Player_spWorker {std::make_shared <Worker> (std::make_unique <WorkerImpl_Player> (spPlayer))};
		const auto Player_jthread {ScopedWorkerThread {Player_spWorker}};
		
		ComposeAndLog (logcontext, [] (std::ostream &os) { os << "Surprise !\n"; });
		
		const auto songExample = Song {}.SetArtistName ("Madonna").SetSongName ("Frozen").SetCodecName ("MP3").SetLength (std::chrono::minutes {5} + std::chrono::seconds {29});
		
		{
			std::this_thread::sleep_until (t0 + Duration {100});
		}
		
		for (;;)
		{
			ComposeAndLog (logcontext, [] (std::ostream &os) { os << ">\n"; });
			std::string Command_sLine;
			{
				if (! std::cin)
					{ ComposeAndLog (logcontext, [] (std::ostream &os) { std::cout << "cin had already failed !\n"; }); return 1; }
				
				if (std::cin.eof ())
					{ ComposeAndLog (logcontext, [] (std::ostream &os) { std::cout << "cin has ended.\n"; }); break; }
				
				if (! std::getline (std::cin, Command_sLine))
					{ ComposeAndLog (logcontext, [] (std::ostream &os) { std::cout << "getline has failed !\n"; }); return 1; }
			}
			
			ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Processing command line: `" << Command_sLine << "`.\n"; });
			
			std::istringstream Command_isLine {Command_sLine};
			Cursor cursor;
			const auto Command_opttoken {ExtractToken (Command_isLine, &cursor)};
			if (! Command_opttoken)
			{
				ComposeAndLog
				(
					logcontext,
					[&] (std::ostream &os) { os << "ExtractToken has failed for input \"" << Command_sLine << "\" ! Please type \"help\" for help.\n"; }
				);
				
				continue;
			}
			const auto &Command_token {*Command_opttoken};
			const std::string Command_sText {Command_token.GetText ()};
			      std::string Command_sTextLo {Command_sText};
			{
				// [2023-07-14] TODO: Transform from UTF-8 to UTF-16 or UTF-32, then switch to lowercase, then transform back.
				std::transform
				(
					Command_sTextLo.begin (), Command_sTextLo.end (),
					Command_sTextLo.begin (),
					[&] (char c) { return ctype.tolower (c); }
				);
			}
			
			std::string Command_sRestOfLine;
			{
				// [2023-07-14] TODO: Consider: We do allow the following call to fail, right ?
				std::getline (Command_isLine, Command_sRestOfLine);
			}
			
			std::istringstream Command_isRestOfLine {Command_sRestOfLine};
			
			const auto tNow {Now ()};
			const auto arg {WorkerImpl::Arg {}};
			ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "arg " << arg << ".\n"; });
			if (Command_sTextLo == "help" || Command_sTextLo == "?")
			{
				std::ostringstream osHelp;
				{
					osHelp
						<<
							"Welcome to the ImaginaryPlayer !\n"
							"\n"
							"Commands (case-insensitive):\n"
							"\n"
							"    Help\n"
							"        Displays this helpful message.\n"
							"\n"
							"    Exit\n"
							"        Enqueues \"exit\" command to worker threads, then exits the main loop too.\n"
							"        That is a nerdy way of saying that we exit the ImaginaryPlayer.\n"
							"\n"
							"    (Show|ShowPlayer)\n"
							"        Displays debugging information about the Player.\n"
							"\n"
							"    ShowSongs\n"
							"        Displays songs.\n"
							"\n"
							"    Verb <level>\n"
							"        Sets the verbosity level for the Player. Default is `0`.\n"
							"\n"
							"    AddSong (<song-description>)\n"
							"        Adds the specified song to the queue.\n"
							"\n"
							"    (Play|Pause)\n"
							"        Toggles the \"playing\" state.\n"
							"\n"
							"    Sleep <number_of_milliseconds>\n"
							"        Puts the main thread to sleep => commands are not going to be processed for the specified duration.\n"
							"        (This command is useful for functional tests.)\n"
							"\n"
							"    (Prev|Next)\n"
							"        Jumps to the previous song or the next song (in history).\n"
							"\n"
							"    Repeat (0|1)\n"
							"        Toggles the \"repeat\" mode.\n"
							"\n"
							"    Random (0|1)\n"
							"        Toggles the \"random\" mode.\n"
							"\n"
							"Example of <song-description>:\n"
							"    " << songExample << "\n"
							"\n"
							"Contact/support: adder_2003@yahoo.com. Thank you soooo !! (-:\n"
							"\n";
				};
				
				ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "\n" << osHelp.str (); });
			}
			else
			if (Command_sTextLo == "exit")
				break;
			else
			if (Command_sTextLo == "showplayer" || Command_sTextLo == "show")
			{
				Player_spWorker->AddWorkItem
				(
					std::make_shared <Worker::WorkItem>
					(
						[=] () { return spPlayer->Show (arg); }
					)
				);
			}
			else
			if (Command_sTextLo == "showsongs")
			{
				Player_spWorker->AddWorkItem
				(
					std::make_shared <Worker::WorkItem>
					(
						[=] () { return spPlayer->ShowSongs (arg); }
					)
				);
			}
			else
			if (Command_sTextLo == "verb")
			{
				if (const auto opti = ExtractIntegral (Command_isRestOfLine))
				{
					ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Verb: New verbosity level " << *opti << ".\n"; });
					Player_spWorker->AddWorkItem
					(
						std::make_shared <Worker::WorkItem>
						(
							[=] () { return spPlayer->Verb (arg, *opti); }
						)
					);
				}
				else
					ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Verb: We have not been able to extract an integral from \"" << Command_sRestOfLine << "\" !\n"; });
			}
			else
			if (Command_sTextLo == "addsong")
			{
				Song song;
				{
					if (! (Command_isRestOfLine >> song))
					{
						ComposeAndLog
						(
							logcontext,
							[&] (std::ostream &os)
							{
								os
									<< "We have not been able to process the `Song` description from \"" << Command_sRestOfLine << "\" !\n"
									<< "Example of an expected `Song` description:\n"
									<< "    " << songExample << "\n";
							}
						);
						
						continue;
					}
				}
				
				ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "We are adding " << song << "...\n"; });
				
				Player_spWorker->AddWorkItem
				(
					std::make_shared <Worker::WorkItem>
					(
						[=] () { return spPlayer->AddSong (arg, song); }
					)
				);
			}
			else
			if (Command_sTextLo == "pause")
			{
				Player_spWorker->AddWorkItem
				(
					std::make_shared <Worker::WorkItem>
					(
						[=] () { return spPlayer->Play (arg, false); }
					)
				);
			}
			else
			if (Command_sTextLo == "play")
			{
				Player_spWorker->AddWorkItem
				(
					std::make_shared <Worker::WorkItem>
					(
						[=] () { return spPlayer->Play (arg, true); }
					)
				);
			}
			else
			if (Command_sTextLo == "sleep")
			{
				if (const auto opti = ExtractIntegral (Command_isRestOfLine))
				{
					ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Sleep: Sleeping for " << std::setw (7) << *opti << " millisecond(s)...\n"; });
					std::this_thread::sleep_until (tNow + std::chrono::milliseconds {*opti});
				}
				else
					ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Sleep: We have not been able to extract an integral from \"" << Command_sRestOfLine << "\" !\n"; });
			}
			else
			if (Command_sTextLo == "repeat")
			{
				if (const auto opti = ExtractIntegral (Command_isRestOfLine))
				{
					const bool bValue (*opti);
					ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Repeat " << bValue << "...\n"; });
					
					Player_spWorker->AddWorkItem
					(
						std::make_shared <Worker::WorkItem>
						(
							[=] () { return spPlayer->Repeat (arg, bValue); }
						)
					);
				}
				else
					ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Repeat: We have not been able to extract an integral from \"" << Command_sRestOfLine << "\" !\n"; });
			}
			else
			if (Command_sTextLo == "random")
			{
				if (const auto opti = ExtractIntegral (Command_isRestOfLine))
				{
					const bool bValue (*opti);
					ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Random " << bValue << "...\n"; });
					
					Player_spWorker->AddWorkItem
					(
						std::make_shared <Worker::WorkItem>
						(
							[=] () { return spPlayer->Random (arg, bValue); }
						)
					);
				}
				else
					ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Random: We have not been able to extract an integral from \"" << Command_sRestOfLine << "\" !\n"; });
			}
			else
			if (Command_sTextLo == "prev")
				Player_spWorker->AddWorkItem (std::make_shared <Worker::WorkItem> ([=] () { return spPlayer->PrevNext (arg, false); }));
			else
			if (Command_sTextLo == "next")
				Player_spWorker->AddWorkItem (std::make_shared <Worker::WorkItem> ([=] () { return spPlayer->PrevNext (arg, true ); }));
			else
				{ ComposeAndLog (logcontext, [&] (std::ostream &os) { os << "Unrecognized command: \"" << Command_sText << "\" !\n"; }); continue; }
		}
	}
	
	std::cout << "Buh-bye, Imaginary World !\n" << std::flush;
}
