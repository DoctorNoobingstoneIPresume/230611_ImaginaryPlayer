#pragma once

#include "WorkerImpl.hpp"

namespace ImaginaryPlayer
{

class Player;

class WorkerImpl_Player:
	public WorkerImpl
{
 private:
	      std::shared_ptr <Player>          _spPlayer;

 public:
	WorkerImpl_Player (const std::shared_ptr <Player> &spPlayer);

 private:
	Duration           Do_GetTimeToWait (const Arg &arg)                 override;
	Worker::WorkItemRV Do_OnWakeUp      (const Arg &arg, bool bWorkToDo) override;
	Worker::WorkItemRV Do_OnTimeout     (const Arg &arg)                 override;
};

}
