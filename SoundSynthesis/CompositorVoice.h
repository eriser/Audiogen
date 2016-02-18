#pragma once

#include "XAudioSynthesis\SourceVoice.h"

using namespace SoundSynthesis::XAudioSynthesis;

namespace SoundSynthesis { namespace Runtime
{
	public ref class CompositorVoice sealed
	{
	public:
		virtual ~CompositorVoice();

		_Check_return_
		bool Start(double x, double y);
		void Move(double x, double y);

	internal:
		static CompositorVoice ^Create(_In_ SourceVoice *voice);

	private:
		CompositorVoice(_In_ SourceVoice *voice);

	private:
		SourceVoice * const m_voice;
	};
}}
