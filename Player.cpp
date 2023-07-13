#include "Player.hpp"
#include "WorkerImpl.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <algorithm>

namespace ImaginaryPlayer
{

Player::Player (const LogContext &logcontext):
	_logcontext     {logcontext},
	_contSongs
	{
		Song {}.SetSongName ("Demo Song of 3 seconds").SetLength (std::chrono::seconds {3}),
		Song {}.SetSongName ("Demo Song of 6 seconds").SetLength (std::chrono::seconds {6})
	},
	_dtWithinSong   {0},
	_bPlaying       {true},
	_tLastPlaying   {Now ()}
{}

std::ostream &Player::Put (std::ostream &os) const
{
	std::ostringstream osTmp;
	{
		osTmp << "_bPlaying " << _bPlaying << ", songs " << _contSongs.size ();
		if (! _contSongs.empty ())
		{
			const Song &song {_contSongs.front ()};
			osTmp << ", first " << song;
		}
		osTmp << ", _dtWithinSong " << _dtWithinSong.count ();
	}
	
	return os << osTmp.str ();
}

Duration Player::GetTimeToWait (const WorkerImpl::Arg &arg)
{
	// [2023-07-12] In the lambda given to `ComposeAndLog`, `__func__` is `"operator()"`. But what we want is `"GetTimeToWait"`. So we prepare that in `psz_func`.
	const char *const psz_func {__func__};
	
	std::ostringstream osMsg;
	{
		osMsg << "Initial: " << *this << "\n";
	}
	
	Duration rv {-1};
	{
		const Song *pSong {_contSongs.empty () ? nullptr : &_contSongs.front ()};
		if (_bPlaying && pSong)
		{
			auto dtSongLength {pSong ? pSong->GetLength () : Duration {0}};
			Azzert (_dtWithinSong <= dtSongLength);
			
			const auto tNow {arg.Now ()};
			
			Duration dtElapsed {tNow - _tLastPlaying};
			{
				if (_dtWithinSong + dtElapsed > dtSongLength)
					dtElapsed = dtSongLength - _dtWithinSong;
			}
			
			_dtWithinSong += dtElapsed;
			Azzert (_dtWithinSong <= dtSongLength);
			if (_dtWithinSong >= dtSongLength)
			{
				_contSongs.pop_front ();
				
				// [2023-07-12] TODO: We have to update all "cached" values !
				pSong = _contSongs.empty () ? nullptr : &_contSongs.front ();
				dtSongLength = pSong ? pSong->GetLength () : Duration {0};
				
				_dtWithinSong = Duration {0};
				
			}
			
			_tLastPlaying = tNow;
			
			if (pSong)
			{
				// [2023-07-12] Have we updated the "cached" value ?
				Azzert (dtSongLength == pSong->GetLength ());
				
				rv = dtSongLength - _dtWithinSong;
				rv = std::min (Duration {1000}, rv);
			}
			
			osMsg << "Updated: " << *this << "\n";
		}
	}
	
	ComposeAndLog
	(
		_logcontext,
		[&] (std::ostream &os)
		{
			os << psz_func << ":\n" << osMsg.str () << "=> rv " << rv.count () << ".\n";
		}
	);
	
	return rv;
}

Worker::WorkItemRV Player::OnTimeout (const WorkerImpl::Arg &arg)
{
	const char *const psz_func {__func__};
	
	ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << psz_func << ": Timeout.\n"; });
	return Worker::RV_Normal;
}

std::ostream &operator<< (std::ostream &os, const Player &object)
{
	return object.Put (os);
}

}
