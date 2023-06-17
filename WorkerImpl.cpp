#include "WorkerImpl.hpp"

namespace ImaginaryPlayer
{

WorkerImpl::~WorkerImpl ()
= default;

TimeRep WorkerImpl::GetTimeToWait () const
{
	const auto rv = Do_GetTimeToWait ();
	// [2023-06-15] TODO: A facility for azzertions (alwayz-enabled-assertions).
	// [2023-06-17] Done.
	Azzert (rv >= 0);
	return rv;
}

}
