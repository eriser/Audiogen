#pragma once

namespace SoundSynthesis { namespace XAudioSynthesis
{

	class WhiteNoise sealed
	{
	public:
		WhiteNoise() noexcept;
		~WhiteNoise() noexcept;

		void Start() noexcept;
		void Stop() noexcept;

	private:
		IXAudio2 *m_xaudio2;
	};
}}
