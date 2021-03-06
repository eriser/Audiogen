﻿namespace Audiogen.Models
{
    using SoundSynthesis.Runtime;
    using System;
    using Windows.Foundation;
    using Windows.Media;
    using Windows.Media.Audio;
    using Windows.Media.Render;
    using ViewModels;

    /// <summary>
    /// Class hides complexity of AudioGraph setup.
    /// </summary>
    sealed class Synthesizer : ISynthesizer
    {
        private AudioGraph _audioGraph;
        private AudioFrameInputNode _frameInputNode;
        private WaveSource _waveSource;
        private uint _channelsNumber;

        public Synthesizer()
        {
        }

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
            using (WavePlayer player = WavePlayer.CreateWavePlayer())
            {
            }

            AudioGraphSettings settings = new AudioGraphSettings(AudioRenderCategory.Media)
            {
                //QuantumSizeSelectionMode = QuantumSizeSelectionMode.LowestLatency
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
                            _waveSource = new WaveSource(_audioGraph.EncodingProperties.SampleRate, _channelsNumber);
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

        void ISynthesizer.BeginTone(PointerPosition position)
        {
            _waveSource.BeginWave(position.X, position.Y);
        }

        void ISynthesizer.ChangeTone(PointerPosition position)
        {
            _waveSource.ChangeWave(position.X, position.Y);
        }

        void ISynthesizer.EndTone()
        {
            _waveSource.EndWave();
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
            if(e.RequiredSamples > 0)
            {
                sender.AddFrame(GenerateAudioFrame(e.RequiredSamples));
            }
        }

        private AudioFrame GenerateAudioFrame(int samplesNumber)
        {
            AudioFrame frame = new AudioFrame((uint)samplesNumber * _channelsNumber * sizeof(float));

            using (AudioBuffer buffer = frame.LockBuffer(AudioBufferAccessMode.Write))
            {
                using (IMemoryBufferReference reference = buffer.CreateReference())
                {
                    _waveSource.GenerateWave(reference, samplesNumber);
                }
            }

            return frame;
        }
    }
}
