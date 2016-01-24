#pragma once

namespace SoundSynthesis { namespace XAudioSynthesis
{
	template<class TInterface>
	class Retainable : public TInterface
	{
	public:
		void Retain() noexcept override
		{
			_InterlockedIncrement(&m_refs);
		}

		void Release() noexcept override
		{
			if (0 == _InterlockedDecrement(&m_refs))
			{
				FinalRelease();
				delete this;
			}
		}

	protected:
		Retainable() noexcept : m_refs(1)
		{
		}

		~Retainable() noexcept
		{
			_ASSERTE(0 == m_refs);
		}

		virtual void FinalRelease() noexcept
		{
		}

	private:
		volatile LONG m_refs;
	};
}}
