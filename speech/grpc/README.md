Original repo: https://github.com/GoogleCloudPlatform/java-docs-samples/tree/master/speech

Steps for using this:
* Install Java
* export GOOGLE_APPLICATION_CREDENTIALS=/path/to/your/credentials-key.json
* mvn package
* mvn compile
* mvn assembly:single

Binaries:
* bin/speech-sample-sync.sh --host=speech.googleapis.com --port=443 --uri=resources/audio.raw --sampling=16000
* bin/speech-sample-async.sh --host=speech.googleapis.com --port=443 --uri=resources/audio.raw --sampling=16000
* bin/speech-sample-streaming.sh --host=speech.googleapis.com --port=443 --file=resources/audio.raw --sampling=16000

Create an audio file:
* Open Audacity
* Record voice (or audio from monitor)
* Set rate to 16,000 Hz
* Change channel mode: Tracks > Stereo Track to Mono
* Export with encoding signed 16-bit PCM
