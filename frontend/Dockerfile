# build stage
FROM node:lts-alpine as build-stage
WORKDIR /app
COPY package*.json ./
RUN npm install
COPY . .
RUN npm run build

# production stage
FROM nginx:1.11 as production-stage
ENV VUE_APP_API_URL=http://localhost
COPY --from=build-stage /app/dist /usr/share/nginx/html
COPY --from=build-stage /app/entrypoint.sh /

EXPOSE 80
RUN chmod +x /entrypoint.sh
ENTRYPOINT [ "/entrypoint.sh" ]