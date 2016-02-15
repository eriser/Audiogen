#include "pch.h"
#include "Compositor.h"
#include "Helpers\Oscillators.h"
#include "XAudioSynthesis\SourceVoice.h"

using namespace SoundSynthesis::Runtime;

Compositor::Compositor()
:	m_audioEngine(nullptr)
{
}

Compositor::~Compositor()
{
	delete m_audioEngine;
	m_audioEngine = nullptr;
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
	return m_audioEngine->StartEngine();
}

void Compositor::Stop()
{
	m_audioEngine->StopEngine();
}

CompositorVoice ^Compositor::GetVoice()
{
	CompositorVoice ^cv = nullptr;
	SourceVoice *voice = m_audioEngine->CreateSourceVoice();

	if (nullptr != voice)
	{
		OutputDebugString(TEXT("Compositor::GetVoice()\n"));
		cv = CompositorVoice::Create(voice);
	}

	return cv;
}

bool Compositor::Initialize()
{
	m_audioEngine = new(std::nothrow) AudioEngine();
	return nullptr != m_audioEngine;
}
