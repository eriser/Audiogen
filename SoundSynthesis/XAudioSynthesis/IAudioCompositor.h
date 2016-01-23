#pragma once

#include "IAudioSource.h"
#include "..\Helpers\Oscillators.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	struct IAudioCompositor
	{
		virtual void Retain() noexcept = 0;
		virtual void Release() noexcept = 0;

		_Check_return_
		virtual IAudioSource *GetOscillatingSource(_In_ Helpers::PFOSCILLATOR oscillator) noexcept = 0;

	protected:
		virtual ~IAudioCompositor() noexcept {}
	};
}}
