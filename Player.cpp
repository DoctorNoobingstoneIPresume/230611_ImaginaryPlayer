#include "Player.hpp"
#include "WorkerImpl.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <algorithm>
#include <tuple>

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
	_tLastPlaying   {Now ()},
	_dtPing         {0}
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
		osTmp << ", _tLastPlaying " << _tLastPlaying.time_since_epoch ().count ();
	}
	
	return os << osTmp.str ();
}

std::pair <Duration, std::string> Player::OnElapsedTime (const WorkerImpl::Arg &arg)
{
	// [2023-07-12] In the lambda given to `ComposeAndLog`, `__func__` is `"operator()"`. But what we want is `"GetTimeToWait"`. So we prepare that in `psz_func`.
	const char *const psz_func {__func__};
	
	std::ostringstream osMsg;
	{
		osMsg << "arg:     " << arg << ".\n";
		osMsg << "Initial: " << *this << ".\n";
	}
	
	Duration rv {-1};
	{
		if (_bPlaying)
		{
			const auto tNow {arg.ThenCrtTime ()};
			Azzert (tNow >= _tLastPlaying);
			
			while (! _contSongs.empty () && _tLastPlaying < tNow)
			{
				const Song &song {_contSongs.front ()};
				const auto dtSong {song.GetLength ()}; Azzert (_dtWithinSong <= dtSong);
				const auto dtSongRemaining {dtSong - _dtWithinSong};
				
				const auto dtElapsed {tNow - _tLastPlaying};
				
				const auto dtConsidered {std::min (dtElapsed, dtSongRemaining)};
				
				const int cc_dt {7}; // [2023-07-13] `cc_dt`: "count of characters for delta-time".
				osMsg
					<< "tNow      "       << std::setw (cc_dt) << tNow.time_since_epoch ().count () << ", "
					<< "dtElapsed "       << std::setw (cc_dt) << dtElapsed               .count () << ", "
					<< "dtSongRemaining " << std::setw (cc_dt) << dtSongRemaining         .count () << ", "
					<< "dtConsidered "    << std::setw (cc_dt) << dtConsidered            .count () << ".\n";
				
				_dtWithinSong += dtConsidered; Azzert (_dtWithinSong <= dtSong);
				_tLastPlaying += dtConsidered;
				bool bSongHasChanged {false};
				{
					if (_dtWithinSong >= dtSong)
					{
						_contSongs.pop_front ();
						_dtWithinSong = Duration {0};
						bSongHasChanged = true;
					}
				}
				
				osMsg << "Updated: " << *this << ".\n";
				
				// [2023-07-13] Protection against _tLastPlaying not advancing:
				if (dtConsidered <= Duration {0} && ! bSongHasChanged)
					break;
			}
			
			if (! _contSongs.empty ())
			{
				const Song &song {_contSongs.front ()};
				const auto dtSong {song.GetLength ()}; Azzert (_dtWithinSong <= dtSong);
				rv = dtSong - _dtWithinSong;
				
				// [2023-07-13] For debugging:
				//rv = std::max (rv, Duration {4000});
				if (_dtPing > Duration {0})
					rv = std::min (rv, _dtPing);
			}
		}
	}
	
	return std::make_pair (rv, osMsg.str ());
}

Duration Player::GetTimeToWait (const WorkerImpl::Arg &arg)
{
	// [2023-07-12] In the lambda given to `ComposeAndLog`, `__func__` is `"operator()"`. But what we want is `"GetTimeToWait"`. So we prepare that in `psz_func`.
	const char *const psz_func {__func__};
	
	#if defined __cpp_structured_bindings
		const auto [rv, sMsg] = OnElapsedTime (arg);
	#else
		Duration rv;
		std::string sMsg;
		{
			std::tie (rv, sMsg) = OnElapsedTime (arg);
		}
	#endif
	
	ComposeAndLog
	(
		_logcontext,
		[&] (std::ostream &os)
		{
			os << psz_func << ":\n" << sMsg << "=> rv " << rv.count () << ".\n\n";
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

Worker::WorkItemRV Player::Show (const WorkerImpl::Arg &arg)
{
	const char *const psz_func {__func__};
	
	std::ostringstream osMsg;
	{
		osMsg << "arg:     " << arg << ".\n";
		osMsg << "Showing: " << *this << ".\n";
	}
	
	if (_bPlaying)
	{
		const auto tNow      {arg.ThenCrtTime ()};
		const auto dtElapsed {tNow - _tLastPlaying};
		
		const int cc_dt {7};
		osMsg
			<< "tNow      "       << std::setw (cc_dt) << tNow.time_since_epoch ().count () << ", "
			<< "dtElapsed "       << std::setw (cc_dt) << dtElapsed               .count () << ".\n";
	}
				
	ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << psz_func << ":\n" << osMsg.str (); });
	return Worker::RV_Normal;
}

Worker::WorkItemRV Player::AddSong (const WorkerImpl::Arg &arg, const Song &song)
{
	const char *const psz_func {__func__};
	
	std::ostringstream osMsg;
	{
		osMsg << "Initial: " << *this << ".\n";
	}
	
	if (_bPlaying && _contSongs.empty ())
	{
		osMsg << "Updating _tLastPlaying...\n";
		_tLastPlaying = arg.ThenCrtTime ();
	}
	
	const auto dtElapsed {arg.ThenCrtTime () - _tLastPlaying};
	
	const int cc_dt {7};
	osMsg
		<< "dtElapsed "       << std::setw (cc_dt) << dtElapsed      .count () << ".\n";
	
	_contSongs.push_back (song);
	
	ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << psz_func << ":\n" << osMsg.str (); });
	
	return Worker::RV_Normal;
}

Worker::WorkItemRV Player::Play (const WorkerImpl::Arg &arg, bool bPlaying)
{
	const char *const psz_func {__func__};
	
	#if defined __cpp_structured_bindings
		const auto [rv, sMsg] = OnElapsedTime (arg);
	#else
		Duration rv; std::string sMsg; { std::tie (rv, sMsg) = OnElapsedTime (arg); }
	#endif
	
	ComposeAndLog
	(
		_logcontext,
		[&] (std::ostream &os)
		{
			os << psz_func << ": Playing status " << _bPlaying << " => " << bPlaying << ":\n" << sMsg << "=> rv " << rv.count () << ".\n\n";
		}
	);
	
	if (bPlaying) _tLastPlaying = arg.ThenCrtTime ();
	if (1)        _bPlaying = bPlaying;
	return Worker::RV_Normal;
}

std::ostream &operator<< (std::ostream &os, const Player &object)
{
	return object.Put (os);
}

}
