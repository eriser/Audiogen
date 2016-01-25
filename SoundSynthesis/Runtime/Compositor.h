#pragma once

#include "XAudioSynthesis\IAudioCompositor.h"

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

	private:
		Compositor();
		bool Initialize();

	private:
		IAudioCompositor *m_compositor;
	};
}}
