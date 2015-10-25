namespace Audiogen.Models
{
    using System;

    /// <summary>
    /// Class hides complexity of AudioGraph setup.
    /// </summary>
    sealed class Synthesizer : ISynthesizer
    {
        /// <summary>
        /// ISynthesizer.Ready
        /// </summary>
        public event EventHandler Ready;

        /// <summary>
        /// ISynthesizer.Failed
        /// </summary>
        public event EventHandler Failed;

        void ISynthesizer.SetUp()
        {
        }

        private void EmitReady()
        {
            this.Ready?.Invoke(this, EventArgs.Empty);
        }

        private void EmitFailed()
        {
            this.Failed?.Invoke(this, EventArgs.Empty);
        }
    }
}
