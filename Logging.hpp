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
	      std::string                       _sThreadName;
	      std::shared_ptr <Worker>          _spWorker;

 public:
	std::string       GetThreadName  ()                       const;
	lyb::string_view  GetThreadNameQ ()                       const;
	LogContext       &SetThreadName  (lyb::string_view value);
	
	std::shared_ptr <Worker>  GetSPWorker ()                                      const;
	LogContext               &SetSPWorker (const std::shared_ptr <Worker> &value);

 public:
	LogContext ();
};

void
ComposeAndLog
(const LogContext &logcontext, const std::function <void (std::ostream &os)> &fnPrepareMessage);

void
ComposeAndLog
(                              const std::function <void (std::ostream &os)> &fnPrepareMessage);

}
