#pragma once

namespace SoundSynthesis { namespace XAudioSynthesis
{

	class WorkerThread abstract
	{
	protected:
		WorkerThread() noexcept;
		virtual ~WorkerThread() noexcept;

		_Check_return_
		bool Start() noexcept;
		void Stop() noexcept;
		void Queue(_In_ PAPCFUNC function, ULONG_PTR parameter) noexcept;

	private:
		static unsigned int __stdcall ThreadStub(void *arg) noexcept;
		void ThreadProc() const noexcept;

	private:
		HANDLE	m_thread;
		HANDLE	m_stop;
	};
}}
