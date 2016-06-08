# sshd
#
# VERSION               0.0.2

FROM     ubuntu:latest
MAINTAINER mdt "mdt@wegewerk.com"

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

RUN apt-get update
RUN apt-get -y upgrade
RUN apt-get install -y apt-utils

# install php and modules needed in cli mode
RUN apt-get install -y curl vim mc

RUN locale-gen de_DE.UTF-8 && update-locale LANG=de_DE.UTF-8

# install nodejs
RUN curl -sL https://deb.nodesource.com/setup_6.x | bash - && DEBIAN_FRONTEND=noninteractive apt-get install -y nodejs
RUN npm install -g cordova

# some dotfiles
ADD .bash_aliases /root/
ADD .gitconfig /root/

# add Install-Script for Android SDK
ADD install_androidsdk.sh /root/
RUN chmod +x /root/install_androidsdk.sh
# run Install Script for Android SDK
RUN /root/install_androidsdk.sh

CMD ["/bin/bash"]
