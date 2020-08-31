# Frontend

The frontend is responsible for connecting to the API, requesting data, and then display that data in a useful fashion.

## Required environment variables

- `VUE_APP_API_URL` url of node.js data api

## Running locally

- `npm install`
- `npm run serve`

## Running via docker

```
docker run -p 5050:80 -e VUE_APP_API_URL=urlhere jvb1993/weather-frontend
```
