#include "Player.hpp"

namespace ImaginaryPlayer
{

Player::Player (const std::shared_ptr <Worker> &spWorkerLogger):
	_spWorkerLogger {spWorkerLogger},
	_contSongs      {Song {}.SetLength (std::chrono::seconds {10}), Song {}.SetLength (std::chrono::seconds {15})},
	_dtWithinSong   {0},
	_bPlaying       {false},
	_tLastPlaying   {Now ()}
{}

Duration Player::GetTimeToWait ()
{
	if (_bPlaying && ! _contSongs.empty ())
	{
		const Song &song {_contSongs.front ()};
		const Duration dtSongLength {song.GetLength ()};
		Azzert (_dtWithinSong <= dtSongLength);
		return dtSongLength - _dtWithinSong;
	}
	else
		return Duration {-1};
}

Worker::WorkItemRV Player::OnTimeout ()
{
	return Worker::Break_0;
}

}
