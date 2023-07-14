#include "WorkerImpl.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

namespace ImaginaryPlayer
{

TimePoint        WorkerImpl::Arg::Now ()                const { return _tNow; }
WorkerImpl::Arg &WorkerImpl::Arg::Now (TimePoint value)       { _tNow = value; return *this; }

WorkerImpl::Arg::Arg ():
	_tNow {Now ()}
{}

std::ostream &WorkerImpl::Arg::Put (std::ostream &os) const
{
	std::ostringstream osTmp;
	{
		osTmp
			<< "("
			<< "_tNow " << _tNow.time_since_epoch ().count ()
			<< ")";
	}
	
	return os << osTmp.str ();
}

std::ostream &operator<< (std::ostream &os, const WorkerImpl::Arg &object)
{
	return object.Put (os);
}

WorkerImpl::~WorkerImpl ()
= default;

Duration WorkerImpl::GetTimeToWait (const Arg &arg)
{
	const auto rv = Do_GetTimeToWait (arg);
	// [2023-06-15] TODO: A facility for azzertions (alwayz-enabled-assertions).
	// [2023-06-17] Done.
	Azzert (rv >= Duration {-1});
	return rv;
}

Worker::WorkItemRV WorkerImpl::OnTimeout (const Arg &arg)
{
	const auto rv = Do_OnTimeout (arg);
	Azzert ((rv & Worker::All_Mask) == rv);
	return rv;
}

}
