namespace Audiogen.Models
{
    using SoundSynthesis.Runtime;
    using System;
    using Windows.Foundation;
    using Windows.Media;
    using Windows.Media.Audio;
    using Windows.Media.Render;

    /// <summary>
    /// Class hides complexity of AudioGraph setup.
    /// </summary>
    sealed class Synthesizer : ISynthesizer
    {
        private AudioGraph _audioGraph;
        private AudioFrameInputNode _frameInputNode;
        private WaveSource _waveSource;
        private uint _channelsNumber;

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
            AudioGraphSettings settings = new AudioGraphSettings(AudioRenderCategory.Media)
            {
                QuantumSizeSelectionMode = QuantumSizeSelectionMode.LowestLatency
            };

            AudioGraph.CreateAsync(settings).AsTask().ContinueWith(graphTask =>
            {
                CreateAudioGraphResult graphResult = graphTask.Result;

                if(graphResult.Status != AudioGraphCreationStatus.Success)
                {
                    this.EmitFailed();
                }
                else
                {
                    graphResult.Graph.CreateDeviceOutputNodeAsync().AsTask().ContinueWith(nodeTask =>
                    {
                        CreateAudioDeviceOutputNodeResult nodeResult = nodeTask.Result;

                        if(nodeResult.Status != AudioDeviceNodeCreationStatus.Success)
                        {
                            this.EmitFailed();
                        }
                        else
                        {
                            _audioGraph = graphResult.Graph;
                            _frameInputNode = _audioGraph.CreateFrameInputNode();
                            _frameInputNode.AddOutgoingConnection(nodeResult.DeviceOutputNode);
                            _frameInputNode.QuantumStarted += this.OnQuantumStarted;
                            _channelsNumber = _audioGraph.EncodingProperties.ChannelCount;
                            _waveSource = new WaveSource();
                            this.EmitReady();
                        }
                    });
                }
            });
        }

        void ISynthesizer.Start()
        {
            _audioGraph.Start();
        }

        void ISynthesizer.Stop()
        {
            _audioGraph.Stop();
        }

        private void EmitReady()
        {
            this.Ready?.Invoke(this, EventArgs.Empty);
        }

        private void EmitFailed()
        {
            this.Failed?.Invoke(this, EventArgs.Empty);
        }

        private void OnQuantumStarted(AudioFrameInputNode sender, FrameInputNodeQuantumStartedEventArgs e)
        {
            AudioFrame frame = new AudioFrame((uint)e.RequiredSamples * _channelsNumber * sizeof(float));

            using (AudioBuffer buffer = frame.LockBuffer(AudioBufferAccessMode.Write))
            {
                using (IMemoryBufferReference reference = buffer.CreateReference())
                {
                    _waveSource.GenerateWave(reference, _channelsNumber, e.RequiredSamples);
                }
            }

            sender.AddFrame(frame);
        }
    }
}
