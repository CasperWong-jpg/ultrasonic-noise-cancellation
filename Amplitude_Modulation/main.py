import numpy as np
import wave
from scipy import signal
import struct

import matplotlib
import matplotlib.pyplot as plt

matplotlib.use('TkAgg')

# Parameters
duration = 20  # Duration of the signal in seconds
sampling_freq = 96000  # Sampling frequency (Hz)
carrier_freq = 40000  # Carrier frequency (Hz)
audible_freq = 440  # Modulating frequency (Hz)  TODO: Input any .wav file here
amplitude = 32767  # Amplitude of the signal. 16384 is max amplitude?

# Generate time array
t = np.linspace(0, duration, int(sampling_freq * duration))

# Generate the audible signal
audible_signal = np.sin(2 * np.pi * audible_freq * t)
# Discard negative baseband of signal
hilbert_signal = np.imag(signal.hilbert(audible_signal))

# Generate the carrier signal (40 kHz)
carrier_sine = np.sin(2 * np.pi * carrier_freq * t)
carrier_cosine = np.cos(2 * np.pi * carrier_freq * t)

# Modulate the carrier signal with the modulating signal
modulated_signal = audible_signal * carrier_cosine - hilbert_signal * carrier_sine  # TODO: Add carrier frequency for audible sound

# Normalize the modulated signal to the range [-1, 1]
modulated_signal /= np.max(np.abs(modulated_signal))

# Convert the modulated signal to 16-bit integers
modulated_signal = (amplitude * modulated_signal).astype(np.int16)

# Plot the modulated signal
# plt.figure(figsize=(10, 5))
# plt.plot(t[:1000], modulated_signal[:1000], color='blue')
# plt.title('Amplitude-Modulated Signal')
# plt.xlabel('Time (s)')
# plt.ylabel('Amplitude')
# plt.grid(True)
# plt.show()

# Create a wave file
output_file = "AM.wav"
with wave.open(output_file, 'w') as wav_file:
    wav_file.setnchannels(1)  # Mono
    wav_file.setsampwidth(2)  # 16-bit
    wav_file.setframerate(sampling_freq)
    for sample in modulated_signal:
        wav_file.writeframes(struct.pack('<h', sample))
