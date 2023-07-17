#include "Player.hpp"
#include "WorkerImpl.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <utility>
#include <algorithm>
#include <tuple>

#include <cstdlib>

namespace ImaginaryPlayer
{

Player::Player (const LogContext &logcontext):
	_logcontext     {logcontext},
	_iVerb          {0},
	_contSongs
	{
		Song {}.SetSongName ("Demo Song of 3 seconds").SetLength (std::chrono::seconds {3}),
		Song {}.SetSongName ("Demo Song of 6 seconds").SetLength (std::chrono::seconds {6})
	},
	_iWithinSongs   {0},
	_iWithinHistory (-1),
	_dtWithinSong   {0},
	_bPlaying       {true},
	_tLastPlaying   {Now ()},
	_bNewSong       {true},
	_bRepeat        {false},
	_bRandom        {false},
	_dtPing         {0}
{}

std::ostream &Player::Put (std::ostream &os) const
{
	std::ostringstream osTmp;
	{
		const int cc_iSong {4};
		osTmp
			<< "_iVerb " << _iVerb
			<< ", "
			<< "_tLastPlaying " << _tLastPlaying.time_since_epoch ().count ()
			<< ", "
			<< "history " << std::setw (cc_iSong) << static_cast <std::ptrdiff_t> (_iWithinHistory) << " of " << std::setw (cc_iSong) << _contHistory.size ()
			<< ", "
			<< "_bPlaying " << _bPlaying
			<< ", "
			<< "song " << std::setw (cc_iSong) << static_cast <std::ptrdiff_t> (_iWithinSongs) << " of " << std::setw (cc_iSong) << _contSongs.size ()
			<< ", "
			<< "_bRepeat " << _bRepeat
			<< ", "
			<< "_bRandom " << _bRandom;
		
		if (_iWithinHistory < _contHistory.size () || _iWithinSongs < _contSongs.size ())
		{
			const Song &song {_iWithinHistory < _contHistory.size () ? _contHistory.at (_iWithinHistory) : _contSongs.at (_iWithinSongs)};
			osTmp
				<< ", "
				<< "first " << song;
		}
		
		osTmp
			<< ", "
			<< "_dtWithinSong " << _dtWithinSong.count ();
	}
	
	return os << osTmp.str ();
}

class Player::NextRV
{
 private:
	      bool                      _bNewSong;
	      std::string               _sShortMsg;

 public:
	bool              GetNewSongFlag ()                       const;
	NextRV           &SetNewSongFlag (bool value);
	
	std::string       GetShortMsg    ()                       const;
	lyb::string_view  GetShortMsgQ   ()                       const;
	NextRV           &SetShortMsg    (lyb::string_view value);

 public:
	NextRV ();
};

bool                      Player::NextRV::GetNewSongFlag ()                       const { return _bNewSong; }
Player::NextRV           &Player::NextRV::SetNewSongFlag (bool value)                   { _bNewSong = value; return *this; }

std::string               Player::NextRV::GetShortMsg    ()                       const { return _sShortMsg; }
lyb::string_view          Player::NextRV::GetShortMsgQ   ()                       const { return _sShortMsg; }
Player::NextRV           &Player::NextRV::SetShortMsg    (lyb::string_view value)       { _sShortMsg = lyb::ViewToString (value); return *this; }

Player::NextRV::NextRV ():
	_bNewSong {false}
{}

Player::NextRV Player::Next (const WorkerImpl::Arg &arg)
{
	const Song *pSong;
	{
		if      (_iWithinHistory < _contHistory.size ()) pSong = &_contHistory.at (_iWithinHistory);
		else if (_iWithinSongs   < _contSongs  .size ()) pSong = &_contSongs  .at (_iWithinSongs)  ;
		else                                             pSong = nullptr;
	}
	
	std::ostringstream osShortMsg;
	{
		if (pSong)
			osShortMsg << "We have finished playing " << *pSong << ".\n";
	}
	
	bool bNewSong {false};
	{
		if (_iWithinHistory < _contHistory.size ())
		{
			Azzert (pSong);
			
			++_iWithinHistory;
			bNewSong = true;
		}
		else
		if (_iWithinSongs < _contSongs.size ())
		{
			Azzert (pSong);
			
			_iWithinHistory = -1;
			_contHistory.push_back (*pSong);
			
			++_iWithinSongs;
			if (_iWithinSongs >= _contSongs.size ())
			{
				Azzert (_iWithinSongs == _contSongs.size ());
				if (_bRepeat)
					_iWithinSongs = 0;
			}
			
			if (_bRandom)
			{
				if (const std::size_t nRem = _contSongs.size () - _iWithinSongs)
				{
					// [2023-07-18] TODO: Support for more than RAND_MAX remaining songs.
					const std::size_t iWhich = _iWithinSongs + std::rand () % nRem;
					if (iWhich != _iWithinSongs)
					{
						Azzert (iWhich > _iWithinSongs);
						
						using std::swap;
						swap (_contSongs.at (iWhich), _contSongs.at (_iWithinSongs));
					}
				}
			}
			
			bNewSong = true;
		}
		else
			Azzert (! pSong);
		
		if (bNewSong)
		{
			_tLastPlaying = arg.ThenCrtTime ();
			_dtWithinSong = Duration {0};
			_bNewSong     = true;
		}
	}
	
	return NextRV {}.SetNewSongFlag (bNewSong).SetShortMsg (osShortMsg.str ());
}

class Player::OnElapsedTimeRV
{
 private:
	      Duration                  _dt;
	      std::string               _sShortMsg;
	      std::string               _sLongMsg;

