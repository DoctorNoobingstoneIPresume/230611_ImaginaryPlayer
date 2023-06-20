#pragma once

#include "Worker.hpp"
#include "WorkerImpl.hpp"  // [2023-06-20] For `WorkerImpl::Arg`.
#include "Song.hpp"

#include <chrono>
#include <memory>
#include <deque>

namespace ImaginaryPlayer
{

class Player
{
 private:
	      std::shared_ptr <Worker>          _spWorkerLogger;
	      std::deque <Song>                 _contSongs;
	      Duration                          _dtWithinSong;
	      bool                              _bPlaying;
	      TimePoint                         _tLastPlaying;

 public:
	Player (const std::shared_ptr <Worker> &spWorkerLogger);

 public:
	Duration GetTimeToWait (const WorkerImpl::Arg &arg);
	Worker::WorkItemRV OnTimeout (const WorkerImpl::Arg &arg);
};

}
