#pragma once

namespace SoundSynthesis { namespace XAudioSynthesis
{
	struct IRetainable
	{
		virtual void Retain() noexcept = 0;
		virtual void Release() noexcept = 0;

	protected:
		virtual ~IRetainable() noexcept {}
	};
}}