 public:
	Duration          GetDuration  ()                       const;
	OnElapsedTimeRV  &SetDuration  (Duration value);
	
	std::string       GetShortMsg  ()                       const;
	lyb::string_view  GetShortMsgQ ()                       const;
	OnElapsedTimeRV  &SetShortMsg  (lyb::string_view value);
	
	std::string       GetLongMsg   ()                       const;
	lyb::string_view  GetLongMsgQ  ()                       const;
	OnElapsedTimeRV  &SetLongMsg   (lyb::string_view value);

 public:
	OnElapsedTimeRV ();
};

Duration                  Player::OnElapsedTimeRV::GetDuration  ()                       const { return _dt; }
Player::OnElapsedTimeRV  &Player::OnElapsedTimeRV::SetDuration  (Duration value)               { _dt = value; return *this; }

std::string               Player::OnElapsedTimeRV::GetShortMsg  ()                       const { return _sShortMsg; }
lyb::string_view          Player::OnElapsedTimeRV::GetShortMsgQ ()                       const { return _sShortMsg; }
Player::OnElapsedTimeRV  &Player::OnElapsedTimeRV::SetShortMsg  (lyb::string_view value)       { _sShortMsg = lyb::ViewToString (value); return *this; }

std::string               Player::OnElapsedTimeRV::GetLongMsg   ()                       const { return _sLongMsg; }
lyb::string_view          Player::OnElapsedTimeRV::GetLongMsgQ  ()                       const { return _sLongMsg; }
Player::OnElapsedTimeRV  &Player::OnElapsedTimeRV::SetLongMsg   (lyb::string_view value)       { _sLongMsg = lyb::ViewToString (value); return *this; }

Player::OnElapsedTimeRV::OnElapsedTimeRV ():
	_dt {-1}
{}

Player::OnElapsedTimeRV Player::OnElapsedTime (const WorkerImpl::Arg &arg)
{
	// [2023-07-12] In the lambda given to `ComposeAndLog`, `__func__` is `"operator()"`. But what we want is `"GetTimeToWait"`. So we prepare that in `psz_func`.
	const char *const psz_func {__func__};
	
	std::ostringstream osMsg;
	{
		osMsg << "arg:     " << arg << ".\n";
		osMsg << "Initial: " << *this << ".\n";
	}
	
	std::ostringstream osShortMsg;
	
	Duration rv {-1};
	{
		if (_bPlaying)
		{
			const auto tNow {arg.ThenCrtTime ()};
			Azzert (tNow >= _tLastPlaying);
			
			while ((_iWithinHistory < _contHistory.size () || _iWithinSongs < _contSongs.size ()) && _tLastPlaying < tNow)
			{
				const Song &song {_iWithinHistory < _contHistory.size () ? _contHistory.at (_iWithinHistory) : _contSongs.at (_iWithinSongs)};
				const auto dtSong {song.GetLength ()}; Azzert (_dtWithinSong <= dtSong);
				//if (! _dtWithinSong.count ())
				//	osShortMsg << "We have startedd playing " << song << ".\n";
				
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
						const auto result = Next (arg);
						osShortMsg << result.GetShortMsg ();
						bSongHasChanged = result.GetNewSongFlag ();
					}
				}
				
				osMsg << "Updated: " << *this << ".\n";
				
				// [2023-07-13] Protection against _tLastPlaying not advancing:
				if (dtConsidered <= Duration {0} && ! bSongHasChanged)
					break;
			}
			
			if (_iWithinHistory < _contHistory.size () || _iWithinSongs < _contSongs.size ())
			{
				const Song &song {_iWithinHistory < _contHistory.size () ? _contHistory.at (_iWithinHistory) : _contSongs.at (_iWithinSongs)};
				const auto dtSong {song.GetLength ()}; Azzert (_dtWithinSong <= dtSong);
				rv = dtSong - _dtWithinSong;
				
				// [2023-07-13] For debugging:
				//rv = std::max (rv, Duration {4000});
				if (_dtPing > Duration {0})
					rv = std::min (rv, _dtPing);
				
				if (_bNewSong)
				{
					osShortMsg << "We have started  playing " << song << ".\n";
					_bNewSong = false;
				}
			}
		}
	}
	
	return OnElapsedTimeRV {}.SetDuration (rv).SetShortMsg (osShortMsg.str ()).SetLongMsg (osMsg.str ());
}

