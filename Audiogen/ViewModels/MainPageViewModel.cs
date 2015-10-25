using Audiogen.Models;
using System;

namespace Audiogen.ViewModels
{
    sealed class MainPageViewModel : ViewModelBase
    {
        private IDispatcher _dispatcher;
        private ISynthesizer _synthesizer;

        public IDispatcher Dispatcher
        {
            get { return _dispatcher; }
            set
            {
                if(this.SetProperty(ref _dispatcher, value))
                {
                    SetUpSynthesizerIfFullyComposed();
                }
            }
        }

        public ISynthesizer Synthesizer
        {
            get { return _synthesizer; }
            set
            {
                ISynthesizer oldSynthesizer = _synthesizer;

                if (this.SetProperty(ref _synthesizer, value))
                {
                    oldSynthesizer.Ready -= this.OnSynthesizerReady;
                    oldSynthesizer.Failed -= this.OnSynthesizerFailed;
                    _synthesizer.Ready += this.OnSynthesizerReady;
                    _synthesizer.Failed += this.OnSynthesizerFailed;

                    SetUpSynthesizerIfFullyComposed();
                }
            }
        }

        private void SetUpSynthesizerIfFullyComposed()
        {
            if(null != _synthesizer && null != _dispatcher)
            {
                _synthesizer.SetUp();
            }
        }

        private void OnSynthesizerReady(object sender, EventArgs e)
        {
        }

        private void OnSynthesizerFailed(object sender, EventArgs e)
        {
        }
    }
}
