FROM node:10-alpine

RUN mkdir -p /home/node/app/node_modules && chown -R node:node /home/node/app

WORKDIR /home/node/app

COPY package*.json ./

USER node

RUN npm install

COPY --chown=node:node . .

EXPOSE 8080

ENV PG_PORT=5432
ENV PG_URL=localhost
ENV PG_USER=postgres
ENV PG_PASS=postgres

CMD [ "node", "app.js" ]