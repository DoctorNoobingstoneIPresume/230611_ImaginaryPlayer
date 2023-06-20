#include "Player.hpp"
#include "WorkerImpl.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

namespace ImaginaryPlayer
{

Player::Player (const std::shared_ptr <Worker> &spWorkerLogger):
	_spWorkerLogger {spWorkerLogger},
	_contSongs      {Song {}.SetLength (std::chrono::seconds {10}), Song {}.SetLength (std::chrono::seconds {15})},
	_dtWithinSong   {0},
	_bPlaying       {true},
	_tLastPlaying   {Now ()}
{}

Duration Player::GetTimeToWait (const WorkerImpl::Arg &arg)
{
	if (_bPlaying && ! _contSongs.empty ())
	{
		const Song &song {_contSongs.front ()};
		const Duration dtSongLength {song.GetLength ()};
		Azzert (_dtWithinSong <= dtSongLength);
		_tLastPlaying = arg.Now ();
		const auto rv = dtSongLength - _dtWithinSong;
		
		{
			std::ostringstream os; { os << __func__ << ": rv " << rv.count () << ".\n"; }
			const std::string sMsg = os.str ();
			_spWorkerLogger->AddWorkItem (std::make_shared <Worker::WorkItem> ([=] () -> Worker::WorkItemRV { std::cout << sMsg << std::flush; return Worker::Break_0; }));
		}
		
		return rv;
	}
	else
		return Duration {-1};
}

Worker::WorkItemRV Player::OnTimeout (const WorkerImpl::Arg &arg)
{
	if (_bPlaying)
	{
		{
			std::ostringstream os; { os << __func__ << ": Timeout.\n" << std::flush; }
			const std::string sMsg = os.str ();
			_spWorkerLogger->AddWorkItem (std::make_shared <Worker::WorkItem> ([=] () -> Worker::WorkItemRV { std::cout << sMsg << std::flush; return Worker::Break_0; }));
		}
		
		if (! _contSongs.empty ())
			_contSongs.pop_front ();
	}
	
	return Worker::Break_0;
}

}
