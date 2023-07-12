#include "Player.hpp"
#include "WorkerImpl.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

namespace ImaginaryPlayer
{

Player::Player (const LogContext &logcontext):
	_logcontext     {logcontext},
	_contSongs
	{
		Song {}.SetSongName ("Demo Song of 10 seconds").SetLength (std::chrono::seconds {10}),
		Song {}.SetSongName ("Demo Song of 15 seconds").SetLength (std::chrono::seconds {15})
	},
	_dtWithinSong   {0},
	_bPlaying       {true},
	_tLastPlaying   {Now ()}
{}

std::ostream &Player::Put (std::ostream &os) const
{
	std::ostringstream osTmp;
	{
		osTmp << "_bPlaying " << _bPlaying << ", songs " << _contSongs.size () << ", _dtWithinSong " << _dtWithinSong.count ();
		if (! _contSongs.empty ())
		{
			const Song &song {_contSongs.front ()};
			osTmp << ", first " << song;
		}
	}
	
	return os << osTmp.str ();
}

Duration Player::GetTimeToWait (const WorkerImpl::Arg &arg)
{
	const char *const psz_func {__func__};
	
	std::ostringstream osMsg;
	{
		osMsg << *this;
	}
	
	Duration rv {-1};
	{
		if (_bPlaying && ! _contSongs.empty ())
		{
			const Song &song {_contSongs.front ()};
			const Duration dtSongLength {song.GetLength ()};
			Azzert (_dtWithinSong <= dtSongLength);
			_tLastPlaying = arg.Now ();
			rv = dtSongLength - _dtWithinSong;
		}
	}
	
	ComposeAndLog
	(
		_logcontext,
		[&] (std::ostream &os)
		{
			os << psz_func << ": " << osMsg.str () << ", rv " << rv.count () << ".\n";
		}
	);
	
	return rv;
}

Worker::WorkItemRV Player::OnTimeout (const WorkerImpl::Arg &arg)
{
	const char *const psz_func {__func__};
	
	if (_bPlaying)
	{
		ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << psz_func << ": Timeout.\n"; });
		if (! _contSongs.empty ())
			_contSongs.pop_front ();
	}
	
	return Worker::Break_0;
}

std::ostream &operator<< (std::ostream &os, const Player &object)
{
	return object.Put (os);
}

}
