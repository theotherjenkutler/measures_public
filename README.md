
# Measures
## An ecosystem for detection of physiological indicators of empathy.
<p>MEASURES is a flexible web based, AI driven eco-system which lends itself to live performance, 
<br>installation in a gallery museum, festival or alone in one's bedroom while being simple enough 
<br>for anyone with WIFI and a smartphone to use.
<br>It fuses technology and emotion where technology is often sterile and intellectual and offers 
<br>tools for self reflection while creating a method of studying the phenomenon of empathy as a whole. 
<br>Ultimately, MEASURES offers a new way to feel connected and to experience one another across borders, 
<br>cultures, socioeconomic classes and other zones of isolation.

<p>Users wear an electronic wristband which provides audio cues and measures the participant's electrodermal
<br>activity. The data is transferred over WIFI to a server which processes the data and generates a unique sound work.
<br>Each sound work produced by a participant is subsequently analyzed for point of “excitation” using Opensmile 
<br>feature extraction trained by a custom piece specific data set. The results are used as the basis of the following 
<br>user's auditory stimuli and physiological data comparison to generate the next sound piece and so on. The sound 
<br>works are added to a public online archive. Participants can choose to disclose their identity or remain anonymous.
<br>To my knowledge, MEASURES is the first system in the artistic and scientific communities designed with the 
<br>specific purpose of quantifying and measuring empathy from auditory cues. MEASURES content originates from an 
<br>infinitely expansive pool of participant data is comprised of empathetic responses for an AI driven art work.

<p>The goals of this piece put the wealth seeking uses of AI such as advertising, facial/voice recognition, fraud 
<br>prevention and other commercial applications in the background. It focuses on AI as a tool to witness people in 
<br>the most human way possible – through experiencing a person feeling empathy.
<br>The form of MEASURES is also unique because the data collection device exists as an electronic art object and 
<br>generates an expansive bio-data driven archive of individual sound works, and is designed to be extremely 
<br>accessible as a tool for installation, real-time performance and solitary use.

<p>The hardware uses a custom EDA sensor based on the AD8608 op-amp and handmade carbon silicone sensor pads.
<br>MP3 decoding is handled by the VS1053b chip connected to the ESP32WROVER over SPI.
<br>For ADC it uses an ADS1115 IC over the I2C Bus. 
 
<p>This repo will include 
<br>- Arduino firmware code for the ESP32-WROVER,
<br>- Node-Red code with respective shell scripts, 
<br>- PureData patches for sonification, 
<br>- EAGLE CAD files of schematics and PCB layout,
<br>- TensorFlow and Pytorch code

<p>Server Requirements:
<br>- Ubuntu or similar
<br>- Icecast2 // icecast.org
<br>- FFMPEG // ffmpeg.org
<br>- PureData // puredata.info
<br>- Node-Red // nodered.org
<br>- inotifytools // https://github.com/inotify-tools/inotify-tools

<p>Device Firmware Requirements:
<br>- Arduino IDE + ESP32 Arduino
<br>- External Libraries:
<br>- Adafruit_VS1053 // https://github.com/adafruit/Adafruit_VS1053_Library
<br>- WiFi Manager // https://github.com/tzapu/WiFiManager
<br>- ADS1X15 // https://github.com/RobTillaart/ADS1X15

<p>Detailed system description:
<br>TODO


<p>Relavent links/Bibliography:
<br>TODO
<br>https://arxiv.org/pdf/2008.09743.pdf Guanghao Yin, Shouqian Sun, Dian Yu and Kejun Zhang*, Member, 
<br>IEEE in their paper A Efficient Multimodal Framework for Large Scale Emotion Recognition by Fusing Music 
<br>and Electrodermal Activity Signals. openSMILE https://audeering.github.io/opensmile/ to obtain external feature 
<br>vectors of audio cues

 <br>Measures is made possible in part by the New York State Council on the Arts with the support of the Office of 
 <br>the Governor and the New York State Legislature and Wave Farm.
<br><img src ="https://images.squarespace-cdn.com/content/v1/5446b835e4b03fe08f25a5ea/cafac78b-01fe-4b1a-acda-f424f0637067/NYSCA-Logo-White.jpg?format=500w">

