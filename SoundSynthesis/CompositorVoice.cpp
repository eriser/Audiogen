#include "pch.h"
#include "CompositorVoice.h"

using namespace SoundSynthesis::Runtime;

CompositorVoice::~CompositorVoice()
{
	m_voice->Stop();
	m_voice->Release();
}

void CompositorVoice::Start(double x, double y)
{
	m_voice->Start(static_cast<float>(x), static_cast<float>(y));
}

CompositorVoice ^CompositorVoice::Create(_In_ IAudioSource *voice)
{
	return ref new CompositorVoice(voice);
}

CompositorVoice::CompositorVoice(_In_ IAudioSource *voice)
:	m_voice(voice)
{
	m_voice->Retain();
	m_voice->Start(0, 0);
}
