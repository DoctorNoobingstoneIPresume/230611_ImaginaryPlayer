#pragma once

#include "Worker.hpp"
#include "WorkerImpl.hpp"  // [2023-06-20] For `WorkerImpl::Arg`.
#include "Song.hpp"
#include "Logging.hpp"

#include <iosfwd>
#include <chrono>
#include <memory>
#include <deque>
#include <tuple>
#include <utility>

namespace ImaginaryPlayer
{

class Player
{
 private:
	      LogContext                        _logcontext;
	      unsigned                          _iVerb;
	      std::deque <Song>                 _contSongs;
	      std::size_t                       _iWithinSongs;
	      std::deque <Song>                 _contHistory;
	      std::size_t                       _iWithinHistory;
	      Duration                          _dtWithinSong;
	      bool                              _bPlaying;
	      TimePoint                         _tLastPlaying;
	      bool                              _bNewSong;
	      bool                              _bRepeat;
	      bool                              _bRandom;
	      Duration                          _dtPing;

 public:
	std::ostream &Put (std::ostream &os) const;

 public:
	explicit Player (const LogContext &logcontext);

 private:
	class NextRV;
	NextRV                            Next            (const WorkerImpl::Arg &arg);
	
	class OnElapsedTimeRV;
	OnElapsedTimeRV                   OnElapsedTime   (const WorkerImpl::Arg &arg);

 public:
	Duration                          GetTimeToWait   (const WorkerImpl::Arg &arg);
	Worker::WorkItemRV                OnWakeUp        (const WorkerImpl::Arg &arg, bool bWorkToDo);
	Worker::WorkItemRV                OnTimeout       (const WorkerImpl::Arg &arg);

 public:
	Worker::WorkItemRV                Show            (const WorkerImpl::Arg &arg);
	Worker::WorkItemRV                ShowSongs       (const WorkerImpl::Arg &arg);
	Worker::WorkItemRV                Verb            (const WorkerImpl::Arg &arg, unsigned iVerb);
	Worker::WorkItemRV                AddSong         (const WorkerImpl::Arg &arg, const Song &song);
	Worker::WorkItemRV                RemoveSong      (const WorkerImpl::Arg &arg, std::size_t iSongToRemove);
	Worker::WorkItemRV                RemoveDups      (const WorkerImpl::Arg &arg);
	Worker::WorkItemRV                Play            (const WorkerImpl::Arg &arg, bool bPlaying);
	Worker::WorkItemRV                PrevNext        (const WorkerImpl::Arg &arg, bool bNext);
	Worker::WorkItemRV                Repeat          (const WorkerImpl::Arg &arg, bool bRepeat);
	Worker::WorkItemRV                Random          (const WorkerImpl::Arg &arg, bool bRandom);
};

std::ostream &operator<< (std::ostream &os, const Player &object);

}