Duration Player::GetTimeToWait (const WorkerImpl::Arg &arg)
{
	// [2023-07-12] In the lambda given to `ComposeAndLog`, `__func__` is `"operator()"`. But what we want is `"GetTimeToWait"`. So we prepare that in `psz_func`.
	const char *const psz_func {__func__};
	
	const auto result0 {OnElapsedTime (arg)};
	
	if (_iVerb)
	ComposeAndLog
	(
		_logcontext,
		[&] (std::ostream &os)
		{
			if (_iVerb)
				os << IndentWithTitle (result0.GetLongMsg (), psz_func) << "=> rv " << result0.GetDuration ().count () << ".\n\n";
		}
	);
	
	if (result0.GetShortMsgQ ().size ())
		ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << IndentWithTitle (result0.GetShortMsg (), psz_func); });
	
	return result0.GetDuration ();
}

Worker::WorkItemRV Player::OnWakeUp (const WorkerImpl::Arg &arg, bool bWorkToDo)
{
	const char *const psz_func {__func__};
	
	const auto result0 {OnElapsedTime (arg)};
	
	std::ostringstream osTitle;
	{
		osTitle << psz_func << " (bWorkToDo " << bWorkToDo << ")";
	}
	
	if (_iVerb)
	ComposeAndLog
	(
		_logcontext,
		[&] (std::ostream &os)
		{
			os << IndentWithTitle (result0.GetLongMsg (), osTitle.str ());
		}
	);
	
	if (result0.GetShortMsgQ ().size ())
		ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << IndentWithTitle (result0.GetShortMsg (), osTitle.str ()); });
	
	return Worker::RV_Normal;

}

Worker::WorkItemRV Player::OnTimeout (const WorkerImpl::Arg &arg)
{
	const char *const psz_func {__func__};
	
	if (_iVerb)
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
				
	ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << IndentWithTitle (osMsg.str (), psz_func); });
	return Worker::RV_Normal;
}

Worker::WorkItemRV Player::ShowSongs (const WorkerImpl::Arg &arg)
{
	const char *const psz_func {__func__};
	
	std::ostringstream osMsg;
	{
		osMsg << "arg:     " << arg << ".\n";
		osMsg << "Showing: " << *this << ".\n";
		
		const int cc_iSong {4};
		
		{
			std::ostringstream os;
			{
				for (std::size_t i = 0, n = _contHistory.size (); i < n; ++i)
					os << std::setw (cc_iSong) << i << ": " << _contHistory.at (i) << ".\n";
			}
			
			osMsg << IndentWithTitle (os.str (), "History:");
		}
		
		{
			std::ostringstream os;
			{
				for (std::size_t i = 0, n = _contSongs.size (); i < n; ++i)
					os << std::setw (cc_iSong) << i << ": " << _contSongs.at (i) << ".\n";
			}
			
			osMsg << IndentWithTitle (os.str (), "Songs:");
		}
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
				
	ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << IndentWithTitle (osMsg.str (), psz_func); });
	return Worker::RV_Normal;
}

