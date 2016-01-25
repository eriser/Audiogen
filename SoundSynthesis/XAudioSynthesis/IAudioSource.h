#pragma once

#include "IRetainable.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	struct IAudioSource : public IRetainable
	{
		virtual void Start(float x, float y) noexcept = 0;
		virtual void Move(float x, float y) noexcept = 0;
		virtual void Stop() noexcept = 0;
	};
}}
