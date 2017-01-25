Original repo: https://github.com/GoogleCloudPlatform/java-docs-samples/tree/master/speech

Google Repo:
* Clone https://github.com/GoogleCloudPlatform/java-docs-samples
* cd speech/gprc
* Replace the bin folder by the one in this repo

Steps for using Google Repo:
* Install Java
* Run the setup.sh in the new bin directory
* mvn package
* mvn compile
* mvn assembly:single

Binaries:
* bin/speech-sample-sync.sh --host=speech.googleapis.com --port=443 --uri=resources/audio.raw --sampling=16000
* bin/speech-sample-async.sh --host=speech.googleapis.com --port=443 --uri=resources/audio.raw --sampling=16000
* bin/speech-sample-streaming.sh --host=speech.googleapis.com --port=443 --file=resources/audio.raw --sampling=16000
