#include "pch.h"
#include "Compositor.h"
#include "XAudioSynthesis\XAudioCompositor.h"
#include "Helpers\Oscillators.h"
#include "XAudioSynthesis\SourceVoice.h"

using namespace SoundSynthesis::Runtime;

Compositor::Compositor()
:	m_compositor(nullptr),
	m_audioEngine(nullptr),
	m_sourceVoice(nullptr)
{
	m_audioEngine = new AudioEngine();
	m_audioEngine->StartEngine();
}

Compositor::~Compositor()
{
	m_audioEngine->StopEngine();
	delete m_audioEngine;
	m_audioEngine = nullptr;

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
	m_sourceVoice = m_audioEngine->CreateSourceVoice();
	m_sourceVoice->Start();
}

void Compositor::StopWhiteNoise()
{
	m_sourceVoice->Stop();
	m_sourceVoice = nullptr;
}

bool Compositor::Initialize()
{
	m_compositor = XAudioCompositor::Create();
	return nullptr != m_compositor;
}
