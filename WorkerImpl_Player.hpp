#include "WorkerImpl.hpp"
#include "Song.hpp"

#include <deque>

namespace ImaginaryPlayer
{

class WorkerImpl_Player:
	public WorkerImpl
{
 private:
	      std::shared_ptr <Worker>          _spWorkerLogger;
	      std::deque <Song>                 _contSongs;
	      TimeRep                           _dtWithinSong;
	      bool                              _bPlaying;

 public:
	WorkerImpl_Player (const std::shared_ptr <Worker> &spWorkerLogger);

 private:
	TimeRep Do_GetTimeToWait () override;
	Worker::WorkItemRV Do_OnTimeout () override;
};

}
