#pragma once

#include "Worker.hpp"
#include "WorkerImpl.hpp"  // [2023-06-20] For `WorkerImpl::Arg`.
#include "Song.hpp"
#include "Logging.hpp"

#include <iosfwd>
#include <chrono>
#include <memory>
#include <deque>
#include <utility>

namespace ImaginaryPlayer
{

class Player
{
 private:
	      LogContext                        _logcontext;
	      std::deque <Song>                 _contSongs;
	      std::deque <Song>                 _contHistory;
	      std::size_t                       _iWithinHistory;
	      Duration                          _dtWithinSong;
	      bool                              _bPlaying;
	      TimePoint                         _tLastPlaying;
	      Duration                          _dtPing;

 public:
	std::ostream &Put (std::ostream &os) const;

 public:
	explicit Player (const LogContext &logcontext);

 private:
	std::pair <Duration, std::string> OnElapsedTime   (const WorkerImpl::Arg &arg);

 public:
	Duration                          GetTimeToWait   (const WorkerImpl::Arg &arg);
	Worker::WorkItemRV                OnWakeUp        (const WorkerImpl::Arg &arg, bool bWorkToDo);
	Worker::WorkItemRV                OnTimeout       (const WorkerImpl::Arg &arg);

 public:
	Worker::WorkItemRV                Show            (const WorkerImpl::Arg &arg);
	Worker::WorkItemRV                AddSong         (const WorkerImpl::Arg &arg, const Song &song);
	Worker::WorkItemRV                Play            (const WorkerImpl::Arg &arg, bool bPlaying);
	Worker::WorkItemRV                PrevNext        (const WorkerImpl::Arg &arg, bool bNext);
};

std::ostream &operator<< (std::ostream &os, const Player &object);

}
