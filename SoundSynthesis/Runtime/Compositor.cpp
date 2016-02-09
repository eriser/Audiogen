#include "pch.h"
#include "Compositor.h"
#include "XAudioSynthesis\XAudioCompositor.h"
#include "Helpers\Oscillators.h"

using namespace SoundSynthesis::Runtime;

Compositor::~Compositor()
{
	m_compositor->Release();
}

Compositor ^Compositor::CreateCompositor()
{
	Compositor ^compositor = ref new Compositor();

	if (!compositor->Initialize())
	{
		delete compositor;
		compositor = nullptr;
	}

	return compositor;
}

bool Compositor::Start()
{
	return m_compositor->Start();
}

void Compositor::Stop()
{
	m_compositor->Stop();
}

CompositorVoice ^Compositor::GetVoice()
{
	CompositorVoice ^cv = nullptr;
	IAudioSource *voice = m_compositor->GetOscillatingSource(SoundSynthesis::Helpers::LadderOscillator);

	if (nullptr != voice)
	{
		cv = CompositorVoice::Create(voice);
		voice->Release();
	}

	return cv;
}

void Compositor::StartWhiteNoise()
{
	m_whiteNoise.Start();
}

void Compositor::StopWhiteNoise()
{
	m_whiteNoise.Stop();
}

Compositor::Compositor()
:	m_compositor(nullptr)
{
}

bool Compositor::Initialize()
{
	m_compositor = XAudioCompositor::Create();
	return nullptr != m_compositor;
}
