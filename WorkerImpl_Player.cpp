#include "WorkerImpl_Player.hpp"

namespace ImaginaryPlayer
{

WorkerImpl_Player::WorkerImpl_Player (const std::shared_ptr <Worker> &spWorkerLogger):
	_spWorkerLogger {spWorkerLogger},
	_contSongs      {Song {}.SetLength (10 * 1000), Song {}.SetLength (15 * 1000)},
	_dtWithinSong   {0},
	_bPlaying       {false}
{}

TimeRep WorkerImpl_Player::Do_GetTimeToWait () // override
{
	return -1;
}

Worker::WorkItemRV WorkerImpl_Player::Do_OnTimeout () // override;
{
	return Worker::Break_0;
}

}
