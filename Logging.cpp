#include "Logging.hpp"
#include "Worker.hpp"

#include <locale>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace ImaginaryPlayer
{

TimePoint   LogContext::StartOfTime ()                const { return _t0; }
LogContext &LogContext::StartOfTime (TimePoint value)       { _t0 = value; return *this; }

std::string       LogContext::GetThreadName  ()                       const { return _sThreadName; }
lyb::string_view  LogContext::GetThreadNameQ ()                       const { return _sThreadName; }
LogContext       &LogContext::SetThreadName  (lyb::string_view value)       { _sThreadName = lyb::ViewToString (value); return *this; }

std::shared_ptr <Worker>  LogContext::GetSPWorker ()                                      const { return _spWorker; }
LogContext               &LogContext::SetSPWorker (const std::shared_ptr <Worker> &value)       { _spWorker = value; return *this; }

LogContext::LogContext (TimePoint t0):
	_t0 {t0}
{}

void
ComposeAndLog
(const LogContext &logcontext, const std::function <void (std::ostream &os)> &fnPrepareMessage)
{
	std::ostringstream os;
	{
		const std::string sThreadName {logcontext.GetThreadName ()};
		if (! sThreadName.empty ())
		{
			const auto tNow = Now ();
			os << "[" << std::setw (6) << (tNow - logcontext.StartOfTime ()).count () << "] ";
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

std::string
Indent
(lyb::string_view sInput,                          unsigned nTabs, lyb::string_view sTab)
{
	const std::ctype <char> &ctype {std::use_facet <std::ctype <char>> (std::locale {})};
	
	std::string rv;
	std::string sEmptyLines;
	bool        bEmptyLines {true};
	{
		std::string sTabs;
		{
			sTabs.reserve (nTabs * sTab.size ());
			for (unsigned iTab = 0; iTab < nTabs; ++iTab)
				#if IMAGINARYPLAYER_STRING_VIEW_BOOST
					sTabs.append (sTab.data (), sTab.size ());
				#else
					sTabs += sTab;
				#endif
		}
		
		// [2023-07-14] Yes, in the following line we have `<=`, not `<`.
		for (std::size_t icInput = 0, ccInput = sInput.size (); icInput <= ccInput; ++icInput)
		{
			const char c {icInput < ccInput ? sInput [icInput] : '\n'};
			if (bEmptyLines)
			{
				if (ctype.is (std::ctype_base::space, c))
					sEmptyLines += c;
				else
				{
					rv += sEmptyLines;
					sEmptyLines.clear ();
					bEmptyLines = false;
					
					rv += sTabs;
					rv += c;
				}
			}
			else
			{
				rv.push_back (c);
				
				if (c == '\n')
					bEmptyLines = true;
			}
		}
	}
	
	return rv;
}

std::string
IndentWithTitle
(lyb::string_view sInput, lyb::string_view sTitle, unsigned nTabs, lyb::string_view sTab)
{
	std::string rv;
	{
		const auto sIndented {Indent (sInput, nTabs, sTab)};
		
		rv.reserve (sTitle.size () + 3 + sIndented.size () + 2);
		
		#if IMAGINARYPLAYER_STRING_VIEW_BOOST
			rv.append (sTitle.data (), sTitle.size ());
		#else
			rv += sTitle;
		#endif
		
		rv += "\n{\n";
		rv += sIndented;
		rv += "}\n";
	}
	
	return rv;
}

}
