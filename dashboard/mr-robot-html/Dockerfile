FROM ubuntu:16.04
RUN apt-get update
RUN apt-get -y install python zip wget make g++ libssl-dev git
WORKDIR /tmp
RUN wget https://github.com/nodejs/node/archive/v6.1.0.zip
RUN unzip v6.1.0.zip
WORKDIR /tmp/node-6.1.0
RUN ./configure
RUN make
RUN make install
RUN ln -s /usr/local/bin/node /usr/bin/node
RUN ln -s /usr/bin/node /usr/bin/nodejs
RUN apt-get install npm -y
RUN npm install npm@3.10.5 -g
RUN rm /usr/bin/npm
RUN ln -s /usr/local/bin/npm /usr/bin/npm
#RUN apt-get -y install nodejs-legacy
#RUN npm install --unsafe-perm node-sass
#RUN npm rebuild node-sass
RUN npm install -g angular-cli@1.0.0-beta.28.3
RUN mkdir -p /opt/mr_robot_html
WORKDIR /opt/mr_robot_html
COPY . /opt/mr_robot_html
RUN rm -rf node_modules
EXPOSE 4100
RUN npm install
CMD ng serve --host=0.0.0.0 --port 4100
