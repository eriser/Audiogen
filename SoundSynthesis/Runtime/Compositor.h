#pragma once

#include "CompositorVoice.h"
#include "XAudioSynthesis\AudioEngine.h"

using namespace Microsoft::WRL;
using namespace SoundSynthesis::XAudioSynthesis;

namespace SoundSynthesis { namespace Runtime
{
	public ref class Compositor sealed
	{
	public:
		virtual ~Compositor();

		static Compositor ^CreateCompositor();

		bool Start();
		void Stop();
		CompositorVoice ^GetVoice();

	private:
		Compositor();
		bool Initialize();

	private:
		AudioEngine	*m_audioEngine;
	};
}}
