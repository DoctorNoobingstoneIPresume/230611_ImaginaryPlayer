#include "Player.hpp"

namespace ImaginaryPlayer
{

Player::Player (const std::shared_ptr <Worker> &spWorkerLogger):
	_spWorkerLogger {spWorkerLogger},
	_contSongs      {Song {}.SetLength (10 * 1000), Song {}.SetLength (15 * 1000)},
	_dtWithinSong   {0},
	_bPlaying       {false}
{}

TimeRep Player::GetTimeToWait ()
{
	return -1;
}

Worker::WorkItemRV Player::OnTimeout ()
{
	return Worker::Break_0;
}

}
