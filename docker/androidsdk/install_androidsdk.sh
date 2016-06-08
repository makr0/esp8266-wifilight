#!/usr/bin/env bash

cd /root/

#Android SDK
ANDROID_SDK_FILENAME=android-sdk_r24.4.1-linux.tgz
ANDROID_SDK=http://dl.google.com/android/$ANDROID_SDK_FILENAME

#sudo apt-get install python-software-properties
#sudo add-apt-repository ppa:webupd8team/java
apt-get update
apt-get install -y openjdk-8-jdk expect
apt-get install -y gcc-multilib lib32z1 lib32stdc++6

curl -O $ANDROID_SDK
tar -xzvf $ANDROID_SDK_FILENAME
#sudo chown -R vagrant android-sdk-linux/
#sudo chown -R vagrant:vagrant /root/.android/

echo "export ANDROID_HOME=~/android-sdk-linux" >> /root/.bashrc
echo "export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64" >> /root/.bashrc
echo "PATH=\$PATH:~/android-sdk-linux/tools:~/android-sdk-linux/platform-tools" >> /root/.bashrc

expect -c '
set timeout -1   ;
spawn /root/android-sdk-linux/tools/android update sdk -u --all --filter android-23,tools,platform-tools,build-tools-23.0.3
expect {
    "Do you accept the license" { exp_send "y\r" ; exp_continue }
    eof
}
'
