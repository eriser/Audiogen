#pragma once

#include "IRetainable.h"
#include "IAudioSource.h"
#include "..\Helpers\Oscillators.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	struct IAudioCompositor : public IRetainable
	{
		_Check_return_
		virtual IAudioSource *GetOscillatingSource(_In_ Helpers::PFOSCILLATOR oscillator) noexcept = 0;
	};
}}
