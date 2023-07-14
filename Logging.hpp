#pragma once

#include "Worker.hpp"

#include "string_view.hpp"

#include <functional>
#include <memory>

namespace ImaginaryPlayer
{

class LogContext
{
 private:
	      TimePoint                         _t0;
	      std::string                       _sThreadName;
	      std::shared_ptr <Worker>          _spWorker;

 public:
	TimePoint   StartOfTime ()                const;
	LogContext &StartOfTime (TimePoint value);
	
	std::string       GetThreadName  ()                       const;
	lyb::string_view  GetThreadNameQ ()                       const;
	LogContext       &SetThreadName  (lyb::string_view value);
	
	std::shared_ptr <Worker>  GetSPWorker ()                                      const;
	LogContext               &SetSPWorker (const std::shared_ptr <Worker> &value);

 public:
	explicit LogContext (TimePoint t0 = Now ());
};

void
ComposeAndLog
(const LogContext &logcontext, const std::function <void (std::ostream &os)> &fnPrepareMessage);

void
ComposeAndLog
(                              const std::function <void (std::ostream &os)> &fnPrepareMessage);

std::string
Indent
(lyb::string_view sInput,                          unsigned nTabs = 1, lyb::string_view sTab = "\t");

std::string
IndentWithTitle
(lyb::string_view sInput, lyb::string_view sTitle, unsigned nTabs = 1, lyb::string_view sTab = "\t");

}
