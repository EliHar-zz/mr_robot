FROM node:6.6
ENV ANG_APP=/opt/mr-robot-html
RUN mkdir $ANG_APP
WORKDIR $ANG_APP
COPY . $ANG_APP
RUN npm install -g angular-cli@1.0.0-beta.28.3 && npm cache clean
RUN rm -rf node_modules
RUN npm install
EXPOSE 4200
CMD ng serve --host=0.0.0.0
