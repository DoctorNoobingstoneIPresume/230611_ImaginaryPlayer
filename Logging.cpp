#include "Logging.hpp"
#include "Worker.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace ImaginaryPlayer
{

std::string       LogContext::GetThreadName  ()                       const { return _sThreadName; }
lyb::string_view  LogContext::GetThreadNameQ ()                       const { return _sThreadName; }
LogContext       &LogContext::SetThreadName  (lyb::string_view value)       { _sThreadName = lyb::ViewToString (value); return *this; }

std::shared_ptr <Worker>  LogContext::GetSPWorker ()                                      const { return _spWorker; }
LogContext               &LogContext::SetSPWorker (const std::shared_ptr <Worker> &value)       { _spWorker = value; return *this; }

LogContext::LogContext ()
= default;

void
ComposeAndLog
(const LogContext &logcontext, const std::function <void (std::ostream &os)> &fnPrepareMessage)
{
	std::ostringstream os;
	{
		const std::string sThreadName {logcontext.GetThreadName ()};
		if (! sThreadName.empty ())
		{
			os << "[" << sThreadName << "] ";
			if (! os)
				throw std::runtime_error {"`ComposeAndLog`: Writing the header has failed !"};
		}
		
		fnPrepareMessage (os);
		if (! os)
			// [2023-07-11] TODO: Should we throw an exception or... ?
			throw std::runtime_error {"`ComposeAndLog`: `fnPrepareMessage (os)` has failed !"};
	}
	
	const std::string s {os.str ()};
	
	const auto fnEmit = [=] ()
	{
		std::cout << s << std::flush;
		return Worker::RV_Normal;
	};
	
	if (const auto spWorker = logcontext.GetSPWorker ())
	{
		spWorker->AddWorkItem (std::make_shared <Worker::WorkItem> (fnEmit));
	}
	else
		fnEmit ();
}

void
ComposeAndLog
(                              const std::function <void (std::ostream &os)> &fnPrepareMessage)
{
	return ComposeAndLog (LogContext {}, fnPrepareMessage);
}

}
