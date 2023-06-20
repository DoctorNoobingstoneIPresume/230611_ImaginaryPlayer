#include "WorkerImpl_Player.hpp"
#include "Player.hpp"

namespace ImaginaryPlayer
{

WorkerImpl_Player::WorkerImpl_Player (const std::shared_ptr <Player> &spPlayer):
	_spPlayer {spPlayer}
{}

TimeRep WorkerImpl_Player::Do_GetTimeToWait () // override
{
	return _spPlayer->GetTimeToWait ();
}

Worker::WorkItemRV WorkerImpl_Player::Do_OnTimeout () // override;
{
	return _spPlayer->OnTimeout ();
}

}