Worker::WorkItemRV Player::Verb (const WorkerImpl::Arg &arg, unsigned iVerb)
{
	iVerb = std::min (iVerb, 1u);
	
	ComposeAndLog
	(
		_logcontext,
		[&] (std::ostream &os)
		{
			os << "Setting verbosity level to " << iVerb << ".\n";
		}
	);
	
	_iVerb = iVerb;
	
	return Worker::RV_Normal;
}

Worker::WorkItemRV Player::AddSong (const WorkerImpl::Arg &arg, const Song &song)
{
	const char *const psz_func {__func__};
	
	std::ostringstream osMsg;
	{
		osMsg << "Initial: " << *this << ".\n";
	}
	
	if (_bPlaying && _iWithinHistory >= _contHistory.size () && _iWithinSongs >= _contSongs.size ())
	{
		osMsg << "Updating _tLastPlaying...\n";
		_tLastPlaying = arg.ThenCrtTime ();
		_bNewSong     = true;
	}
	
	const auto dtElapsed {arg.ThenCrtTime () - _tLastPlaying};
	
	const int cc_dt {7};
	osMsg
		<< "dtElapsed "       << std::setw (cc_dt) << dtElapsed      .count () << ".\n";
	
	_contSongs.push_back (song);
	
	if (_iVerb)
		ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << IndentWithTitle (osMsg.str (), psz_func); });
	
	return Worker::RV_Normal;
}

Worker::WorkItemRV Player::Play (const WorkerImpl::Arg &arg, bool bPlaying)
{
	if (! _bPlaying && bPlaying)
	{
		_tLastPlaying = arg.ThenCrtTime ();
		// [2023-07-17] TODO: Fix/eliminate the following:
		//if (! _dtWithinSong.count ())
		//	ComposeAndLog (_logcontext, [&] (std::ostream &os) { os << "We have startedd playing " << _contSongs.front () << ".\n"; });
	}
	
	_bPlaying = bPlaying;
	
	return Worker::RV_Normal;
}

Worker::WorkItemRV Player::PrevNext (const WorkerImpl::Arg &arg, bool bNext)
{
	if (bNext)
	{
		// [2023-07-18] TODO: Consider the return value of the following call:
		Next (arg);
	}
	else
	{
		if (! _contHistory.empty ())
		{
			if (_iWithinHistory >= _contHistory.size ())
				_iWithinHistory = _contHistory.size () - 1;
			else
			if (_iWithinHistory)
				--_iWithinHistory;
			
			// [2023-07-17] The following is no longer needed -- the logic now selects a `Song` either from `_contHistory` or `_contSongs`.
			//_contSongs.push_front (_contHistory.at (_iWithinHistory));
		}
		
		_tLastPlaying = arg.ThenCrtTime ();
		_dtWithinSong = Duration {0};
		_bNewSong     = true;
	}
	
	return Worker::RV_Normal;
}

Worker::WorkItemRV Player::Repeat (const WorkerImpl::Arg &arg, bool bRepeat)
{
	if (! _bRepeat && bRepeat && _bPlaying && _iWithinHistory >= _contHistory.size () && _iWithinSongs >= _contSongs.size ())
	{
		_iWithinSongs = 0;
		_tLastPlaying = arg.ThenCrtTime ();
	}
	
	_bRepeat = bRepeat;
	
	return Worker::RV_Normal;
}

Worker::WorkItemRV Player::Random (const WorkerImpl::Arg &arg, bool bRandom)
{
	_bRandom = bRandom;
	
	return Worker::RV_Normal;
}

std::ostream &operator<< (std::ostream &os, const Player &object)
{
	return object.Put (os);
}

}
