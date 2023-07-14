#include "WorkerImpl_Player.hpp"
#include "Player.hpp"

namespace ImaginaryPlayer
{

WorkerImpl_Player::WorkerImpl_Player (const std::shared_ptr <Player> &spPlayer):
	_spPlayer {spPlayer}
{}

Duration           WorkerImpl_Player::Do_GetTimeToWait (const Arg &arg)                 // override
{
	return _spPlayer->GetTimeToWait (arg);
}

Worker::WorkItemRV WorkerImpl_Player::Do_OnWakeUp      (const Arg &arg, bool bWorkToDo) // override
{
	return _spPlayer->OnWakeUp (arg, bWorkToDo);
}

Worker::WorkItemRV WorkerImpl_Player::Do_OnTimeout     (const Arg &arg)                 // override;
{
	return _spPlayer->OnTimeout (arg);
}

}
