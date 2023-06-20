#pragma once

#include "Worker.hpp"
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
	      TimeRep                           _dtWithinSong;
	      bool                              _bPlaying;
	      TimePoint                         _tLastPlaying;

 public:
	Player (const std::shared_ptr <Worker> &spWorkerLogger);

 public:
	TimeRep GetTimeToWait ();
	Worker::WorkItemRV OnTimeout ();
};

}
