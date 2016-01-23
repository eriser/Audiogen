#pragma once

namespace SoundSynthesis { namespace XAudioSynthesis
{
	struct IAudioSource
	{
		virtual void Stop() noexcept = 0;

	protected:
		virtual ~IAudioSource() noexcept {}
	};
}}
