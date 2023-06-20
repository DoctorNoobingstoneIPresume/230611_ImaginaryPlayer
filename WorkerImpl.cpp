#include "WorkerImpl.hpp"

namespace ImaginaryPlayer
{

TimePoint        WorkerImpl::Arg::Now ()                const { return _tNow; }
WorkerImpl::Arg &WorkerImpl::Arg::Now (TimePoint value)       { _tNow = value; return *this; }

WorkerImpl::Arg::Arg ():
	_tNow {Now ()}
{}

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
