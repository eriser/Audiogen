namespace Audiogen.Models
{
    using System;

    interface ISynthesizer
    {
        /// <summary>
        /// Event emitted on a worker thread when the synthesizer is fully set up and ready to be started.
        /// </summary>
        event EventHandler Ready;

        /// <summary>
        /// Event emitted on a worker thread if the synthesizer has failed to initialize.
        /// </summary>
        event EventHandler Failed;

        /// <summary>
        /// Set up the synthesizer and emit Ready or Failed event upon completion of initialization.
        /// </summary>
        void SetUp();
    }
}
