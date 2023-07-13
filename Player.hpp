#pragma once

#include "Worker.hpp"
#include "WorkerImpl.hpp"  // [2023-06-20] For `WorkerImpl::Arg`.
#include "Song.hpp"
#include "Logging.hpp"

#include <iosfwd>
#include <chrono>
#include <memory>
#include <deque>

namespace ImaginaryPlayer
{

class Player
{
 private:
	      LogContext                        _logcontext;
	      std::deque <Song>                 _contSongs;
	      Duration                          _dtWithinSong;
	      bool                              _bPlaying;
	      TimePoint                         _tLastPlaying;

 public:
	std::ostream &Put (std::ostream &os) const;

 public:
	explicit Player (const LogContext &logcontext);

 public:
	Duration GetTimeToWait (const WorkerImpl::Arg &arg);
	Worker::WorkItemRV OnTimeout (const WorkerImpl::Arg &arg);
};

std::ostream &operator<< (std::ostream &os, const Player &object);

}
